#include "ws3.h"

int main()
{
	
	char *test_src = "Michael";
	char test_array[1024] = "Hello World";
	char *test_str = "Bar";
	char empty_array[500];
	char *test_dst = NULL;
	/*
	StrCpy(empty_array, test_src);
	printf("Expect to get different adresses with same strings - \n%p\n%p\n%s\n%s\n",empty_array, test_src, empty_array, test_src);
	
	printf("StrCpy test Ended\n\n");
	
	printf("Expect to get 'Mi' - %s\n", StrnCpy(empty_array, test_src, 2));
	printf("Library func. output :  - %s\n", strncpy(empty_array, test_src, 2));
	printf("Expect to get 'Micha': %s\n", StrnCpy(empty_array, test_src, 5));
	printf("Library func. output :  - %s\n", strncpy(empty_array, test_src, 5));
	printf("Expect to get 'Michael':%s\n", StrnCpy(empty_array, test_src, 23));
	printf("Library func. output :  - %s\n", strncpy(empty_array, test_src, 23)); 
	
	printf("StrnCpy test Ended\n\n");
	
	printf("Expect to get Positive value: %d\n", StrnCmp(test_src, test_str, 2));
	printf("Library Func output: %d\n", strncmp(test_src, test_str, 2));
	printf("Expect to get Negative value: %d\n", StrnCmp(test_str, test_src, 5));
	printf("Library Func output: %d\n", strncmp(test_str, test_src, 5));
	printf("Expect to get Zero: %d\n", StrnCmp(test_src, "Michael", 5));
	printf("Library Func output: %d\n", strncmp(test_src, "Michael", 5));
	printf("Expect to get Non-Zero - casing test: %d\n", StrnCmp(test_src, "michael", 5));
	printf("Library Func output: %d\n", strncmp(test_src, "michael", 5));
		
	printf("StrnCmp test Ended\n\n");
	
	printf("Expect to get Zero: %d\n", StrCaseCmp(test_src, "michael"));
	printf("Library Func Output: %d\n", strcasecmp(test_src, "michael"));
	printf("Expect to get Negative value: %d\n", StrCaseCmp("mic", test_src));
	printf("Library Func Output: %d\n", strcasecmp("mic", test_src));
	printf("Expect to get Positive value: %d\n", StrCaseCmp("michael",test_str));
	printf("Library Func Output: %d\n", strcasecmp("michael", test_str));
	
	printf("StrCaseCmp test Ended\n\n");
	
	printf("Input'c'- Expect to get the 'chael' back: %s\n", StrChr(test_src, 'c'));
	printf("Library Func output: %s\n", strchr(test_src, 'c'));
	printf("Input'X'expect to get null back: %s\n", StrChr(test_src, 'X'));
	printf("Library Func output: %s\n", strchr(test_src, 'X'));
	
	printf("StrChr test Ended\n\n");
	
	test_dst = StrDup(test_src);
	printf("Expect to get different adresses with same strings - \n%p\n%p\n%s\n%s\n",empty_array, test_src, test_src, empty_array);
	printf("Expect to be able to free without error\n");
	free(test_dst);
	test_dst = NULL;
	printf("Succesfully Freed\n");
	
	printf("StrDup test Ended\n\n");
	
	printf("Expect to concat to Hello World Bar: %s\n", StrCat(test_array, test_str));
	test_array[11] = '\0';
	printf("Library Func output: %s\n", strcat(test_array, test_str));
	test_array[11] = '\0';
	printf("Expect to to get 'Hello World+ LRD': %s\n", StrCat(test_array, "LRD"));
	test_array[11] = '\0';
	printf("Library Func output: %s\n", strcat(test_array, "LRD"));
	test_array[11] = '\0';
	printf("Expect to to get Hello World + 'nothing': %s\n", StrCat(test_array, ""));
	printf("Library Func output: %s\n", strcat(test_array, ""));
	
	printf("StrCat test Ended\n\n");
	
	printf("Expect to concat to Hello WorldB: %s\n", StrnCat(test_array, "Bar", 1));
	test_array[11] = '\0';
	printf("Library Func output %s\n", strncat(test_array, test_str, 1));
	test_array[11] = '\0';
	printf("Expect to concat to Hello World LRD: %s\n", StrnCat(test_array, "LRD", 8));
	test_array[11] = '\0';
	printf("Library Func output %s\n", strncat(test_array, "LRD", 8));
	test_array[11] = '\0';
	printf("Expect to to get Hello World + 'nothing': %s\n", StrnCat(test_array, "", 2));
	printf("Library Func output %s\n", strncat(test_array, "", 2));
	
	printf("StrnCat test Ended\n\n");
	
	
	printf("Expect to get the substring 'chael':  %s\n", StrStr(test_src, "ch"));
	printf("Library Func output:  %s\n", strstr(test_src, "ch"));
	printf("Expect to get input string Michael back:' %s\n", StrStr(test_src, ""));
	printf("Library Func output:  %s\n", strstr(test_src, ""));
	printf("Expect to get no-match - NULL back:' %s\n", StrStr(test_src, "xor"));
	printf("Library Func output:  %s\n", strstr(test_src, "xor"));
	
	printf("StrStr test Ended\n\n");
		
	printf("Input M - expect to get 1: %lu\n", StrSpn(test_src, "M"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "M"));
	printf("Input Mic - expect to get 3: %lu\n", StrSpn(test_src, "Mic"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "Mic"));
	printf("Input iMc113cc - expect to get 3: %lu\n", StrSpn(test_src, "iMc113cc"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "iMc113cc"));
	printf("Input x - expect to get 0: %lu\n", StrSpn(test_src, "x"));
	printf("Library Func. Output - : %lu\n", strspn(test_src, "x"));
	
	printf("StrSpn test Ended\n\n");
	
	printf("Testing if polindrome for 113321 - expected:1\n Returned: %d\n",IsPolindrome("113321")); 
	printf("Testing if polindrome for 1134321 - expected:0\n Returned: %d\n",IsPolindrome("1134321")); 
	printf("Testing if polindrome for #!sSkSs!# - expected:1\n Returned: %d\n",IsPolindrome("#!sSkSs!#"));
	printf("Testing if polindrome for 0Abba0 - expected:1\n Returned: %d\n",IsPolindrome("0Abba0")); 
	
	printf("IsPolindrome test Ended\n\n");
	*/
	printf("%s\n", RevieWhiteSpaces("  This is a       string\tthat is \vbeing  checked"));

	return 0;
}
