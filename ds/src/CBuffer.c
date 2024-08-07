/*			.. Circular Buffer Implementation ..
(\.../)		.. Authored by Michael Bar 15/07/2024 .. 
(=';'=) 	.. code reviewd by Yonatan I. 16/07/2024..
(")-("))	.. The only hard day was yesterday ! ..
*/

#include <stdlib.h> /*Malloc*/
#include <assert.h>
#include "CBuffer.h"

struct cbuffer
{
	size_t capacity;
	size_t read_idx;
	size_t write_idx;
	char buffer[1];
};

cbuffer_t *CBufferCreate(size_t capacity)
{
	cbuffer_t *new_buffer = malloc(sizeof(cbuffer_t) + capacity);
	if(NULL == new_buffer)
	{
		return NULL;
	}
	
	new_buffer->write_idx = -1;
	new_buffer->read_idx = -1;
	new_buffer->capacity = capacity;
	
	return new_buffer;
}


void CBufferDestroy(cbuffer_t *buffer)
{

	assert(NULL != buffer);	
	free(buffer);
	buffer = NULL;	
}


int CBufferIsEmpty(const cbuffer_t *buffer)
{
	assert(NULL != buffer);	
	if(buffer->write_idx == -1 && buffer->read_idx == -1)
	{
		return 1;
	}
	return 0;
}


size_t  CBufferBufSize(const cbuffer_t *buffer)
{
	assert(NULL != buffer);	
	return buffer->capacity;
}

size_t CBufferFreeSpace(const cbuffer_t *buffer)
{
	assert(NULL != buffer);	
	return buffer->capacity - (buffer->write_idx - buffer->read_idx);
}

ssize_t CBufferWrite(cbuffer_t *buffer, size_t n_bytes, const void *src)
{
	char *source = (char *)src;
	size_t free_space = CBufferFreeSpace(buffer);
	ssize_t written_bytes = 0;
	assert(NULL != buffer);
	assert(NULL != src);
	if(1 == CBufferIsEmpty(buffer))
	{
		buffer->write_idx = 0;
		buffer->read_idx = 0;
	}
	
	if(0 == free_space)
		return -1;
	
	while(n_bytes > 0 && CBufferFreeSpace(buffer) > 0);
	{
		
		buffer->buffer[buffer->write_idx] = *source;
		++source;
		buffer->write_idx = (buffer->write_idx +1) % buffer->capacity;
		++written_bytes;
		--n_bytes;
		
	}
	return written_bytes;
}
ssize_t CBufferRead(cbuffer_t *buffer, size_t n_bytes, void *dst)
{
	char *destionation = (char *)dst;
	ssize_t read_bytes = 0;
	int is_empty_after_read = 0;
	
	assert(NULL != buffer);
	assert(NULL != dst);
	
	if(CBufferFreeSpace(buffer) == buffer->capacity - 1)
		is_empty_after_read = 1;
	if(1 == CBufferIsEmpty(buffer))
		return -1;
	
	while(n_bytes > 0 && buffer->read_idx != buffer->write_idx)
	{
		destionation[read_bytes] = buffer->buffer[buffer->read_idx];
		buffer->read_idx = (buffer->read_idx +1) % buffer->capacity;
		++read_bytes;
		--n_bytes;
	}
	
	if(1 == is_empty_after_read)
	{
		buffer->read_idx = -1;
		buffer->write_idx = -1;
	}
	
	return read_bytes;
}




