
#include <stdio.h>
#include <string.h>

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
	int i;	
	for(i=0; i<n; i++)
	{
		if(*str1 != *str2)
		{
			return (int)*str1 - (int)*str2;
		}
		if(*str1 == '\0' ||* str2 == '\0' )
		{
			return 0;
		}
		str1++;
		str2++;
	}
	
	return 0;

}


