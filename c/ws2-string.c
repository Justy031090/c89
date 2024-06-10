#include <stdio.h>

/*implement strcmp*/

int strcmp(const char *s1, const char *s2)
{	
	int res = 0;
	
	for(;*s1 == *s2 && *s1 != '\0';)
	{
		s1++;
		s2++;
	}
	
	return (int)*s1 - (int)*s2;
}


/*implement strlen*/

 size_t MyStrlen(const char *s)
 {
 	
 }








int main()
{
	const char *s1 = "BOOM!!!";
	const char *s2 = "BOOM!!";
	int result = strcmp(s1, s2);
	printf("%d\n\n", result);
	return 0;
}
