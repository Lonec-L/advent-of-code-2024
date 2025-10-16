#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void count_corners(const char data[150][150], const int n, const int m, int *corners, int x, int y)
{
	const int x_diff[2] = {-1, 1};
	const int y_diff[2] = {-1, 1};

	uint8_t x_match, y_match, xy_match;
	char neighbour_x, neighbour_y, neighbour_xy;
	char curr = data[x][y];

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			uint8_t xi = x + x_diff[i];
			uint8_t yi = y + y_diff[j];

			neighbour_xy = 0;
			if (xi >= n || xi < 0) {
				neighbour_x = '.';
				neighbour_xy = '.';
			} else {
				neighbour_x = data[xi][y];
			}
			if (yi >= m || yi < 0) {
				neighbour_y = '.';
				neighbour_xy = '.';
			} else {
				neighbour_y = data[x][yi];
			}

			if (neighbour_xy == 0) {
				neighbour_xy = data[xi][yi];
			}

			// outer corner
			/*
			{-1, 0},
			{0, 1},
			*/
			if (curr != neighbour_x && curr != neighbour_y) {
				*corners += 1;
			}

			// inner corner
			/*
		    {0, 1},
		    {1, 1},
			*/
			if (curr == neighbour_x && curr == neighbour_y && curr != neighbour_xy) {
				*corners += 1;
			}
		}
	}
}

void solve_pt1(const char data[150][150], const int n, const int m, uint8_t visited[150][150],
	       int *cells, int *borders, int *corners, int x, int y)
{
	if (visited[x][y] != 0) {
		return;
	}
	visited[x][y]++;
	*cells += 1;
	if (x + 1 < n) {
		if (data[(x + 1)][y] == data[x][y]) {
			solve_pt1(data, n, m, visited, cells, borders, corners, x + 1, y);
		} else if (data[(x + 1)][y] != data[x][y]) {
			*borders += 1;
		}
	} else {
		*borders += 1;
	}
	if (x - 1 >= 0) {
		if (data[(x - 1)][y] == data[x][y]) {
			solve_pt1(data, n, m, visited, cells, borders, corners, x - 1, y);
		} else if (data[(x - 1)][y] != data[x][y]) {
			*borders += 1;
		}
	} else {
		*borders += 1;
	}

	if (y + 1 < m) {
		if (data[x][(y + 1)] == data[x][y]) {
			solve_pt1(data, n, m, visited, cells, borders, corners, x, y + 1);
		} else if (data[x][(y + 1)] != data[x][y]) {
			*borders += 1;
		}
	} else {
		*borders += 1;
	}
	if (y - 1 >= 0) {
		if (data[x][(y - 1)] == data[x][y]) {
			solve_pt1(data, n, m, visited, cells, borders, corners, x, y - 1);
		} else if (data[x][(y - 1)] != data[x][y]) {
			*borders += 1;
		}
	} else {
		*borders += 1;
	}

	count_corners(data, n, m, corners, x, y);
}

int main()
{
	FILE *in_file = fopen("in", "r");
	char buff[200];
	char data[150][150];
	uint8_t visited[150][150];
	int n, m;
	n = 0;
	m = 0;
	memset(data, 0, sizeof(data));
	memset(visited, 0, sizeof(visited));
	while (fgets(buff, 200, in_file)) {
		buff[strcspn(buff, "\n")] = 0;
		if (m == 0) {
			m = strlen(buff);
		} else if (strlen(buff) != m) {
			perror("n != buff_len");
			exit(1);
		}
		strcpy(data[n], buff);
		n++;
	}
	long long res1 = 0;
	long long res2 = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (visited[i][j] == 0) {
				int cells = 0;
				int borders = 0;
				int corners = 0;
				solve_pt1(data, n, m, visited, &cells, &borders, &corners, i, j);
				res1 += cells * borders;
				res2 += cells * corners;
			}
		}
	}
	printf("pt1: %lli\n", res1);
	printf("pt2: %lli\n", res2);
}
