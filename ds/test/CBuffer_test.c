
#include <stdio.h>
#include "CBuffer.h"


int main ()
{	
	size_t is_empty = -1;
	size_t is_empty_2 = -1;
	long int x = 8;
	size_t size1 = -1;
	int read_buffer[80];
	size_t free1 = -1;
	size_t free2 = -1;
	size_t free3 = -1;
	size_t free4 = -1;
	
	
	cbuffer_t *buffer = CBufferCreate(80);
	if(buffer)
		printf("Succesfully Created\n");
	else
		printf("Failed to Create Buffer\n");
		
	free1 = CBufferFreeSpace(buffer);
	size1 = CBufferBufSize(buffer);
	is_empty = CBufferIsEmpty(buffer);
	CBufferWrite(buffer, 8, &x);
	
	free2 = CBufferFreeSpace(buffer);
	is_empty_2 = CBufferIsEmpty(buffer);
	if(is_empty && !(is_empty_2))
		printf("Is Empty Checked Succesfully\n");
	else
		printf("Is Empty Function Failed\n");
		
	CBufferWrite(buffer, 8, &x);
	free3 = CBufferFreeSpace(buffer);
	CBufferRead(buffer, 8, read_buffer);
	
	free4 = CBufferFreeSpace(buffer);
	
	printf("%d   %d   %d   %d\n", free1, free2, free3, free4);
	if(free1 == 80 && free2 == 72 && free3 == 64 && free4 == 72)
		printf("Free Space Checked Succesfully\n");
	else
		printf("Free Space Function Failed\n");
	
	if(size1 == 80)
		printf("Size Checked Succesfully\n");
	else
		printf("Size Function Failed\n");
	
	CBufferDestroy(buffer);
	return 0;
}





