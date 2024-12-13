fn get_blocks(input: &[String]) -> Vec<isize> {
    let mut iter = input[0].chars();
    let mut vec = vec![];

    let mut i = 0;
    loop {
        let Some(n) = iter.next() else { break };
        let n = n.to_digit(10).unwrap();
        vec.append(&mut (0..n).map(|_| i).collect());

        i += 1;

        let Some(n) = iter.next() else { break };
        let n = n.to_digit(10).unwrap();
        vec.append(&mut (0..n).map(|_| -1).collect());
    }

    vec
}

fn calc_checksum(blocks: &[isize]) -> usize {
    blocks
        .iter()
        .enumerate()
        .filter(|(_i, v)| **v >= 0)
        .map(|(i, &c)| i * c as usize)
        .sum()
}

fn task_one(input: &[String]) -> usize {
    let mut blocks = get_blocks(input);

    let mut i = 0;
    let mut j = blocks.len() - 1;

    loop {
        while i < blocks.len() {
            if blocks[i] == -1 {
                break;
            }

            i += 1;
        }

        loop {
            if blocks[j] != -1 {
                break;
            }

            j -= 1;
        }

        if i >= j {
            break;
        }

        let temp = blocks[i];
        blocks[i] = blocks[j];
        blocks[j] = temp;
    }

    calc_checksum(&blocks)
}

fn find_sections(blocks: &[isize]) -> Vec<(usize, usize)> {
    let mut sections = vec![];

    let mut s = 0;
    let mut e = 0;

    loop {
        while e < blocks.len() && blocks[s] == blocks[e] {
            e += 1;
        }

        sections.push((s, e - 1));
        s = e;

        if e == blocks.len() {
            break;
        }

        while blocks[s] == -1 {
            s += 1;
        }

        e = s;
    }

    sections
}

fn find_empty_sections(blocks: &[isize]) -> Vec<(usize, usize)> {
    let mut start = 0;
    let mut empty = vec![];

    loop {
        let next = find_next_empty_section(start, &blocks);

        if next.0 > next.1 {
            break;
        }

        empty.push(next);
        start = next.1;
    }

    empty
}

fn find_next_empty_section(start: usize, blocks: &[isize]) -> (usize, usize) {
    let mut ns = start + 1;

    while ns < blocks.len() && blocks[ns] != -1 {
        ns += 1;
    }

    let mut ne = ns;
    while ne < blocks.len() && blocks[ne] == -1 {
        ne += 1;
    }

    (ns, ne - 1)
}

fn task_two(input: &[String]) -> usize {
    let mut blocks = get_blocks(input);

    let mut empty = find_empty_sections(&blocks);
    let mut sections = find_sections(&blocks);

    while let Some(file) = sections.pop() {
        if let Some(index) = empty.iter().position(|e| (e.1 - e.0) >= (file.1 - file.0)) {
            let spot = empty[index];
            if spot.1 > file.0 {
                continue;
            }

            let id = blocks[file.0];
            for (fi, i) in (file.0..=file.1).zip(spot.0..=spot.1) {
                blocks[i] = id;
                blocks[fi] = -1;
            }

            empty = find_empty_sections(&blocks);
        }
    }

    calc_checksum(&blocks)
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
