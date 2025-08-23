use std::collections::BinaryHeap;

#[derive(Copy, Clone, Eq, PartialEq)]
struct Tile {
    pos: (usize, usize),
    dir: (isize, isize),
    cost: usize,
}

impl Ord for Tile {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        other
            .cost
            .cmp(&self.cost)
            .then_with(|| self.pos.cmp(&other.pos))
    }
}

impl PartialOrd for Tile {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

#[derive(Clone)]
struct Info {
    cost: usize,
    prev: (usize, usize),
}

fn get_neighbours(
    map: &[Vec<char>],
    (y, x): (usize, usize),
) -> Vec<((usize, usize), (isize, isize))> {
    const DIRS: [(isize, isize); 4] = [(-1, 0), (0, 1), (1, 0), (0, -1)];
    let mut ns = vec![];

    for &(dy, dx) in &DIRS {
        let ny = (y as isize + dy) as usize;
        let nx = (x as isize + dx) as usize;
        if map[ny][nx] == '.' {
            ns.push(((ny, nx), (dy, dx)));
        }
    }

    ns
}

fn dir_to_idx(dir: (isize, isize)) -> usize {
    match dir {
        (-1, 0) => 0,
        (0, 1) => 1,
        (1, 0) => 2,
        (0, -1) => 3,
        _ => 0,
    }
}

fn calculate_cost(map: &[Vec<char>], start: (usize, usize)) -> Vec<Vec<Vec<Info>>> {
    // init
    let s = Tile {
        pos: start,
        dir: (0, 1),
        cost: 0,
    };

    let mut unvisited: BinaryHeap<Tile> = BinaryHeap::default();
    unvisited.push(s);

    let mut visited: Vec<Vec<Vec<Info>>> = vec![
        vec![
            vec![
                Info {
                    cost: usize::MAX,
                    prev: (0, 0)
                };
                4
            ];
            map[0].len()
        ];
        map.len()
    ];
    visited[s.pos.0][s.pos.1][dir_to_idx(s.dir)].cost = 0;

    // dijkstra
    while let Some(current) = unvisited.pop() {
        for (np, nd) in get_neighbours(&map, current.pos) {
            let nc;
            if nd == current.dir {
                nc = current.cost + 1;
            } else {
                nc = current.cost + 1001;
            }

            let n = Tile {
                pos: np,
                dir: nd,
                cost: nc,
            };

            let idx = dir_to_idx(n.dir);
            if n.cost < visited[n.pos.0][n.pos.1][idx].cost {
                visited[n.pos.0][n.pos.1][idx].cost = n.cost;
                visited[n.pos.0][n.pos.1][idx].prev = current.pos;
                unvisited.push(n);
            }
        }
    }

    return visited;
}

fn task_one(input: &[String]) -> usize {
    let mut map: Vec<Vec<_>> = input.iter().map(|line| line.chars().collect()).collect();

    let e = (1, map[1].len() - 2);
    let s = (map.len() - 2, 1);

    map[e.0][e.1] = '.';
    map[s.0][s.1] = '.';

    let costs = calculate_cost(&map, s);
    costs[e.0][e.1].iter().map(|i| i.cost).min().unwrap()
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
