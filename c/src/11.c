#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stone {
	long long value;
	struct stone *next;
};

// brute force ad hoc solution
int solve_pt1(struct stone *start)
{
	int T = 25;
	int N = 0;
	while (T--) {
		printf("%i\n", T);
		struct stone *tmp = start;
		while (tmp) {
			if (tmp->value == 0) {
				tmp->value = 1;
			} else if (((int)log10(tmp->value) + 1) % 2 == 0) {
				int exp = ((int)log10(tmp->value) + 1) / 2;
				int mask = pow(10, exp);
				int right = tmp->value % mask;
				int left = tmp->value / mask;

				struct stone *new_stone = malloc(sizeof(struct stone));
				new_stone->next = tmp->next;
				tmp->next = new_stone;
				new_stone->value = right;
				tmp->value = left;
				tmp = tmp->next;
			} else {
				tmp->value *= 2024;
			}
			tmp = tmp->next;
		}
	}
	struct stone *tmp = start;
	while (tmp) {
		N++;
		tmp = tmp->next;
	}
	return N;
}

// recursive solution with memoization
long long solve_pt2(long long m[10000][80], long long value, int depth)
{
	if (value < 10000 && m[value][depth] != 0) {
		return m[value][depth];
	}
	if (depth-- <= 0) {
		return 1;
	}
	long long sum = 0;
	if (value == 0) {
		sum += solve_pt2(m, 1, depth);
	} else if (((int)log10(value) + 1) % 2 == 0) {
		int exp = ((int)log10(value) + 1) / 2;
		long long mask = pow(10, exp);
		long long right = value % mask;
		long long left = value / mask;
		sum += solve_pt2(m, left, depth);
		sum += solve_pt2(m, right, depth);
	} else {
		sum += solve_pt2(m, value * 2024, depth);
	}
	if (value < 10000) {
		m[value][depth + 1] = sum;
	}
	return sum;
}

int main()
{
	struct stone *start = NULL;
	{ // read data from input file
		FILE *in_file = fopen("in", "r");
		int tmp;
		struct stone *curr = NULL;
		struct stone *prev = NULL;
		while (fscanf(in_file, "%i", &tmp) != EOF) {
			curr = malloc(sizeof(struct stone));
			curr->value = tmp;

			if (prev != NULL) {
				prev->next = curr;
			}

			if (start == NULL) {
				start = curr;
			}
			prev = curr;
		}
		if (fclose(in_file) != 0) {
			perror("Closing input file failed!\n");
			exit(1);
		}
	}

	// memoization for recursion
	long long m[10000][80];
	memset(m, 0, sizeof(m));

	struct stone *tmp = start;
	long long ret = 0;
	while (tmp) {
		ret += solve_pt2(m, tmp->value, 75);
		fflush(stdout);
		tmp = tmp->next;
	}

	printf("part1: %i\n", solve_pt1(start));
	printf("part2: %lli\n", ret);

	// cleanup
	tmp = start;
	while (tmp) {
		struct stone *tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
}
