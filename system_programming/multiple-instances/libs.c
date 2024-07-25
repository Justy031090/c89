#include <dlfcn.h>
#include <stddef.h>


#include "staticlib.h"

int main ()
{
	
	void *first = dlopen("./dynamiclib.so", RTLD_LAZY | RTLD_GLOBAL);
	void *sec = dlopen("./dynamiclib2.so", RTLD_LAZY | RTLD_GLOBAL);
	if(first == NULL)
	{
		return NULL;
	}
	void (*fn)() = (void(*)())dlsym(first, "foo");
	void (*fn2)() = (void(*)())dlsym(sec, "foo2");
	if(fn == NULL)
	{
		return NULL;
	}
	PrintGlobalVariableAdress();
	fn();
	fn2();
	return 0;
}
