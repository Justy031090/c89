/*					Dynamic Vector Implementation.
(\.../)		.. Authored by Michael Bar 15/07/2024
(=';'=) .. code reviewd by ..
(")-("))	..
*/

#include "CBuffer.h"

struct cbuffer
{
	size_t capacity;
	size_t front;
	size_t rear;
	char buffer[1];
};

cbuffer_t *CBufferCreate(size_t capacity)
{
	char *buff = malloc(sizeof(cbuffer.buffer) + capacity);
	cbuffer_t new_buffer = malloc(sizeof(cbuffer_t);
	if(NULL == new_buffer)
	{
		return NULL:
	}
	
	new_buffer.rear = -1;
	new_buffer.front = -1;
	new_buffer.capacity = capacity;
	new_buffer.buffer =  *buff;
	
	return new_buffer;
}


void CBufferDestroy(cbuffer_t *buffer)
{
	free(buffer.buffer);
	buffer.buffer = NULL;
	free(buffer);
	buffer = NULL;	
}


int CBufferIsEmpty(const cbuffer_t *buffer)
{
	if(buffer.rear == -1 && buffer.front == -1)
	{
		return 1;
	}
	return 0;
}


size_t  CBufferBufSize(const cbuffer_t *buffer)
{
	return buffer.capacity;
}

size_t CBufferFreeSpace(const cbuffer_t *buffer)
{
	size_t space = buffer.front-buffer.back;
	return space > 0 ? space : -space;
}

ssize_t write(cbuffer_t *buffer, size_t n_bytes, const void *src)
{
	char *source = (char *)src;
	size_t free_space = CBufferFreeSpace(buffer);
	ssize_t written_bytes = 0;
	if(1 == CBufferIsEmpty(buffer))
	{
		buffer.rear = 0;
		buffer.front = 0;
	}
	
	if(0 == free_space)
		return -1;
	
	while(n_bytes > 0 )
	{
		buffer.buffer[rear] = source;
		++source;
		++buffer.rear;
		++written_bytes;
		--n_bytes;
	}
	return written_bytes;
}
ssize_t read(cbuffer_t *buffer, size_t n_bytes, void *dst)
{
	char *destionation = (char *)src;
	ssize_t read_bytes = 0;
	if(1 == CBufferIsEmpty(buffer))
		return -1;
	
	while(n_bytes > 0 )
	{

		buffer.buffer[front] = destionation[read_bytes];
		++buffer.front;
		++read_bytes;
		--n_bytes;
	}
	
	if(CBufferFreeSpace(buffer) == buffer.capacity - 1)
	{
		buffer.front = -1;
		buffer.read = -1;
	}
		
	return read_bytes;
}




