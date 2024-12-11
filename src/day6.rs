use std::process::exit;

#[derive(Debug, Eq, PartialEq)]
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
}

impl Direction {
    fn turn(&self) -> Self {
        use Direction::*;
        match *self {
            UP => RIGHT,
            RIGHT => DOWN,
            DOWN => LEFT,
            LEFT => UP,
        }
    }
}

#[aoc(day6, part1)]
pub fn part1(input: &str) -> u64 {
    let mut res = 1;

    let mut data: Vec<Vec<char>> = input.lines().map(|l| l.chars().collect()).collect();

    let (mut x, mut y): (usize, usize) = (0, 0);

    for i in 0..data.len() {
        for j in 0..data[i].len() {
            if data[i][j] == '^' {
                (x, y) = (i, j);
                data[x][y] = 'X';
            }
        }
    }

    let mut d = Direction::UP;

    let (mut nx, mut ny): (usize, usize);
    loop {
        match d {
            Direction::UP => {
                if x as i32 - 1 < 0 {
                    break;
                }
                nx = x - 1;
                ny = y;
            }
            Direction::RIGHT => {
                if y as i32 + 1 >= data[x].len() as i32 {
                    break;
                }
                nx = x;
                ny = y + 1;
            }
            Direction::DOWN => {
                if x as i32 + 1 >= data.len() as i32 {
                    break;
                }
                nx = x + 1;
                ny = y;
            }
            Direction::LEFT => {
                if y as i32 - 1 < 0 {
                    break;
                }
                nx = x;
                ny = y - 1;
            }
        }
        match data[nx][ny] {
            '#' => d = d.turn(),
            'X' => {
                x = nx;
                y = ny;
            }
            '.' => {
                x = nx;
                y = ny;
                res += 1;
                data[x][y] = 'X';
            }
            _ => exit(1),
        }
    }

    res
}

// I am not proud of this solution, it is slow and inefficient. It is a limited brute force, and a
// full brute force for loop checking (just counting that we didi way too many steps). This
// shouldve been done in a smarter way, and code should not be duplicated.
//
// Anyhow, I spent way too much time on this problem, so this will have to do, for now.
#[aoc(day6, part2)]
pub fn part2(input: &str) -> u32 {
    let mut data: Vec<Vec<char>> = input.lines().map(|l| l.chars().collect()).collect();

    let (mut x, mut y): (usize, usize) = (0, 0);

    let (mut bx, mut by): (usize, usize) = (0, 0);

    for i in 0..data.len() {
        for j in 0..data[i].len() {
            if data[i][j] == '^' {
                (x, y) = (i, j);
                (bx, by) = (i, j);
                data[x][y] = 'X';
            }
        }
    }

    let mut d = Direction::UP;

    let (mut nx, mut ny): (usize, usize);
    loop {
        match d {
            Direction::UP => {
                if x as i32 - 1 < 0 {
                    break;
                }
                nx = x - 1;
                ny = y;
            }
            Direction::RIGHT => {
                if y as i32 + 1 >= data[x].len() as i32 {
                    break;
                }
                nx = x;
                ny = y + 1;
            }
            Direction::DOWN => {
                if x as i32 + 1 >= data.len() as i32 {
                    break;
                }
                nx = x + 1;
                ny = y;
            }
            Direction::LEFT => {
                if y as i32 - 1 < 0 {
                    break;
                }
                nx = x;
                ny = y - 1;
            }
        }
        match data[nx][ny] {
            '#' => d = d.turn(),
            'X' => {
                x = nx;
                y = ny;
            }
            '.' => {
                x = nx;
                y = ny;
                data[x][y] = 'X';
            }
            _ => exit(1),
        }
    }

    let mut res = 0;
    for i in 0..data.len() {
        for j in 0..data[i].len() {
            if data[i][j] != 'X' || (i, j) == (bx, by) {
                continue;
            }
            let mut inner_data = data.clone();
            inner_data[i][j] = '#';
            (x, y) = (bx, by);
            d = Direction::UP;
            let mut steps: usize = 0;
            loop {
                match d {
                    Direction::UP => {
                        if x as i32 - 1 < 0 {
                            break;
                        }
                        nx = x - 1;
                        ny = y;
                    }
                    Direction::RIGHT => {
                        if y as i32 + 1 >= data[x].len() as i32 {
                            break;
                        }
                        nx = x;
                        ny = y + 1;
                    }
                    Direction::DOWN => {
                        if x as i32 + 1 >= data.len() as i32 {
                            break;
                        }
                        nx = x + 1;
                        ny = y;
                    }
                    Direction::LEFT => {
                        if y as i32 - 1 < 0 {
                            break;
                        }
                        nx = x;
                        ny = y - 1;
                    }
                }
                match inner_data[nx][ny] {
                    '#' => d = d.turn(),
                    'X' | '.' => {
                        x = nx;
                        y = ny;
                        steps += 1;
                    }
                    _ => exit(1),
                }

                if steps > data.len() * data.len() * data.len() {
                    res += 1;
                    break;
                }
            }
        }
    }

    res
}
