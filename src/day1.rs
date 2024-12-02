use std::collections::{BinaryHeap, HashMap};

#[aoc(day1, part1)]
pub fn part1(input: &str) -> u32 {
    let mut sum: u32 = 0;
    let mut left: BinaryHeap<u32> = BinaryHeap::new();
    let mut right: BinaryHeap<u32> = BinaryHeap::new();
    for (i, e) in input.split_whitespace().enumerate() {
        let num: u32 = e.parse().unwrap();
        if i % 2 == 0 {
            left.push(num);
        } else {
            right.push(num);
        }
    }

    while !left.is_empty() {
        sum += left.pop().unwrap().abs_diff(right.pop().unwrap());
    }

    return sum;
}

#[aoc(day1, part2)]
pub fn part2(input: &str) -> u32 {
    let mut left = Vec::new();
    let mut map: HashMap<u32, u32> = HashMap::new();
    for (i, e) in input.split_whitespace().enumerate() {
        let num: u32 = e.parse().unwrap();
        if i % 2 == 0 {
            left.push(num);
        } else {
            match map.get_mut(&num) {
                Some(count) => {
                    *count += 1;
                }
                None => {
                    map.insert(num, 1);
                }
            }
        }
    }

    let mut sum: u32 = 0;
    for e in &left {
        sum += e * match map.get(e) {
            Some(n) => n,
            None => &0,
        };
    }
    return sum;
}
