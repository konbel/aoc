use std::collections::{ VecDeque, HashSet };

fn create_map(input: &[String], max_coords: usize, num_bytes: usize) -> Vec<Vec<bool>> {
    let mut map: Vec<Vec<bool>> = vec![vec![false; max_coords + 1]; max_coords + 1];

    for (i, line) in input.iter().enumerate() {
        if i == num_bytes {
            break;
        }

        let mut iter = line.split(",");
        let x = iter.next().unwrap().parse::<usize>().unwrap();
        let y = iter.next().unwrap().parse::<usize>().unwrap();

        map[y][x] = true;
    }

    map
}

fn find_min_steps(map: &[Vec<bool>], start: (usize, usize), end: (usize, usize)) -> usize {
    // find best path
    let mut queue: VecDeque<((usize, usize), usize)> = VecDeque::new();
    queue.push_back((start, 0));

    let mut seen: HashSet<(usize, usize)> = HashSet::new();

    while let Some((pos, steps)) = queue.pop_front() {
        seen.insert(pos);

        if pos == end {
            return steps;
        }

        // get neighbors
        const DIRS: [(isize, isize); 4] = [(0, -1), (1, 0), (0, 1), (-1, 0)];
        for (dx, dy) in DIRS {
            let x = pos.0 as isize + dx;
            let y = pos.1 as isize + dy;

            // check boundaries
            if x < 0 || x > end.0 as isize || y < 0 || y > end.1 as isize {
                continue;
            }

            let x = x as usize;
            let y = y as usize;

            // check if already seen
            if !seen.insert((x, y)) {
                continue;
            }

            // check if corrupted field
            if map[y][x] {
                continue;
            }

            queue.push_back(((x, y), steps + 1));
        }
    }

    // no path found
    0
}

fn task_one(input: &[String]) -> usize {
    const MAX_COORDS: usize = 70;
    const NUM_BYTES: usize = 1024;
    let map = create_map(input, MAX_COORDS, NUM_BYTES);
    find_min_steps(&map, (0, 0), (MAX_COORDS, MAX_COORDS))
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
        .unwrap_or_else(|| "../src/input".to_string())
}
