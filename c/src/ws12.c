#include <stddef.h>
#include <stdio.h>

#define WORD_SIZE (8)

void *MemSet(void *str, int c, size_t n)
{	
	int i = 0;
	unsigned char *p = str;
	size_t to_align_start = (size_t)p%(WORD_SIZE);
	
	/*fill the unaligned first n-bytes, byte by byte*/
	if(0 != to_align_start && n > 8)
	{
		for(;0 < to_align_start; --to_align_start, --n, ++p)
		{
			*p = c;
		}
	}
	
	/*Copy chunks of words*/
	for(; i <(n/WORD_SIZE); --n, ++p, ++i)
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
	size_t to_align_start = (size_t)src%(WORD_SIZE);
	
	/*fill the unaligned first n-bytes, byte by byte*/
	
	if(0 != to_align_start && n > 8)
	{
		for(;0 < to_align_start; --to_align_start, --n, ++p, ++q)
		{
			*q = *p;
		}
	}
	
	/*Copy chunks of words*/
	for(; i <(n/WORD_SIZE); --n, ++p, ++q)
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




void *MemMove(void *dest ,const void *src, size_t n)
{
	size_t i = 0;
	char *p = (char *)src;
	char *q = (char *)dest;
	size_t to_align_start = (size_t)src%(WORD_SIZE);
	
	/* p-----q overlap*/
	if(p > q && q-p < n)
	{
		for(i = n-1; i>=0 ; --i)
		{
			q[i] = p[i];
		}
		q[n] = '\0';
		return dest;
	}
	i = 0;
	
	/* q-----p overlap*/
	if(p < q && p-q < n)
	{
		for(i = 0; i<n; ++i)
		{
			q[i] = p[i];
		}
		q[n] = '\0';
		return dest;
	}
	
	/* no overlaps :) */
	MemCpy(dest, src, n);
	return dest;
}

int main()
{	
	char *source = "Hello World";
	char destination[13];
	
	printf("%s\n", (char *)MemMove(destination, source, 12));
	printf("%s\n", (char *)MemMove(destination, source, 1));
	printf("%s\n", (char *)MemMove(destination, source, 0));
	printf("%s\n", (char *)MemMove(destination, source, 11));
	printf("%s\n", (char *)MemMove(destination, source, 13));
	printf("%s\n", (char *)MemMove(destination, source, 6));


	
	
	return 0;
}
