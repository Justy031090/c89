/*			.. Variable Memory Allocator Implementation ..
(\.../)		.. Authored by Michael Bar 02/08/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/

#include <stddef.h>/*size_t*/
#include <math.h>

#include "vsa.h"

struct vsa
{
	long mem_count;
};

static long _Abs (long x);

static vsa_t *_JumpNext(vsa_t *index);

static void _FragmentationFix(vsa_t *runner, vsa_t *next_index);



vsa_t *VSAInit(void *memory_pool, size_t mem_size)
{
	vsa_t *first_index = (vsa_t *)memory_pool;
	vsa_t *last_index = NULL;
	first_index->mem_count = mem_size - (2 * (sizeof(vsa_t)));
	last_index = ((vsa_t *)(char *)memory_pool + (mem_size-1) - sizeof(vsa_t));
	last_index->mem_count = 0;  
		
	return (vsa_t *)memory_pool;
}

void VSAFreeBlock(void *to_free)
{
	((vsa_t *)((char *)to_free - sizeof(vsa_t)))->mem_count *= -1;
}

void *VSAAllocate(vsa_t *vsa, size_t size_of_block)
{
	vsa_t *runner = vsa;
	vsa_t *next_index = _JumpNext(runner);
	
	while(next_index->mem_count != 0)
	{	
		if(runner->mem_count > 0 && next_index->mem_count > 0)
		{
				_FragmentationFix(runner, next_index);
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
		}
	}
	
	if(next_index->mem_count == 0 && (runner->mem_count < 0 || runner->mem_count < (long)size_of_block))
		return NULL;
	
	return (void *)(runner + sizeof(vsa_t));		
}		


size_t VSALargestFreeBlock(const vsa_t *vsa)
{
	vsa_t *runner = (vsa_t *)vsa;
	vsa_t *next_index = _JumpNext(runner);
	size_t largest_block = 0;
	
	while(next_index->mem_count != 0)
	{
		if(runner->mem_count > (long)largest_block)
		{
			largest_block = runner->mem_count;
		}
		if(runner->mem_count > 0 && next_index->mem_count > 0)
		{
			_FragmentationFix(runner, next_index);
			continue;		
		}
	}
	return largest_block;
}

static long _Abs (long x)
{
	return x >= 0 ? x : (-x);
}

static vsa_t *_JumpNext(vsa_t *index)
{
	return index + _Abs(index->mem_count);
}

static void _FragmentationFix(vsa_t *runner, vsa_t *next_index)
{
	runner->mem_count = runner->mem_count + next_index->mem_count;
	next_index = _JumpNext(runner);		
}
