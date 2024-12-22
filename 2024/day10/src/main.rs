use std::collections::HashSet;

fn get_grid(input: &[String]) -> Vec<Vec<usize>> {
    input
        .iter()
        .map(|line| {
            line.chars()
                .map(|c| c.to_string().parse::<usize>().unwrap())
                .collect()
        })
        .collect::<Vec<Vec<usize>>>()
}

fn get_trail_heads(grid: &[Vec<usize>]) -> Vec<(usize, usize)> {
    let mut trail_heads = vec![];

    for y in 0..grid.len() {
        for x in 0..grid[0].len() {
            if grid[y][x] == 0 {
                trail_heads.push((y, x));
            }
        }
    }

    trail_heads
}

fn calculate_score(grid: &[Vec<usize>], trail_head: (usize, usize)) -> usize {
    let dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)];
    let mut queue: Vec<(isize, isize, usize)> = vec![];
    let mut score = 0;
    let mut seen: HashSet<(usize, usize)> = HashSet::default();

    queue.push((trail_head.0 as isize, trail_head.1 as isize, 0));

    while !queue.is_empty() {
        if let Some(current) = queue.pop() {
            for d in dirs {
                let y = current.0 + d.0;
                let x = current.1 + d.1;
                let num = current.2 + 1;

                if y >= 0 && y < grid.len() as isize && x >= 0 && x < grid[0].len() as isize {
                    let v = grid[y as usize][x as usize];

                    if v == num {
                        if seen.insert((y as usize, x as usize)) {
                            if v == 9 {
                                score += 1;
                                continue;
                            }

                            queue.push((y, x, num));
                        }
                    }
                }
            }
        }
    }

    score
}

fn calculate_score2(grid: &[Vec<usize>], trail_head: (usize, usize)) -> usize {
    let dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)];
    let mut queue: Vec<(isize, isize, usize)> = vec![];
    let mut score = 0;

    queue.push((trail_head.0 as isize, trail_head.1 as isize, 0));

    while !queue.is_empty() {
        if let Some(current) = queue.pop() {
            for d in dirs {
                let y = current.0 + d.0;
                let x = current.1 + d.1;
                let num = current.2 + 1;

                if y >= 0 && y < grid.len() as isize && x >= 0 && x < grid[0].len() as isize {
                    let v = grid[y as usize][x as usize];

                    if v == num {
                        if v == 9 {
                            score += 1;
                            continue;
                        }

                        queue.push((y, x, num));
                    }
                }
            }
        }
    }

    score
}

fn task_one(input: &[String]) -> usize {
    let grid = get_grid(input);
    let trail_heads = get_trail_heads(&grid);

    trail_heads
        .iter()
        .map(|&head| calculate_score(&grid, head))
        .sum()
}

fn task_two(input: &[String]) -> usize {
    let grid = get_grid(input);
    let trail_heads = get_trail_heads(&grid);

    trail_heads
        .iter()
        .map(|&head| calculate_score2(&grid, head))
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
