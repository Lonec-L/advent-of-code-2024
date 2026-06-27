#include <stdio.h>
#include <stdlib.h>

long long solve(long long x, int count, int memo[25][25][25][25])
{
	long long tmp = x;
	int visited[25][25][25][25] = {0};

	int diffs[4] = {0};

	int prev = x % 10;
	int curr = 0;

	for (int i = 0; i < count; i++) {
		tmp = ((tmp * 64) ^ tmp) % 16777216;
		tmp = ((tmp / 32) ^ tmp) % 16777216;
		tmp = ((tmp * 2048) ^ tmp) % 16777216;

		curr = tmp % 10;

		diffs[0] = diffs[1];
		diffs[1] = diffs[2];
		diffs[2] = diffs[3];
		diffs[3] = curr - prev;

		if (i > 2 &&
		    visited[10 + diffs[0]][10 + diffs[1]][10 + diffs[2]][10 + diffs[3]] == 0) {
			memo[10 + diffs[0]][10 + diffs[1]][10 + diffs[2]][10 + diffs[3]] += curr;
			visited[10 + diffs[0]][10 + diffs[1]][10 + diffs[2]][10 + diffs[3]]++;
		}

		prev = curr;
	}

	return tmp;
}

int main()
{
	FILE *in_file = fopen("in", "r");

	char buffer[20] = {0};

	int memo[25][25][25][25] = {0};

	long long res = 0;
	long long x = 0;

	int count = 0;
	while (fgets(buffer, 20, in_file)) {
		x = atoi(buffer);
		res += solve(x, 2000, memo);
		count++;
	}

	long long res2 = 0;
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			for (int k = 0; k < 25; k++) {
				for (int l = 0; l < 25; l++) {
					if (memo[i][j][k][l] > res2) {
						res2 = memo[i][j][k][l];
					}
				}
			}
		}
	}

	printf("pt1: %lli\n", res);
	printf("pt1: %lli\n", res2);
}
