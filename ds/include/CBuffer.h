#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include <stddef.h> /* size_t */
#include <sys/types.h> /*ssize_t*/


/* type of the cbuffer management struct */
typedef struct cbuffer cbuffer_t;


/* CbufferCreate:
Description - create the cbuffer data structure.
Params:
	size_t capacity - capacity of the buffer.
return value - cbuffer_t *
time complexity - O(1)
space complexity - O(1)
*/
cbuffer_t *CBufferCreate(size_t capacity);


/* CbufferDestroy:
Description - delete the cbuffer.
Params:
	cbuffer_t *cbuffer - management struct.
return value - NONE.
time complexity - O(1)
space complexity - O(1)
*/
void CBufferDestroy(cbuffer_t *cbuffer);


/* CBufferWrite:
Description - write element to cbuffer.
Params:
	cbuffer_t *cbuffer - management struct.
	size_t n_bytes - number of bytes to write.
	const char * src - pointer to the vlue to write.
return value - ssize_t.
time complexity - O(1)
space complexity - O(1)
*/
ssize_t CBufferWrite(cbuffer_t *cbuffer, size_t n_bytes, const void *src);


/* CBufferRead:
Description - read element from cbuffer.
Params:
	cbuffer_t *cbuffer - management struct.
	size_t n_bytes - number of bytes to read.
	const char *src - pointer to the vlue to read.
return value - ssize_t.
time complexity - O(1)
space complexity - O(1)
*/
ssize_t CBufferRead(cbuffer_t *cbuffer, size_t n_bytes, void *dest);


/* CBufferIsEmpty:
Description - returns 1 if empty 0 if not.
Params:
	const cbuffer_t *cbuffer - management struct.
return value - int.
time complexity - O(1)
space complexity - O(1)
*/
int CBufferIsEmpty(const cbuffer_t *cbuffer);


/* CBufferBufSize:
Description - returns capacity cbuffer.
Params:
	const cbuffer_t *cbuffer - management struct.
return value - size_t.
time complexity - O(1)
space complexity - O(1)
*/
size_t CBufferBufSize(const cbuffer_t *cbuffer);


/* CBufferFreeSpace:
Description - returns free space in the buffer.
Params:
	const cbuffer_t *cbuffer - management struct.
return value - size_t.
time complexity - O(1)
space complexity - O(1)
*/
size_t CBufferFreeSpace(const cbuffer_t *cbuffer);


#endif /*__CBUFFER_H__*/



