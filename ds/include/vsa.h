#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /*size_t*/

typedef struct vsa vsa_t;


vsa_t *VSAInit(void *memory_pool, size_t mem_size);
void *VSAAllocate(vsa_t *vsa, size_t size_of_block);
void VSAFreeBlock(void *to_free);
size_t VSALargestFreeBlock(const vsa_t *vsa);

#endif /*__VSA_H__*/
