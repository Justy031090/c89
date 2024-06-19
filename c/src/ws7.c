#include <stdio.h>
#include "ws7.h"


void Print(int x)
{
	printf("Printing integer %d\n", x);
}

/*Creating a pointer to the Print() function*/
func_ptr print_ptr = Print;

/*Creating an Array of print_me structures*/
struct print_me struct_prints[10];

/*Helper to fill the Array with data*/
void FillArray()
{
	int i=0;
	while(i<10)
	{
		struct_prints[i].x = i;
		struct_prints[i].print_ptr = print_ptr;
		++i;
	}
}

/*Function Passed To Main*/
void PrintInMain()
{
	int i=0;
	FillArray();
	while(i<10)
	{
		struct_prints[i].print_ptr(struct_prints[i].x);
		++i;
	}
	
}



