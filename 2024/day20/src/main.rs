use std::collections::HashMap;

const DIRS: [(isize, isize); 4] = [(-1, 0), (0, 1), (1, 0), (0, -1)];

fn get_neighbors(map: &[Vec<char>], (y, x): (usize, usize)) -> Vec<(usize, usize)> {
    let mut neighbors = vec![];

    for (dy, dx) in DIRS {
        let (ny, nx) = ((y as isize + dy) as usize, (x as isize + dx) as usize);

        if ny < map.len() && nx < map[0].len() {
            neighbors.push((ny, nx));
        }
    }

    neighbors
}

fn get_start_end(map: &[Vec<char>]) -> ((usize, usize), (usize, usize)) {
    let mut  s = (0, 0);
    let mut e = (0, 0);

    for y in 0..map.len() {
        for x in 0..map[y].len() {
            if map[y][x] == 'S' {
                s = (y, x);
            } else if map[y][x] == 'E' {
                e = (y, x);
            }
        }
    }

    (s, e)
}

fn get_path(map: &[Vec<char>], start: (usize, usize), end: (usize, usize)) -> HashMap<(usize, usize), usize> {
    let mut path: HashMap<(usize, usize), usize> = HashMap::default();
    path.insert(start, 0);

    let mut stack: Vec<(usize, usize)> = vec![start];
    let mut time = 0;

    while let Some(current) = stack.pop() {
        if current == end {
            break;
        }

        time += 1;
        for n in get_neighbors(&map, current) {
            if map[n.0][n.1] == '#' {
                continue;
            }

            if !path.contains_key(&n) {
                stack.push(n);
                path.insert(n, time);
                break;
            }
        }
    }

    path
}

fn get_possible_cheats_in_pos(map: &[Vec<char>], current: (usize, usize)) -> Vec<((usize, usize), usize)> {
    let mut cheat_tiles = vec![];

    for neighbor in get_neighbors(map, current) {
        // up to two picoseconds means exactly two in this context, so this is not needed
        // insert direct neighbor if not wall tile
        // if map[neighbor.0][neighbor.1] != '#' {
        //     cheat_tiles.push((neighbor, 1_usize));
        // }

        // insert neighbor of neighbor if not wall tile
        for new_pos in get_neighbors(map, neighbor) {
            if new_pos == current {
                continue;
            }

            if map[new_pos.0][new_pos.1] != '#' {
                cheat_tiles.push((new_pos, 2_usize));
            }
        }
    }

    cheat_tiles
}

fn get_all_cheats(map: &[Vec<char>], path: &HashMap<(usize, usize), usize>) -> HashMap<((usize, usize), (usize, usize)), usize> {
    let mut cheats: HashMap<((usize, usize), (usize, usize)), usize> = HashMap::default();

    for current in path {
        for cheat in get_possible_cheats_in_pos(&map, *current.0) {
            let cheat_time = current.1 + cheat.1;
            let path_time = path[&cheat.0];

            if cheat_time < path_time {
                let time_save = path_time - cheat_time;
                let cheat_path = (*current.0, cheat.0);
                cheats.entry(cheat_path).or_insert(time_save);
            }
        }
    }

    cheats
}

fn task_one(input: &[String]) -> usize {
    let map = input.iter().map(|s| s.chars().collect()).collect::<Vec<Vec<char>>>();
    let (start, end) = get_start_end(&map);
    
    let path = get_path(&map, start, end);
    let cheats = get_all_cheats(&map, &path);

    cheats.values().filter(|time_save| time_save >= &&100).count()
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
        .unwrap_or_else(|| "../src/test".to_string())
}
