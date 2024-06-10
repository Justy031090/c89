#ifndef __WS2_H__
#define __WS2_H__
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void SwapInt(int *val1, int *val2);

void CopyArray(int *source, int *target, int size);

void PrintAddresses();

void SwapTwoSize_t(size_t *ptr1, size_t *ptr2);

void SwapTwoPointers(size_t **ptr1, size_t **ptr2);

size_t StrLen(const char *str);

int StrCmp(const char* s1, const char* s2);

#endif /* __WS2_H__ */
