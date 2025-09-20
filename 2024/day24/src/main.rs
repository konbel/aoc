use std::collections::{HashMap, HashSet};
use std::hash::Hash;

#[derive(Clone, PartialEq)]
enum Operation {
    None,
    And,
    Or,
    Xor,
}

#[derive(Clone)]
struct Instruction {
    input1: String,
    input2: String,
    output: String,
    operation: Operation,
}

fn get_initial_wire_state(input: &[String]) -> HashMap<String, Option<usize>> {
    let mut wires = HashMap::new();
    for wire in input.iter() {
        let (name, value) = wire.split_once(": ").unwrap();
        wires.insert(name.to_owned(), Some(value.parse::<usize>().unwrap()));
    }
    wires
}

fn get_instructions(
    input: &[String],
    wires: &mut HashMap<String, Option<usize>>,
) -> Vec<Instruction> {
    let mut instructions: Vec<Instruction> = vec![];

    for instruction in input.iter() {
        // parse
        let (input, output) = instruction.split_once(" -> ").unwrap();

        let output = output.to_owned();
        wires.insert(output.clone(), None);

        let mut parts = input.split(' ');
        let input1 = parts.next().unwrap().to_owned();
        let operation = match parts.next().unwrap() {
            "AND" => Operation::And,
            "OR" => Operation::Or,
            "XOR" => Operation::Xor,
            _ => Operation::None,
        };
        let input2 = parts.next().unwrap().to_owned();

        // insert if not present
        if !wires.contains_key(&input1) {
            wires.insert(input1.clone(), None);
        }

        if !wires.contains_key(&input2) {
            wires.insert(input2.clone(), None);
        }

        // create instruction
        instructions.push(Instruction {
            input1,
            input2,
            output,
            operation,
        });
    }

    instructions
}

fn handle_instructions(
    wires: &mut HashMap<String, Option<usize>>,
    instructions: &mut Vec<Instruction>,
) {
    while !instructions.is_empty() {
        instructions.retain(|instruction| {
            let input1 = wires.get(&instruction.input1).unwrap();
            let input2 = wires.get(&instruction.input2).unwrap();

            if input1.is_none() || input2.is_none() {
                return true;
            }

            let input1 = input1.unwrap();
            let input2 = input2.unwrap();

            let res: usize = match instruction.operation {
                Operation::And => {
                    if input1 == 1 && input2 == 1 {
                        1
                    } else {
                        0
                    }
                }
                Operation::Or => {
                    if input1 == 1 || input2 == 1 {
                        1
                    } else {
                        0
                    }
                }
                Operation::Xor => {
                    if input1 != input2 {
                        1
                    } else {
                        0
                    }
                }
                _ => panic!(),
            };

            wires
                .entry(instruction.output.clone())
                .and_modify(|v| *v = Some(res));

            return false;
        });
    }
}

fn task_one(input: &[String]) -> usize {
    let split_idx = input.iter().position(|s| s == &"").unwrap();
    let mut wires = get_initial_wire_state(&input[..split_idx]);
    let mut instructions = get_instructions(&input[(split_idx + 1)..], &mut wires);

    handle_instructions(&mut wires, &mut instructions);

    let mut output_wires = wires
        .into_iter()
        .filter(|(name, _)| name.starts_with('z'))
        .collect::<Vec<(String, Option<usize>)>>();
    output_wires.sort();
    output_wires.reverse();

    let binary = &output_wires
        .iter()
        .map(|wire| wire.1.unwrap().to_string())
        .collect::<String>();
    usize::from_str_radix(binary.as_str(), 2).unwrap()
}

fn make_wire(c: char, num: usize) -> String {
    format!("{}{:0>2}", c, num)
}

fn verify_z(instructions: &HashMap<String, Instruction>, wire: &str, num: usize) -> bool {
    if let Some(instruction) = instructions.get(wire) {
        if instruction.operation != Operation::Xor {
            return false;
        }
        if num == 0 {
            let lhs: HashSet<_> = vec![instruction.input1.to_owned(), instruction.input2.to_owned()]
                .into_iter()
                .collect();
            let rhs: HashSet<_> = vec!["x00".to_string(), "y00".to_string()]
                .into_iter()
                .collect();
            return lhs == rhs;
        }
        (verify_intermediate_xor(instructions, &instruction.input1, num)
            && verify_carry_bit(instructions, &instruction.input2, num))
            || (verify_intermediate_xor(instructions, &instruction.input2, num)
                && verify_carry_bit(instructions, &instruction.input1, num))
    } else {
        false
    }
}

fn verify_intermediate_xor(
    instructions: &HashMap<String, Instruction>,
    wire: &str,
    num: usize,
) -> bool {
    if let Some(instruction) = instructions.get(wire) {
        if instruction.operation != Operation::Xor {
            return false;
        }
        let lhs: HashSet<_> = vec![instruction.input1.to_owned(), instruction.input2.to_owned()]
            .into_iter()
            .collect();
        let rhs: HashSet<_> = vec![make_wire('x', num), make_wire('y', num)]
            .into_iter()
            .collect();
        lhs == rhs
    } else {
        false
    }
}

fn verify_carry_bit(instructions: &HashMap<String, Instruction>, wire: &str, num: usize) -> bool {
    if let Some(instruction) = instructions.get(wire) {
        if num == 1 {
            if instruction.operation != Operation::And {
                return false;
            }
            let lhs: HashSet<_> = vec![instruction.input1.to_owned(), instruction.input2.to_owned()]
                .into_iter()
                .collect();
            let rhs: HashSet<_> = vec!["x00".to_string(), "y00".to_string()]
                .into_iter()
                .collect();
            return lhs == rhs;
        }
        if instruction.operation != Operation::Or {
            return false;
        }
        (verify_direct_carry(instructions, &instruction.input1, num - 1)
            && verify_recarry(instructions, &instruction.input2, num - 1))
            || (verify_direct_carry(instructions, &instruction.input2, num - 1)
                && verify_recarry(instructions, &instruction.input1, num - 1))
    } else {
        false
    }
}

fn verify_direct_carry(instructions: &HashMap<String, Instruction>, wire: &str, num: usize) -> bool {
    if let Some(instruction) = instructions.get(wire) {
        if instruction.operation != Operation::And {
            return false;
        }
        let lhs: HashSet<_> = vec![instruction.input1.to_owned(), instruction.input2.to_owned()]
            .into_iter()
            .collect();
        let rhs: HashSet<_> = vec![make_wire('x', num), make_wire('y', num)]
            .into_iter()
            .collect();
        lhs == rhs
    } else {
        false
    }
}

fn verify_recarry(instructions: &HashMap<String, Instruction>, wire: &str, num: usize) -> bool {
    if let Some(instruction) = instructions.get(wire) {
        if instruction.operation != Operation::And {
            return false;
        }
        (verify_intermediate_xor(instructions, &instruction.input1, num)
            && verify_carry_bit(instructions, &instruction.input2, num))
            || (verify_intermediate_xor(instructions, &instruction.input2, num)
                && verify_carry_bit(instructions, &instruction.input1, num))
    } else {
        false
    }
}

fn verify(instructions: &HashMap<String, Instruction>, num: usize) -> bool {
    verify_z(instructions, &make_wire('z', num), num)
}

fn progress(instructions: &HashMap<String, Instruction>) -> usize {
    let mut i = 0;
    while verify(instructions, i) {
        i += 1;
    }
    i
}

fn task_two(input: &[String]) -> usize {
    let split_idx = input.iter().position(|s| s == &"").unwrap();
    let mut instruction_lines = vec![];
    for line in &input[(split_idx + 1)..] {
        instruction_lines.push(line.to_owned());
    }

    let mut instructions = HashMap::new();
    for line in instruction_lines {
        let line = line.replace(" -> ", " ");
        let parts: Vec<&str> = line.split_whitespace().collect();
        let (x, op, y, z) = (parts[0], parts[1], parts[2], parts[3]);
        let operation = match op {
            "XOR" => Operation::Xor,
            "AND" => Operation::And,
            "OR" => Operation::Or,
            _ => Operation::None,
        };
        instructions.insert(
            z.to_string(),
            Instruction {
                input1: x.to_string(),
                input2: y.to_string(),
                output: z.to_string(),
                operation,
            },
        );
    }

    let mut swaps = Vec::new();

    for _ in 0..4 {
        let baseline = progress(&instructions);
        'outer: for x in instructions.clone().keys() {
            for y in instructions.clone().keys() {
                if x == y {
                    continue;
                }
                instructions.swap(x, y);
                if progress(&instructions) > baseline {
                    swaps.push(x.clone());
                    swaps.push(y.clone());
                    break 'outer;
                }
                instructions.swap(x, y);
            }
        }
    }

    swaps.sort();
    println!("{}", swaps.join(","));

    0
}

trait SwapKeys<K, V> {
    fn swap(&mut self, key1: &K, key2: &K);
}

impl<K: Eq + Hash + Clone, V: Clone> SwapKeys<K, V> for HashMap<K, V> {
    fn swap(&mut self, key1: &K, key2: &K) {
        if let (Some(v1), Some(v2)) = (self.get(key1).cloned(), self.get(key2).cloned()) {
            self.insert(key1.clone(), v2);
            self.insert(key2.clone(), v1);
        }
    }
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
