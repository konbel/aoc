use std::collections::HashSet;

const WIDTH: isize = 101;
const HEIGHT: isize = 103;

fn parse_robots(input: &[String]) -> Vec<((isize, isize), (isize, isize))> {
    let mut robots = vec![];

    let parse_vec = |i: &str| -> (isize, isize) {
        let mut split = i.split(',');
        let x = split
            .next()
            .unwrap()
            .chars()
            .skip(2)
            .collect::<String>()
            .parse::<isize>()
            .unwrap();
        let y = split.next().unwrap().parse::<isize>().unwrap();

        (x, y)
    };

    for line in input {
        let mut split = line.split(' ');
        let p = parse_vec(split.next().unwrap());
        let v = parse_vec(split.next().unwrap());
        robots.push((p, v));
    }

    robots
}

fn task_one(input: &[String]) -> usize {
    let mut q1 = 0;
    let mut q2 = 0;
    let mut q3 = 0;
    let mut q4 = 0;

    let half_width = (WIDTH - 1) / 2;
    let half_height = (HEIGHT - 1) / 2;

    for (p, v) in parse_robots(input) {
        let x = (p.0 + v.0 * 100).rem_euclid(WIDTH);
        let y = (p.1 + v.1 * 100).rem_euclid(HEIGHT);

        let left = x < half_width;
        let right = x > half_width;
        let top = y < half_height;
        let bottom = y > half_height;

        if left && top {
            q2 += 1;
        } else if right && top {
            q1 += 1;
        } else if left && bottom {
            q3 += 1;
        } else if right && bottom {
            q4 += 1;
        }
    }

    q1 * q2 * q3 * q4
}

fn task_two(input: &[String]) -> usize {
    let robots = parse_robots(input);

    for i in 0..10000 {
        let mut seen: HashSet<(isize, isize)> = HashSet::default();
        for (p, v) in &robots {
            let x = (p.0 + v.0 * i).rem_euclid(WIDTH);
            let y = (p.1 + v.1 * i).rem_euclid(HEIGHT);

            seen.insert((x, y));
        }

        if seen.len() == robots.len() {
            let mut grid = vec![vec!['.'; WIDTH as usize]; HEIGHT as usize];

            for &(x, y) in seen.iter() {
                grid[y as usize][x as usize] = '#';
            }

            for r in grid {
                println!("{}", r.iter().collect::<String>());
            }

            return i as usize;
        }
    }

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
