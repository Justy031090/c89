#ifndef __WS7_H__
#define __WS7_H__


typedef void (*func_ptr)(int);
typedef int (*fun)(char *);

struct print_me
{
	int x;
	func_ptr print_ptr;
};



void PrintInMain();




#endif /* __WS7_H__ */
