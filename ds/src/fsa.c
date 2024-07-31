/*			.. Fixed-Size Allocator Implementation ..
(\.../)		.. Authored by Michael Bar 31/07/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/

struct fsa
{
    size_t next_free; 
};

size_t FSAMinPoolSize(size_t size_of_block size_t num_of_blocks)
{
		return (size_of_blocks*num_of_blocks) + sizeof(fsa_t);
}

void *FSAAllocateBlock(fsa_t *fsa)
{
	return (void *)fsa->next_free;
}

size_t FSACountNumOfFreeBlocks(const fsa_t *fsa);

void FSAFreeBlock(fsa_t *fsa, void *to_free)
{
	
}

fsa_t *FSAInit(void *memory_pool, size_t size_of_block, size_t num_of_blocks)
{
	int i = 0;
	fsa_t new_fsa;
	new_fsa->next_free = 0;
	assert(NULL != memory_pool);
	*memory_pool = new_fsa;
	memory_pool = memory_pool + sizeof(new_fsa);
	while(i<num_of_blocks)
	{
		*memory_pool = i;
		memory_pool += size_of_block;
		++i;		
	}
	memory_pool +
	return new_fsa;
}
