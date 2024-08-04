#include <stdio.h> /*printf*/
#include "vsa.h"
int main()
{
	char mem_block[128];
	vsa_t *new_vsa = VSAInit((void *) mem_block, 128);
	void *a = NULL;
	void *b = NULL;
	void *c = NULL;
	void *d = NULL;
	void *e = NULL;
	if ((*((size_t *)new_vsa)) = 48)
	{
		printf("Init success\n");
	}
	else
	{
		printf("Init FAIL!!");
	}
	
	a = VSAAllocate(new_vsa, 7);
	b = VSAAllocate(new_vsa, 15);
	c = VSAAllocate(new_vsa, 7);
	d = VSAAllocate(new_vsa, 17);/*NULL*/

	if (NULL != c && NULL != a && NULL != b)
	{
		printf("Allocate success\n");
	}
	else
	{
		printf("Allocate FAIL!!\n");
	}
	
	if (0 == VSALargestFreeBlock(new_vsa))
	{
		printf("LargestFreeBlock full success\n");
	}
	else
	{
		printf("LargestFreeBlock full FAIL!!\n");
	}
	
	VSAFreeBlock(a);
	
	if (8 == VSALargestFreeBlock(new_vsa))
	{
		printf("Free success\n");
		printf("LargestFreeBlock notfull success\n");
	}
	d = VSAAllocate(new_vsa, 7);
	if (0 == VSALargestFreeBlock(new_vsa))
	{
		printf("ReAllocate success\n");
	}
	else
	{
		printf("ReAllocate FAIL!!\n");
	}
	VSAFreeBlock(a);
	VSAFreeBlock(a);
	e = VSAAllocate(new_vsa, 17);
	if (e != NULL && 0 == VSALargestFreeBlock(new_vsa))
	{
		printf("ReAllocate full  success\n");
	}
	return 0;
}

