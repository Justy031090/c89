#include <stddef.h>
#include <stdio.h>

#define EIGHT_BIT (8)

void *MemSet(void *str, int c, size_t n)
{	
	int i = 0;
	unsigned char *p = str;
	size_t to_align_start = (size_t)p%(EIGHT_BIT);
	
	/*fill the unaligned first n-bytes, byte by byte*/
	for(;0 < to_align_start; --to_align_start, --n, ++p)
	{
		*p = c;
	}
	
	/*Copy chunks of words*/
	for(; i <(n/EIGHT_BIT); --n, ++p, ++i)
	{
		*p = c;
	}
	
	/*fill the unaligned last n-bytes, byte by byte*/
	for(; 0 < n; --n, ++p)
	{
		*p = c;
	}
	
	return str;
}

void *MemCpy(void *dest ,const void *src, size_t n)
{
	size_t i = 0;
	char *p = (char *)src;
	char *q = (char *)dest;
	size_t to_align_start = (size_t)src%(EIGHT_BIT);
	
	/*fill the unaligned first n-bytes, byte by byte*/
	for(;0 < to_align_start; --to_align_start, --n, ++p, ++q)
	{
		*q = *p;
	}
	
	/*Copy chunks of words*/
	for(; i < (n/8); --n, ++p, ++q)
	{
		*q = *p;	
	}
	
	/*fill the unaligned last n-bytes, byte by byte*/
	for(; 0 < n; --n, ++p, ++q)
	{
			*q = *p;
	}
	*q = '\0';
	
	return dest;
}

void *MemCpy(void *dest ,const void *src, size_t n)
{
	size_t i = 0;
	char *p = (char *)src;
	char *q = (char *)dest;
	size_t to_align_start = (size_t)src%(EIGHT_BIT);
	
	/*fill the unaligned first n-bytes, byte by byte*/
	for(;0 < to_align_start; --to_align_start, --n, ++p, ++q)
	{
		*q = *p;
	}
	
	/*Copy chunks of words*/
	for(; i < (n/8); --n, ++p, ++q)
	{
		*q = *p;	
	}
	
	/*fill the unaligned last n-bytes, byte by byte*/
	for(; 0 < n; --n, ++p, ++q)
	{
			*q = *p;
	}
	*q = '\0';
	
	return dest;
}



int main()
{	
	char *source = "Hello World";
	char destination[13];
	
	MemCpy(destination, source, 13);
	
	
	return 0;
}
