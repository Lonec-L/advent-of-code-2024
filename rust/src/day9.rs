#[aoc(day9, part1)]
pub fn part1(input: &str) -> u128 {
    let mut v: Vec<u32> = input
        .chars()
        .map(|e| e.to_digit(10).expect("failed to parse digit"))
        .collect();
    let mut res: u128 = 0;
    let mut lid: u32 = 0;
    let mut hid = (v.len() / 2) as u32;

    let mut cpos: u32 = 0;

    let mut i: usize = 0;
    let mut j: usize = v.len() - 1;
    while i <= j {
        if i % 2 == 0 {
            for _ in 0..v[i] {
                res += (lid * cpos) as u128;
                cpos += 1;
            }
            i += 1;
            lid += 1;
        } else if v[i] <= v[j] {
            for _ in 0..v[i] {
                res += (hid * cpos) as u128;
                cpos += 1;
            }
            v[j] -= v[i];
            i += 1;
        } else {
            for _ in 0..v[j] {
                res += (hid * cpos) as u128;
                cpos += 1;
            }
            v[i] -= v[j];
            j -= 2;
            hid -= 1;
        }
        if v[j] == 0 {
            j -= 2;
            hid -= 1;
        }
    }
    res
}

// want to come up with a samrt solution, i dont have it yet
#[aoc(day9, part2)]
pub fn part2(input: &str) -> u128 {
    0
}
