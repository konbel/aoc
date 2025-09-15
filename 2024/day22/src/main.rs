fn mix(secret_number: usize, mix: usize) -> usize {
    secret_number ^ mix
}

fn prune(secret_number: usize) -> usize {
    secret_number % 16777216
}

fn generate_secret_numbers(initial: usize, count: usize) -> usize {
    let mut secret_number = initial;
    for _ in 0..count {
        let step1 = prune(mix(secret_number, secret_number * 64));
        let step2 = prune(mix(step1, step1 / 32));
        let step3 = prune(mix(step2, step2 * 2048));
        secret_number = step3;
    }
    secret_number
}

fn task_one(input: &[String]) -> usize {
    input.into_iter().map(|initial| initial.parse::<usize>().unwrap()).map(|initial| generate_secret_numbers(initial, 2000)).sum()
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
        .unwrap_or_else(|| "../src/test".to_string())
}
