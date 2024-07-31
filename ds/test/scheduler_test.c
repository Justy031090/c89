#include <stdio.h>/*printf*/

#include "scheduler.h"

typedef struct params
{
	int a;
	sd_t *sd;
}p_t;
time_t printfunc2(void *params)
{
	static int p = 0;
	if (3 == p)
	{
		SCHEDStop(((p_t *)params)->sd);
	}
	printf("%d\n", (p+((p_t *)params)->a+3));
	++p;
	return 1;
}

time_t printfunc1(void *params)
{
	static int p = 0;
	if (3 == p)
	{
		return 0;
	}
	printf("%d\n", (p+((p_t *)params)->a));
	++p;
	return 1;
}


static void CleanUp(void *params)
{
	(void)params;
}






int main()
{
	sd_t *new_sd = SCHEDCreate();
	p_t params1;
	my_uid_t task1_id; 
	my_uid_t task2_id;
	params1.a = 1;
	params1.sd = new_sd;
	
	if(1 == SCHEDIsEmpty(new_sd))
	{
		printf("IsEmpty: empty success\n");
	}	
	else
	{
		printf("IsEmpty: empty FAIL\n");
	}
	task1_id = SCHEDAddTask(new_sd, time(NULL)+1, printfunc1, (void *)&params1, CleanUp, (void *)&params1);
	task2_id = SCHEDAddTask(new_sd, time(NULL)+1, printfunc2, (void *)&params1, CleanUp, (void *)&params1);
	
	
	
	if(0 == SCHEDIsEmpty(new_sd))
	{
		printf("IsEmpty: full success\n");
	}	
	else
	{
		printf("IsEmpty: empty FAIL\n");
	}
	if(2 == SCHEDSize(new_sd))
	{
		printf("Size success\n");
	}	
	else
	{
		printf("Size FAIL\n");
	}
	
	
	printf("BEFORE RUN\n");
	SCHEDRun(new_sd);
	
	
	
	SCHEDDestroy(new_sd);
	return 0;
}

