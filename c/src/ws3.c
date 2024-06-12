
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
	size_t i;
	for(i=0; i<dsize; i++)
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
	return (char *)s;
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
	char *concatenated = (char *)malloc((sizeof(char) * length) +1);
	
	StrCpy(concatenated, dst);
	concatenated = concatenated + dst_length;
	StrCpy(concatenated, src);
	concatenated = concatenated - dst_length;
	return concatenated;
	
}































