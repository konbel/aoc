fn parse_machine(input: &[String]) -> ((usize, usize), (usize, usize), (usize, usize)) {
    let mut vals = vec![];

    for line in input {
        let Some((v1, v2)) = line.split_once(',') else {
            return ((0, 0), (0, 0), (0, 0));
        };

        let n1: usize = v1
            .chars()
            .skip_while(|c| !c.is_digit(10))
            .take_while(|c| c.is_digit(10))
            .collect::<String>()
            .parse()
            .unwrap();

        let n2: usize = v2
            .chars()
            .skip_while(|c| !c.is_digit(10))
            .take_while(|c| c.is_digit(10))
            .collect::<String>()
            .parse()
            .unwrap();

        vals.push((n1, n2));
    }

    (vals[0], vals[1], vals[2])
}

fn parse_machines(input: &[String]) -> Vec<((usize, usize), (usize, usize), (usize, usize))> {
    let mut machine = vec![];
    let mut machines = vec![];

    for line in input {
        if line.is_empty() {
            machines.push(parse_machine(&machine));
            machine.clear();
        } else {
            machine.push(line.to_owned());
        }
    }

    machines.push(parse_machine(&machine));

    machines
}

fn det_2x2(matrix: &[Vec<usize>]) -> f64 {
    matrix[0][0] as f64 * matrix[1][1] as f64 - matrix[0][1] as f64 * matrix[1][0] as f64
}

fn try_get_whole(a: f64, b: f64) -> Option<(usize, usize)> {
    let epsilon = 1e-3;
    if (a - a.round()).abs() < epsilon && (b - b.round()).abs() < epsilon {
        Some((a.round() as usize, b.round() as usize))
    } else {
        None
    }
}

fn solve((v1, v2, p): ((usize, usize), (usize, usize), (usize, usize)), part2: bool) -> usize {
    let mut p = (p.0 as f64, p.1 as f64);

    if part2 {
        p.0 += 10000000000000.0;
        p.1 += 10000000000000.0;
    }

    let m = vec![vec![v1.0, v2.0], vec![v1.1, v2.1]];
    let det = det_2x2(&m);

    if det == 0.0 {
        return 0;
    }

    let m = vec![
        [(v2.1 as f64) / det, -(v2.0 as f64) / det],
        [-(v1.1 as f64) / det, (v1.0 as f64) / det],
    ];

    let a = m[0][0] * p.0 + m[0][1] * p.1;
    let b = m[1][0] * p.0 + m[1][1] * p.1;

    if let Some((i, j)) = try_get_whole(a, b) {
        if part2 || i <= 100 && j <= 100 {
            return 3 * i + j;
        }
    }

    0
}

fn task_one(input: &[String]) -> usize {
    parse_machines(input).iter().map(|m| solve(*m, false)).sum()
}

fn task_two(input: &[String]) -> usize {
    parse_machines(input).iter().map(|m| solve(*m, true)).sum()
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
