#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#define PAGE_SIZE sysconf(_SC_PAGESIZE)
#define ALIGNMENT sizeof(void *)
#define align(x) ((x % 8) ? (x + (8 - x % 8)) : x)

/**
 * struct block_s - stores information about blocks
 *
 * @size: size of block
 * @next: pointer to next block
 * @prev: pointer to prev block
 * @free: flag for if space is free
 * @data: block data
 * @ptr: pointer
 */
typedef struct block_s
{
	size_t size;
	struct block *next;
	struct block *prev;
	int free;
	char data[1];
	void *ptr;
} block;

#define BLOCK_SIZE sizeof(struct block)
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
void *_calloc(size_t nmemb, size_t size);
void *_realloc(void *ptr, size_t size);

#endif /* __MALLOC_H_ */
