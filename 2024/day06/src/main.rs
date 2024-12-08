use std::collections::HashSet;

fn rotate(dir: (isize, isize)) -> (isize, isize) {
    match dir {
        (-1, 0) => (0, 1),
        (0, 1) => (1, 0),
        (1, 0) => (0, -1),
        (0, -1) => (-1, 0),
        _ => unreachable!(),
    }
}

fn next(grid: &[Vec<u8>], pos: (usize, usize), dir: (isize, isize)) -> Option<(usize, usize)> {
    let Some(y) = pos.0.checked_add_signed(dir.0) else {
        return None;
    };

    let Some(x) = pos.1.checked_add_signed(dir.1) else {
        return None;
    };

    (y < grid.len() && x < grid[0].len()).then_some((y, x))
}

fn explore_map(
    grid: &[Vec<u8>],
    pos: (usize, usize),
    dir: (isize, isize),
    brick_pos: Option<(usize, usize)>,
) -> Option<HashSet<(usize, usize)>> {
    let mut seen: HashSet<((usize, usize), (isize, isize))> = HashSet::default();

    let mut pos = pos;
    let mut dir = dir;

    seen.insert((pos, dir));

    while let Some(next @ (y, x)) = next(grid, pos, dir) {
        match grid[y][x] {
            b'#' => {
                dir = rotate(dir);
            }
            _c if Some(next) == brick_pos => {
                dir = rotate(dir);
            }
            b'.' => {
                if !seen.insert((next, dir)) {
                    return None;
                }

                pos = next;
            }
            _ => unreachable!(),
        }
    }

    Some(
        seen.into_iter()
            .map(|(pos, _dir)| pos)
            .collect::<HashSet<_>>(),
    )
}

fn task_one(input: &[String]) -> usize {
    let mut grid: Vec<Vec<u8>> = input.iter().map(|line| line.as_bytes().to_vec()).collect();

    let pos = grid.iter().flatten().position(|c| *c == b'^').unwrap();

    let pos @ (y, x) = (pos / grid.len(), pos % grid.len());
    let dir = (-1, 0);
    grid[y][x] = b'.';

    explore_map(&grid, pos, dir, None).unwrap().len()
}

fn task_two(input: &[String]) -> usize {
    let mut grid: Vec<Vec<u8>> = input.iter().map(|line| line.as_bytes().to_vec()).collect();

    let pos = grid.iter().flatten().position(|c| *c == b'^').unwrap();

    let pos @ (y, x) = (pos / grid.len(), pos % grid.len());
    let dir = (-1, 0);
    grid[y][x] = b'.';

    let mut tiles = explore_map(&grid, pos, dir, None).unwrap();
    tiles.retain(|ppos| pos != *ppos);

    tiles
        .into_iter()
        .filter(|brick| explore_map(&grid, pos, dir, Some(*brick)).is_none())
        .count()
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
