#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

struct point {
	int x, y;
};

struct q_node {
	struct point p;
	struct q_node *next;
};

struct queue {
	struct q_node *front;
	struct q_node *back;
};

int push_queue(struct queue *q, struct point p)
{
	struct q_node *tmp = malloc(sizeof(struct q_node));
	if (tmp == NULL) {
		return ENOMEM;
	}

	tmp->p = p;
	tmp->next = NULL;

	if (q->front == NULL) {
		q->front = tmp;
		q->back = tmp;
		return 0;
	}

	assert(q->back != NULL && "q->front non NULL and q->back NULL - invalid state!");

	q->back->next = tmp;
	q->back = tmp;

	return 0;
}

struct point pop_queue(struct queue *q)
{
	assert(q->front != NULL && "Cannot pop from empty queue!");
	struct point p = q->front->p;
	if (q->front == q->back) {
		assert(q->front->next == NULL);
		q->front = NULL;
		q->back = NULL;
		return p;
	}
	q->front = q->front->next;
	return p;
}

int solve(char map[200][200], struct point start, struct point end, int cheat_len)
{
	int distances[200][200];
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			distances[i][j] = INT_MAX;
		}
	}

	distances[end.x][end.y] = 0;
	struct queue q = {NULL, NULL};
	push_queue(&q, end);
	while (q.front != NULL) {
		struct point p = pop_queue(&q);
		int d = distances[p.x][p.y];
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if ((i != 0 && j != 0) || map[p.x + i][p.y + j] == '#') {
					continue;
				}
				if (d + 1 < distances[p.x + i][p.y + j]) {
					distances[p.x + i][p.y + j] = d + 1;
					push_queue(&q, (struct point){p.x + i, p.y + j});
				}
			}
		}
	}

	int res = 0;
	for (int x = cheat_len; x < 200 - cheat_len; x++) {
		for (int y = cheat_len; y < 200 - cheat_len; y++) {
			if (distances[x][y] == INT_MAX) {
				continue;
			}
			for (int dx = -cheat_len; dx <= cheat_len; dx++) {
				for (int dy = -cheat_len; dy <= cheat_len; dy++) {
					if (abs(dx) + abs(dy) > cheat_len ||
					    distances[x][y] <= distances[x + dx][y + dy]) {
						continue;
					}
					if (distances[x][y] - distances[x + dx][y + dy] -
						    (abs(dx) + abs(dy)) >=
					    100) {
						res++;
					}
				}
			}
		}
	}

	return res;
}

int main()
{
	FILE *in_file = fopen("in", "r");

	char buffer[150] = {0};
	char map[200][200] = {'#'};
	struct point end = {0, 0};
	struct point start = {0, 0};
	memset(map, '#', sizeof(map));
	int i = 20;
	while (fgets(buffer, 150, in_file)) {
		for (int j = 0; j < strlen(buffer) - 1; j++) {
			map[i][j + 20] = buffer[j];
			if (map[i][j + 20] == 'S') {
				start = (struct point){i, j + 20};
			} else if (map[i][j + 20] == 'E') {
				end = (struct point){i, j + 20};
			}
		}
		i++;
	}

	printf("pt1: %i\n", solve(map, start, end, 2));
	printf("pt2: %i\n", solve(map, start, end, 20));
	return 0;
}
