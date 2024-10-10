#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{
    void *handle = NULL;
    void (*func)() = NULL;
    if (argc != 3) return -1;
    
    /*Open library in args*/
    handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) return -1;

    /*Get the symbol (function)*/
    func = dlsym(handle, argv[2]);

    func();

    /*Close the library*/
    dlclose(handle);
    return 0;
}
