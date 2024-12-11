
fn solve_p1(r: i64, c: i64, k: usize, values: &Vec<i64>, concat: bool) -> bool {
    if k == values.len() {
        if r == c {
            return true;
        };
        return false;
    }
    let res = solve_p1(r, c + values[k], k + 1, values, concat)
        || solve_p1(r, c * values[k], k + 1, values, concat);

    if concat {
        return res
            || solve_p1(
                r,
                (c.to_string() + &values[k].to_string())
                    .parse()
                    .expect("failed to parse int"),
                k + 1,
                values,
                concat,
            );
    } else {
        return res;
    }
}

#[aoc(day7, part1)]
pub fn part1(input: &str) -> i64 {
    let mut res = 0;
    for l in input.lines() {
        let data: Vec<&str> = l.split(":").collect();
        let result: i64 = data[0].parse().expect("failed to parse result");
        let values: Vec<i64> = data[1]
            .split_whitespace()
            .map(|e| e.parse().expect("failed to parse int"))
            .collect();

        if solve_p1(result, values[0], 1, &values, false) {
            res += result;
        }
    }
    res
}

#[aoc(day7, part2)]
pub fn part2(input: &str) -> i64 {
    let mut res = 0;
    for l in input.lines() {
        let data: Vec<&str> = l.split(":").collect();
        let result: i64 = data[0].parse().expect("failed to parse result");
        let values: Vec<i64> = data[1]
            .split_whitespace()
            .map(|e| e.parse().expect("failed to parse int"))
            .collect();

        if solve_p1(result, values[0], 1, &values, true) {
            res += result;
        }
    }
    res
}
