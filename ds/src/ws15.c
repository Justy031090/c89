

stack_t *StackCreate(size_t capacity, size_t size_of_element)
{
	
}
void StackDestroy(stack_t *stack);
void StackPush(const void *val, stack_t *stack);
void StackPop(stack_t *stack);
void *StackPeek(const stack_t *stack);
int StackIsEmpty(const stack_t *stack);
size_t StackSize(const stack_t *stack);
size_t StackCapacity(const stack_t *stack);
