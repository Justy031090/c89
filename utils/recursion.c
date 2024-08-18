#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ds/include/stack.h"
#include "../ds/include/sll.h"

int Fibonacci(int el_index)
{
    if (el_index < 2)
		return 1;

    return Fibonacci(el_index -1) + Fibonacci (el_index - 2);
}

typedef struct node
{
	void *data;
	node_t *next;
} node_t;



node_t *FlipList(node_t *first)
{
	node_t *runner;	
    if (first == NULL) return NULL;
	if (first->next == NULL) return first;

    runner = FlipList(first->next);
    runner->next = first;
    first->next->next = NULL;
    return runner;
	
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

int StackOverFlow(int num)
{
	printf("%d\n", num);
	return StackOverFlow(num+1);
}
int StackOverFlowIter(int num)
{
	printf("%d\n", num);
	return num;
}

void printNodes(node_t *node)
{
	while(node->next != NULL)
	{
		printf("List Flip %p\n", node);
		node = node->next;
	}
}

stack_t *SortStack(stack_t *stack)
{
	int tmp = 0, top = 0;
    if (StackIsEmpty(stack))
		return;

    tmp = *(int *)StackPeek(stack);
    StackPop(stack);

    SortStack(stack);

    if (StackIsEmpty(stack) || *(int *)StackPeek(stack) <= tmp) {
        StackPush(&tmp, stack);
    } else {
        top = *(int *)StackPeek(stack);
		StackPop(stack);
		SortStack(stack);
		StackPush(&top, stack);

	}
}
int main()
{
	char dest[15];
	char dest_for_cat[20] = "My Name Is Michael ";
	int data = 5;
	int i = 0;
	stack_t *stack = StackCreate(10, sizeof(int));
	sll_t *sll = SLLCreate();
	sll_iterator_t nd1;
	sll_iterator_t nd2;
	sll_iterator_t nd3;
	sll_iterator_t nd4;
	sll_iterator_t nd5;
	nd1 = SLLBegin(sll);
	nd2 = SLLInsert(SLLEnd(sll), &data,sll);
	nd3 = SLLInsert(SLLEnd(sll), &data,sll);
	nd4 = SLLInsert(SLLEnd(sll), &data,sll);
	nd5 = SLLInsert(SLLEnd(sll), &data,sll);
	int arr[10] = {12,1,30,-6,10,-20, 50, 0, 90, 4};
	int arr2[10] ={-20, -6, 0, 1, 4, 10, 12, 30, 50, 90};

	printf("Fibonnaci %d\n", Fibonacci(5));
	printf("StrLen %lu\n", StrLen("Hello"));
	printf("StrCmp False %d\n", StrCmp("Hello", "Jello "));
	printf("StrCmp Positive %d\n", StrCmp("Hello", "Hello"));
	printf("StrCpy %s\n", StrCpy(dest, "Hello"));
	printf("StrCat %s\n", StrCat(dest_for_cat, "Bar"));
	printf("StrStr Recursive %s\n", StrStr("ChupChupaChKabra", "Chupa"));
	printf("strstr from String %s\n", strstr("ChupChupaChKabra", "Chupa"));

	printNodes(nd1);
	FlipList(SLLBegin(sll));
	printf("\n\n\n\n");
	printNodes(SLLEnd(sll));
	
	

	while(i<10)
	{
		StackPush(&arr[i], stack);
		++i;
	}

	SortStack(stack);

	for (i = 9; i != 0; i--)
	{
		printf("From Sorted Array %d    From Stack %d\n", arr2[i], *(int *)StackPeek(stack));
		if(arr2[i] != *(int *)(StackPeek(stack)))
		{
			
		}
		StackPop(stack);
	}
	


	

	
	return 0;
}
