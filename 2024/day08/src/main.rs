use std::collections::{HashMap, HashSet};

fn get_antennas(grid: &[Vec<char>]) -> HashMap<char, HashSet<(usize, usize)>> {
    let mut antennas: HashMap<char, HashSet<(usize, usize)>> = HashMap::new();

    for (y, line) in grid.iter().enumerate() {
        for (x, c) in line.iter().enumerate() {
            if *c != '.' {
                if antennas.contains_key(c) {
                    antennas.get_mut(c).unwrap().insert((y, x));
                } else {
                    antennas.insert(*c, HashSet::from([(y, x)]));
                }
            }
        }
    }

    antennas
}

fn in_bounds(value: isize, max: isize) -> bool {
    value >= 0 && value < max
}

fn get_antinodes(
    antennas: &HashMap<char, HashSet<(usize, usize)>>,
    y_max: usize,
    x_max: usize,
) -> HashSet<(usize, usize)> {
    let mut antinodes: HashSet<(usize, usize)> = HashSet::default();

    for k in antennas.keys() {
        let values: Vec<(usize, usize)> = antennas
            .get(k)
            .unwrap()
            .iter()
            .map(|&(y, x)| (y, x))
            .collect();

        for &(y1, x1) in &values {
            for &(y2, x2) in &values {
                if (y1, x1) == (y2, x2) {
                    continue;
                }

                let vx: isize = x2 as isize - x1 as isize;
                let vy: isize = y2 as isize - y1 as isize;

                let nx1 = (x1 as isize).checked_sub(vx).unwrap_or(x_max as isize);
                let ny1 = (y1 as isize).checked_sub(vy).unwrap_or(y_max as isize);

                if !in_bounds(nx1, x_max as isize) || !in_bounds(ny1, y_max as isize) {
                    continue;
                }

                antinodes.insert((ny1 as usize, nx1 as usize));

                let nx2 = (x2 as isize).checked_add(vx).unwrap_or(x_max as isize);
                let ny2 = (y2 as isize).checked_add(vy).unwrap_or(y_max as isize);

                if !in_bounds(nx2, x_max as isize) || !in_bounds(ny2, y_max as isize) {
                    continue;
                }

                antinodes.insert((ny2 as usize, nx2 as usize));
            }
        }
    }

    antinodes
}

fn get_antinodes2(
    antennas: &HashMap<char, HashSet<(usize, usize)>>,
    y_max: usize,
    x_max: usize,
) -> HashSet<(usize, usize)> {
    let mut antinodes: HashSet<(usize, usize)> = HashSet::default();

    for k in antennas.keys() {
        let values: Vec<(usize, usize)> = antennas
            .get(k)
            .unwrap()
            .iter()
            .map(|&(y, x)| (y, x))
            .collect();

        for &(y1, x1) in &values {
            for &(y2, x2) in &values {
                if (y1, x1) == (y2, x2) {
                    continue;
                }

                let vx: isize = x2 as isize - x1 as isize;
                let vy: isize = y2 as isize - y1 as isize;

                let mut i = 1;

                loop {
                    let mut done = true;

                    let nx = (x1 as isize).checked_sub(vx * i).unwrap_or(x_max as isize);
                    let ny = (y1 as isize).checked_sub(vy * i).unwrap_or(y_max as isize);

                    if in_bounds(nx, x_max as isize) && in_bounds(ny, y_max as isize) {
                        antinodes.insert((ny as usize, nx as usize));
                        done = false;
                    }

                    let nx = (x1 as isize).checked_add(vx * i).unwrap_or(x_max as isize);
                    let ny = (y1 as isize).checked_add(vy * i).unwrap_or(y_max as isize);

                    if in_bounds(nx, x_max as isize) && in_bounds(ny, y_max as isize) {
                        antinodes.insert((ny as usize, nx as usize));
                        done = false;
                    }

                    if done {
                        break;
                    }

                    i += 1;
                }
            }
        }
    }

    antinodes
}

fn task_one(input: &[String]) -> usize {
    let grid: Vec<Vec<char>> = input.iter().map(|line| line.chars().collect()).collect();
    let antennas = get_antennas(&grid);

    get_antinodes(&antennas, grid.len(), grid[0].len()).len()
}

fn task_two(input: &[String]) -> usize {
    let grid: Vec<Vec<char>> = input.iter().map(|line| line.chars().collect()).collect();
    let antennas = get_antennas(&grid);

    get_antinodes2(&antennas, grid.len(), grid[0].len()).len()
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
