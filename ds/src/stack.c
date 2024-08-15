/*
(\.../)		..
(=';'=) .. code reviewd by Johnatan.I ..
(")-("))	..
*/

#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/
#include <string.h> /*memcpy*/
#include "stack.h"


struct stack
{ 
    char *peek;
    size_t size;
    size_t size_of_element;
    size_t capacity;
} new_stack;



stack_t *StackCreate(size_t capacity, size_t size_of_element)
{
	stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
	assert(capacity > size_of_element);
	
	if(NULL == stack)
	{
		return NULL;
	}
	stack->peek = (char *)malloc(size_of_element * capacity);
	if(NULL == stack->peek)
	{
		free(stack);
		return NULL;
	}
	
	stack->size = 0;
	stack->size_of_element = size_of_element;
	stack->capacity = capacity;
	
	return stack;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL !=stack);
	if(stack->size)
	{
		free(stack->peek - ((stack->size-1) * stack->size_of_element));
	} 
	else
	{
		free(stack->peek);
	}
	
	free(stack);
	stack = NULL;
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL !=stack);
	return stack->size;
}

void *StackPeek(const stack_t *stack)	
{
	assert(NULL !=stack);
	return (void *)(stack->peek + (stack->size -1)* stack->size_of_element);
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL !=stack);
	return stack->size == 0;
}

size_t StackCapacity(const stack_t *stack)
{
	assert(NULL !=stack);
	return stack->capacity;
}

void StackPush(const void *val, stack_t *stack)
{
    assert(NULL != stack && val != NULL);
    if (stack->size >= stack->capacity)
    {
        return;
    }
    
    memcpy(stack->peek + stack->size * stack->size_of_element, val, stack->size_of_element);
    stack->size++;
}
void StackPop(stack_t *stack)
{
	stack->size--;
}





/*
For each fun add:
description
params
return value 
time complexity 
space complexity 
*/
