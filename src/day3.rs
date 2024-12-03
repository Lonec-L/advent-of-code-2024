use regex::Regex;
use std::collections::{BinaryHeap, HashMap};

#[aoc(day3, part1)]
pub fn part1(input: &str) -> u32 {
    let re: Regex = Regex::new(r"mul\(([0-9]+),([0-9]+)\)").unwrap();
    let mut res: u32 = 0;
    for (_, [a, b]) in re.captures_iter(input).map(|m| m.extract()) {
        res += a.parse::<u32>().unwrap() * b.parse::<u32>().unwrap();
    }
    res
}

#[aoc(day3, part2)]
pub fn part2(input: &str) -> u32 {
    let dont: Vec<&str> = input.split("don't()").collect();
    let re: Regex = Regex::new(r"mul\(([0-9]+),([0-9]+)\)").unwrap();
    let mut x: Vec<&str> = Vec::new();
    x.push(dont[0]);
    for e in &dont[1..] {
        let mut temp: Vec<&str> = e.split("do()").skip(1).collect();
        x.append(&mut temp);
    }

    let mut res: u32 = 0;
    for s in x {
        for (_, [a, b]) in re.captures_iter(s).map(|m| m.extract()) {
            res += a.parse::<u32>().unwrap() * b.parse::<u32>().unwrap();
        }
    }
    res
}
