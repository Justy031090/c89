#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h>

typedef struct fsa fsa_t;


size_t FSAMinPoolSize(size_t size_of_block, size_t num_of_blocks);
fsa_t *FSAInit(void *memory_pool, size_t size_of_block, size_t num_of_blocks);
void *FSAAllocateBlock(fsa_t *fsa);
void FSAFreeBlock(fsa_t *fsa, void *to_free);
size_t FSACountNumOfFreeBlocks(const fsa_t *fsa);

#endif
