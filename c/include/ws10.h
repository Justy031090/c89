#ifndef __WS10_H__
#define __WS10_H__

#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#define MAX3(a,b,c) ((c) > (MAX2(a,b)) ? (C) : (MAX2(a,b)))

typedef struct funcs funcs_t;

typedef struct element
{
    void *data;
    funcs_t *funcs;
}element_t;

typedef void(* Print_t)(element_t *);
typedef int(* Add_t)(element_t *, int);
typedef void(* Clean_t)(element_t *);

struct funcs
{
	Print_t print;
	Add_t add;    
	Clean_t clean;
};

void InitInt(int x, element_t *arr);
void InitFloat(float x, element_t *arr);
int InitString(char *x, element_t *arr);

void PrintAll(element_t *arr, int num_of_element);
int AddAll(element_t *arr, int num_of_element, int to_add); 
void CleanAll(element_t *arr, int num_of_element);

#endif /* __WS10_H__ */
