#include <stdio.h>

#include "sll.h"

int Match (void *num1, void *num2)
{
	if(num1 && num2)
		return *(int *)num1 == *(int *)num2 ? 1 : 0 ;
	return 0;
}

int Action (void *data, void *param)
{
	if(NULL != data && NULL !=param)
		return (*(int *)data) * (*(int *)param);
	return 0;
} 

int ActionPrint(void *data, void *param)
{
	(void)param;
	printf("%d\n", *(int*)data);
}


int main ()
{

	sll_iterator_t nd1 = NULL;
	sll_iterator_t nd2 = NULL;
	sll_iterator_t nd3 = NULL;
	sll_iterator_t nd4= NULL;
	sll_iterator_t nd5= NULL;
	sll_iterator_t nd6= NULL;
	sll_iterator_t nd7= NULL;
	sll_iterator_t nd8= NULL;
	
	size_t size_before = 0;
	size_t size_middle = 0;
	size_t size_after = 0;
	
	size_t isempty1 = 12;
	size_t isempty2 = 0;
	
	int get_data = 0;
	int get_data_2 = 0;
	int is_equal = -2;
	
	int set_data = 5;
	int data = 15;
	int multiplier = 2;
	int *ptr_to_data = &set_data;
	int *ptr_to_data2 = &data;

	
	
	is_match_t match = Match;
	action_t action = Action;
	action_t print = ActionPrint;

	sll_t *sll = SLLCreate();
	size_before = SLLSize(sll);
	printf("SLL Create\n");
	if(NULL != sll)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	isempty1 = SLLIsEmpty(sll);
	
	nd1 = SLLBegin(sll);
	printf("SLL Begin\n");
	if(NULL != nd1)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	nd2 = SLLInsert(nd1, ptr_to_data, sll);
	size_middle = SLLSize(sll);
	nd3 = SLLInsert(nd2, ptr_to_data2, sll);
	printf("SLL Insert\n");
	if(NULL != nd2 && NULL != nd3)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	size_after = SLLSize(sll);
	printf("SLL Size\n");
	if(size_before == 0 && size_middle == 1 && size_after == 2)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	isempty2 = SLLIsEmpty(sll);
	printf("SLL Is Empty\n");
	
	if(isempty1 == 1 && isempty2 == 0)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	get_data = *(int *)SLLGetData(nd1);
	get_data_2 = *(int*)SLLGetData(nd2);
	printf("SLL Get Data\n");
	if(get_data == 5 && get_data_2 == 15 )
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	
	SLLSetData(nd1, &get_data_2);
	SLLSetData(nd2, &set_data);
	get_data = *(int *)SLLGetData(nd1);
	get_data_2 = *(int*)SLLGetData(nd2);
	
	printf("SLL Set Data\n");
	if(get_data == 15 && get_data_2 == 5 )
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	nd4 = SLLEnd(sll);
	printf("SLL End\n");
	if(nd4 == nd3)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	nd5 = SLLNext(nd1);
	printf("SLL Next\n");
	if(nd5 == nd2)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	
	is_equal = SLLIsEqual(nd2, nd5);
	printf("SLL Is Equal\n");
	if(is_equal == 1)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	size_before = 0;
	size_after = 0;
	size_before = SLLSize(sll);
	SLLRemove(nd5, sll);
	size_after = SLLSize(sll);
	printf("SLL Remove\n");
	if(size_before != size_after)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	SLLSetData(nd1, &data);
	SLLSetData(nd4, &data);
	SLLSetData(nd3, &set_data);
	nd6 = SLLFind(nd4, nd1, match, &set_data);
	get_data = *(int *)SLLGetData(nd3);
	get_data_2 = *(int*)SLLGetData(nd6);
	printf("SLL Find\n");
	if(get_data == get_data_2 && nd6 == nd4)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	nd2 = SLLInsert(nd1, ptr_to_data, sll);
	nd5 = SLLInsert(nd2, ptr_to_data2, sll);
	nd6 = SLLInsert(nd5, ptr_to_data2, sll);
	
	get_data = SLLForEach(nd1,nd6,print,&multiplier);

	printf("SLL For Each\n");
	if(get_data == 8)
	{
		
		printf("Successfully !\n");
	}
	else
	{
		printf("Failed !\n");
	}
	
	printf("\n\n\n");
	
	SLLDestroy(sll);
	
	return 0;
}
