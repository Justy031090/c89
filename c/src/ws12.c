#include "ws12.h"

void *MemSet(void *str, int c, size_t n)
{	
	size_t i = 0;
	unsigned char *p = str;
	size_t to_align_start = (size_t)p%(WORD_SIZE);
	
	if(0 != to_align_start && n > WORD_SIZE)
	{
		for(;0 < to_align_start; --to_align_start, --n, ++p)
		{
			*p = c;
		}
	}
	
	for(; i <(n/WORD_SIZE); --n, ++p, ++i)
	{
		*p = c;
	}
	
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
	
	if(0 != to_align_start && n >= WORD_SIZE)
	{
		for(;0 < to_align_start; --to_align_start, --n, ++p, ++q)
		{
			*q = *p;
		}
	}
	
	for(; i <(n/WORD_SIZE); --n, ++p, ++q)
	{
		*q = *p;	
	}
	
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
	char *frm = (char *)src;
	char *to = (char *)dest;
	
	if((frm > to) && (to-frm) < (long int) n)
	{
		for(i = n-1; i > 0; --i)
		{
			to[i] = frm[i];
		}
		
		to[n] = '\0';
		return dest;
	}
	
	/* q-----p overlap*/
	if((frm < to) && (frm-to) < (long int) n)
	{
		for(i = 0; i < n; ++i)
		{
			to[i] = frm[i];
		}
		
		to[n] = '\0';
		return dest;
	}
	
	/* no overlaps :) */
	MemCpy(dest, src, n);
	return dest;
}


