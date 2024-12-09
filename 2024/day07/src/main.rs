fn get_equations(input: &[String]) -> Vec<(usize, Vec<usize>)> {
    let mut equations = vec![];

    for line in input {
        let (test_value, numbers) = line.split_once(": ").unwrap();
        let test_value: usize = test_value.parse().unwrap();
        let numbers: Vec<usize> = numbers
            .split(' ')
            .into_iter()
            .map(|n| n.parse::<usize>().unwrap())
            .collect();

        equations.push((test_value, numbers));
    }

    equations
}

fn evaluate(test_value: usize, numbers: &[usize], current: usize, index: usize) -> bool {
    if current > test_value {
        return false;
    }

    if current == test_value {
        return true;
    }

    if index >= numbers.len() {
        return false;
    }

    let one = evaluate(test_value, numbers, current + numbers[index], index + 1);
    let two = evaluate(test_value, numbers, current * numbers[index], index + 1);
    one || two
}

fn evaluate2(test_value: usize, numbers: &[usize], current: usize, index: usize) -> bool {
    if current > test_value {
        return false;
    }

    if current == test_value {
        return true;
    }

    if index >= numbers.len() {
        return false;
    }

    let one = evaluate2(test_value, numbers, current + numbers[index], index + 1);
    let two = evaluate2(test_value, numbers, current * numbers[index], index + 1);

    let combined: usize = (current.to_string() + &numbers[index].to_string())
        .parse()
        .unwrap();
    let three = evaluate2(test_value, &numbers, combined, index + 1);

    one || two || three
}

fn task_one(input: &[String]) -> usize {
    let mut sum = 0;

    for (test_value, numbers) in get_equations(input) {
        if evaluate(test_value, &numbers, numbers[0], 1) {
            sum += test_value;
        }
    }

    sum
}

fn task_two(input: &[String]) -> usize {
    let mut sum = 0;

    for (test_value, numbers) in get_equations(input) {
        if evaluate2(test_value, &numbers, numbers[0], 1) {
            sum += test_value;
        }
    }

    sum
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
