fn get_blocks(input: &String) -> Vec<Option<usize>> {
    let mut blocks = vec![];

    let mut id = 0;

    for (i, c) in input.chars().enumerate() {
        let size: usize = c.to_string().parse().unwrap();

        if i % 2 == 0 {
            for _ in 0..size {
                blocks.push(Some(id));
            }

            id += 1;
        } else {
            for _ in 0..size {
                blocks.push(None);
            }
        }
    }

    blocks
}

fn calc_checksum(blocks: &[Option<usize>]) -> usize {
    blocks
        .iter()
        .filter(|v| v.is_some())
        .enumerate()
        .map(|(i, c)| i * c.unwrap())
        .sum()
}

fn task_one(input: &[String]) -> usize {
    let mut blocks = get_blocks(&input[0]);

    'outer: for i in 0..blocks.len() {
        let c = blocks[i];

        if c.is_none() {
            for j in (0..blocks.len()).rev() {
                let s = blocks[j];

                if let Some(s) = s {
                    blocks[i] = Some(s);
                    blocks[j] = None;

                    if blocks[(i + 1)..blocks.len()].iter().all(|v| v.is_none()) {
                        break 'outer;
                    }

                    break;
                }
            }
        }
    }

    calc_checksum(&blocks)
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
