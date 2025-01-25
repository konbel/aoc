use std::collections::{HashSet, VecDeque};

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

fn expand(map: &[Vec<char>]) -> Vec<Vec<char>> {
    let mut new = vec![];

    for r in map {
        let mut row = vec![];

        for &c in r {
            if c == '#' {
                row.push('#');
                row.push('#');
            } else if c == '.' {
                row.push('.');
                row.push('.');
            } else if c == 'O' {
                row.push('[');
                row.push(']');
            } else if c == '@' {
                row.push('@');
                row.push('.');
            }
        }

        new.push(row);
    }

    new
}

fn simulate2(map: &mut [Vec<char>], moves: &[char], robot: (usize, usize)) {
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
        let mut queue = VecDeque::from([robot]);

        if dir == (0, 1) || dir == (0, -1) {
            while let Some(pos) = queue.pop_front() {
                let y = (pos.0 as isize + dir.0) as usize;
                let x = (pos.1 as isize + dir.1) as usize;

                let tile = map[y][x];

                if tile == '#' {
                    updates.clear();
                    break;
                } else if tile == '[' || tile == ']' {
                    updates.push((y, x));
                    queue.push_back((y, x));
                }
            }
        } else {
            let mut seen = HashSet::from([robot]);

            while let Some(pos) = queue.pop_front() {
                let y = (pos.0 as isize + dir.0) as usize;
                let x = (pos.1 as isize + dir.1) as usize;

                let tile = map[y][x];

                if tile == '#' {
                    updates.clear();
                    break;
                } else if tile == '[' || tile == ']' {
                    if seen.insert((y, x)) {
                        updates.push((y, x));
                        queue.push_back((y, x));

                        let nx;

                        if tile == '[' {
                            nx = x + 1;
                        } else {
                            nx = x - 1;
                        }

                        if seen.insert((y, nx)) {
                            updates.push((y, nx));
                            queue.push_back((y, nx));
                        }
                    }
                }
            }
        }

        for u in updates.iter().skip(1).rev() {
            let y = (u.0 as isize + dir.0) as usize;
            let x = (u.1 as isize + dir.1) as usize;

            let old = map[u.0][u.1];
            map[y][x] = old;
            map[u.0][u.1] = '.';
        }

        if updates.len() > 0 {
            let y = (updates[0].0 as isize + dir.0) as usize;
            let x = (updates[0].1 as isize + dir.1) as usize;
            robot = (y, x);
            map[y][x] = '.';
        }
    }
}

fn gps(map: &[Vec<char>], v: char) -> usize {
    let mut sum = 0;

    for (y, r) in map.iter().enumerate() {
        for (x, c) in r.iter().enumerate() {
            if *c == v {
                sum += y * 100 + x;
            }
        }
    }

    sum
}

fn task_one(input: &[String]) -> usize {
    let (mut map, moves) = parse(input);
    let robot = find_robot(&map);
    map[robot.0][robot.1] = '.';

    simulate(&mut map, &moves, robot);

    gps(&map, 'O')
}

fn task_two(input: &[String]) -> usize {
    let (map, moves) = parse(input);
    let mut map = expand(&map);
    let robot = find_robot(&map);
    map[robot.0][robot.1] = '.';

    simulate2(&mut map, &moves, robot);

    gps(&map, '[')
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
