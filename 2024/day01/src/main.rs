use std::usize;

fn get_lists(input: &[String]) -> (Vec<usize>, Vec<usize>) {
    let mut list_one = vec![];
    let mut list_two = vec![];

    for line in input {
        let mut parts = line
            .split_whitespace()
            .filter_map(|s| s.parse::<usize>().ok());

        list_one.push(parts.next().unwrap_or(0));
        list_two.push(parts.next().unwrap_or(0));
    }

    list_one.sort();
    list_two.sort();

    (list_one, list_two)
}

fn task_one(input: &[String]) -> usize {
    let (list_one, list_two) = get_lists(input);

    (0..list_one.len()).fold(0, |res, i| res + list_one[i].abs_diff(list_two[i]))
}

fn task_two(input: &[String]) -> usize {
    let (list_one, list_two) = get_lists(input);

    list_one.iter().fold(0, |res, num| {
        let count = list_two.iter().filter(|&n| *n == *num).count();
        res + num * count
    })
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
