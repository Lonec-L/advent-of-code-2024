#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point {
	int x, y;
};

char move(char data[60][60], struct point pos, struct point v)
{
	if (data[pos.x][pos.y] == '#') {
		return '#'; // no op
	} else if (data[pos.x][pos.y] == '.') {
		return '.';
	} else if (data[pos.x][pos.y] == 'O' || data[pos.x][pos.y] == '@') {
		struct point new_pos = {pos.x + v.x, pos.y + v.y};
		char ret = move(data, new_pos, v);
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

int main()
{
	FILE *in_file = fopen("in", "r");
	char data[60][60];
	char buff[200];
	int n, m;
	n = 0;
	m = 0;
	memset(data, 0, sizeof(data));
	while (fgets(buff, 200, in_file)) {
		if (buff[0] == '\n') {
			printf("Found empty line\n");
			break;
		}
		buff[strcspn(buff, "\n")] = 0;
		printf("%s\n", buff);
		if (n == 0) {
			n = strlen(buff);
		} else if (strlen(buff) != n) {
			perror("n != buff_len");
			exit(1);
		}
		strcpy(data[m++], buff);
	}
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
			char ret = move(data, pos, v);
			if (ret == '.') {
				pos.x += v.x;
				pos.y += v.y;
			}
		}
	}
	long long res1 = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (data[i][j] == 'O') {
				res1 += i * 100 + j;
			}
		}
		printf("%s\n", data[i]);
	}
	printf("pt1: %lli\n", res1);
}
