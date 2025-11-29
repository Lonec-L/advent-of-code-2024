/*
 * Day 15 pt 1 and pt 2 solution
 *
 * It aint pretty but it works, did not want to refactor the main() after solving it, so some of the
 * code is repeated and inlined when it should probably be in helper functions.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point {
	int x, y;
};

char move_pt1(char data[60][120], struct point pos, struct point v)
{
	if (data[pos.x][pos.y] == '#') {
		return '#'; // no op
	} else if (data[pos.x][pos.y] == '.') {
		return '.';
	} else if (data[pos.x][pos.y] == 'O' || data[pos.x][pos.y] == '@') {
		struct point new_pos = {pos.x + v.x, pos.y + v.y};
		char ret = move_pt1(data, new_pos, v);
		if (ret == '.') {
			data[new_pos.x][new_pos.y] = data[pos.x][pos.y];
			if (data[pos.x][pos.y] == '@') {
				data[pos.x][pos.y] = '.';
			}
		}
		return ret;
	}
	return 'e';
}

char dfs_pt2(const char data[60][120], char to_move[60][120], struct point pos, struct point v)
{
	if (to_move[pos.x][pos.y] != 0) {
		return '.';
	}
	if (data[pos.x][pos.y] == '#') {
		return '#'; // no op
	} else if (data[pos.x][pos.y] == '.') {
		return '.';
	} else if (data[pos.x][pos.y] == '[' || data[pos.x][pos.y] == ']') {
		struct point new_pos = {pos.x + v.x, pos.y + v.y};
		to_move[pos.x][pos.y] = data[pos.x][pos.y];
		char ret1 = dfs_pt2(data, to_move, new_pos, v);
		if (v.x != 0) {
			if (data[pos.x][pos.y] == '[') {
				assert(data[pos.x][pos.y + 1] == ']' && "Data is wrong somehow!");
				to_move[pos.x][pos.y + 1] = ']';
				new_pos.y += 1;
			} else if (data[pos.x][pos.y] == ']') {
				assert(data[pos.x][pos.y - 1] == '[' && "Data is wrong somehow!");
				to_move[pos.x][pos.y - 1] = '[';
				new_pos.y -= 1;
			}
		}
		char ret2 = dfs_pt2(data, to_move, new_pos, v);
		if (ret1 == '.' && ret2 == '.') {
			return '.';
		} else {
			return '#';
		}
	} else if (data[pos.x][pos.y] == '@') {
		struct point new_pos = {pos.x + v.x, pos.y + v.y};
		char ret1 = dfs_pt2(data, to_move, new_pos, v);
		return ret1;
	}
	return 'e';
}

int main()
{
	// variables
	char data[60][120];
	char buff[200];
	int n, m;
	n = 0;
	m = 0;

	// reading input
	FILE *in_file = fopen("in", "r");
	memset(data, 0, sizeof(data));
	while (fgets(buff, 200, in_file)) {
		if (buff[0] == '\n') {
			break;
		}
		buff[strcspn(buff, "\n")] = 0;
		if (n == 0) {
			n = strlen(buff);
		} else if (strlen(buff) != n) {
			perror("n != buff_len");
			exit(1);
		}
		strcpy(data[m++], buff);
	}

	// find robot initial position
	struct point pos = {-1, -1};
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (data[i][j] == '@') {
				pos.x = i;
				pos.y = j;
				break;
			}
		}
		if (pos.x != -1) {
			break;
		}
	}

	if (pos.x == -1) {
		perror("Robot not found!\n");
		exit(-1);
	}

	// solve pt 1
	while (fgets(buff, 200, in_file)) {
		buff[strcspn(buff, "\n")] = 0;
		for (int i = 0; i < strlen(buff); i++) {
			struct point v = {0, 0};
			if (buff[i] == '>') {
				v.y = 1;
			} else if (buff[i] == '<') {
				v.y = -1;
			} else if (buff[i] == '^') {
				v.x = -1;
			} else if (buff[i] == 'v') {
				v.x = 1;
			}
			char ret = move_pt1(data, pos, v);
			if (ret == '.') {
				pos.x += v.x;
				pos.y += v.y;
			}
		}
	}
	long long res = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (data[i][j] == 'O') {
				res += i * 100 + j;
			}
		}
	}
	printf("pt1: %lli\n", res);

	// pt 2
	n = 0;
	m = 0;
	fseek(in_file, 0, SEEK_SET);
	memset(data, 0, sizeof(data));
	while (fgets(buff, 200, in_file)) {
		if (buff[0] == '\n') {
			break;
		}
		buff[strcspn(buff, "\n")] = 0;
		if (n == 0) {
			n = strlen(buff);
		} else if (strlen(buff) != n) {
			perror("n != buff_len");
			exit(1);
		}
		strcpy(data[m++], buff);
	}

	// pt 2 data augmentation
	for (int i = 0; i < m; i++) {
		for (int j = n - 1; j >= 0; j--) {
			if (data[i][j] == 'O') {
				data[i][j * 2] = '[';
				data[i][j * 2 + 1] = ']';
			} else if (data[i][j] == '@') {
				data[i][j * 2] = '@';
				data[i][j * 2 + 1] = '.';
				pos.x = i;
				pos.y = j * 2;
			} else {
				data[i][j * 2] = data[i][j];
				data[i][j * 2 + 1] = data[i][j];
			}
		}
	}

	// find robot initial position
	pos.x = -1;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (data[i][j] == '@') {
				pos.x = i;
				pos.y = j;
				break;
			}
		}
		if (pos.x != -1) {
			break;
		}
	}

	if (pos.x == -1) {
		perror("Robot not found!\n");
		exit(-1);
	}

	char to_move[60][120];
	memset(to_move, 0, sizeof(to_move));
	while (fgets(buff, 200, in_file)) {
		buff[strcspn(buff, "\n")] = 0;
		for (int i = 0; i < strlen(buff); i++) {
			struct point v = {0, 0};
			if (buff[i] == '>') {
				v.y = 1;
			} else if (buff[i] == '<') {
				v.y = -1;
			} else if (buff[i] == '^') {
				v.x = -1;
			} else if (buff[i] == 'v') {
				v.x = 1;
			}
			memset(to_move, 0, sizeof(to_move));
			// we should just DFS from robot and make a helper 2d array where we mark
			// floodfilled cells. Then we move them in separate loop if we dont hit a
			// wall
			char ret = dfs_pt2(data, to_move, pos, v);
			if (ret != '.') {
				continue;
			}
			// perform the move
			for (int _ = 0; _ < 2; _++) {
				for (int i = 0; i < m; i++) {
					for (int j = 0; j < 2 * n; j++) {
						if (to_move[i][j] != 0) {
							if (_ == 0) { // clear old data
								data[i][j] = '.';
							} else {
								data[i + v.x][j + v.y] =
									to_move[i][j];
							}
						}
					}
				}
			}
			// finally move the robot
			data[pos.x + v.x][pos.y + v.y] = '@';
			data[pos.x][pos.y] = '.';
			pos.x += v.x;
			pos.y += v.y;
		}
	}

	// calculate result
	res = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n * 2; j++) {
			if (data[i][j] == '[') {
				res += i * 100 + j;
			}
		}
	}
	printf("pt2: %lli\n", res);
}
