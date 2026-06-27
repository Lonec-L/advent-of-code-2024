#include <stdio.h>
#include <stdlib.h>

long long solve(long long x, int count)
{
	long long tmp = x;

	for (int i = 0; i < count; i++) {
		tmp = ((tmp * 64) ^ tmp) % 16777216;
		tmp = ((tmp / 32) ^ tmp) % 16777216;
		tmp = ((tmp * 2048) ^ tmp) % 16777216;
	}

	return tmp;
}

int main()
{
	FILE *in_file = fopen("in", "r");

	char buffer[20] = {0};
	long long res = 0;
	long long x = 0;
	while (fgets(buffer, 20, in_file)) {
		x = atoi(buffer);
		res += solve(x, 2000);
	}

	printf("pt1: %lli\n", res);
}
