
#include "ws3.h"

 size_t StrLen(const char *str)
{
 	size_t length = 0;
 	while(*str != '\0')
 	{
 		length++;
 		str++;
 	}
 	return length;
}
 
char *StrCpy(char *dst, const char *src)
{	
    int len = 0;
	while (*src != '\0') 
	{
	        *dst = *src;
	        dst++;
	        src++;
	        len++;
	}
	
	*dst = '\0';
	dst -= len;
	return dst;
}

char *StrnCpy(char *dst, const char *src, size_t dsize)
{
	size_t i = 0;
	for(; i<dsize; i++)
	{
		if (*src != '\0')
		{
			*dst = *src;
			dst++;
			src++;
		}
		else
		{	
			*dst = 0;
			dst++;
		}
	
	}
	
	*dst = '\0';
	dst -= dsize;
	return dst;
}

int StrnCmp(const char *str1, const char *str2, size_t n)
{
	size_t i;	
	for(i=0; i<n; i++)
	{
		if(*str1 != *str2)
		{
			return (int)*str1 - (int)*str2;
		}
		if(*str1 == '\0' || *str2 == '\0' )
		{
			return 0;
		}
		str1++;
		str2++;
	}
	
	return 0;

}

int StrCaseCmp(const char *s1, const char *s2)
{
	for(;tolower(*s1) == tolower(*s2) && *s1 != '\0';)
	{
		s1++;
		s2++;
	}
	return (int)*s1 - (int)*s2;
}


char *StrChr(const char *s, int c)
{
	while(*s != '\0')
	{
		if(*s == c)
		{
			return (char *)s;
		}
		s++;
	}
	return NULL;
}

char *StrDup(const char *s)
{
	int length = StrLen(s);
	char *duplicate_string = (char *)malloc(sizeof(char)*length);
	return StrCpy(duplicate_string, s);
}

char *StrCat(char *dst, const char *src)
{
	size_t dst_length = StrLen(dst);
	size_t src_length = StrLen(src);
	size_t length = dst_length + src_length;
	
	while('\0' != *dst)
		dst++;
		
	while('\0' != *src)
	{
		*dst = *src;
		dst++;
		src++;
	}
	dst++;
	*dst = '\0';
	dst = dst - length -1;
	

	return dst;
	
}

char *StrnCat(char *dst, const char *src, size_t ssize)
{
	size_t dst_length = StrLen(dst);
	size_t src_length = StrLen(src);
	size_t length = dst_length + src_length;
	size_t i = 0;
	
	while('\0' != *dst)
		dst++;
		
	while(i<ssize && *src)
	{
		*dst = *src;
		dst++;
		src++;
		i++;
	}
	dst++;
	*dst = '\0';
	dst = dst - length -1;
	

	return dst;
}


char *StrStr(const char *haystack, const char *needle)
{
	size_t length = StrLen(needle);
	size_t counts = 0; 
	
	if(0 == length)
		return (char *)haystack;
	
	while('\0' !=*haystack)
	{
		if(counts == length)
			break;
		else if(*haystack == *needle) 
		{
			counts++;
			haystack++;
			needle++;
		}
		else
		{
			counts = 0;
			haystack++;
		}
	}
	
	if(counts == 0)
		return NULL;
	haystack -= counts;
	return (char *)haystack;
}


size_t StrSpn(const char *s, const char *accept)
{
	size_t counts = 0;
	const char *iterable;
	
	for (;*s!='\0'; s++)
	{
		for(iterable = accept; *iterable != '\0'; iterable++)
		{
			if(*iterable == '\0')
				break;
			if(*iterable == *s)
			{
				counts++;
				break;
			}

		}
	}
			
	return counts;
}

int IsPolindrome(const char *s)
{
	char *reverse = (char *)s;
	
	while(*reverse != '\0')
	{
		reverse++;
	}
	reverse--;
	
	for(; reverse >= s;)
	{
		if(tolower(*s) == tolower(*reverse))
		{
			s++;
			reverse--;
		}
		else
			return 0;
	}	
	return 1;
}

/*
char *RevieWhiteSpaces(char *s)
{
	Loop with isspace, if true - trim. 
	possibe to move every item step down through the loop.
	
}
*/























