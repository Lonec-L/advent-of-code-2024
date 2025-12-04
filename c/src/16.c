#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum direction {
	EAST,
	SOUTH,
	WEST,
	NORTH,
	DIRECTION_COUNT
};

struct vertex {
	int x, y;
	enum direction d;
	int p; // price
};

// very simplified O(N) pq, because we have small N
struct priority_queue {
	struct vertex buffer[150 * 150 * 4];
	int buffer_size;
	int size;
};

// we dont care about the vertex array, since we will overwiret it when we push items
void pq_init(struct priority_queue *pq)
{
	pq->size = 0;
	pq->buffer_size = 0;
}

// get the lowest price vertex from pq
struct vertex pq_pop(struct priority_queue *pq)
{
	assert(pq->size > 0);
	assert(pq->buffer_size >= pq->size);
	int last_occupied = 0;
	int best = -1;
	for (int i = 0; i < pq->buffer_size; i++) {
		if (pq->buffer[i].p != -1) {
			last_occupied = i;
			if (best == -1) {
				best = i;
				continue;
			}
			if (pq->buffer[i].p < pq->buffer[best].p) {
				best = i;
			}
		}
	}
	if (last_occupied < pq->buffer_size - 1) {
		pq->buffer_size = last_occupied + 1;
	}
	assert(best != -1);
	pq->size--;
	struct vertex v = pq->buffer[best];
	pq->buffer[best].p = -1;
	return v;
}

// put the vertex into pq
void pq_push(struct priority_queue *pq, struct vertex v)
{
	if (pq->size < pq->buffer_size) {
		for (int i = 0; i < pq->buffer_size; i++) {
			if (pq->buffer[i].p == -1) {
				pq->buffer[i] = v;
				pq->size++;
				return;
			}
		}
	} else {
		pq->buffer[pq->buffer_size++] = v; // just put it at the end
		pq->size++;
		return;
	}
	perror("failed to insert to pq, exiting\n");
	exit(-1);
}

int dfs(char maze[150][150], int visited[150][150][DIRECTION_COUNT], int best_paths[150][150],
	int x, int y, enum direction d, int price)
{
	// only take paths that are the best move for given direction into the current tile
	if (price > visited[x][y][d] || visited[x][y][d] == INT_MAX) {
		return 0;
	}

	if (maze[x][y] == 'E') {
		// double check we have the lowest possible price on finish
		for (int dir = EAST; dir < DIRECTION_COUNT; dir++) {
			if (price > visited[x][y][dir]) {
				return 0;
			}
		}
		// mark ending as part of best path
		best_paths[x][y] = 1;
		return 1;
	}

	enum direction directions[3] = {
		d,
		(d + DIRECTION_COUNT + 1) % DIRECTION_COUNT,
		(d + DIRECTION_COUNT - 1) % DIRECTION_COUNT,
	};

	int ret = 0;
	for (int i = 0; i < 3; i++) {
		int dx = 0, dy = 0;
		if (directions[i] == EAST) {
			dy = 1;
		} else if (directions[i] == WEST) {
			dy = -1;
		} else if (directions[i] == NORTH) {
			dx = -1;
		} else if (directions[i] == SOUTH) {
			dx = 1;
		}
		int nx = x + dx;
		int ny = y + dy;

		if (nx < 0 || ny < 0 || nx >= 150 || ny >= 150) {
			return 0;
		}
		int new_price = price + 1 + (i > 0 ? 1 : 0) * 1000;
		if (dfs(maze, visited, best_paths, nx, ny, directions[i], new_price) > 0) {
			ret = 1;
			// we are on one of the best paths
			best_paths[x][y] = 1;
		}
	}
	return ret;
}

int main()
{
	FILE *in_file = fopen("in", "r");
	char maze[150][150];
	char buffer[150];
	memset(maze, 0, sizeof(maze));
	int m = 0, n = 0;
	int ix, iy; // initial position of the deer
	int dest_x, dest_y;
	while (fscanf(in_file, "%s\n", buffer) != EOF) {
		if (n == 0) {
			n = strlen(buffer);
		} else {
			assert(n == strlen(buffer));
		}
		for (int i = 0; i < n; i++) {
			if (buffer[i] == 'S') {
				ix = m;
				iy = i;
			}
			if (buffer[i] == 'E') {
				dest_x = m;
				dest_y = i;
			}
		}
		memcpy(maze[m++], buffer, n);
	}

	// dijkstra, but keep record of lowest path for each direction in each spot in maze. So each
	// spot has a price and 4 directions, defined as "how much does it cost to get to this point
	// and turn to direction". Example: If we arrive at the spot from east, we should also
	// update directions north and south if any are cheaper. Then we can just perform dijkstra
	int visited[150][150][DIRECTION_COUNT];
	for (int i = 0; i < 150; ++i) {
		for (int j = 0; j < 150; ++j) {
			for (int d = 0; d < DIRECTION_COUNT; ++d) {
				visited[i][j][d] = INT_MAX;
			}
		}
	}

	struct priority_queue pq;
	pq_init(&pq);

	// initial dijkstra configuration
	visited[ix][iy][EAST] = 0;
	visited[ix][iy][SOUTH] = 1000;
	visited[ix][iy][WEST] = 2000;
	visited[ix][iy][NORTH] = 1000;
	for (int d = EAST; d < DIRECTION_COUNT; d++) {
		struct vertex v = {ix, iy, d, visited[ix][iy][d]};
		pq_push(&pq, v);
	}

	struct vertex v;
	int x, y;
	while (pq.size > 0) {
		v = pq_pop(&pq);
		// try to step in the direction we are currently facing in v. If we cant, continue
		// to next
		int dx = 0, dy = 0;
		if (v.d == EAST) {
			dy = 1;
		} else if (v.d == WEST) {
			dy = -1;
		} else if (v.d == NORTH) {
			dx = -1;
		} else if (v.d == SOUTH) {
			dx = 1;
		}
		x = v.x + dx;
		y = v.y + dy;
		if (x < 0 || y < 0 || x >= m || y >= m || maze[x][y] == '#' ||
		    visited[x][y][v.d] <= v.p + 1) {
			continue;
		}

		visited[x][y][v.d] = v.p + 1;
		struct vertex tmp = {x, y, v.d, v.p + 1};
		pq_push(&pq, tmp);
		// also check the other 2 rotations, we dont check 3rd because the optimal path
		// never has a 180
		if (visited[x][y][(v.d + DIRECTION_COUNT + 1) % DIRECTION_COUNT] > v.p + 1001) {
			visited[x][y][(v.d + DIRECTION_COUNT + 1) % DIRECTION_COUNT] = v.p + 1001;
			struct vertex tmp = {x, y, (v.d + DIRECTION_COUNT + 1) % DIRECTION_COUNT,
					     v.p + 1001};
			pq_push(&pq, tmp);
		}
		if (visited[x][y][(v.d + DIRECTION_COUNT - 1) % DIRECTION_COUNT] > v.p + 1001) {
			visited[x][y][(v.d + DIRECTION_COUNT - 1) % DIRECTION_COUNT] = v.p + 1001;
			struct vertex tmp = {x, y, (v.d + DIRECTION_COUNT - 1) % DIRECTION_COUNT,
					     v.p + 1001};
			pq_push(&pq, tmp);
		}
	}

	int res = INT_MAX;
	for (int d = 0; d < DIRECTION_COUNT; d++) {
		if (res > visited[dest_x][dest_y][d]) {
			res = visited[dest_x][dest_y][d];
		}
	}

	printf("pt1: %i\n", res);

	// pt2
	int best_paths[150][150];
	memset(best_paths, 0, sizeof(best_paths));
	int prices[4] = {0, 1000, 2000, 1000};
	for (int d = EAST; d < DIRECTION_COUNT; d++) {
		dfs(maze, visited, best_paths, ix, iy, d, prices[d]);
	}

	int res2 = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (best_paths[i][j]) {
				res2++;
			}
		}
	}
	printf("pt2: %i\n", res2);
	return 0;
}
