/*			.. Variable Memory Allocator Implementation ..
(\.../)		.. Authored by Michael Bar 02/08/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/

#include <stddef.h>/*size_t*/
#include <assert.h>

#include "vsa.h"
#define ALIGNMENT sizeof(size_t)
#define MIN_BLOCK_SIZE (16)

struct vsa
{
	long mem_count;
};

static long _Abs (long x);
static vsa_t *_JumpNext(vsa_t *index);
static void _FragmentationFix(vsa_t *runner, vsa_t *next_index);
static size_t _AlignBlock(size_t size_of_block);

vsa_t *VSAInit(void *memory_pool, size_t mem_size)
{
	vsa_t *first_index = NULL;
	vsa_t *last_index = NULL;
	
	if(NULL == memory_pool || mem_size < sizeof(vsa_t) * 2)
	{
		return NULL;
	}
	
	first_index = (vsa_t *)memory_pool;
	last_index = (vsa_t *)((char *)memory_pool + mem_size - sizeof(vsa_t));
	
	first_index->mem_count = mem_size - sizeof(vsa_t);
	last_index->mem_count = 0;  
		
	return first_index;
}

void VSAFreeBlock(void *to_free)
{
	vsa_t *free_index = NULL;
	assert(NULL != to_free);
	
	free_index = (vsa_t *)((char *)to_free - sizeof(vsa_t));
	free_index->mem_count = _Abs(free_index->mem_count);
}

void *VSAAllocate(vsa_t *vsa, size_t size_of_block)
{
	vsa_t *runner = vsa;
	vsa_t *next_index = _JumpNext(runner);
	size_of_block = _AlignBlock(size_of_block);
	
	while(0 != next_index->mem_count)
	{	
		if(runner->mem_count > 0 && next_index->mem_count > 0)
		{
				_FragmentationFix(runner, next_index);
				next_index = _JumpNext(runner);
				continue;		
		}
		
		if(runner->mem_count < 0 || runner->mem_count < (long)size_of_block)
		{
			runner = _JumpNext(runner);
			next_index = _JumpNext(runner);
		}
		
		else
		{
			runner->mem_count = size_of_block + sizeof(vsa_t);
			return (void *)((char *)runner + sizeof(vsa_t));
		}
	}
	
	if(next_index->mem_count == 0 && (runner->mem_count < 0 || runner->mem_count < (long)size_of_block))
		return NULL;
	
	return NULL;		
}		


size_t VSALargestFreeBlock(const vsa_t *vsa)
{
	vsa_t *runner = (vsa_t *)vsa;
	vsa_t *next_index = _JumpNext(runner);
	long largest_block = 0;
	
	while(next_index->mem_count != 0)
	{
		if(runner->mem_count > largest_block)
		{
			largest_block = runner->mem_count;
		}
		if(runner->mem_count > 0 && next_index->mem_count > 0)
		{
			_FragmentationFix(runner, next_index);
			next_index = _JumpNext(runner);
			continue;		
		}
		
		runner = _JumpNext(runner);
		next_index = _JumpNext(runner);
	}
	return largest_block;
}

static long _Abs (long x)
{
	return x >= 0 ? x : -x;
}

static vsa_t *_JumpNext(vsa_t *index)
{
	return (vsa_t *)((char *)index + _Abs(index->mem_count));
}

static void _FragmentationFix(vsa_t *runner, vsa_t *next_index)
{
	runner->mem_count = runner->mem_count + next_index->mem_count;	
}

static size_t _AlignBlock(size_t size_of_block)
{
	size_of_block = (size_of_block + ALIGNMENT - 1) & ~(ALIGNMENT - 1 );
	return size_of_block >= MIN_BLOCK_SIZE ? size_of_block : MIN_BLOCK_SIZE;
}
