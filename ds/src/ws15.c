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
	static stack_t stack;
	stack.peek = (char *)malloc(size_of_element * capacity);
	if(NULL == stack.peek)
	{
		free(stack.peek);
		return NULL;
	}
	
	stack.size = 0;
	stack.size_of_elemenet = size_of_element;
	size.capacity = capacity;
	
	return &stack;
}

void StackDestroy(stack_t *stack)
{
	if(0 == stack->size)
	{
		free(stack->peek);
	} 
	
	else
	{
		stack->peek -= ((stack->size) *(stack->size_of_element))
		free(stack->peek);
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
	return stack->size == 0 ? 1:0;
}

size_t StackCapacity(const stack_t *stack)
{
	return stack->capacity;
}

void StackPush(const void *val, stack_t *stack)
{
	
}
void StackPop(stack_t *stack);





/*
For each fun add:
description
params
return value 
time complexity 
space complexity 
*/
