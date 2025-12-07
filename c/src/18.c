#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pair {
	int x, y;
};

struct queue {
	int N; // max_size
	int head, tail;
	struct pair *buffer;
};

int q_init(struct queue *q, int N)
{
	q->head = 0;
	q->tail = 0;
	q->N = N;
	q->buffer = (struct pair *)malloc(sizeof(struct pair) * N);
	if (q->buffer == NULL) {
		printf("Malloc failed with errno: %i", errno);
		return -1;
	}
	return 0;
}

void q_free(struct queue *q)
{
	q->head = 0;
	q->tail = 0;
	q->N = 0;
	free(q->buffer);
}

void q_clear(struct queue *q)
{
	q->head = 0;
	q->tail = 0;
}

int q_push(struct queue *q, struct pair p)
{
	if ((q->tail + 1) % q->N != q->head) {
		q->buffer[q->tail++] = p;
		q->tail %= q->N;
		return 0;
	} else {
		printf("queue full!\n");
		return -1;
	}
}

int q_pop(struct queue *q, struct pair *p)
{
	if (q->head != q->tail) {
		*p = q->buffer[q->head];
		q->head = (q->head + 1) % q->N;
		return 0;
	} else {
		printf("queue empty!\n");
		return -1;
	}
}

int main()
{
	FILE *in_file = fopen("in", "r");
	int mem[71][71];
	int x, y;
	memset(mem, 0, sizeof(mem));
	int count = 0;
	int N = 71;
	int visited[71][71];
	struct queue q;
	struct pair last;
	int ret = q_init(&q, N * N);
	if (ret != 0) {
		printf("Error initializing queue!\n");
		return 1;
	}
	while (fscanf(in_file, "%i,%i,\n", &y, &x) != EOF) {
		mem[x][y] = 1;
		count++;
		if (count < 1024) {
			continue;
		}
		last.x = x;
		last.y = y;

		// pt1: BFS
		memset(visited, 0, sizeof(visited));
		struct pair p = {0, 0};
		q_push(&q, p);
		while (q.head != q.tail) {
			if (q_pop(&q, &p) != 0) {
				perror("Popping from queue failed!");
				return 1;
			}

			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == dy || (dx != 0 && dy != 0)) {
						continue;
					}
					int x = p.x + dx;
					int y = p.y + dy;
					if (x >= N || y >= N || x < 0 || y < 0 ||
					    visited[x][y] > 0 || mem[x][y] == 1 ||
					    (x == 0 && y == 0)) {
						continue;
					}
					visited[x][y] = visited[p.x][p.y] + 1;
					struct pair tmp = {x, y};
					q_push(&q, tmp);
				}
			}
		}
		q_clear(&q);
		if (count == 1024) {
			printf("pt1: %i\n", visited[N - 1][N - 1]);
		}
		if (visited[N - 1][N - 1] == 0) {
			printf("pt2: %i,%i\n", last.y,
			       last.x); // we swapped the coordinates when reading
			break;
		}
	}
	q_free(&q);
}
