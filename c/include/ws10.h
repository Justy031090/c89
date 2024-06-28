#ifndef __WS10_H__
#define __WS10_H__

#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#define MAX3(a,b,c) ((c) > (MAX2(a,b)) ? (c) : (MAX2(a,b)))
#define SIZE_OF(var) ((((size_t)(&var)+1)-(size_t)(&var)))
#define TYPE_SIZE(type) (SIZE_OF( __typeof__(type)))

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
void InitArray(element_t *arr, int size);
void InitInt(int, element_t *);
void InitFloat(float, element_t *);
int InitString(char *, element_t *);

void PrintAll(element_t *arr, int num_of_element);
int AddAll(element_t *arr, int num_of_element, int to_add); 
void CleanAll(element_t *arr, int num_of_element);

#endif /* __WS10_H__ */
