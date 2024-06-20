#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{	
	int result = 1;
	char buffer[200];
	char *test = NULL;
	char *ntest;
	FILE *fp = fopen("test2.txt", "a+");
		
	printf("Let's write a book!\n");
	
	while(result == 1)
	{
		test = fgets(buffer, 199, stdin);
		if(strncmp(test, "<", 1) == 0)
		{
			test = test+1;
			strcpy(ntest, test);
			freopen("test2.txt", "a+", fp);
			fwrite(buffer,1, strlen(test), fp);
		}
		fwrite(buffer,1, strlen(test), fp);
	}
	
	
	return 0;
}
