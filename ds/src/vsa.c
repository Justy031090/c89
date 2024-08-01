/*			.. Variable Memory Allocator Implementation ..
(\.../)		.. Authored by Michael Bar 28/07/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/

#include <stddef.h>/*size_t*/

#include "vsa.h"

struct vsa
{
	long mem_count;
};

vsa_t *VSAInit(void *memory_pool, size_t mem_size)
{
	vsa_t *last_index = NULL;
	((vsa_t *)memory_pool)->mem_count = mem_size - (2 * (sizeof(vsa_t)));
	last_index = ((vsa_t *)(char *)memory_pool + mem_size - sizeof(vsa_t));
	last_index->mem_count = 0;  
		
	return (vsa_t *)memory_pool;
}

void *VSAAllocate(vsa_t *vsa, size_t size_of_block)
{
	while(vsa->mem_count < size_of_block || vsa->mem_count == 0)
	{
		vsa = vsa + mem_count +1;
				
	}
}

void VSAFreeBlock(void *to_free)
{
	
}
size_t VSALargestFreeBlock(const vsa_t *vsa)
{
	return 0;
}

