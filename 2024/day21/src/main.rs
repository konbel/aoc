use std::collections::{HashMap, VecDeque};
use itertools::Itertools;
use cached::proc_macro::cached;

type Coords = (isize, isize);

const NUMPAD: [[char; 3]; 4] = [
    ['7', '8', '9'],
    ['4', '5', '6'],
    ['1', '2', '3'],
    ['-', '0', 'A'],
];

const DIRPAD: [[char; 3]; 2] = [
    ['-', '^', 'A'],
    ['<', 'v', '>'],
];

fn calculate_sequences(keypad: &[[char; 3]]) -> HashMap<(char, char), Vec<String>> {
    let mut map: HashMap<char, Coords> = HashMap::new();
    for row in 0..keypad.len() {
        for col in 0..keypad[row].len() {
            if keypad[row][col] != '-' {
                map.insert(keypad[row][col], (row as isize, col as isize));
            }
        }
    }

    let mut sequences: HashMap<(char, char), Vec<String>> = HashMap::new();
    for x in map.keys() {
        for y in map.keys() {
            if *x == *y {
                sequences.insert((*x, *y), vec![String::from("A")]);
                continue;
            }

            let mut possibilities = vec![];
            let mut queue = VecDeque::new();
            queue.push_back((map[x], String::from("")));
            let mut optimal = usize::MAX;
            'outer: while let Some(((r, c), moves)) = queue.pop_front() {
                for (nr, nc, nm) in [(r - 1, c, '^'), (r + 1, c, 'v'), (r, c - 1, '<'), (r, c + 1, '>')] {
                    if nr < 0 || nc < 0 || nr >= keypad.len() as isize || nc >= keypad[nr as usize].len() as isize { continue; }
                    if keypad[nr as usize][nc as usize] == '-' { continue; }

                    if keypad[nr as usize][nc as usize] == *y {
                        if optimal < moves.len() + 1 {
                            break 'outer;
                        }
                        optimal = moves.len() + 1;
                        possibilities.push(format!("{}{}A", moves, nm));
                    } else {
                        queue.push_back(((nr, nc), format!("{}{}", moves, nm)));
                    }
                }
            }

            sequences.insert((*x, *y), possibilities);
        }
    }

    sequences
}


fn calculate_moves(string: &String, sequences: &HashMap<(char, char), Vec<String>>) -> Vec<String> {
    let mut options = vec![];

    for (x, y) in format!("A{}", string).chars().zip(string.chars()) {
        options.push(sequences[&(x, y)].clone());
    }

    let mut res = vec![];
    options.into_iter().multi_cartesian_product().for_each(|option| {
        res.push(option.join(""));
    });
    res
}

#[cached(
    key = "(String, usize)",
    convert = r#"{ (sequence.clone(), depth) }"#
)]
fn calculate_length(dir_sequences: &HashMap<(char, char), Vec<String>>, dir_lengths: &HashMap<(char, char), usize>, sequence: &String, depth: usize) -> usize {
    if depth == 1 {
        let mut sum = 0;
        for (a, b) in format!("A{}", sequence).chars().zip(sequence.chars()) {
            sum += dir_lengths[&(a, b)];
        }
        return sum;
    }

    let mut length = 0;
    for (a, b) in format!("A{}", sequence).chars().zip(sequence.chars()) {
        let mut optimal = usize::MAX;
        for subseq in dir_sequences[&(a, b)].iter() {
            optimal = optimal.min(calculate_length(dir_sequences, dir_lengths, subseq, depth - 1));
        }
        length += optimal;
    }

    length
}

fn task_one(input: &[String]) -> usize {
    let numpad_sequences = calculate_sequences(&NUMPAD);
    let dirpad_sequences = calculate_sequences(&DIRPAD);

    let mut dir_lengths = HashMap::new();
    for (&key, value) in &dirpad_sequences {
        dir_lengths.insert(key, value[0].len());
    }

    let mut complexity = 0;

    for code in input {
        let moves = calculate_moves(code, &numpad_sequences);
        let length = moves.into_iter().map(|seq| calculate_length(&dirpad_sequences, &dir_lengths, &seq, 2)).min().unwrap();

        let code_num = code.chars().filter(|c| c.is_numeric()).collect::<String>().parse::<usize>().unwrap_or(0);
        // println!("{}: {} * {}", code, length, code_num);
        complexity += length * code_num;
    }
    
    complexity
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
