fn parse(input: &[String]) -> (Vec<Vec<char>>, Vec<char>) {
    let i: usize = input.iter().position(|line| line.is_empty()).unwrap();

    let map: Vec<Vec<_>> = input[..i]
        .iter()
        .map(|line| line.chars().collect())
        .collect();

    let moves: Vec<_> = input[(i + 1)..]
        .iter()
        .flat_map(|line| line.chars())
        .collect();

    (map, moves)
}

fn find_robot(map: &[Vec<char>]) -> (usize, usize) {
    for (y, row) in map.iter().enumerate() {
        for (x, c) in row.iter().enumerate() {
            if *c == '@' {
                return (y, x);
            }
        }
    }

    (0, 0)
}

fn simulate(map: &mut [Vec<char>], moves: &[char], robot: (usize, usize)) {
    let mut robot = robot;

    for m in moves {
        let dir = match m {
            '^' => (-1, 0),
            '>' => (0, 1),
            'v' => (1, 0),
            '<' => (0, -1),
            _ => (0, 0),
        };

        let mut updates = vec![robot];
        let mut tmp = robot;

        loop {
            let y = (tmp.0 as isize + dir.0) as usize;
            let x = (tmp.1 as isize + dir.1) as usize;

            let tile = map[y][x];

            if tile == '#' {
                updates.clear();
                break;
            } else if tile == 'O' {
                updates.push((y, x));
                tmp = (y, x);
            } else if tile == '.' {
                break;
            }
        }

        for u in updates.iter().skip(1) {
            let y = (u.0 as isize + dir.0) as usize;
            let x = (u.1 as isize + dir.1) as usize;

            map[y][x] = 'O';
        }

        if updates.len() > 0 {
            let y = (updates[0].0 as isize + dir.0) as usize;
            let x = (updates[0].1 as isize + dir.1) as usize;
            robot = (y, x);
            map[y][x] = '.';
        }
    }
}

fn task_one(input: &[String]) -> usize {
    let (mut map, moves) = parse(input);
    let robot = find_robot(&map);
    map[robot.0][robot.1] = '.';

    // simulate
    simulate(&mut map, &moves, robot);

    // sum
    let mut sum = 0;
    for (y, r) in map.iter().enumerate() {
        for (x, c) in r.iter().enumerate() {
            if *c == 'O' {
                sum += y * 100 + x;
            }
        }
    }

    sum
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
