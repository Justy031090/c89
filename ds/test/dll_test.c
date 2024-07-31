#include <stdio.h>
#include "dll.h"


int Match (const void *num1, const void *num2)
{
	if(num1 && num2)
		return *(int *)num1 == *(int *)num2 ? 1 : 0 ;
	return 0;
}

size_t Action (void *data, void *param)
{
	if(NULL != data && NULL !=param)
		return (*(int *)data) * (*(int *)param);
	return 0;
} 

size_t ActionPrint(void *data, void *param)
{
	(void)param;
	printf("%p\n", data);
	return 0;
}




int main ()
{
	
	int is_empty1 = -1;
	int is_empty2 = -1;
	size_t size1 = -1;
	size_t size2 = -1;
	size_t size3 = -1;
	int data1 = 5;
	int data2 = 7;
	dll_iterator_t iter1 = NULL;
	dll_iterator_t iter2 = NULL;
	dll_iterator_t iter3 = NULL;
	dll_iterator_t iter4 = NULL;
	dll_iterator_t iter5 = NULL;
	dll_iterator_t iter6 = NULL;
	dll_iterator_t iter7 = NULL;
	dll_iterator_t iter8 = NULL;
	is_match_t match = Match;
	action_t action = Action;
	action_t print = ActionPrint;
	
	dll_t *dll = DLLCreate();
	dll_t *dll2 = DLLCreate();
	dll_t *dll3 = DLLCreate();
	if(NULL != dll)
	{
		printf("TEST FOR DLLCreate FUNCTION SUCCESFULLY\n");
	}
	else
	{
		printf("TEST FOR DLLCreate FUNCTION FAILED\n");
	}	
	size1 = DLLSize(dll);
	is_empty1 = DLLIsEmpty(dll);
	
	iter1 = DLLBegin(dll);
	iter2 = DLLInsert(iter1, &data1, dll);
	
	is_empty2 = DLLIsEmpty(dll);
	if(is_empty1 && !is_empty2)
	{
		printf("TEST FOR DLLIsEmpty FUNCTION SUCCESFULLY\n");
	}
	else
	{
		printf("TEST FOR DLLIsEmpty FUNCTION FAILED\n");
	}
	
	
	DLLPushFront(dll, &data2);
	DLLPushBack(dll, &data2);
	iter3 = DLLInsert(iter2, &data1, dll);
	size2 = DLLSize(dll);
	
	DLLRemove(iter2, dll);
	DLLPopFront(dll);
	DLLPopBack(dll);

	size3 = DLLSize(dll);
	
	if(size1 == 0 && size2 == 4 && size3 == 1)
	{
		printf("TEST FOR SIZE FUNCTION SUCCESS\n");
		printf("TEST FOR INSERT FUNCTION SUCCESS\n");
		printf("TEST FOR REMOVE FUNCTION SUCCESS\n");
		printf("TEST FOR PopFront & PopBack FUNCTIONS SUCCESS\n");
		printf("TEST FOR PushFront & PushBack FUNCTIONS SUCCESS\n");
	}
	else
	{
		printf("TEST FOR SIZE FUNCTION FAILED\n");
	}
	iter1 = DLLBegin(dll);
	iter2 = DLLEnd(dll);
	iter4 = DLLPushBack(dll, &data2);
	iter5 = DLLFind(iter1, iter2, match, &data2);
	if(iter4==iter5)
	{
		printf("TEST FOR FIND FUNCTION SUCCESS\n");
	}
	else
	{
		printf("TEST FOR FIND FUNCTION FAILED\n");
	}
	iter2 = DLLEnd(dll);
	size1 = DLLForEach(iter1, iter2, print, &data1);
	if(size1 == 2)
	{
		printf("TEST FOR FOR EACH FUNCTION SUCCESS\n");
	}
	else
	{
		printf("TEST FOR FOR EACH FUNCTION FAILED\n");
	}
	
	DLLPushBack(dll, &data2);
	DLLPushBack(dll, &data1);
	DLLPushBack(dll, &data1);
	DLLPushBack(dll, &data2);
	iter2 = DLLEnd(dll);
	iter3 = DLLPrev(iter2);
	iter4 = DLLNext(iter1);
	
	iter6 = DLLMultiFind(iter1, iter2, match, &data2, dll2);
	size1 = DLLSize(dll2);
	if(size1 == 3)
	{
		printf("TEST FOR MULTI FIND FUNCTION SUCCESS\n");
	}
	else
	{
		printf("TEST FOR MULTI FIND FUNCTION FAILED\n");
	}
	
	DLLDestroy(dll);
	DLLDestroy(dll2);
	DLLDestroy(dll3);
	return 0;
}
