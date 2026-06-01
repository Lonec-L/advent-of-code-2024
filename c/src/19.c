#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int to_index(const char c)
{
	int res = -1;
	switch (c) {
	case 'w':
		res = 0;
		break;
	case 'u':
		res = 1;
		break;
	case 'b':
		res = 2;
		break;
	case 'r':
		res = 3;
		break;
	case 'g':
		res = 4;
		break;
	default:
		assert(0 && "Character not in alphabet!");
		break;
	}
	return res;
}

struct trie_node {
	uint8_t end;
	struct trie_node *c[5];
	struct trie_node *root;
};

int trie_insert(struct trie_node *root, const char *s)
{
	struct trie_node *current = root;
	for (int i = 0; i < strlen(s); i++) {
		int index = to_index(s[i]);
		if (current->c[index] == NULL) {
			current->c[index] = malloc(sizeof(struct trie_node));
			current->c[index]->end = 0;
			current->c[index]->root = NULL;
			memset(current->c[index]->c, 0, sizeof(struct trie_node *) * 5);
		}
		current = current->c[index];
	}
	current->end++;
	current->root = root;
	return 0;
}

long long is_pattern_possible(struct trie_node *node, long long memo[200], const char *const s,
			      int i)
{
	if (strlen(s) == i) {
		return node->end > 0 ? 1 : 0;
	}
	int index = to_index(s[i]);
	long long res = 0;
	if (node->c[index] != NULL) {
		res += is_pattern_possible(node->c[index], memo, s, i + 1);
	}
	if (node->end > 0) {
		assert(node->root != NULL);
		if (memo[i] == -1) {
			memo[i] = is_pattern_possible(node->root, memo, s, i);
		}
		// save to some memo for this pointer in string
		if (memo[i] > 0) {
			res += memo[i];
		}
	}
	return res;
}

int main()
{
	struct trie_node root = {0, {NULL}, NULL};
	FILE *in_file = fopen("in", "r");

	char *token, *buffer, *tofree;

	tofree = buffer = malloc(sizeof(char) * 3000);
	assert(buffer != NULL);

	fgets(buffer, sizeof(char) * 3000, in_file);

	while ((token = strsep(&buffer, ", ")) != NULL) {
		if (strlen(token) == 0) {
			continue;
		}
		if (token[strlen(token) - 1] == '\n') {
			token[strlen(token) - 1] = 0;
		}
		trie_insert(&root, token);
	}
	buffer = tofree;

	int res = 0;
	long long res2 = 0;
	long long memo[200];

	while (fgets(buffer, sizeof(char) * 3000, in_file)) {
		if (buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = 0;
		}
		if (strlen(buffer) == 0) {
			continue;
		}
		printf("%s\n", buffer);
		for (int i = 0; i < 200; i++) {
			memo[i] = -1;
		}
		long long ret = is_pattern_possible(&root, memo, buffer, 0);
		res += ret > 0 ? 1 : 0;
		res2 += ret;
	}

	free(tofree);
	printf("part 1: %i\n", res);
	printf("part 2: %lli\n", res2);
}
