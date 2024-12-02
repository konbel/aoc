use std::iter::zip;
use std::usize;

fn is_safe(levels: &Vec<usize>) -> bool {
    let increasing = levels[0] < levels[levels.len() - 1];

    zip(levels[0..(levels.len() - 1)].iter(), levels[1..].iter())
        .filter(|(p, c)| c != p)
        .filter(|(p, c)| c.abs_diff(**p) <= 3)
        .filter(|(p, c)| !(increasing && c < p || !increasing && c > p))
        .count()
        == levels.len() - 1
}

fn is_safe_dampener(levels: &Vec<usize>) -> bool {
    (0..levels.len())
        .map(|i| {
            let reduced = [&levels[..i], &levels[i + 1..]].concat();
            is_safe(&reduced)
        })
        .any(|b| b == true)
}

fn task_one(input: &[String]) -> usize {
    let mut count = 0;

    for line in input {
        let levels = line
            .split_whitespace()
            .filter_map(|n| n.parse::<usize>().ok())
            .collect();

        if is_safe(&levels) {
            count += 1;
        }
    }

    count
}

fn task_two(input: &[String]) -> usize {
    let mut count = 0;

    for line in input {
        let levels = line
            .split_whitespace()
            .filter_map(|n| n.parse::<usize>().ok())
            .collect();

        if is_safe_dampener(&levels) {
            count += 1;
        }
    }

    count
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
