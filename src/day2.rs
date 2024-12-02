use std::{
    cmp::{max, min},
    collections::{BinaryHeap, HashMap},
};

#[aoc(day2, part1)]
pub fn part1(input: &str) -> u32 {
    let mut res: u32 = 0;
    let mut ok_desc;
    let mut ok_asc;
    let mut damp_asc;
    let mut damp_desc;
    let mut i;
    for line in input.lines() {
        let l: Vec<u32> = line
            .split_whitespace()
            .map(|e| e.parse().unwrap())
            .collect();
        if l.len() == 1 {
            res += 1;
        }
        ok_asc = true;
        ok_desc = true;
        damp_asc = false;
        damp_desc = false;
        i = 1;
        while i < l.len() {
            if ok_asc && l[i - 1] >= l[i] || l[i] - l[i - 1] > 3 {
                if !damp_asc {
                    damp_asc = true;
                    if i == l.len() - 1 {
                        break;
                    }
                    let mut inner_ok = false;
                    let lower = min(i, 2);
                    for j in (i - lower..=i).rev() {
                        if j == 0 {
                            i = j + 1;
                            inner_ok = true;
                            break;
                        } else if !(l[j - 1] >= l[j + 1] || l[j + 1] - l[j - 1] > 3) {
                            i = j + 1;
                            inner_ok = true;
                            break;
                        }
                    }
                    if !inner_ok {
                        ok_asc = false;
                        break;
                    }
                } else {
                    ok_asc = false;
                    break;
                }
            }
            i += 1;
        }
        i = 1;
        while i < l.len() {
            if ok_desc && l[i - 1] <= l[i] || l[i - 1] - l[i] > 3 {
                if !damp_desc {
                    damp_desc = true;
                    if i == l.len() - 1 {
                        break;
                    }
                    let mut inner_ok = false;
                    let lower = min(i, 2);
                    for j in (i - lower..=i).rev() {
                        if j == 0 {
                            i = j + 1;
                            inner_ok = true;
                            break;
                        } else if !(l[j - 1] <= l[j + 1] || l[j - 1] - l[j + 1] > 3) {
                            i = j + 1;
                            inner_ok = true;
                            break;
                        }
                    }
                    if !inner_ok {
                        ok_desc = false;
                        break;
                    }
                } else {
                    ok_desc = false;
                }
            }
            i += 1;
        }
        if ok_asc || ok_desc {
            res += 1;
        }
    }
    res
}
