#include <dlfcn.h>
#include <stddef.h>


#include "staticlib.h"

int main ()
{
	void *first = dlopen("./dynamiclib.so", RTLD_LAZY | RTLD_GLOBAL);
	if(first == NULL)
	{
		return NULL;
	}
	void (*fn)() = (void(*)())dlsym(first, "foo");
	if(fn == NULL)
	{
		return NULL;
	}
	PrintGlobalVariableAdress();
	fn();
	return 0;
}
