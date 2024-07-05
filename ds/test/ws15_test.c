#include <stdio.h>
#include "ws15.h"

int main()
{

int x = 4;
int *xp = &x;
stack_t *myStack = StackCreate(100, 8);


printf("size ? should return 0. %ld\n", StackSize(myStack));
printf("Is Empty ? should return true. %d\n", StackIsEmpty(myStack));
StackPush(xp, myStack);
printf("Capacity. Should return 100 . %ld\n" ,StackCapacity(myStack));
printf("size ? should return 1 . %ld\n",StackSize(myStack));
printf("Is Empty 2nd ? should return false. %d\n",StackIsEmpty(myStack));
StackPeek(myStack);
StackPop(myStack);
printf("size ? should return 0.%ld\n", StackSize(myStack));
printf("Is Empty ? should return true.%d\n", StackIsEmpty(myStack));
StackDestroy(myStack);

	return 0;
}
