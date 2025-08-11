#ifndef _ARENA_H
#define _ARENA_H

#include <assert.h>
#define at_assert assert

#include <sys/mman.h>
#define at_mmap   mmap
#define at_munmap munmap

#define at_memset(dst, c, n)\
do {                        \
	char  *__t__;             \
	size_t __i__;             \
  __t__=(char *)&(dst)[0];  \
	__i__=0;                  \
	for (;__i__<(n);++__i__)  \
		  (dst)[__i__]=(c);     \
} while (0);                \


typedef struct {
	size_t current;
	size_t capacity;
	char *buffer;
} arena_t;

#define at_copy(src, dst, len)        \
do {                                  \
  {                                   \
		char *__tmp1__=(char *)&(src)[0]; \
		char *__tmp2__=(char *)&(dst)[0]; \
		size_t __i__=0;                   \
		for (;__i__<(len);++__i__)        \
			__tmp1__[__i__]=__tmp2__[__i__];\
	}                                   \
} while (0);                          \

// return NULL on error
arena_t *at_init(arena_t *a, size_t capacity);
int at_free(arena_t *a);
char *at_alloc(arena_t *a, size_t n);
void at_reset(arena_t *a);

#ifdef ARENA_IMPLEMENTATION 

#define page_size 4096
arena_t *at_init(arena_t *a, size_t n_pages)
{
	a->current = 0;
	a->capacity = n_pages<1 ? page_size : page_size*n_pages;
	a->buffer = at_mmap(NULL, a->capacity,
	PROT_READ|PROT_WRITE, MAP_ANONYMOUS,-1, (off_t)0);
	return a->buffer == MAP_FAILED ? NULL : a;
}

int at_free(arena_t *a)
{
	return at_munmap(a->buffer, a->capacity);
}

char *at_alloc(arena_t *a, size_t n)
{
	char *buffer;

	if (a->current + n > a->capacity) {
		if ((size_t)(a->capacity*2) < a->capacity)
			return NULL;
		at_copy(a->buffer, buffer, a->capacity);
		if (at_munmap(a->buffer, a->capacity) == -1)
			return NULL;
		a->capacity *= 2;
		a->buffer = at_mmap(NULL,a->capacity,
		PROT_READ | PROT_WRITE, MAP_ANONYMOUS,-1, (off_t)0);
		if (a->buffer == MAP_FAILED)
			return NULL;
		at_copy(buffer, a->buffer, a->capacity);
	}

	a->current += n;
	return &a->buffer[a->current-n];
}

void at_reset(arena_t *a)
{
	a->current = 0;
	at_memset(a->buffer, 0, a->capacity);
}

#endif // ARENA_IMPLEMENTATION
#endif // _ARENA_H
