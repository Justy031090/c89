#include "ws1.h"

void PrintHello()
{
	char s[] = {0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x22};
        printf("%s", s);
}


double PowerOfTen(int n)
{	
	float chosen_num = 10.0;
	int i;
	double result = 1.0;
	if(n < 0)
	{
		n = n * (-1);
		chosen_num = 1/chosen_num;
	}

	for (i =  0; i < n; i++)
	{
		result *= chosen_num;
	}

	return result;
}

int FlipOrder(int n)
{
	int result = 0;
	int digit;

	while ( 0 != n )
	{
		digit = n % 10;
		result = (result * 10) + digit;
		n = n/10;
	}
	
	return result;
}

void Swap(void *a, void *b);
}
	int temp = *a;
	a* = *b;
	*b = temp;
}
