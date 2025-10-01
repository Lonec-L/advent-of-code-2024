use std::process::exit;

#[derive(Clone)]
struct Node {
    pub parents: Vec<usize>,
    pub legal: bool,
}

impl Node {
    pub fn new() -> Self {
        Self {
            parents: Vec::new(),
            legal: true,
        }
    }
}

fn generator(lines: &Vec<&str>) -> (usize, Vec<Node>) {
    let mut g: Vec<Node> = vec![Node::new(); 100];
    let mut start = 0;

    for i in 0..lines.len() {
        if lines[i].is_empty() {
            start = i + 1;
            break;
        }
        let (a, b): (usize, usize) = match lines[i].split_once('|') {
            Some(x) => (
                x.0.parse().expect("failed to parse number"),
                x.1.parse().expect("Failed to parse number"),
            ),
            None => exit(1),
        };

        g[b].parents.push(a);
    }
    (start, g)
}

#[aoc(day5, part1)]
pub fn part1(input: &str) -> u32 {
    let mut res = 0;
    let lines: Vec<&str> = input.lines().collect();
    let (start, mut g) = generator(&lines);

    for i in start..lines.len() {
        for x in &mut g {
            x.legal = true;
        }
        let v: Vec<usize> = lines[i]
            .split(',')
            .map(|x| x.parse().expect("failes to parse int"))
            .collect();

        let mut good = true;

        for e in &v {
            if !g[*e].legal {
                good = false;
                break;
            }
            for j in 0..g[*e].parents.len() {
                let value = g[*e].parents[j];
                g[value].legal = false;
            }
        }

        if good {
            res += v[v.len() / 2] as u32;
        }
    }
    res
}

#[aoc(day5, part2)]
pub fn part2(input: &str) -> u32 {
    let mut res: u32 = 0;
    let lines: Vec<&str> = input.lines().collect();
    let (start, mut g) = generator(&lines);

    for i in start..lines.len() {
        for x in &mut g {
            x.legal = true;
        }
        let mut v: Vec<usize> = lines[i]
            .split(',')
            .map(|x| x.parse().expect("failes to parse int"))
            .collect();

        for e in &v {
            if !g[*e].legal {
                res += solve_incorrect(&mut v, &g);
                break;
            }
            for j in 0..g[*e].parents.len() {
                let value = g[*e].parents[j];
                g[value].legal = false;
            }
        }
    }
    res
}

fn solve_incorrect(v: &mut Vec<usize>, g: &Vec<Node>) -> u32 {
    let mut i = 0;
    while i < v.len() {
        for j in (i + 1)..v.len() {
            if g[v[i]].parents.contains(&v[j]) {
                v.swap(i, j);
                continue;
            }
        }
        i += 1;
    }
    v[v.len() / 2] as u32
}
