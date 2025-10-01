#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FREE_SPACE -1

struct block {
	int file_id;
	size_t size;
	struct block *next, *prev;
};

int main()
{
	struct block *head = NULL;
	struct block *tmp = NULL;
	{
		char *input;
		input = malloc(sizeof(char) * 20100);
		scanf("%s", input);
		printf("strlen: %lu\n", strlen(input));

		for (int i = 0; i < strlen(input); i++) {
			struct block *current = malloc(sizeof(struct block));
			current->prev = NULL;
			current->next = NULL;
			if (head == NULL) {
				head = current;
			}
			// current block is a file
			if (i % 2 == 0) {
				current->size = input[i] - '0';
				current->file_id = i / 2;
			} else {
				current->size = input[i] - '0';
				current->file_id = FREE_SPACE; // current block is free space
			}

			// connect linked list nodes
			if (tmp != NULL) {
				current->prev = tmp;
				tmp->next = current;
			}

			tmp = current; //store last node
		}

		free(input); // input parsing complete, we can free.
	}

	struct block *b1, *b2;
	b1 = tmp;

	b2 = head;

	while (b1 != NULL) {
		b2 = head;
		while (b2 != NULL && b1->file_id != FREE_SPACE && b1 != b2) {
			if (b2->file_id != FREE_SPACE) {
				b2 = b2->next;
				continue;
			}
			if (b1->size <= b2->size) {
				b2->size -= b1->size;

				struct block *b1_copy = malloc(sizeof(struct block));

				*b1_copy = *b1;

				// mark as free space, since we never move files here we
				// dont need to merge with neighbors
				b1->file_id = FREE_SPACE;

				// insert b1 into its new position, before b2
				b1_copy->prev = b2->prev;
				if (b1_copy->prev != NULL) {
					b1_copy->prev->next = b1_copy;
				}
				b1_copy->next = b2;
				b2->prev = b1_copy;
				break;
			}
			b2 = b2->next;
		}
		b1 = b1->prev;
	}

	/*
	 * calculate checksum, freeing the nodes along the way
	 */
	b1 = head;
	long long pos = 0;
	long long checksum = 0;
	while (b1 != NULL) {
		if (b1->file_id == FREE_SPACE) {
			pos += b1->size;
			tmp = b1;
			b1 = b1->next;
			free(tmp);
			continue;
		}
		for (long long i = pos; i < pos + b1->size; i++) {
			checksum += i * b1->file_id;
		}
		pos += b1->size;
		tmp = b1;
		b1 = b1->next;
		free(tmp);
	}
	printf("checksum: %lld\n", checksum);
}
