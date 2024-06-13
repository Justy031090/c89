#include "ws3.h"

int main()
{
	char *s = "Michael";
	char *k = "Bar";
	/*printf("%s\n", StrCpy(k, s));
	printf("%s\n", StrnCpy(k, s, 2));
	printf("%s\n", StrnCpy(k, s, 5));
	printf("%s\n", StrnCpy(k, s, 23)); 
	printf("%d\n", StrnCmp(k, s, 2));
	printf("%d\n", StrnCmp(k, s, 5));
	printf("%d\n", StrnCmp(s, k, 5));
	printf("%d\n", StrCaseCmp(k, s));
	printf("%d\n", StrCaseCmp(s, k));*/
	printf("%d\n", StrSpn("Mich is cool", "Mich is"));

	return 0;
}
