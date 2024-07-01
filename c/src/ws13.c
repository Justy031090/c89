
#include <stdio.h>
#include <stdlib.h>

static int val(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else
		return c - 'A' + 10;
}
char *IntToString(int x, char *str, int base)
{	
	int res = 0;
	int neg_flag = 0;
	int temp = 0;
	char *ptr = str;
	char *sptr = str;
	
	if(x < 0)
	{
		x = x * (-1);
		neg_flag = 1;
	}
	
	*ptr = '\0';
	++ptr;
	
	while(x != 0)
	{
		res = x % base;
		x = x / base;
		*ptr = res > 9 ? res + 'A' - 10 : res + '0';
		++ptr;
	}	
		
	if(neg_flag == 1&& base == 10)
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
	while(*str)
	{
		result = (result * base) + val(*str);
		++str;
	}
	return result;
}




int main ()

{
	char buff[10];
	char *test = "FF";
	printf("%s\n", IntToString(255, buff, 16));
	printf("%d\n", StringToInt(test, 16));
	return 0;
}
