#include <stdio.h>
#define ARENA_IMPLEMENTATION
#include "arena.h"

int main(void)
{
	arena_t a;

	if (at_init(&a, 1024) == NULL)
	{
		fprintf(stderr, "failed to init arena\n");
		return 1;
	}

	printf("hello world\n");
	return 0;
}
