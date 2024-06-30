#include <stdio.h>
#include "ws12.h"

int main()
{	
	char *source = "Hello World";
	char destination[13];
	
	printf("%s\n", (char *)MemMove(destination, source, 12));
	printf("%s\n", (char *)MemMove(destination, source, 1));
	printf("%s\n", (char *)MemMove(destination, source, 0));
	printf("%s\n", (char *)MemMove(destination, source, 11));
	printf("%s\n", (char *)MemMove(destination, source, 13));
	printf("%s\n", (char *)MemMove(destination, source, 6));


	
	
	return 0;
}
