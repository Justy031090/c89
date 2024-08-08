#include <stdio.h>
#include "staticlib.h"

void PrintGlobalVariableAdress()
{
	printf("%p\n", (void *)&x);
}
