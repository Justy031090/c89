#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsa.h"


int main ()
{
	size_t min_size = FSAMinPoolSize(8, 10);
	char *pool = malloc(min_size);
	fsa_t *fsa = FSAInit(pool, 8, 10);
	size_t *first_block = NULL;
	size_t *sec_block = NULL;
	size_t size_after_allocation_1 = 0;
	size_t size_after_allocation_2 = 0;
	size_t size_after_free_1 = 0 ;
	size_t size_after_free_2 = 0;
	size_t block1 = 5;
	size_t block2 = 7;
	
	if(min_size == 80 + 8)
	{
		printf("FSA Min Pool Size Success\n");
	}
	
	if(NULL != fsa)
	{
		printf("FSA Init Sucess\n");
	}
	
	first_block = FSAAllocateBlock(fsa);
	
	size_after_allocation_1 = FSACountNumOfFreeBlocks(fsa);
	printf("size after allocation 1 -  %lu\n", size_after_allocation_1);
	sec_block = FSAAllocateBlock(fsa);
	size_after_allocation_2 = FSACountNumOfFreeBlocks(fsa);
	printf("size after allocation 2 -  % lu \n", size_after_allocation_2);
	FSAFreeBlock(fsa, &first_block);
	size_after_free_1 = FSACountNumOfFreeBlocks(fsa);
	printf("Count after FIRST free %lu\n", size_after_free_1);
	FSAFreeBlock(fsa, &sec_block);
	size_after_free_2 = FSACountNumOfFreeBlocks(fsa);
	printf("Count after SEC free %lu\n", size_after_free_2);
	if(9 == size_after_allocation_1 && 8 == size_after_allocation_2)
	{
		printf("FS Allocate Blocks Success\n");
	}
	
	if(9 == size_after_free_1 && 10 == size_after_free_2)
	{
		printf("FS Free Block Success\n");
		printf("FS Count Free Blocks Success\n");
	}
	
	free(pool);
	return 0;
}
