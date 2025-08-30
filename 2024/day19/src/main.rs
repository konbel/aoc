fn is_possible(available: &[String], desired: &String) -> bool {
    let mut dp = vec![false; desired.len() + 1];
    dp[0] = true;

    for i in 0..desired.len() {
        if dp[i] == true {
            for word in available {
                if desired[i..].starts_with(word) {
                    dp[i + word.len()] = true;
                }
            }
        }
    }

    dp[desired.len()]
}

fn num_possibilities(available: &[String], desired: &String) -> usize {
    let mut dp: Vec<usize> = vec![0; desired.len() + 1];
    dp[0] = 1;

    for i in 0..desired.len() {
        if dp[i] > 0 {
            for word in available {
                if desired[i..].starts_with(word) {
                    dp[i + word.len()] += dp[i];
                }
            }
        }
    }

    dp[desired.len()]
}

fn task_one(input: &[String]) -> usize {
    let available = input[0].split(", ").map(|s| s.to_owned()).collect::<Vec<String>>();
    let desired = input[2..].iter().map(|s| s.to_owned()).collect::<Vec<String>>();
    desired.iter().filter(|d| is_possible(&available, d)).count()
}

fn task_two(input: &[String]) -> usize {
    let available = input[0].split(", ").map(|s| s.to_owned()).collect::<Vec<String>>();
    let desired = input[2..].iter().map(|s| s.to_owned()).collect::<Vec<String>>();
    desired.iter().map(|d| num_possibilities(&available, d)).sum()
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
