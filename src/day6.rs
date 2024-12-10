use std::process::exit;

#[derive(Debug)]
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
                if y as i32 + 1 < 0 {
                    break;
                }
                nx = x;
                ny = y + 1;
            }
            Direction::DOWN => {
                if x as i32 + 1 < 0 {
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

#[aoc(day6, part2)]
pub fn part2(input: &str) -> u32 {
    //if guard is on the already visited square, and turning him
    //right would put point him in the same direction that he was turned at the last time he was on
    //that square, then this is a possible loop. Need to also count like he walked up to the starting position
    //
    //we need one array to store all directions he was turned (can be out main array, just use some
    //chars to represent directions), and another one to store obstacles, to not count obstacles
    //twice

    let mut res = 0;

    let mut data: Vec<Vec<char>> = input.lines().map(|l| l.chars().collect()).collect();

    let mut obstacles = data.clone();

    let (mut x, mut y): (usize, usize) = (0, 0);

    'find: for i in 0..data.len() {
        for j in 0..data[i].len() {
            if data[i][j] == '^' {
                (x, y) = (i, j);

                for k in x..data.len() {
                    match data[k][y] {
                        '#' => break,
                        _ => data[k][y] = 'u',
                    }
                }
                break 'find;
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
                if y as i32 + 1 < 0 {
                    break;
                }
                nx = x;
                ny = y + 1;
            }
            Direction::DOWN => {
                if x as i32 + 1 < 0 {
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

        use Direction::*;
        match data[nx][ny] {
            '#' => {
                d = d.turn();
                data[x][y] = match d {
                    LEFT => 'l',
                    RIGHT => 'r',
                    UP => 'u',
                    DOWN => 'd',
                };
            }
            _ => {
                if obstacles[nx][ny] != '#' {
                    println!("{}, {:?}, {}", data[x][y], d.turn(), obstacles[nx][ny]);
                    match (d.turn(), data[x][y]) {
                        (LEFT, 'l') | (RIGHT, 'r') | (UP, 'u') | (DOWN, 'd') => {
                            res += 1;
                            obstacles[nx][ny] = '#';
                        }
                        _ => (),
                    }
                }
                data[x][y] = match d {
                    LEFT => 'l',
                    RIGHT => 'r',
                    UP => 'u',
                    DOWN => 'd',
                };
                x = nx;
                y = ny;
            }
        }
    }
    let v: Vec<String> = data.into_iter().map(|v| String::from_iter(v)).collect();
    for l in v {
        println!("{:?}", l);
    }
    res
}
