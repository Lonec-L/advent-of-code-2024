use std::collections::{HashMap, HashSet};

#[aoc(day8, part1)]
pub fn part1(input: &str) -> usize {
    let data: Vec<Vec<char>> = input.lines().map(|e| e.chars().collect()).collect();
    let mut m: HashMap<char, Vec<(i64, i64)>> = HashMap::new();
    for i in 0..data.len() {
        for j in 0..data[i].len() {
            if data[i][j] != '.' {
                match m.get_mut(&data[i][j]) {
                    Some(v) => v.push((i as i64, j as i64)),
                    None => {
                        m.insert(data[i][j], vec![(i as i64, j as i64)]);
                    }
                }
            }
        }
    }

    let mut s: HashSet<(i64, i64)> = HashSet::new();

    for key in m.keys() {
        match m.get(key) {
            Some(v) => {
                for i in 0..v.len() {
                    for j in (i + 1)..v.len() {
                        let dx: i64 = v[i].0 - v[j].0;
                        let dy: i64 = v[i].1 - v[j].1;

                        let (x1, y1) = (v[i].0 + dx, v[i].1 + dy);
                        let (x2, y2) = (v[j].0 - dx, v[j].1 - dy);

                        if x1 >= 0 && x1 < data.len() as i64 && y1 >= 0 && y1 < data[0].len() as i64
                        {
                            s.insert((x1, y1));
                        }

                        if x2 >= 0 && x2 < data.len() as i64 && y2 >= 0 && y2 < data[0].len() as i64
                        {
                            s.insert((x2, y2));
                        }
                    }
                }
            }
            None => panic!("ERROR, no value in map"),
        }
    }
    s.iter().count()
}

#[aoc(day8, part2)]
pub fn part2(input: &str) -> usize {
    let data: Vec<Vec<char>> = input.lines().map(|e| e.chars().collect()).collect();
    let mut m: HashMap<char, Vec<(i64, i64)>> = HashMap::new();
    for i in 0..data.len() {
        for j in 0..data[i].len() {
            if data[i][j] != '.' {
                match m.get_mut(&data[i][j]) {
                    Some(v) => v.push((i as i64, j as i64)),
                    None => {
                        m.insert(data[i][j], vec![(i as i64, j as i64)]);
                    }
                }
            }
        }
    }

    let mut s: HashSet<(i64, i64)> = HashSet::new();

    for key in m.keys() {
        match m.get(key) {
            Some(v) => {
                for i in 0..v.len() {
                    for j in (i + 1)..v.len() {
                        let dx: i64 = v[i].0 - v[j].0;
                        let dy: i64 = v[i].1 - v[j].1;

                        let (mut x1, mut y1) = (v[i].0, v[i].1);
                        let (mut x2, mut y2) = (v[j].0, v[j].1);

                        while x1 >= 0
                            && x1 < data.len() as i64
                            && y1 >= 0
                            && y1 < data[0].len() as i64
                        {
                            s.insert((x1, y1));
                            x1 += dx;
                            y1 += dy;
                        }

                        while x2 >= 0
                            && x2 < data.len() as i64
                            && y2 >= 0
                            && y2 < data[0].len() as i64
                        {
                            s.insert((x2, y2));
                            x2 -= dx;
                            y2 -= dy;
                        }
                    }
                }
            }
            None => panic!("ERROR, no value in map"),
        }
    }
    s.iter().count()
}
