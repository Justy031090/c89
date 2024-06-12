#include "ws3.h"
#include <string.h>

int main()
{
	char *s = "Michael ";
	const char *q = "Bar";
	char *k;
	/*char c = 'c';
	char l = 'l';
	char *res1, *res2;
	char *k = "mich";
	printf("%s\n", StrCpy(k, s));
	printf("%s", strcpy(k, s));
	printf("%s\n", StrnCpy(k, s, 2));
	printf("%s\n", StrnCpy(k, s, 5));
	printf("%s\n", StrnCpy(k, s, 23)); 
	printf("%d\n", StrnCmp(k, s, 2));
	printf("%d\n", StrnCmp(k, s, 5));
	printf("%d\n", StrnCmp(s, k, 5));
	printf("%d\n", StrCaseCmp(k, s));
	printf("%d\n", StrCaseCmp(s, k));
	res1 = StrChr(s, c);
	res2 = StrChr(s, l);
	printf("%p\n", res1);
	printf("%p\n", res2);*/

	k = StrCat(s, q);
	printf("%s\n", k);
	
	
	return 0;
}
