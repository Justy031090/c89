#ifndef __WS12_H__
#define __WS12_H__

#include <stddef.h> /*size_t*/


void *MemSet(void *str, int c, size_t n);

void *MemCpy(void *dest ,const void *src, size_t n);

void *MemMove(void *dest ,const void *src, size_t n);

#endif /* __WS12_H__ */
