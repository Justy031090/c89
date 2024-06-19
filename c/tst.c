#include <stdio.h>
#include <stdlib.h>

typedef void (*func_ptr)(int);


void Print(int x)
{
	printf("Printing integer %d\n", x);
}

func_ptr ptr_to_print = Print;

struct print_me
{
	int x;
	func_ptr print_func;
};



struct print_me struct_prints[10];

void FillArray()
{
	int i=0;
	while(i<10)
	{
		struct_prints[i].x = i;
		struct_prints[i].print_func = ptr_to_print;
		++i;
	}
}






int main()
{
	int i = 0;
	FillArray();
	while(i<10)
	{
		struct_prints[i].print_func(struct_prints[i].x);
		i++;
	}
	
	return 0;
}






