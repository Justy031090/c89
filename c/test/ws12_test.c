#include <stdio.h>
#include <string.h>
#include "ws12.h"

int main()
{	
	char *source = "Hello World";
	char destination[16] = {0};
	int i = 0;
	
	/*
	printf("%s\n", (char *)MemSet(destination, 's', 9));
	
	printf("%s\n", (char *)MemSet(destination, 'c', 12));
	printf("%s\n", (char *)MemSet(destination, 'a', 1));
	printf("%s\n", (char *)MemSet(destination, 'h', 0));
	printf("%s\n", (char *)MemSet(destination, 'g', 11));
	printf("%s\n", (char *)MemSet(destination, 'k', 13));
	*/
	while(i<16)
	{
		printf("%s\n", (char *)MemMove(destination, source, i));
		printf("%s\n\n", (char *)memmove(destination, source, i));
		i++;
	}


	
	
	return 0;
}
