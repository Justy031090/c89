#include <stdio.h>
#include "staticlib.h"

int x __attribute__((weak));
void PrintGlobalVariableAdress()
{
	printf("%p\n", (void *)&x);
}
