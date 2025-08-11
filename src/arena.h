#ifndef _ARENA_H
#define _ARENA_H

typedef struct {
	size_t current;
	size_t capacity;
	char *buffer;
} arena_t;

arena_t *at_init(arena_t *);
void at_free(arena_t *);
void *at_alloc(arena_t *, size_t);
void at_reset(arena_t *);

#ifdef ARENA_IMPLEMENTATION 
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define at_size_t_max (size_t)(0-1)
#define at_pagesize 4096

arena_t *at_init(arena_t *a)
{
	a->current = 0;
	a->capacity = at_pagesize;
	a->buffer = malloc(a->capacity);
	if (a->buffer == NULL) return NULL;
  return a;
}

void at_free(arena_t *a)
{
	free(a->buffer);
}

void *at_alloc(arena_t *a, size_t n)
{
	if (a->current + n > a->capacity) {
		char buffer[a->capacity];

		if ((size_t)(a->capacity*2) < a->capacity)
			return NULL;
		memmove(buffer, a->buffer, a->capacity);
		at_free(a);

		if (a->capacity == at_size_t_max)
			return NULL;
		a->capacity *= 2;
		a->buffer = malloc(a->capacity);

		if(a->buffer == NULL) 
			return NULL;
		memmove(a->buffer, buffer, a->capacity);
	}

	a->current += n;
	return &a->buffer[a->current-n];
}

void at_reset(arena_t *a)
{
	a->current = 0;
	memset(a->buffer, 0, a->capacity);
}

#endif // ARENA_IMPLEMENTATION
#endif // _ARENA_H
