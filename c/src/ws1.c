#include "ws1.h"

void PrintHello()
{
	char s[] = {0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x221, 0x22};
        printf("%s", s);
}


int PowerOfTen(int n)
{
	/*check if number of iterations till n= n**10 */
	/*no point in checking positive or negative, the power is positive means n will be positive*/
	/* the number could be very large. probably good idea to use unsigned long int. */
	
	return 0;
}

int FlipOrder(int n)
{
	/* A way to separate the digits inside of that number. division or modulo could be a good way.
	each separated Number could be multiplied by its distance from the begging position
	e.g 1034 ---> 4301. there are 4 digits, 4 is 3 steps away so ---> 4*1000 + (3 - 2 steps away) 3*100 + (0 - 1 step away) 0*-10 +  (1 is 0 steps away) 1;
	this will produce 4000 + 300 + 0 + 1 --> 4301, the desires number. 
	*/
	return 0;
}

void Swap(void *a, void *b);
{
	/*
	well, it speaks by itself. receving the 2 adresses we enter their values. 
	Can make a temp. variable to store one of the values. 
	*/
}
