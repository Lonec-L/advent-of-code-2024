#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point {
	long long x, y;
};

struct machine {
	struct point prize, a, b;
};

struct solution {
	long long a_count, b_count;
	long long price;
};

int solve_pt1(const struct machine *m)
{
	struct solution s = {0, 0, 0};

	for (int i = 0; i <= 100; i++) {
		for (int j = 0; j <= 100; j++) {
			if (((i * m->a.x + j * m->b.x) == m->prize.x &&
			     (i * m->a.y + j * m->b.y) == m->prize.y) &&
			    ((i * 3 + j) < s.price || s.price == 0)) {
				s.price = i * 3 + j;
			}
		}
	}
	return s.price;
}

long long solve_pt2(const struct machine *m)
{
	double D = m->a.x * m->b.y - m->a.y * m->b.x;

	if (fabs(D) < 0.1) {
		printf("D near zero, returning 0\n");
		printf("%f\n", D);
		return 0;
	}

	double x = (m->prize.x * m->b.y - m->b.x * m->prize.y) / D;
	double y = (m->prize.y * m->a.x - m->a.y * m->prize.x) / D;

    if(x < 0 || y < 0){
        printf("negative coeficient, returning 0\n");
        return 0;
    }

    if(fabs(x - round(x)) < 0.000000001 && fabs(y - round(y)) < 0.000000001){
        return round(x) * 3 + round(y)*1;
    }
	return 0;
}

int main()
{
	FILE *in_file = fopen("in", "r");
	char buff[10];
	long long sum1 = 0;
	long long sum2 = 0;
	while (1) {
		struct machine m;
		int ret = fscanf(in_file, "Button A: X+%lli, Y+%lli\n", &m.a.x, &m.a.y);
		if (ret == EOF) {
			break;
		}
		if (ret != 2) {
			fprintf(stderr, "Parse error on Button A line\n");
			break;
		}

		ret = fscanf(in_file, "Button B: X+%lli, Y+%lli\n", &m.b.x, &m.b.y);
		if (ret != 2) {
			fprintf(stderr, "Parse error on Button B line\n");
			break;
		}

		ret = fscanf(in_file, "Prize: X=%lli, Y=%lli\n", &m.prize.x, &m.prize.y);
		if (ret != 2) {
			fprintf(stderr, "Parse error on Prize line\n");
			break;
		}
		sum1 += solve_pt1(&m);
		m.prize.x += 10000000000000;
		m.prize.y += 10000000000000;
		sum2 += solve_pt2(&m);
	}
	printf("pt1: %lli\n", sum1);
	printf("pt2: %lli\n", sum2);
}
