#include <stdio.h> /*printf*/
#include <string.h> /*strlen*/
#include <ctype.h> /*tolower*/
#include <assert.h> /*assert*/

#include "ws13.h"
#define BASE_TEN (10)
#define ASCII (128)
#define MAX_BASE (37)
#define MIN_BASE (1)

static int val(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else
		return toupper(c) - 'A' + BASE_TEN;
}

char *IntToString(int x, char *str, int base)
{	
	
	int res = 0;
	int neg_flag = 0;
	int temp = 0;
	char *ptr = str;
	char *sptr = str;
	
	assert(base > MIN_BASE && base < MAX_BASE);
	assert(NULL != str);
	
	if(x < 0)
	{
		x = x * (-1);
		neg_flag = 1;
	}
	
	*ptr = '\0';
	++ptr;
	
	while(0 != x)
	{
		res = x % base;
		x = x / base;
		*ptr = res > 9 ? res + 'A' - BASE_TEN : res + '0';
		++ptr;
	}	
		
	if(neg_flag == 1&& base == BASE_TEN)
	{
		*ptr = '-';
		++ptr;
	}
	
	--ptr;
	
	while(str < ptr)
	{
		temp = *ptr;
		*ptr = *str;
		*str = temp;
		++str; --ptr;
	}
	
	return sptr++;
}



int StringToInt(char *str, int base)
{	
	int result = 0;
	
	assert(base > MIN_BASE && base < MAX_BASE);
	assert(NULL != str);
	
	while(*str)
	{
		result = (result * base) + val(*str);
		++str;
	}
	return result;
}


char *IntToString10(int x, char *str)
{
	assert(NULL != str);
	return IntToString(x, str, BASE_TEN);
}

int StringToInt10(char *str)
{
	assert(NULL != str);
	return StringToInt(str, BASE_TEN);
}

void ThreeArr (char *arr1, char *arr2, char *arr3)
{
	int arr[ASCII] = {0};
	size_t i = 0;
	
	while(i<strlen(arr1))
	{
		arr[(int)arr1[i]] = arr1[i];
		i++;
	}
	
	i = 0;
	
	while(i<(strlen(arr3)))
	{
		if(0 != arr[(int)arr3[i]])
		{
			arr[(int)arr3[i]] = 0;
		}
		i++;
	}
	i = 0;
	while(i<strlen(arr2))
	{
		if(0 != (int)arr[(int)arr2[i]])
		{
			printf("%d\n",arr[(int)arr2[i]]);
		}
		i++;
	}
}

int IsLittleEndian()
{	
	int n = 1;
	if(*(char *)&n == 1)
	{
		printf("LITTLE");
	}
	else
		printf("BIG");
		
	return 0;
}
























