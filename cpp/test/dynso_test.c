#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{
    void *handle = NULL;
    void (*func)() = NULL;
    if (argc != 3) return -1;
    
    handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) return -1;

    func = dlsym(handle, argv[2]);

    func();

    dlclose(handle);
    return 0;
}
