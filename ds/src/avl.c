/**************************************************************|	
|		    .. AVL Tree Implementation ..              ********|
|  (\.../)	.. Authored by Michael Bar 19/08/2024 ..   ********|
|  (=';'=) 	.. code reviewd by TBD ..                  ********|
|  (")-("))	.. The only hard day was yesterday ! ..    ********|
***************************************************************/
#include <stdlib.h> /*malloc, free*/
#include "avl.h"

struct avl
{
    node_t *root_dummy;
    compare_func_t cmp_func;
};
enum children {
    LEFT = 0,
    RIGHT = 1,
    NUMBER_OF_CHILD = 2
};


typedef struct node
{
    void *data;
    size_t height;
    node_t *children[NUMBER_OF_CHILD];
} node_t;


static node_t Traverse(node_t *root);





avl_t *AVLCreate(compare_func_t cmp_func)
{
    avl_t *new_avl = malloc(sizeof(avl_t));
    if(NULL == new_avl)
        return NULL;
    node_t *dummy = malloc(sizeof(node_t));
    if(NULL == dummy)
        return NULL;

    new_avl->root_dummy = dummy;
    new_avl->cmp_func = cmp_func;
    new_avl->root_dummy->data = NULL;
    new_avl->root_dummy.height = 0;
    new_avl->root_dummy.children[LEFT] = NULL;
    new_avl->root_dummy.children[RIGHT] = NULL;

    return new_avl;
}

size_t AVLHeight(const avl_t *avl)
{
    return avl->root_dummy->height;   
}

int AVLIsEmpty(const avl_t *avl)
{
    return avl->root_dummy->children[LEFT] ? 1 : 0;
}

void AVLDestroy(avl_t *avl)
{
    node_t *root = avl->root_dummy;
    free(Traverse(root));
    free(avl);
}

void *AVLFind(const avl_t *avl, const void *data)
{
    node_t *root = avl->root_dummy;
    node_t *runner = NULL;
    if(runner == NULL)
        return NULL;
    if(runner->data > data)
        runner = Traverse(runner->children[LEFT])
    else if(runner->data < data)
        runner = Traverse(runner->children[RIGHT]);
    else
        return (void *)runner->data;


    
}

size_t AVLSize(const avl_t *avl)
{
    node_t *root = avl->root_dummy;
    node_t *runner = NULL;
    if(runner == NULL) return 0;
    else
    {
        runner = Traverse(root);
        return 1;
    }
    
}




int AVLForEach(avl_t *avl, action_func_t action_func, void *param)
{
    node_t *root = avl->root_dummy;
    node_t *runner = NULL;

    if(runner == NULL) return 0;
    else
    {
        action_func(runner->data, param); 
    }
    runner = Traverse(root);
}



void AVLRemove(avl_t *avl, void *data)
{
    node_t *root = avl->root_dummy;
    node_t *runner = NULL;
    if(runner == NULL)
        return runner;
    if(runner->children[LEFT]->data == data)
    {
        free(runner->children[LEFT]);
        runner->children[LEFT] = NULL;
        return;
    }
    if(runner->children[LEFT]->data == data)
    {
        free(runner->children[RIGHT]);
        runner->children[RIGHT] = NULL;
        return;
    }

    runner = Traverse(root);
}

int AVLInsert(avl_t *avl, const void *data); /*Duplicate will be undefined*/

int AVLMultiFind(const avl_t *avl, void * param, int (*Is_Match)(void *data, void *param), dll_t *list);

int MultiRemove(avl_t *avl, void *param, int (*Is_Match)(void *data, void *param), dll_t *list);





static node_t Traverse(node_t *root)
{   
    if(root->children[RIGHT]== NULL) return;

    if(root->children[LEFT] == NULL)
        return Traverse(root->children[RIGHT]);
    
    return Traverse(root->children[LEFT]);
}
