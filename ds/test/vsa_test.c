#include <printf.h>
#include <stdlib.h>


#include "vsa.h"

int main ()
{
	vsa_t *vsa = NULL;
	size_t *pool = malloc(200);
	if(!pool)
		return 1;
		
	vsa = VSAInit(pool, 200);

	
	
	
	free(pool);

	return 0;
}



void *VSAAllocate(vsa_t *vsa, size_t size_of_block);
void VSAFreeBlock(void *to_free);
size_t VSALargestFreeBlock(const vsa_t *vsa);
