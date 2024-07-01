#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *TF(char *str)
{
	int length = strlen(str)-1;
	char *start = str;
	char *end = str+length;
	char tmp = 0;
	size_t i = 0;
	
	
	for (; i < length/2; ++i)
	{
		
		tmp = tolower(*start);
		*start = tolower(*end);
		*end = tmp;
		
		--end;
		++start;
		
	}
	
	return str;
}



int main ()
{
	char * str = "test-strinG";
	char *string = malloc(strlen(str)+1);
	char *tst = NULL;
	strcpy(string, str);
	
	if(!string)
		printf("malloc meh");
	tst = TF(string);
	printf("%s\n", tst);

	return 0; 	
}


