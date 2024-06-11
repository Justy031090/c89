#include "ws3.h"

int main()
{
	char *s = "Michae";
	char *k = "mich";
	/*printf("%s\n", StrCpy(k, s));
	printf("%s", strcpy(k, s));
	printf("%s\n", StrnCpy(k, s, 2));
	printf("%s\n", StrnCpy(k, s, 5));
	printf("%s\n", StrnCpy(k, s, 23)); 
	printf("%d\n", StrnCmp(k, s, 2));
	printf("%d\n", StrnCmp(k, s, 5));
	printf("%d\n", StrnCmp(s, k, 5));*/
	printf("%d\n", StrCaseCmp(k, s));
	printf("%d\n", StrCaseCmp(s, k));
	
	return 0;
}
