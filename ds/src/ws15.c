#include <stddef.h> /*size_t*/


typedef struct stack
{
    char *peek;
    size_t size;
    size_t size_of_element;
    size_t capacity;
} stack_t;



stack_t *StackCreate(size_t capacity, size_t size_of_element)
{
	
	stack_t stack = (stack_t *)malloc(sizeof(stack_t));
	if(NULL == stack)
	{
		return NULL;
	}
	stack->peek = (char *)malloc(size_of_element * capacity);
	if(NULL == stack->peek)
	{
		return NULL;
	}
	
	stack->size = 0;
	stack->size_of_elemenet = size_of_element;
	size->capacity = capacity;
	
	return stack;
}

void StackDestroy(stack_t *stack)
{
	if(0 != stack->size)
	{
		free(stack->peek - ((stack->size-1))*stack->size_of_element);
	} 
	
	else
	{
		free(stack->peek - ((stack->size)*stack->size_of_element);
	}
}

size_t StackSize(const stack_t *stack)
{
	return stack->size;
}

/*check to return ?*/
void *StackPeek(const stack_t *stack)
{
	return stack->peek;
}

int StackIsEmpty(const stack_t *stack)
{
	return 0 == stack->size ? 1:0;
}

size_t StackCapacity(const stack_t *stack)
{
	return stack->capacity;
}

void StackPush(const void *val, stack_t *stack)
{
	size_t i = 0;
	char *add_val = (char *)val;
	char *peekc = stack->peek;
	
	if(stack->size >= stack->capacity)
	{
		return ;
	}
	if( 0 != stack->size )
	{
		stack->peek = stack->peek + size_of_element;
	}
	
	while(i<stack->size_of_element)
	{
		peekc[i] = add_val[i];
		++i;
	}
	stack->size = stack->size + 1;
}
void StackPop(stack_t *stack)
{
	if(0 == stack->size)
	{
		return;
	}
	if(1 == stack->size)
	{
		stack->size = 0;
		return;
	}
	stack->peek = stack->peek - stack->size_of_element;
	stack->size = stack->size - 1;
}





/*
For each fun add:
description
params
return value 
time complexity 
space complexity 
*/
