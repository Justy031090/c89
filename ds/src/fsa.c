/*			.. Fixed-Size Allocator Implementation ..
(\.../)		.. Authored by Michael Bar 01/08/2024 .. 
(=';'=) 	.. code reviewd by Tamir 04/08/2024..
(")-("))	.. The only hard day was yesterday ! ..
*/


#include "fsa.h"

#define BLOCK_SIZE (sizeof(size_t))

struct fsa
{
    size_t next_free; 
};

static size_t _AlignBlock(size_t size_of_block);

size_t FSAMinPoolSize(size_t size_of_block, size_t num_of_blocks)
{
		size_t block = _AlignBlock(size_of_block);
		assert(0 != num_of_blocks);
		return (block*num_of_blocks) + sizeof(fsa_t);
}

void *FSAAllocateBlock(fsa_t *fsa)
{
	char *block = NULL;
	if( 0 ==fsa->next_free)
		return NULL;
	
	block = ((char *)fsa +(fsa->next_free));
	fsa->next_free = *(size_t *)block;
	return (void *)block;
}

size_t FSACountNumOfFreeBlocks(const fsa_t *fsa)
{
	size_t count = 0;
	size_t runner = 0;
	assert(NULL != fsa);
	runner = fsa->next_free;
	
	while(0 != runner)
	{
		runner = *(size_t *)(((char *)fsa) + runner);
		++count;
	}
	return count;
}

void FSAFreeBlock(fsa_t *fsa, void *to_free)
{
	size_t *free_block = NULL;
	assert(NULL != fsa);
	assert(NULL != to_free);
	free_block = (size_t *)to_free;
	*free_block = fsa->next_free;
	fsa->next_free = ((size_t)fsa - (size_t)to_free);
}

fsa_t *FSAInit(void *memory_pool, size_t size_of_block, size_t num_of_blocks)
{
	size_t i = 0;
	size_t *insert =(size_t *)memory_pool + 1;
	size_t block = _AlignBlock(size_of_block);
	assert(0 != num_of_blocks);
	
	((fsa_t *)memory_pool)->next_free = BLOCK_SIZE;
	
	while(i<num_of_blocks-1)
	{
		*insert = (i+1)*size_of_block + BLOCK_SIZE;
		insert = (size_t *)(((char *)insert) + size_of_block);
		++i;		
	}
	*insert = 0;
	return (fsa_t *)memory_pool;
}

static size_t _AlignBlock(size_t size_of_block)
{
	size_of_block = (size_of_block + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1 );
	return size_of_block;
}


