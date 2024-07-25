#ifndef _MEMORY_MAPPING_H_
#define _MEMORY_MAPPING_H_

static int static_global_var1 = 11;
static int static_global_var2 = 22;
static int static_global_var3 = 33;

extern int global_var1;
extern int global_var2;
extern int global_var3;

int __attribute__((weak)) g_weak_symbol = 0;

static const int something = 7;

int MemoryMappingCode1(int functions_arg_1,float functions_arg_2,double functions_arg_3);
int MemoryMappingCode2(int functions_arg_1,float functions_arg_2,double functions_arg_3);


#endif /* _MEMORY_MAPPING_H_ */
