
#include <stdio.h>
#include "CBuffer.h"


int main ()
{	
	size_t is_empty = -1;
	size_t is_empty_2 = -1;
	long int x = 8;
	size_t size1 = -1;
	int read_buffer[180];
	size_t free1 = -1;
	size_t free2 = -1;
	size_t free3 = -1;
	size_t free4 = -1;
	ssize_t write = 0;
	ssize_t read = 0;
	
	
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
		
	write = CBufferWrite(buffer, 8, &x);
	free3 = CBufferFreeSpace(buffer);
	read = CBufferRead(buffer, 16, read_buffer);
	printf("Read Buffer    %lu    , %lu\n", (long int)read_buffer[0],(long int)read_buffer[1]);
	printf("Write & Read    %ld    , %ld\n", write, read);
	free4 = CBufferFreeSpace(buffer);
	
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





