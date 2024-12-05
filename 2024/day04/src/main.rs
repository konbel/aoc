fn next(grid: &Vec<Vec<char>>, pos: (usize, usize), dir: (isize, isize)) -> Option<(usize, usize)> {
    let y_max = grid.len();
    let x_max = grid[0].len();

    let next = (pos.0 as isize + dir.0, pos.1 as isize + dir.1);

    if next.0 >= 0 && next.0 < y_max as isize && next.1 >= 0 && next.1 < x_max as isize {
        Some((next.0 as usize, next.1 as usize))
    } else {
        None
    }
}

fn check(grid: &Vec<Vec<char>>, pos: (usize, usize), dir: (isize, isize)) -> bool {
    let mut pos = pos;

    for ch in ['M', 'A', 'S'] {
        let Some(new) = next(grid, pos, dir) else {
            return false;
        };

        if grid[new.0][new.1] != ch {
            return false;
        }

        pos = new;
    }

    true
}

fn check2(grid: &Vec<Vec<char>>, x: usize, y: usize) -> bool {
    let mut s = String::with_capacity(3);
    s.push(grid[y + 1][x + 1]);
    s.push(grid[y][x]);
    s.push(grid[y - 1][x - 1]);

    if s != "MAS" && s != "SAM" {
        return false;
    }

    s.clear();
    s.push(grid[y + 1][x - 1]);
    s.push(grid[y][x]);
    s.push(grid[y - 1][x + 1]);

    if s != "MAS" && s != "SAM" {
        return false;
    }

    true
}

fn task_one(input: &[String]) -> usize {
    let grid: Vec<Vec<char>> = input.iter().map(|line| line.chars().collect()).collect();

    let xs: Vec<_> = grid
        .iter()
        .enumerate()
        .flat_map(|(y, row)| {
            row.iter()
                .enumerate()
                .filter_map(move |(x, &c)| (c == 'X').then_some((y, x)))
        })
        .collect();

    let dirs = [
        (0, 1),
        (1, 1),
        (1, 0),
        (1, -1),
        (0, -1),
        (-1, -1),
        (-1, 0),
        (-1, 1),
    ];

    let mut sum = 0;

    for x in xs {
        sum += dirs.iter().filter(|dir| check(&grid, x, **dir)).count();
    }

    sum
}

fn task_two(input: &[String]) -> usize {
    let grid: Vec<Vec<char>> = input.iter().map(|line| line.chars().collect()).collect();

    let xs: Vec<_> = grid
        .iter()
        .enumerate()
        .flat_map(|(y, row)| {
            row.iter()
                .enumerate()
                .filter_map(move |(x, &c)| (c == 'A').then_some((y, x)))
        })
        .collect();

    let mut sum = 0;

    for a in xs {
        if a.0 == 0 || a.0 == grid.len() - 1 || a.1 == 0 || a.1 == grid.len() - 1 {
            continue;
        }

        if check2(&grid, a.1, a.0) {
            sum += 1;
        }
    }

    sum
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
