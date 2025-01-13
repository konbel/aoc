use std::collections::{HashSet, VecDeque};

fn flood(seen: &mut HashSet<(usize, usize)>, grid: &[Vec<char>], start: (usize, usize)) -> usize {
    let dirs = vec![(1, 0), (-1, 0), (0, 1), (0, -1)];
    let letter = grid[start.0][start.1];

    let mut queue = VecDeque::from([start]);
    seen.insert(start);
    let mut perimiter = 0;
    let mut area = 0;

    while !queue.is_empty() {
        let tile = queue.pop_front().unwrap();
        area += 1;

        for d in dirs.iter() {
            let y: isize = tile.0 as isize + d.0;
            let x: isize = tile.1 as isize + d.1;

            if y < 0 || y >= grid.len() as isize || x < 0 || x >= grid[0].len() as isize {
                perimiter += 1;
                continue;
            }

            let y = y as usize;
            let x = x as usize;

            if grid[y][x] == letter {
                let tile = (y, x);
                if seen.insert(tile) {
                    queue.push_back(tile);
                }
            } else {
                perimiter += 1;
            }
        }
    }

    area * perimiter
}

fn task_one(input: &[String]) -> usize {
    let grid: Vec<Vec<char>> = input.iter().map(|line| line.chars().collect()).collect();

    let mut seen: HashSet<(usize, usize)> = HashSet::default();
    let mut price = 0;

    for y in 0..grid.len() {
        for x in 0..grid[0].len() {
            let tile = (y, x);
            if !seen.contains(&tile) {
                price += flood(&mut seen, &grid, tile);
            }
        }
    }

    price
}

fn task_two(_input: &[String]) -> usize {
    0
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
