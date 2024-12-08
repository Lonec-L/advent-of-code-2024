#[aoc(day4, part1)]
pub fn part1(input: &str) -> u64 {
    let vertical: Vec<i32> = vec![1, 2, 3];
    let horizontals_and_diagonals: Vec<Vec<(i32, i32)>> = vec![
        vec![(0, 1), (0, 2), (0, 3)],
        vec![(1, 1), (2, 2), (3, 3)],
        vec![(-1, 1), (-2, 2), (-3, 3)],
    ];

    let data: Vec<Vec<char>> = input.lines().map(|l| l.chars().collect()).collect();

    let s = vec!['M', 'A', 'S'];

    let mut res: u64 = 0;

    for i in 0..data.len() {
        for j in 0..data[i].len() {
            if data[i][j] == 'X' {
                for v in &horizontals_and_diagonals {
                    let (mut positive, mut negative) = (true, true);
                    for k in 0..v.len() {
                        let (x, y) = (i as i32 + v[k].0, j as i32 + v[k].1);
                        if positive
                            && (x < 0
                                || x as usize >= data.len()
                                || y < 0
                                || y as usize >= data[i].len()
                                || data[x as usize][y as usize] != s[k])
                        {
                            positive = false;
                        }
                        let (x, y) = (i as i32 + v[k].0, j as i32 - v[k].1);
                        if negative
                            && (x < 0
                                || x as usize >= data.len()
                                || y < 0
                                || y as usize >= data[i].len()
                                || data[x as usize][y as usize] != s[k])
                        {
                            negative = false;
                        }
                    }
                    if positive {
                        res += 1;
                    }
                    if negative {
                        res += 1;
                    }
                }

                let (mut positive, mut negative) = (true, true);
                for k in 0..vertical.len() {
                    let (x, y) = (i as i32 + vertical[k], j as i32);
                    if positive
                        && (x < 0
                            || x as usize >= data.len()
                            || y < 0
                            || y as usize >= data[i].len()
                            || data[x as usize][y as usize] != s[k])
                    {
                        positive = false;
                    }
                    let (x, y) = (i as i32 - vertical[k], j as i32);
                    if negative
                        && (x < 0
                            || x as usize >= data.len()
                            || y < 0
                            || y as usize >= data[i].len()
                            || data[x as usize][y as usize] != s[k])
                    {
                        negative = false;
                    }
                }
                if positive {
                    res += 1;
                }
                if negative {
                    res += 1;
                }
            }
        }
    }
    res
}

#[aoc(day4, part2)]
pub fn part2(input: &str) -> u32 {
    let mut res: u32 = 0;

    let data: Vec<Vec<char>> = input.lines().map(|l| l.chars().collect()).collect();

    let s = vec!['M', 'M', 'S', 'S'];
    for i in 1..data.len() - 1 {
        for j in 1..data[i].len() - 1 {
            if data[i][j] == 'A' {
                for k in 0..4 {
                    if data[i - 1][j - 1] == s[(0 + k) % 4]
                        && data[i - 1][j + 1] == s[(1 + k) % 4]
                        && data[i + 1][j + 1] == s[(2 + k) % 4]
                        && data[i + 1][j - 1] == s[(3 + k) % 4]
                    {
                        res += 1;
                        break;
                    }
                }
            }
        }
    }

    res
}
