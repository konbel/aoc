fn parse_input(input: &[String]) -> (Vec<usize>, Vec<usize>) {
    let registers: Vec<usize> = input[0..3].iter().map(|s| s.split(' ').last().unwrap().parse::<usize>().unwrap()).collect();
    let program: Vec<usize> = input.iter().last().unwrap()[9..].split(',').map(|c| c.parse::<usize>().unwrap()).collect();
    (registers, program)
}

fn get_combo_value(registers: &[usize], operand: usize) -> usize {
    match operand {
        0..=3 => operand,
        4..=6 => registers[operand - 4],
        _ => unreachable!(),
    }
}

fn run_program(registers: &mut [usize], program: &[usize]) -> Vec<usize> {
    let mut output: Vec<usize> = vec![];

    let mut pointer = 0;
    while pointer < program.len() - 1 {
        let opcode = program[pointer];
        let operand = program[pointer + 1];

        if opcode == 0 { // adv
            registers[0] = registers[0] / 2usize.pow(get_combo_value(&registers, operand) as u32);
            pointer += 2;
        } else if opcode == 1 { // bxl
            registers[1] = registers[1] ^ operand;
            pointer += 2;
        } else if opcode == 2 { // bst
            registers[1] = get_combo_value(&registers, operand) % 8;
            pointer += 2;
        } else if opcode == 3 { // jnz
            if registers[0] != 0 {
                pointer = operand;
            } else {
                pointer += 2;
            }
        } else if opcode == 4 { // bxc
            registers[1] = registers[1] ^ registers[2];
            pointer += 2;
        } else if opcode == 5 { // out
            output.push(get_combo_value(&registers, operand) % 8);
            pointer += 2;
        } else if opcode == 6 { // bdv
            registers[1] = registers[0] / 2usize.pow(get_combo_value(&registers, operand) as u32);
            pointer += 2;
        } else if opcode == 7 { // cdv
            registers[2] = registers[0] / 2usize.pow(get_combo_value(&registers, operand) as u32);
            pointer += 2;
        }
    }

    output
}

fn task_one(input: &[String]) -> usize {
    let (mut registers, program) = parse_input(input);
    println!("{}", run_program(&mut registers, &program).iter().map(|o| o.to_string()).collect::<Vec<String>>().join(","));
    0
}

fn task_two(input: &[String]) -> usize {
    let (mut registers, program) = parse_input(input);

    let mut candidates = vec![0];
    for l in 0..program.len() {
        let mut next_candidates = vec![];
        for val in candidates {
            for i in 0..8usize {
                let target = (val << 3) + i;
                registers[0] = target;
                registers[1] = 0;
                registers[2] = 0;
                if run_program(&mut registers, &program) == program[(program.len() - l - 1)..] {
                    next_candidates.push(target);
                }
            }
        }
        candidates = next_candidates;
    }

    *candidates.iter().min().unwrap()
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
        .unwrap_or_else(|| "../src/input".to_string())
}
