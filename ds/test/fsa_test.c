#include <stddef.h>
#include <stdlib.h>
#include "fsa.h"


int main ()
{
	size_t min_size = FSAMinPoolSize(8, 10);
	char *pool = malloc(min_size);
	fsa_t *fsa = FSAInit(pool, 8, 8);
	char *first_block = NULL;
	char *sec_block = NULL;
	size_t size_after_allocation_1 = 0;
	size_t size_after_allocation_2 = 0;
	size_t size_after_free_1 = 0 ;
	size_t size_after_free_2 = 0;
	
	if(min_size == 80 + sizeof(fsa_t))
	{
		printf("FSA Min Pool Size Success\n");
	}
	
	if(NULL != fsa)
	{
		printf("FSA Init Sucess\n");
	}
	
	first_block = FSAAllocateBlock(fsa);
	
	size_after_allocation_1 = FSACountNumOfFreeBlocks(fsa);
	
	sec_block = FSAAllocateBlock(fsa);
	
	size_after_allocation_2 = FSACountNumOfFreeBlocks(fsa);
	
	FSAFreeBlock(fsa, &block);
	
	size_after_free_1 = FSACountNumOfFreeBlocks(fsa);
	
	FSAFreeBlock(fsa, &block2);
	
	size_after_free_2 = FSACountNumOfFreeBlocks(fsa);
	
	if(9 == size_after_allocation_1 && 8 == size_after_allocation_2)
	{
		printf("FS Allocate Blocks Success\n");
	}
	
	if(9 == size_after_free_1 && 10 == size_after_free_2)
	{
		printf("FS Free Block Success\n");
		printf("FS Count Free Blocks Success\n");
	}
	
	return 0;
}
