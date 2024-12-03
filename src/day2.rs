use std::arch::x86_64::*;
use std::{
    cmp::{max, min},
    collections::{BinaryHeap, HashMap},
};

fn is_valid_sequence_simd_u8(l: &[u8], ascending: bool, dampener: bool) -> bool {
    if l.len() < 2 {
        return true;
    }

    let mut padded = [0u8; 16];
    padded[..l.len()].copy_from_slice(l);

    unsafe {
        let current = _mm_loadu_si128(padded.as_ptr() as *const __m128i);

        let next = _mm_srli_si128(current, 1);

        let diff = if ascending {
            _mm_sub_epi8(next, current)
        } else {
            _mm_sub_epi8(current, next)
        };

        let zero = _mm_set1_epi8(0);
        let three = _mm_set1_epi8(3);

        let cmp_positive = if ascending {
            _mm_cmpgt_epi8(diff, zero)
        } else {
            _mm_cmpgt_epi8(zero, diff)
        };

        let cmp_within_bound = {
            let cmp_greater_than_three = _mm_cmpgt_epi8(diff, three);
            _mm_andnot_si128(cmp_greater_than_three, _mm_set1_epi8(-1))
        };

        let valid_mask = _mm_and_si128(cmp_positive, cmp_within_bound);

        let invalid_mask = _mm_xor_si128(valid_mask, _mm_set1_epi8(-1));
        let invalid_bits = _mm_movemask_epi8(invalid_mask);

        if invalid_bits == 0 {
            return true;
        } else if !dampener {
            return false;
        }

        let mut invalid_indices = Vec::new();
        for i in 0..l.len() - 1 {
            if (invalid_bits & (1 << i)) != 0 {
                invalid_indices.push(i);
            }
        }

        if invalid_indices.len() <= 2 {
            for &remove_index in &invalid_indices {
                let mut adjusted = Vec::with_capacity(l.len() - 1);
                adjusted.extend_from_slice(&l[..remove_index]);
                adjusted.extend_from_slice(&l[remove_index + 1..]);

                if is_valid_sequence_simd_u8(&adjusted, ascending, false) {
                    return true;
                }
            }
        }
    }
    false
}

#[aoc(day2, part1)]
pub fn part1(input: &str) -> u32 {
    let mut res: u32 = 0;
    for line in input.lines() {
        let l: Vec<u8> = line
            .split_whitespace()
            .map(|e| e.parse().unwrap())
            .collect();

        if is_valid_sequence_simd_u8(&l, true, false) {
            res += 1;
        } else if is_valid_sequence_simd_u8(&l, true, false) {
            res += 1;
        }
    }
    res
}

#[aoc(day2, part2)]
pub fn part2(input: &str) -> u32 {
    let mut res: u32 = 0;
    for line in input.lines() {
        let l: Vec<u8> = line
            .split_whitespace()
            .map(|e| e.parse().unwrap())
            .collect();
        if is_valid_sequence_simd_u8(&l, true, true) {
            res += 1;
        } else if is_valid_sequence_simd_u8(&l, true, true) {
            res += 1;
        }
    }
    res
}
