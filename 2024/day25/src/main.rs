type Lock = [usize; 5];
type Key = [usize; 5];

fn parse_lock(schematic: &Vec<String>) -> Lock {
    let mut lock: Lock = [0; 5];

    for row in &schematic[1..] {
        for (i, c) in row.chars().enumerate() {
            if c == '#' {
                lock[i] += 1;
            }
        }
    }

    lock
}

fn parse_key(schematic: &Vec<String>) -> Lock {
    let mut key: Key = [0; 5];

    for row in &schematic[1..] {
        for (i, c) in row.chars().enumerate() {
            if c == '#' {
                key[i] += 1;
            }
        }
    }

    key
}

fn parse_schematics(schematics: &[Vec<String>]) -> (Vec<Lock>, Vec<Key>) {
    let mut locks = vec![];
    let mut keys = vec![];

    for schematic in schematics {
        if schematic[0].starts_with('#') {
            locks.push(parse_lock(schematic));
        } else {
            keys.push(parse_key(schematic));
        }
    }

    (locks, keys)
}

fn key_fit_lock(key: &Key, lock: &Lock) -> bool {
    let mut fits = 0;

    for i in 0..5 {
        if key[i] + lock[i] <= 6 {
            fits += 1;
        }
    }

    fits == 5
}

fn task_one(input: &[String]) -> usize {
    let schematics: Vec<Vec<String>> = input.split(|line| line.is_empty()).map(|section| section.to_vec()).collect();
    let (locks, keys) = parse_schematics(&schematics);

    let mut fits = 0;
    for lock in &locks {
        for key in &keys {
            if key_fit_lock(key, lock) {
                fits += 1;
            }
        }
    }

    fits
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
