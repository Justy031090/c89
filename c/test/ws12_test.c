#include <stdio.h>
#include <string.h>
#include "ws12.h"

int main()
{	
	char *source = "Hello World";
	char *source2 = "Hello World";
	char destination[16] = "Hello World";
	char destination2[16] = {0};
	char destination3[16] = {0};
	char destination4[16] = {0};
	int i = 0;
	destination[15] = '\0';
	
	/*
	printf("%s\n", (char *)MemSet(destination, 's', 9));
	
	printf("%s\n", (char *)MemSet(destination, 'c', 12));
	printf("%s\n", (char *)MemSet(destination, 'a', 1));
	printf("%s\n", (char *)MemSet(destination, 'h', 0));
	printf("%s\n", (char *)MemSet(destination, 'g', 11));
	printf("%s\n", (char *)MemSet(destination, 'k', 13));
	*/
	printf("%s\n", (char *)MemSet(destination, 'c', 9));
/*	while(i<16)
	{
		
		
		printf("%s\n", (char *)memset(destination2, 'c', 12));
		printf("%s\n\n", (char *)memmove(destination4, source, i));
		printf("%s\n", (char *)MemMove(destination3, source2, i));
		printf("%s\n", (char *)MemSet(destination, 'c', 9));
		i++;
	}

*/
	
	
	return 0;
}
