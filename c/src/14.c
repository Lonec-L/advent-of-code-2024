#include <stdio.h>
#include <string.h>

const int m = 103;
const int n = 101;

struct point {
	int x, y;
};

struct robot {
	struct point p, v;
};

struct point calc_pos(struct robot *r, int seconds)
{
	struct point pos = r->p;
	pos.x = (r->p.x + r->v.x * seconds) % n;
	if (pos.x < 0) {
		pos.x += n;
	}
	pos.y = (r->p.y + r->v.y * seconds) % m;
	if (pos.y < 0) {
		pos.y += m;
	}
	return pos;
}

void solve_p1(struct robot *r, int q[4], int seconds)
{
	struct point pos = calc_pos(r, seconds);
	if (pos.x < n / 2 && pos.y < m / 2) {
		q[0]++;
	} else if (pos.x > n / 2 && pos.y < m / 2) {
		q[1]++;
	} else if (pos.x < n / 2 && pos.y > m / 2) {
		q[2]++;
	} else if (pos.x > n / 2 && pos.y > m / 2) {
		q[3]++;
	}
	printf("pos: %i, %i\n", pos.x, pos.y);
}

int canvas[101][103];

int solve_p2(struct robot r[500])
{
	int s = 0;
	while (1) {
		s++;
		memset(canvas, 0, sizeof(canvas));
		for (int i = 0; i < 500; i++) {
			struct point p = calc_pos(&r[i], s);
			canvas[p.y][p.x]++;
		}

		int ok = 0;
		for (int i = 0; i < m; i++) {
			int count = 0;
			int max_c = 0;
			for (int j = 0; j < n; j++) {
				if (j > 0 && canvas[i][j] > 0 && canvas[i - 1][j] > 0) {
					count++;
					max_c = count > max_c ? count : max_c;
				} else {
					count = 0;
				}
			}
			if (max_c >= 3) {
				ok++;
			}
		}
		if (ok >= 10) {
			printf("Seconds: %i\n\n", s);
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					if (canvas[i][j] > 0) {
						printf(".");
					} else {
						printf(" ");
					}
				}
				printf("\n");
			}
            return s;
		}
	}
}

int main()
{
	FILE *in_file = fopen("in", "r");
	char buff[10];
	int q[4] = {0, 0, 0, 0};

	struct robot all[500];
	int i = 0;
	while (1) {
		struct robot r;
		int ret = fscanf(in_file, "p=%i,%i v=%i,%i\n", &r.p.x, &r.p.y, &r.v.x, &r.v.y);
		if (ret == EOF) {
			break;
		} else if (ret != 4) {
			perror("Error parsing input values!!\n");
			return -1;
		}
		all[i++] = r;
		solve_p1(&r, q, 100);
	}
	int res1 = 1;
	for (int i = 0; i < 4; i++) {
		printf("Q%i: %i\n", i, q[i]);
		res1 *= q[i];
	}
	int res2 = solve_p2(all);
	printf("pt1: %i\n", res1);
	printf("pt2: %i\n", res2);
}
