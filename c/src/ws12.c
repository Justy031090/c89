#include <stdint.h> /*int8_t*/
#include <string.h> /*strlen*/ 
#include <assert.h> /*assert*/

#include "ws12.h"
#define WORD_SIZE ((sizeof(size_t)))

void *MemSet(void *str, int c, size_t n)
{	
	size_t i = 0;
	char *p = (char *)str;
	size_t to_align_start = (size_t)p%(WORD_SIZE);
	assert(NULL != str);

	for(;0 < to_align_start && n >= WORD_SIZE; --to_align_start)
	{
			*p = c;
			--n;
			++p;
	}
	for(; i <(n/WORD_SIZE); ++i)
	{
		*(int8_t *)p = *(int8_t *)c;	
		n = n - WORD_SIZE;
		p = p + WORD_SIZE;
	}
	
	for(; 0 < n; --n)
	{
		*p = c;
		++p;
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
	
	for(;0 < to_align_start && n >= WORD_SIZE ; --to_align_start)
	{
		*q = *p;
		++p;
		++q;
		--n;
	}
	
	for(i = 0 ;i < (n/WORD_SIZE); i++)
	{
		*(int8_t *)q = *(int8_t *)p;	
		n = n - WORD_SIZE;
		p = p + WORD_SIZE;
		q = q + WORD_SIZE;
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
	char *src_ = (char *)src;
	char *dest_ = (char *)dest;
	size_t to_align_start = 0;
	assert(NULL != src);
	if(n > strlen(src))
	{
		return src_;
	}
	/*Destination overlaps with Source*/
	/*Need to align & copy from the end to the start*/
	if((src_ > dest_) && (dest_ - src_) < (long int) n)
	{
		dest_ = dest_ + n;
		src_ = src_ +n ;
		to_align_start = WORD_SIZE - ((size_t)src%(WORD_SIZE));
		
		/*copy untill first word boundary backwards*/
		for(;0 < to_align_start && n >= WORD_SIZE ; --to_align_start)
		{
			*dest_ = *src_;
			--n;
			--dest_;
			--src_;
		}
		
		/*copy word-size chunks backwards*/
		for(i = 0 ;i < (n/WORD_SIZE); i++)
		{
			n = n - WORD_SIZE;
			src_ = src_ - WORD_SIZE;
			dest_ = dest_ - WORD_SIZE;
			*(int8_t *)dest_ = *(int8_t *)src_;	
		}
		/*copy the left bytes backwards*/
		for(; 0 < n; )
		{
			*dest_ = *src_;
			--dest_;
			--src_;
			--n;
		}
		return dest_;
	}
	/*Source overlaps with Destination*/
	MemCpy(dest, src, n);
	
	return dest;
}



