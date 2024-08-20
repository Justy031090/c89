#ifndef __AVL_H__
#define __AVL_H__

#include "dll.h"

typedef struct avl avl_t;
typedef struct node avl_node_t;

typedef int (*compare_func_t)(const void *data, const void *param);
typedef int (*action_func_t)(void *data, void *param);

/*O(1)*/ 
avl_t *AVLCreate(compare_func_t );

/*O(n)*/
void AVLDestroy(avl_t *avl);

/*O(logn)*/
int AVLInsert(avl_t *avl, const void *data); /*Duplicate will be undefined*/

/*O(logn)*/
void AVLRemove(avl_t *avl, void *data);

/*O(logn)*/
void *AVLFind(const avl_t *avl, const void *data);

/*O(n)*/
size_t AVLSize(const avl_t *avl);

/*O(1)*/
int AVLIsEmpty(const avl_t *avl);

/*O(n)*/
int AVLForEach(avl_t *avl, action_func_t action_func, void *param);

/*O(1)*/
size_t AVLHeight(const avl_t *avl);

/*O(n)*/
int AVLMultiFind(const avl_t *avl, void * param, int (*Is_Match)(void *data, void *param), dll_t *list);

/*O(n)*/
int MultiRemove(avl_t *avl, void *param, int (*Is_Match)(void *data, void *param), dll_t *list);

#endif /*AVL_H*/
