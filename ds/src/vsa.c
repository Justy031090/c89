/*			.. Variable Memory Allocator Implementation ..
(\.../)		.. Authored by Michael Bar 02/08/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/

#include <stddef.h>/*size_t*/
#include <assert.h>

#include "vsa.h"
#define ALIGNMENT sizeof(size_t)
#define HEADER_SIZE sizeof(vsa_t)
#define KEY 123456789

struct vsa
{
	long mem_count;
	#ifndef NDBUG
	size_t magic_num;
	#endif
};

static long _Abs (long x);
static vsa_t *_JumpNext(vsa_t *index);
static long _FragmentationFix(vsa_t *runner, vsa_t *next_index);
static size_t _AlignBlock(size_t size_of_block);

vsa_t *VSAInit(void *memory_pool, size_t mem_size)
{
	vsa_t *first_index = NULL;
	vsa_t *last_index = NULL;
	size_t two_headers = HEADER_SIZE * 2;
	
	if(NULL == memory_pool || mem_size < two_headers + ALIGNMENT)
	{
		return NULL;
	}
	
	first_index = (vsa_t *)memory_pool;
	first_index->mem_count = mem_size - two_headers - (mem_size % ALIGNMENT);
	
	last_index = ((vsa_t *)(char *)memory_pool + (first_index->mem_count + HEADER_SIZE));
	last_index->mem_count = 0;  
	return first_index;
}

void VSAFreeBlock(void *to_free)
{
	vsa_t *free_index = NULL;
	assert(NULL != to_free);
	
	free_index = (vsa_t *)((char *)to_free - HEADER_SIZE);
	
	assert(free_index->magic_num == KEY);
	free_index->mem_count = _Abs(free_index->mem_count);
}

void *VSAAllocate(vsa_t *vsa, size_t size_of_block)
{
	vsa_t *runner = vsa;
	vsa_t *next_index = _JumpNext(runner);
	long temp;
	
	size_of_block = _AlignBlock(size_of_block);
	
	while (0 != next_index->mem_count && (runner->mem_count < (long)size_of_block) && runner->mem_count != 0)
	{
		if(runner->mem_count < 0)
		{
			runner = _JumpNext(runner);
			next_index = _JumpNext(runner);
		}
		else
		{
			if (next_index->mem_count > 0)
			{
				runner->mem_count += next_index->mem_count + HEADER_SIZE;
			}
			else
			{
				runner = _JumpNext(runner);
				next_index = _JumpNext(runner);
			}
		}
	}
	
	if(runner->mem_count >= ((long)size_of_block))
	{	
		if (runner->mem_count != (long)size_of_block)
		{
			temp = runner->mem_count - HEADER_SIZE - size_of_block;
			((vsa_t *)(((char *)runner) + HEADER_SIZE + size_of_block ))->mem_count = temp;
		}
		#ifndef NDBUG
		runner->magic_num = KEY;
		#endif
		runner->mem_count = -size_of_block*(temp > 0) + (-runner->mem_count * (temp <=0));
		return (void *)(((char *)runner)+ HEADER_SIZE);
	}
	
	return NULL;
}
	
size_t VSALargestFreeBlock(const vsa_t *vsa)
{
	vsa_t *runner = (vsa_t *)vsa;
	vsa_t *next_index = _JumpNext(runner);
	long largest_block = 0;
	
	while(0 != runner->mem_count)
	{
		if(runner->mem_count < 0)
		{
			runner = _JumpNext(runner);
			next_index = _JumpNext(runner);
			continue;
		}
		
		if(next_index->mem_count > 0)
		{
			runner->mem_count = _FragmentationFix(runner, next_index);
			next_index = _JumpNext(runner);
		}
		if(runner->mem_count > largest_block)
		{
			largest_block = runner->mem_count;
			runner = _JumpNext(runner);
			next_index = _JumpNext(runner);
		}
	}
	
	return largest_block;
}

static long _Abs (long x)
{
	return x >= 0 ? x : -x;
}

static vsa_t *_JumpNext(vsa_t *index)
{
	return (vsa_t *)((char *)index + _Abs(index->mem_count) + HEADER_SIZE);
}

static long _FragmentationFix(vsa_t *runner, vsa_t *next_index)
{
	return runner->mem_count += next_index->mem_count + HEADER_SIZE;	
}

static size_t _AlignBlock(size_t size_of_block)
{
	size_of_block = (size_of_block + ALIGNMENT - 1) & ~(ALIGNMENT - 1 );
	return size_of_block;
}
