use std::collections::{ HashMap, HashSet };

fn build_network(connections: &[String]) -> HashMap<String, HashSet<String>> {
    let mut network: HashMap<String, HashSet<String>> = HashMap::new();

    for connection in connections {
        let (first, second) = connection.split_once('-').unwrap();
        let first = first.to_owned();
        let second = second.to_owned();

        network.entry(first.clone()).and_modify(|c| { c.insert(second.clone()); }).or_insert(HashSet::from([second.clone()]));
        network.entry(second).and_modify(|c| { c.insert(first.clone()); }).or_insert(HashSet::from([first]));
    }

    network
}

fn find_groups(network: &HashMap<String, HashSet<String>>) -> HashSet<[String; 3]> {
    let mut groups = HashSet::new();

    for (computer, connections) in network {
        for connection1 in connections {
            for connection2 in connections {
                if connection1 == connection2 {
                    continue;
                }

                if network.get(connection1).unwrap().contains(connection2) {
                    let mut group = [computer.clone(), connection1.clone(), connection2.clone()];
                    group.sort();
                    groups.insert(group);
                }
            }
        }
    }

    groups
}

fn task_one(input: &[String]) -> usize {
    let network = build_network(input);
    let groups = find_groups(&network);

    groups.iter().filter(|group| {
        group.iter().any(|computer| computer.starts_with('t'))
    }).count()
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
