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
	if(data && param)
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
	
	size_t size = 0;
	int dat = 5;
	int data = 15;
	int getdata = 0;
	int onemore = 1245;
	int *ptr_to_data = &dat;
	int *ptr_to_data2 = &data;
	int *ptr_to_data3 = &onemore;
	is_match_t match = Match;
	action_t action = Action;
	action_t print = ActionPrint;

	sll_t *sll = SLLCreate();
	nd1 = SLLBegin(sll);
	nd2 = SLLInsert(nd1, ptr_to_data, sll);
	nd3 = SLLInsert(nd2, ptr_to_data2, sll);
	nd4 = SLLInsert(nd3, ptr_to_data3, sll);
	
	nd7 = SLLBegin(sll);
	nd5 = SLLFind(nd7, nd4, match, ptr_to_data3);
	
	getdata = SLLForEach(nd7, nd4, action, ptr_to_data);
	printf("Counter %d\n", getdata);
	getdata = SLLForEach(nd7, nd4, print, ptr_to_data);
	
	

	SLLDestroy(sll);
	
	return 0;
}





/*
sll_iterator_t *SLLFind(const sll_iterator_t *from, const sll_iterator_t *to, is_match_t is_match, void *param);
sll_iterator_t *SLLRemove(sll_iterator_t *iter);
void *SLLForEach(const sll_iterator_t *from, const sll_iterator_t *to, action_t *action_func, void *param);*/

