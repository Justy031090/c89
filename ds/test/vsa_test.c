#include <stdlib.h> /* malloc / free */
#include <stdio.h> /* print */

#include "vsa.h"

int main()
{
	size_t size_of_memory = 200;
	
	vsa_t *vsa = (void *)malloc(size_of_memory);
	
	size_t size1, size2, size3, size4;
	
	void *res_z;
	void *res_x;
	void *res_c;
	void *res_v;
	void *res_b;
	void *res_n;
	void *res_m;
	
	vsa = VSAInit(vsa, size_of_memory);
	
	size1 = VSALargestFreeBlock(vsa);
	
	res_z = VSAAllocate(vsa, 4);
	res_v = VSAAllocate(vsa, 12);
	res_x = VSAAllocate(vsa, 2);
	

	if (NULL != res_z && 168 == size1)
	{
		printf("Init Success\n");
		printf("Allocate Success\n");
	}
	else
	{
		printf("Init FAIL!!\n");
		printf("Allocate FAIL!!\n");
	}
	
	size2 = VSALargestFreeBlock(vsa);
	
	if (168 == size1 && 88 == size2)
	{
		printf("Largest Free Block Success\n");
	}
	else
	{
		printf("Largest Free Block FAIL!!\n");
	}
	
	res_c = VSAAllocate(vsa, 4);
	
	VSAFreeBlock(res_c);
	
	size3 = VSALargestFreeBlock(vsa);
	
	if (88 == size3)
	{
		printf("Free Block Success\n");
	}
	else
	{
		printf("Free Block FAIL!!\n");
	}
	
	res_b = VSAAllocate(vsa, 20);
	
	res_n = VSAAllocate(vsa, 50);
	
	res_m = VSAAllocate(vsa, 42);
	
	size3 = VSALargestFreeBlock(vsa);
	
	if(NULL == res_n && size3 == 0)
	{
		printf("Allocate overflow success\n");
		printf("LargestFree full success\n");
	}
	else
	{
		printf("Allocate overflow FAIL!!\n");
		printf("LargestFree full FAIL!!\n");
	}
	VSAFreeBlock(res_v);
	VSAFreeBlock(res_x);
	
	size4 = VSALargestFreeBlock(vsa);
	res_v = VSAAllocate(vsa, 12);
	res_x = VSAAllocate(vsa, 2);
	
	VSAFreeBlock(res_v);
	VSAFreeBlock(res_x);
	
	res_n = VSAAllocate(vsa, 38);
	
	if (40 == size4 && NULL != res_n)
	{
		printf("LargestFree defrag success\n");
		printf("Allocate defrag success\n");
	}
	else
	{
		printf("LargestFree defrag FAIL!!\n");
		printf("Allocate defrag FAIL!!\n");
	}
	
	
	free(vsa);
	
	return 0;
}


