#include "malloc.h"

static void *base;

/**
 * split_block - splits block
 *
 * @chunk: chunk to split
 * @size: size of chunk
 */
void split_block(struct block *chunk, size_t size)
{
	struct block *new_block;

	new_block = (struct block *)(chunk->data + size);
	new_block->size = chunk->size - BLOCK_SIZE;
	new_block->next = chunk->next;
	new_block->prev = chunk;
	new_block->free = 1;
	new_block->ptr	= new_block->data;
	if (new_block->next)
		new_block->next->prev = new_block;
	chunk->size	= size;
	chunk->next	= new_block;
}

/**
 * find_unused_block - finds unused block to allocate
 *
 * @last: pointer to last block
 * @size: size of block
 * Return: found block
 */
block *find_unused_block(struct block **last, size_t size)
{
	struct block *traverse = base;

	while (traverse && !(traverse->free && traverse->size >= size))
	{
		*last = traverse;
		traverse = traverse->next;
	}
	return (traverse);
}

/**
 * extend_heap - calls sbrk to extend size of heap
 *
 * @last: last block allocated
 * @size: size requested
 * Return: new block
 */
block *extend_heap(struct block *last, size_t size)
{
	struct block *chunk;
	void *test;

	chunk = sbrk(0);
	test = sbrk(BLOCK_SIZE + size);
	if (test == (void *) -1)
		return (NULL);

	chunk->size = size;
	chunk->next = NULL;
	chunk->prev = last;
	chunk->ptr = chunk->data;
	if (last)
		last->next = chunk;
	chunk->free = 0;
	return (chunk);
}

/**
 * naive_malloc - dynamically allocate memory to the heap
 *
 * @size: number of bytes to allocate
 * Return: pointer to payload
 */
void *naive_malloc(size_t size)
{
	struct block *chunk;
	struct block *last;

	size = align(size);
	if (base)
	{
		last = base;
		chunk = find_unused_block(&last, size);
		if (chunk)
		{
			if ((chunk->size - size) >= (BLOCK_SIZE + 4))
				split_block(chunk, size);
			chunk->free = 0;
		}
		else
		{
			chunk = extend_heap(last, size);
			if (!chunk)
				return (NULL);
		}
	}
	else
	{
		chunk = extend_heap(NULL, size);
		if (!chunk)
			return (NULL);
		base = chunk;
	}
	return (chunk->data);
}
