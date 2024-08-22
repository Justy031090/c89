#include <stdlib.h>
#include <stdio.h>
#include "yuval.h"

enum children {
    LEFT = 0,
    RIGHT = 1,
    NUMBER_OF_CHILD = 2
};

typedef struct node avl_node_t;

struct node
{
    void *data;
    size_t height;
    avl_node_t *children[NUMBER_OF_CHILD];
};

struct avl
{
    avl_node_t *root_dummy;
    compare_func_t cmp_func;
};

static void *RecurFind(void *data, compare_func_t cmp_func, avl_node_t *node);
static int NoFunc(void *param, void *data);
static void RecurDestroy(avl_node_t *node);
static avl_node_t *RotateL(avl_node_t *node);
static avl_node_t *RotateR(avl_node_t *node);
static size_t Max(size_t a, size_t b);
static size_t GetHeight(avl_node_t *node);
static int RecurForEach(void *param, action_func_t action_func, avl_node_t *node);
static int RecurMultiFind(avl_node_t *node, void *param, int (*Is_Match)(void *data, void *param), dll_t *list);
static int RecurInsert(avl_node_t *node, avl_node_t *new, void *data, compare_func_t compare_func);
static avl_node_t *MostLeft(avl_node_t *node);
static avl_node_t *RecurRemove(avl_node_t *node, void *data, compare_func_t compare_func);
static size_t FixHeight(avl_node_t *node);
static size_t GetBalance(avl_node_t *node);
static int RecurMultiRemove(avl_node_t *node, void *param, int (*Is_Match)(void *data, void *param), dll_t *list, int count, compare_func_t compare_func);


avl_t *AVLCreate(compare_func_t cmp_func)
{
    avl_t *avl = (avl_t*)malloc(sizeof(avl_t));
    if ( NULL == avl )
    {
        return NULL;
    }

    avl->root_dummy = (avl_node_t*)malloc(sizeof(avl_node_t));
    if ( NULL == avl->root_dummy )
    {
        free(avl);
        return NULL;
    }

    avl->root_dummy->data = NULL;
    avl->root_dummy->height = 0;
    avl->root_dummy->children[LEFT] = NULL;
    avl->root_dummy->children[RIGHT] = NULL;

    avl->cmp_func = cmp_func;
    
    return avl;
     }

void AVLDestroy(avl_t *avl)
{
    RecurDestroy(avl->root_dummy->children[LEFT]);
    free(avl->root_dummy);
    free(avl);
    avl = NULL;
}

int AVLInsert(avl_t *avl, const void *data)
{
    int check = 0;
    avl_node_t *runner = NULL;
    avl_node_t *new = (avl_node_t*)malloc(sizeof(avl_node_t));
    if ( NULL == new )
    {
        return -1;
    }

    new->data = (void *)data; 
    new->children[LEFT]= NULL;
    new->children[RIGHT] = NULL;
    new->height = 1;

    if (NULL == avl->root_dummy->children[LEFT])
    {
        avl->root_dummy->children[LEFT] = new;
        return 0;
    }
    runner = avl->root_dummy->children[LEFT];
    check = RecurInsert(runner, new, (void*)data, avl->cmp_func);
    if ( check != 0 )
    {
        avl->root_dummy->children[LEFT] = new;
    }
    return 0;
}

/*O(logn)*/
void AVLRemove(avl_t *avl, void *data)
{
    avl->root_dummy->children[LEFT] = RecurRemove(avl->root_dummy->children[LEFT], data, avl->cmp_func);
}

/*O(logn)*/
void *AVLFind(const avl_t *avl, const void *data)
{
    return RecurFind((void*)data, avl->cmp_func, avl->root_dummy->children[LEFT]);
}

size_t AVLSize(const avl_t *avl)
{
    if ( AVLIsEmpty(avl) == 1 )
    {
        return 0;
    }
    return AVLForEach((avl_t*)avl, NoFunc, NULL);
}

/*O(1)*/
int AVLIsEmpty(const avl_t *avl)
{
    return (NULL == avl->root_dummy->children[LEFT]);
}

/*O(n)*/
int AVLForEach(avl_t *avl, action_func_t action_func, void *param)
{
    return RecurForEach(param, action_func, avl->root_dummy->children[LEFT]);
}

/*O(1)*/
size_t AVLHeight(const avl_t *avl)
{
    return avl->root_dummy->children[LEFT]->height;
}

/*O(n)*/
int AVLMultiFind(const avl_t *avl, void * param, int (*Is_Match)(void *data, void *param), dll_t *list)
{
    return RecurMultiFind(avl->root_dummy->children[LEFT], param, Is_Match, list);
}

/*O(n)*/
int AVLMultiRemove(avl_t *avl, void *param, int (*Is_Match)(void *data, void *param), dll_t *list)
{
    int to_remove = RecurMultiFind(avl->root_dummy->children[LEFT], param, Is_Match, list);
    RecurMultiRemove(avl->root_dummy->children[LEFT], param, Is_Match, list, to_remove, avl->cmp_func);
    return to_remove;
}

static int NoFunc(void *param, void *data)
{
	(void)data;
	(void)param;
	return 1;
}	

static void *RecurFind(void *data, compare_func_t cmp_func, avl_node_t *node)
{
    if ( NULL == node )
        {
            return NULL;
        }
   
    if ( 0 == cmp_func(data, node->data) )
    {
        return node->data;
    }


    if ( 0 > cmp_func(data, node->data) )
    {
        return RecurFind(data, cmp_func, node->children[RIGHT]);
    }

    else 
    {
        return RecurFind(data, cmp_func, node->children[LEFT]);
    }
}

static void RecurDestroy(avl_node_t *node)
{
    if ( NULL == node )
    {
        return;
    }

    RecurDestroy(node->children[LEFT]);
    RecurDestroy(node->children[RIGHT]);
    free(node);
    node = NULL;
}

static avl_node_t *RotateL(avl_node_t *node)
{
    avl_node_t *new_root = node->children[RIGHT];
    avl_node_t *tmp = new_root->children[LEFT];

    new_root->children[LEFT] = node;
    node->children[RIGHT] = tmp;

    node->height = Max(GetHeight(node->children[LEFT]), GetHeight(node->children[RIGHT]));
    new_root->height = Max(GetHeight(new_root->children[LEFT]), GetHeight(new_root->children[RIGHT]));

    return new_root;
}

static avl_node_t *RotateR(avl_node_t *node)
{
    avl_node_t *new_root = node->children[LEFT];
    avl_node_t *tmp = new_root->children[RIGHT];

    new_root->children[RIGHT] = node;
    node->children[LEFT] = tmp;

    node->height = Max(GetHeight(node->children[LEFT]), GetHeight(node->children[RIGHT]));
    new_root->height = Max(GetHeight(new_root->children[LEFT]), GetHeight(new_root->children[RIGHT]));

    return new_root;
}

static size_t Max(size_t a, size_t b)
{
    return a > b ? a : b;
}

static size_t GetHeight(avl_node_t *node)
{
    return node->children[LEFT]->height;
}

static int RecurForEach(void *param, action_func_t action_func, avl_node_t *node)
{
    if ( NULL == node )
    {
        return 0;
    }
   
    return action_func(node->data, param) + RecurForEach(param, action_func, node->children[LEFT]) + RecurForEach(param, action_func, node->children[RIGHT]);
}

static int RecurInsert(avl_node_t *node, avl_node_t *new, void *data, compare_func_t compare_func)
{
    int balanced = 0;

    if ( 0 == compare_func(data, node->data))
    {
        return -1;
    }

    if ( 0 < compare_func(data, node->data))
    {
        if (NULL != node->children[RIGHT])
        {
            RecurInsert(node->children[RIGHT], new, data, compare_func);
        }
        else
        {
            node->children[RIGHT] = new;
            return 0;
        }
    }
    else
    {
        if (NULL != node->children[LEFT])
        {
            RecurInsert(node->children[LEFT], new, data, compare_func);
        }
        else
        {
            node->children[LEFT] = new;
       
            return 0;
        }
    }

    node->height = FixHeight(node);
    balanced = GetBalance(node);
    
        if(balanced > 1 && GetBalance(node->children[LEFT]) >= 0)
            RotateR(node);

        if(balanced > 1 && GetBalance(node->children[LEFT])  < 0)
        {
            node->children[LEFT] = RotateL(node->children[LEFT]);
            RotateR(node);
        }

        if(balanced < -1 && GetBalance(node->children[RIGHT]) <= 0)
            RotateL(node);

        if(balanced < -1 && GetBalance(node->children[RIGHT]) > 0)
        {
            node->children[RIGHT] = RotateR(node->children[RIGHT]);
            RotateL(node);
        }

    node->height = FixHeight(node);
    return 0;
}

static int RecurMultiFind(avl_node_t *node, void *param, int (*Is_Match)(void *data, void *param), dll_t *list)
{
    if ( NULL == node || NULL == list )
    {
        return 0;
    }
    
    if ( 1 == Is_Match(node->data, param) )
    {
        if ( DLLEnd(list) == DLLInsert(DLLBegin(list), node->data, list))
        {
            DLLDestroy(list);
        }
        else
        {
        return 1 + RecurMultiFind(node->children[LEFT], param, Is_Match, list) + RecurMultiFind(node->children[RIGHT], param, Is_Match, list);
        }
    }
    else
    {
        RecurMultiFind(node->children[LEFT], param, Is_Match, list) + RecurMultiFind(node->children[RIGHT], param, Is_Match, list);
    }    
    
    return 0;
}

static avl_node_t *RecurRemove(avl_node_t *node, void *data, compare_func_t compare_func)
{
    avl_node_t *tmp = NULL;
    avl_node_t *next = NULL;
    
    if ( 0 > compare_func(data, node->data))
    {
        node->children[RIGHT] = RecurRemove(node->children[RIGHT], data, compare_func);
    }
    else if ( 0 < compare_func(data, node->data)) 	
    {
        node->children[LEFT] = RecurRemove(node->children[LEFT], data, compare_func);
        
    }
    else
    {
        if ( NULL == node->children[LEFT] && NULL == node->children[RIGHT] )
        {
            printf("leaf\n");
            free(node);
            return NULL;
        }
        else if ( NULL == node->children[LEFT] && NULL != node->children[RIGHT])
        {
            printf("left child\n");
            tmp = node->children[RIGHT];
            free(node);
            return tmp;
        }
        else if ( NULL != node->children[LEFT] && NULL == node->children[RIGHT])
        {
            printf("right child\n");
            tmp = node->children[LEFT];
            free(node);
            return tmp;
        }
        else
        {
            if (NULL != node->children[RIGHT]->children[LEFT])
            {
                tmp = MostLeft(node->children[RIGHT]);
                printf("2222222222222\n");
                if (NULL != tmp->children[LEFT]->children[RIGHT])
                {
                    *(void**)&node->data = tmp->children[LEFT]->data;
                    next = tmp->children[LEFT]->children[RIGHT];
                    free(tmp->children[LEFT]);
                    tmp->children[LEFT] = next;
                    return node;
                }
                else
                {
                    *(void **)&(node->data) = tmp->children[LEFT]->data;
                    free(tmp->children[LEFT]);/*do swop*/
                    tmp->children[LEFT] = NULL;
                    return node;

                }
            }
            else 
            {
                tmp = node->children[RIGHT];
                *(void **)&(node->data) = tmp->data;
                node->children[RIGHT] = tmp->children[RIGHT];
                free(tmp);
                return node;
            }
        }
    }
    FixHeight(node);
    return node;
}

static avl_node_t *MostLeft(avl_node_t *node)
{
    if ( NULL != node && NULL != node->children[LEFT] && NULL == node->children[LEFT]->children[LEFT] )
    {
        return node;
    }
    return MostLeft(node->children[LEFT]);
}

static size_t FixHeight(avl_node_t *node)
{
    size_t lheight = 0;
    size_t rheight = 0;

    if ( NULL == node )
    {
        return 0;
    }
   
    if (NULL != node->children[LEFT])
    {
        lheight = node->children[LEFT]->height;
    }
    if (NULL != node->children[RIGHT])
    {
        rheight = node->children[RIGHT]->height;
    }
    node->height = 1 + Max(lheight, rheight);
    return node->height;
}

static size_t GetBalance(avl_node_t *node)
{
   return node ? FixHeight(node->children[LEFT]) - FixHeight(node->children[RIGHT]) : 0;
}

static int RecurMultiRemove(avl_node_t *node, void *param, int (*Is_Match)(void *data, void *param), dll_t *list, int count, compare_func_t compare_func)
{
    if ( 0 == count )
    {
        return 0;
    }
    RecurRemove(node->children[LEFT], param, compare_func);
    return RecurMultiRemove(node->children[LEFT], param, Is_Match, list, count-1, compare_func);
}




   
