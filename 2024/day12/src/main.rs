use std::collections::{HashSet, VecDeque};

const DIRS: [(isize, isize); 4] = [(0, 1), (1, 0), (0, -1), (-1, 0)];

fn neighbours(grid: &[Vec<char>], tile: (isize, isize)) -> impl Iterator<Item = (isize, isize)> {
    let my = grid.len() as isize;
    let mx = grid[0].len() as isize;
    DIRS.iter()
        .map(move |dir| (tile.0 + dir.0, tile.1 + dir.1))
        .filter(move |&(y, x)| y >= 0 && y < my && x >= 0 && x < mx)
}

fn flood(grid: &[Vec<char>], start: (isize, isize)) -> (HashSet<(isize, isize)>, usize) {
    let mut queue = VecDeque::from([start]);
    let mut shape = HashSet::from([start]);

    let plant = grid[start.0 as usize][start.1 as usize];
    let mut perimiter = 0;

    while let Some(tile) = queue.pop_front() {
        let ns: Vec<_> = neighbours(&grid, tile)
            .filter(|&(y, x)| grid[y as usize][x as usize] == plant)
            .collect();

        perimiter += 4 - ns.len();

        for n in ns {
            if shape.insert(n) {
                queue.push_back(n);
            }
        }
    }

    (shape, perimiter)
}

fn count_sides(shape: &HashSet<(isize, isize)>) -> usize {
    let mut sum = 0;

    for d in DIRS {
        let sides: HashSet<(isize, isize)> = shape
            .iter()
            .map(|pos| (pos.0 + d.0, pos.1 + d.1))
            .filter(|next| !shape.contains(next))
            .collect();

        let mut remove: HashSet<(isize, isize)> = HashSet::default();
        for side in &sides {
            let mut tmp = (side.0 + d.1, side.1 + d.0);
            while sides.contains(&tmp) {
                remove.insert(tmp);
                tmp = (tmp.0 + d.1, tmp.1 + d.0);
            }
        }

        sum += sides.len() - remove.len();
    }

    sum
}

fn task_one(input: &[String]) -> usize {
    let grid: Vec<Vec<char>> = input.iter().map(|line| line.chars().collect()).collect();
    let mut seen: HashSet<(isize, isize)> = HashSet::default();
    let mut price = 0;

    for y in 0..grid.len() {
        for x in 0..grid[0].len() {
            let tile = (y as isize, x as isize);

            if seen.insert(tile) {
                let (shape, perimiter) = flood(&grid, tile);
                price += shape.len() * perimiter;
                seen.extend(shape);
            }
        }
    }

    price
}

fn task_two(input: &[String]) -> usize {
    let grid: Vec<Vec<char>> = input.iter().map(|line| line.chars().collect()).collect();
    let mut seen: HashSet<(isize, isize)> = HashSet::default();
    let mut price = 0;

    for y in 0..grid.len() {
        for x in 0..grid[0].len() {
            let tile = (y as isize, x as isize);

            if seen.insert(tile) {
                let (shape, _) = flood(&grid, tile);
                let sides = count_sides(&shape);
                price += shape.len() * sides;
                seen.extend(shape);
            }
        }
    }

    price
}

fn main() {
    let input = read_input(get_input_file());
    time(Task::One, task_one, &input);
    time(Task::Two, task_two, &input);
}

fn read_input<P>(path: P) -> Vec<String>
where
    P: AsRef<std::path::Path>,
{
    std::fs::read_to_string(path)
        .unwrap()
        .lines()
        .map(String::from)
        .collect()
}

enum Task {
    One,
    Two,
}

fn time<F, T, U>(task: Task, f: F, arg: T)
where
    F: Fn(T) -> U,
    U: std::fmt::Display,
{
    let t = std::time::Instant::now();
    let res = f(arg);
    let elapsed = t.elapsed();
    let fmt = std::env::var("TASKUNIT").unwrap_or("ms".to_owned());

    let (u, elapsed) = match fmt.as_str() {
        "ms" => ("ms", elapsed.as_millis()),
        "ns" => ("ns", elapsed.as_nanos()),
        "us" => ("μs", elapsed.as_micros()),
        "s" => ("s", elapsed.as_secs() as u128),
        _ => panic!("unsupported time format"),
    };

    match task {
        Task::One => {
            println!("({}{u})\tTask one: \x1b[0;34;34m{}\x1b[0m", elapsed, res);
        }
        Task::Two => {
            println!("({}{u})\tTask two: \x1b[0;33;10m{}\x1b[0m", elapsed, res);
        }
    };
}

fn get_input_file() -> String {
    std::env::args()
        .nth(1)
        .unwrap_or_else(|| "input".to_string())
}
