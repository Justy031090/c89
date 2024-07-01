#include <stdint.h>
#include <assert.h>

#include "ws12.h"
#define WORD_SIZE (8)



/*asserts*/
/*if n > strlen return*/

void *MemSet(void *str, int c, size_t n)
{	
	
	
	size_t i = 0;
	char *p = (char *)str;
	size_t to_align_start = (size_t)p%(WORD_SIZE);
	assert(NULL != str);

	
	for(;0 < to_align_start && n >= WORD_SIZE; --to_align_start,  ++p)
	{
			*p = c;
			--n;
	}
	for(; i <(n/WORD_SIZE);++i, n=n- WORD_SIZE, p = p + WORD_SIZE)
	{
		*(int8_t *)p = *(int8_t *)c;	
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
	assert(NULL != src);
	
	if(n > strlen(src))
	{
		return p;
	}
	
	for(;0 < to_align_start && n >= WORD_SIZE ; --to_align_start,  ++p, ++q)
	{
		*q = *p;
		--n;
	}
	
	for(i = 0 ;i < (n/WORD_SIZE); i++)
	{
		*(int8_t *)q = *(int8_t *)p;	
		n = n - 8;
		p = p + 8;
		q = q + 8;
	}
	
	for(; 0 < n; )
	{
		*q = *p;
		++p;
		++q;
		--n;
	}
	
	
	return dest;
}


void *MemMove(void *dest ,const void *src, size_t n)
{
	
	size_t i = 0;
	char *frm = (char *)src;
	char *to = (char *)dest;
	
	assert(NULL != src);
	if((frm > to) && (to-frm) < (long int) n)
	{
		for(i = n-1; i > 0; --i)
		{
			to[i] = frm[i];
		}
		
		to[n] = '\0';
		return dest;
	}
	
	if((frm < to) && (frm-to) < (long int) n)
	{
		for(i = 0; i < n; ++i)
		{
			to[i] = frm[i];
		}
		
		to[n] = '\0';
		return dest;
	}
	
	MemCpy(dest, src, n);
	
	return dest;
}



