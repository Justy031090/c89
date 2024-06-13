#include "ws3.h"

int main()
{
	
	char *test_src = "Michael";
	char *test_dst;
	char *test_str = "Bar";
	
	StrCpy(test_dst, test_src);
	printf("Expect to get different adresses with same strings - \n%p\n%p\n%s\n%s\n",test_dst, test_src, test_dst, test_src);
	
	printf("StrCpy test Ended\n\n");

	printf("Expect to get 'Mi' - %s\n", StrnCpy(test_dst, test_src, 2));
	printf("Expect to get 'Micha' -'%s\n", StrnCpy(test_dst, test_src, 5));
	printf("Expect to get 'Michael' -'%s\n", StrnCpy(test_dst, test_src, 23)); 
	
	printf("StrnCpy test Ended\n\n");
	
	printf("Expect to get Positive value: %d\n", StrnCmp(test_src, test_str, 2));
	printf("Expect to get Negative value: %d\n", StrnCmp(test_str, test_src, 5));
	printf("Expect to get Zero: %d\n", StrnCmp(test_src, "Michael", 5));
	printf("Expect to get Non-Zero - casing test: %d\n", StrnCmp(test_src, "michael", 5));
	printf("StrnCmp test Ended\n\n");
	
	printf("Expect to get Zero: %d\n", StrCaseCmp(test_src, "michael"));
	printf("Expect to get Negative value: %d\n", StrCaseCmp("mic", test_src));
	printf("Expect to get Positive value: %d\n", StrCaseCmp("michael",test_str));
	
	printf("StrCaseCmp test Ended\n\n");
	
	printf("Input'c'- Expect to get the 'chae' back: %s\n", StrChr(test_src, 'c'));
	printf("Input'X'expect to get null back: %s\n", StrChr(test_src, 'X'));
	
	printf("StrChr test Ended\n\n");
	
	test_dst = StrDup(test_src);
	printf("Expect to get different adresses with same strings - \n%p\n%p\n%s\n%s\n",test_dst, test_src, test_src, test_dst);
	printf("Expect to be able to free without error\n");
	free(test_dst);
	test_dst = NULL;
	printf("Succesfully Freed\n");
	
	printf("StrDup test Ended\n\n");
	
	printf("Expect to concat to MichaelBar: %s\n", StrCat(test_src, "Bar"));
	printf("Expect to concat to I LRD: %s\n", StrCat("I", "LRD"));
	printf("Expect to to get 'nothing+ LRD': %s\n", StrCat("", "LRD"));
	printf("Expect to to get I + 'nothing': %s\n", StrCat("I", ""));
	
	printf("StrCat test Ended\n\n");
	
	printf("Expect to concat to MichaelB: %s\n", StrnCat(test_src, "Bar", 1));
	printf("Expect to concat to I LRD: %s\n", StrnCat("I", "LRD", 8));
	printf("Expect to to get 'nothing+ LRD': %s\n", StrnCat("", "LRD",5));
	printf("Expect to to get I + 'nothing': %s\n", StrnCat("I", "", 2));
	printf("Expect to to get 'nothing': %s\n", StrnCat("", "", 2));
	
	printf("StrnCat test Ended\n\n");
	
	printf("Expect to get the substring 'chael':  %s\n", StrStr(test_src, "ch"));
	printf("Expect to get input string Michael back:' %s\n", StrStr(test_src, ""));
	printf("Expect to get no-match - NULL back:' %s\n", StrStr(test_src, "xor"));
	
	printf("StrStr test Ended\n\n");
	
	printf("Input M - expect to get 1: %lu\n", StrSpn(test_src, "M"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "M"));
	printf("Input Mic - expect to get 3: %lu\n", StrSpn(test_src, "Mic"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "Mic"));
	printf("Input iMc123cc - expect to get 3: %lu\n", StrSpn(test_src, "iMc123cc"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "iMc123cc"));
	printf("Input x - expect to get 0: %lu\n", StrSpn(test_src, "x"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "x"));
	
	printf("StrSpn test Ended\n\n");
	
	printf("Testing if polindrome for 123321 - expected:1\n Returned: %d\n",IsPolindrome("123321")); 
	printf("Testing if polindrome for 1234321 - expected:0\n Returned: %d\n",IsPolindrome("1234321")); 
	printf("Testing if polindrome for #!sSkSs!# - expected:1\n Returned: %d\n",IsPolindrome("#!sSkSs!#"));
	printf("Testing if polindrome for 0Abba0 - expected:1\n Returned: %d\n",IsPolindrome("0Abba0")); 
	
	printf("IsPolindrome test Ended\n\n");
	
	/*
	printf("%d\n", IsPolindrome("1234321"));

	*/

	return 0;
}
