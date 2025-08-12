#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define ARENA_IMPLEMENTATION
#include "arena.h"

int main(void)
{
	arena_t a;

	if ( at_init(&a) == NULL )
	{
		fprintf(stderr, "failed to init arena\n");
		return 1;
	}

	char *buffer = at_alloc(&a, sizeof(char)*32);
	if ( buffer == NULL ) {
		fprintf(stderr, "failed to alloc bytes 32\n");
		return 2;
	}

	memmove(buffer, "hello world\n", 13);
  write(STDOUT_FILENO, buffer, 13);

	at_reset(&a);
  write(STDOUT_FILENO, buffer, 13);

	memmove(buffer, "maple cherry\n", 13);
  write(STDOUT_FILENO, buffer, 13);

	at_free(a.buffer);
	return 0;
}
