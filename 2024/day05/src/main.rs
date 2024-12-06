use std::collections::HashMap;
use std::collections::HashSet;

fn get_rules(input: &[String]) -> HashMap<usize, HashSet<usize>> {
    let mut rules = HashMap::<usize, HashSet<usize>>::new();

    let lines = input.iter().take_while(|line| !line.is_empty());

    for line in lines {
        let (first, second) = line.split_once('|').unwrap();

        let key: usize = first.parse().unwrap();
        let value: usize = second.parse().unwrap();

        if rules.contains_key(&key) {
            rules.get_mut(&key).unwrap().insert(value);
        } else {
            rules.insert(key, HashSet::from([value]));
        }
    }

    rules
}

fn get_pages_list(input: &[String]) -> Vec<Vec<usize>> {
    input
        .iter()
        .rev()
        .take_while(|line| !line.is_empty())
        .map(|line| {
            line.split(',')
                .map(|p| p.parse::<usize>().unwrap())
                .collect::<Vec<usize>>()
        })
        .collect()
}

fn is_valid(rules: &HashMap<usize, HashSet<usize>>, pages: &Vec<usize>) -> bool {
    let mut valid = true;

    for (i, page) in pages.iter().enumerate() {
        if rules.contains_key(&page) {
            let after = rules.get(&page).unwrap();

            for &cur in after {
                valid = !pages[0..i].iter().any(|&v| v == cur);

                if !valid {
                    break;
                }
            }

            if !valid {
                break;
            }
        }
    }

    valid
}

fn sort(rules: &HashMap<usize, HashSet<usize>>, pages: &Vec<usize>) -> Vec<usize> {
    let mut pages = pages.clone();

    loop {
        let mut changed = 0;

        for i in 0..pages.len() {
            let page = pages[i];

            if rules.contains_key(&page) {
                let after = rules.get(&page).unwrap();

                let mut skip = false;

                for &cur in after {
                    for j in 0..i {
                        let page2 = pages[j];

                        if page2 == cur {
                            let temp = pages[i];
                            pages[i] = page2;
                            pages[j] = temp;

                            skip = true;
                            changed += 1;
                            break;
                        }
                    }

                    if skip {
                        break;
                    }
                }
            }
        }

        if changed == 0 {
            break;
        }
    }

    pages
}

fn task_one(input: &[String]) -> usize {
    let rules = get_rules(input);

    get_pages_list(input)
        .iter()
        .filter(|pages| is_valid(&rules, pages))
        .map(|pages| pages[pages.len() / 2])
        .sum()
}

fn task_two(input: &[String]) -> usize {
    let rules = get_rules(input);

    get_pages_list(input)
        .iter()
        .filter(|pages| !is_valid(&rules, pages))
        .map(|pages| sort(&rules, pages))
        .map(|pages| pages[pages.len() / 2])
        .sum()
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
