#include <stddef.h>
#include <string.h>
#include <stdio.h>

int Fibonacci(int el_index)
{
    if (el_index < 2)
		return 1;

    return Fibonacci(el_index -1) + Fibonacci (el_index - 2);
}




node_t *FlipList(node_t *node)
{
	node_t *runner = NULL;
    if (node == NULL) return NULL;
	if (node-> next = NULL) return node;

    runner = FlipList(node->next);
    runner->next = node;
    node->next = NULL;
    return node;
	
}


stack_t *SortStack(stack_t *stack)
{
	int tmp = 0, top = 0;
    if (StackIsEmpty(stack))
		return;

    tmp = StackPeek(stack);
    StackPop(stack);

    SortStack(stack);

    if (StackIsEmpty(stack) || StackPeek(stack) <= tmp) {
        StackPush(stack, tmp);
    } else {
        top = StackPeek(stack);
        StackPop(stack);
        SortStack(stack);
        StackPush(stack, top);
        StackPush(stack, tmp);
	}
}




 size_t StrLen(const char *str)
{
	if(*str != '\0')
	{
		return 1 + StrLen(str+1);
	}
	return 0;
}
 
int StrCmp(const char *s1, const char *s2)
{
	if(*s1 != *s2)
		return 0;
	
	if(*s1 != '\0' && *s2 != '\0')
		StrCmp(s1 +1, s2+1);

	return 1;
	
}

char *StrCpy(char *dst, const char *src)
{	
		if(*src != '\0')
		{
			*dst = *src;
			StrCpy(dst+1, src+1);
		}
		else
		{
			++dst;
			*dst = '\0';
		}
	return dst;
}

char *StrCat(char *dst, const char *src)
{
	StrCpy(dst+StrLen(dst), src);
	return dst;	
}

char *StrStr(const char *haystack, const char *needle)
{	
    if (haystack == NULL)
        return NULL;
    else if (strncmp(haystack, needle, strlen(needle)) == 0)
        return (char *)haystack;
    else 
        return(StrStr(haystack+1, needle));
}


int main()
{
	char dest[15];
	char dest_for_cat[20] = "My Name Is Michael ";
	printf("Fibonnaci %d\n", Fibonacci(5));
	printf("StrLen %lu\n", StrLen("Hello"));
	printf("StrCmp False %d\n", StrCmp("Hello", "Jello "));
	printf("StrCmp Positive %d\n", StrCmp("Hello", "Hello"));
	printf("StrCpy %s\n", StrCpy(dest, "Hello"));
	printf("StrCat %s\n", StrCat(dest_for_cat, "Bar"));
	printf("StrStr Recursive %s\n", StrStr("ChupChupaChKabra", "Chupa"));
	printf("strstr from String %s\n", strstr("ChupChupaChKabra", "Chupa"));

	return 0;
}
