#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_pt1(const char *data, const int n, const int m, u_int8_t *visited, int x, int y)
{
	if (data[x * m + y] == '9') {
		if (visited[x * m + y]++ == 0) {
			return 1;
		} else {
			return 0;
		}
	}

	int sum = 0;
	if (x + 1 < n && data[(x + 1) * m + y] - data[x * m + y] == 1) {
		sum += solve_pt1(data, n, m, visited, x + 1, y);
	}
	if (x - 1 >= 0 && data[(x - 1) * m + y] - data[x * m + y] == 1) {
		sum += solve_pt1(data, n, m, visited, x - 1, y);
	}
	if (y + 1 < m && data[x * m + (y + 1)] - data[x * m + y] == 1) {
		sum += solve_pt1(data, n, m, visited, x, y + 1);
	}
	if (y - 1 >= 0 && data[x * m + (y - 1)] - data[x * m + y] == 1) {
		sum += solve_pt1(data, n, m, visited, x, y - 1);
	}
	return sum;
}

int main()
{
	char buf[50];
	FILE *in_file = fopen("in", "r");
	int n = 0;
	int m = 0;
	char *data = malloc(sizeof(char) * 50 * 50);
	while (fgets(buf, 50, in_file)) {
		if (m == 0) {
			m = strlen(buf);
		} else if (m != strlen(buf)) {
			perror("N not equal to strlen!!");
			exit(1);
		}
		strcpy(&data[n * m], buf);
		n++;
	}
	if (fclose(in_file) != 0) {
		perror("Closing input file failed!\n");
        exit(1);
	}
	printf("N: %i\nM: %i\n", n, m);
	u_int8_t *visited = malloc(n * m * sizeof(u_int8_t));

	int ret1 = 0;
	int ret2 = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (data[i * m + j] == '0') {
				memset(visited, 0, n * m * sizeof(u_int8_t));

				// get number of reachable 9s
				ret1 += solve_pt1(data, n, m, visited, i, j);

				// for part 2 we can just sum up the number of visits to each 9
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						ret2 += visited[i * m + j];
					}
				}
			}
		}
	}
	printf("Part 1: %i\n", ret1);
	printf("Part 2: %i\n", ret2);
}
