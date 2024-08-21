/**************************************************************|	
|		    .. AVL Tree Implementation ..              ********|
|  (\.../)	.. Authored by Michael Bar 19/08/2024 ..   ********|
|  (=';'=) 	.. code reviewd by TBD ..                  ********|
|  (")-("))	.. The only hard day was yesterday ! ..    ********|
***************************************************************/
#include <stdlib.h> /*malloc, free*/
#include "avl.h"

enum children {
    LEFT = 0,
    RIGHT = 1,
    NUMBER_OF_CHILD = 2
};

struct node
{
    void *data;
    size_t height;
    avl_node_t *children[NUMBER_OF_CHILD];
};

struct avl
{
    avl_node_t *root;
    compare_func_t cmp_func;
};


/* Helpers */

static avl_node_t *GetMin(avl_node_t *node);
static size_t Height(avl_node_t *node);
static int UpdateHeight(avl_node_t *node);
static int GetBalance(avl_node_t *node);
static avl_node_t *CreateNode(void *data);
static avl_node_t *RotateRight(avl_node_t *node);
static avl_node_t *RotateLeft(avl_node_t *node);
static avl_node_t *Balance(avl_node_t *node, void *data, compare_func_t compare_func);


/* Wrappers */
static void FreeTree(avl_node_t *node);
static avl_node_t *Insert(avl_node_t *node, void *data, compare_func_t compare_func);
static void *Find(avl_node_t *node, const void *data, compare_func_t compare_func);
static avl_node_t *Remove(avl_node_t *node, const void *data, compare_func_t compare_func);
static int ForEach(avl_node_t *node, action_func_t action_func, void *param);
static size_t Size(avl_node_t *node);
static int MultiFind(avl_node_t *node, void *param, avl_is_match_t IsMatch, dll_t *list);
static int MultiRemove(avl_node_t *node, void *param, avl_is_match_t IsMatch, dll_t *list, compare_func_t compare_func, dll_iterator_t iter);




/*API Functions*/

avl_t *AVLCreate(compare_func_t cmp_func)
{
    avl_t *new_avl = malloc(sizeof(avl_t));
    if(NULL == new_avl)
        return NULL;

    new_avl->root = NULL;
    new_avl->cmp_func = cmp_func;

    return new_avl;
}

size_t AVLHeight(const avl_t *avl)
{
    return avl->root->height;   
}

int AVLIsEmpty(const avl_t *avl)
{
    return avl->root == NULL;
}

void AVLDestroy(avl_t *avl)
{
    FreeTree(avl->root);
    free(avl);
}

void *AVLFind(const avl_t *avl, const void *data)
{
    return Find(avl->root, data, avl->cmp_func);
}

size_t AVLSize(const avl_t *avl)
{
    return avl->root ? Size(avl->root) : 0;
}

int AVLForEach(avl_t *avl, action_func_t action_func, void *param)
{
    return ForEach(avl->root, action_func, param);
}

void AVLRemove(avl_t *avl, void *data)
{
    avl->root = Remove(avl->root, data, avl->cmp_func);
}

int AVLInsert(avl_t *avl, const void *data) /*Duplicate will be undefined*/
{
    avl->root = Insert(avl->root, (void *)data, avl->cmp_func);
    return avl->root ? 1 : 0 ;
} 

int AVLMultiFind(const avl_t *avl, void *param, avl_is_match_t IsMatch , dll_t *list)
{
    return MultiFind(avl->root, param, IsMatch, list);
}

int AVLMultiRemove(avl_t *avl, void *param, avl_is_match_t IsMatch, dll_t *list)
{

    if(1 == DLLIsEmpty(list))
    {
        MultiFind(avl->root, param, IsMatch, list);
    }

    return MultiRemove(avl->root, param, IsMatch, list, avl->cmp_func, DLLBegin(list));
}







static size_t Height(avl_node_t *node)
{
    return node != NULL ? node->height : 0;
}

static int GetBalance(avl_node_t *node)
{
    return (int)Height(node->children[LEFT]) - (int)Height(node->children[RIGHT]);
}

static int UpdateHeight(avl_node_t *node)
{
    size_t left_height = Height(node->children[LEFT]);
    size_t right_height = Height(node->children[RIGHT]);
    node->height = 1 + (left_height > right_height ? left_height : right_height);
    return 1;

}

static avl_node_t *CreateNode(void *data)
{
    avl_node_t *new_node = malloc(sizeof(avl_node_t));
    if(NULL == new_node)
        return NULL;
     
    new_node->data = data;
    new_node->height = 1;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    return new_node;
}

static avl_node_t *RotateRight(avl_node_t *node)
{
    avl_node_t *left = node->children[LEFT];
    avl_node_t *right = left->children[RIGHT];

    left->children[RIGHT] = node;
    node->children[LEFT] = right;

    UpdateHeight(node);
    UpdateHeight(left);

    return left;
}

static avl_node_t *RotateLeft(avl_node_t *node)
{
    avl_node_t *right = node->children[RIGHT];
    avl_node_t *left = right->children[LEFT];

    right->children[LEFT] = node;
    node->children[RIGHT] = left;

    UpdateHeight(node);
    UpdateHeight(right);
    

    return right;
}

static avl_node_t *Insert(avl_node_t *node, void *data, compare_func_t compare_func)
{
    int compare = 0;
    if(NULL == node)
        return CreateNode(data);
    
    compare = compare_func(data, node->data);

    if(compare < 0)
        node->children[LEFT] = Insert(node->children[LEFT], data, compare_func);
    else if(compare > 0)
        node->children[RIGHT] = Insert(node->children[RIGHT], data, compare_func);
    else
        return node;

    UpdateHeight(node);

    return Balance(node, data, compare_func);
}

static void FreeTree(avl_node_t *node)
{
    if(node == NULL)
        return;
    
    FreeTree(node->children[LEFT]);
    FreeTree(node->children[RIGHT]);
    free(node);
}

static void *Find(avl_node_t *node, const void *data, compare_func_t compare_func)
{
    int compare = -9;
    if(NULL == node)
        return NULL;
    
    compare = compare_func((void *)data, node->data);
    
    if(compare < 0)
        return Find(node->children[LEFT], data, compare_func);
    if(compare > 0)
        return Find(node->children[RIGHT], data, compare_func);
    else 
        return node->data;
}

static avl_node_t *Remove(avl_node_t *node, const void *data, compare_func_t compare_func)
{
    avl_node_t *temp = NULL;
    int compare;

    if (node == NULL) {
        return NULL;
    }

    compare = compare_func(data, node->data);

    if (compare < 0) {
        node->children[LEFT] = Remove(node->children[LEFT], data, compare_func);
    } else if (compare > 0) {
        node->children[RIGHT] = Remove(node->children[RIGHT], data, compare_func);
    } else {
        if (node->children[LEFT] == NULL && node->children[RIGHT] == NULL) 
        {
            free(node);
            return NULL;
        } 
        else if (node->children[LEFT] == NULL) 
        {
            temp = node->children[RIGHT];
            free(node);
            return temp;
        }
         else if (node->children[RIGHT] == NULL) 
        {
            temp = node->children[LEFT];
            free(node);
            return temp;
        }

        temp = GetMin(node->children[RIGHT]);
        node->data = temp->data;
        node->children[RIGHT] = Remove(node->children[RIGHT], temp->data, compare_func);
    }

    UpdateHeight(node);
    return Balance(node, (void *)data, compare_func);
}

static int ForEach (avl_node_t *node, action_func_t action_func, void *param)
{
    avl_node_t *left = NULL;
    avl_node_t *right = NULL;
    if(NULL == node) 
        return 0;
    
    left = node->children[LEFT];
    right = node->children[RIGHT];
    return action_func(node->data, param) + ForEach(left, action_func, param) + ForEach(right, action_func, param);
}

static size_t Size(avl_node_t *node)
{
    if (NULL == node)
        return 0;
    
    return 1 + Size(node->children[LEFT]) + Size(node->children[RIGHT]);
}

static avl_node_t *Balance(avl_node_t *node, void *data, compare_func_t compare_func)
{
        int balance = GetBalance(node);
    
        if(balance > 1 && compare_func(data, node->children[LEFT]->data) < 0)
            return RotateRight(node);

        if(balance > 1 && compare_func(data, node->children[LEFT]->data) > 0)
        {
            node->children[LEFT] = RotateLeft(node->children[LEFT]);
            return RotateRight(node);
        }

        if(balance < -1 && compare_func(data, node->children[RIGHT]->data) > 0)
            return RotateLeft(node);

        if(balance < -1 && compare_func(data, node->children[RIGHT]->data) < 0)
        {
            node->children[RIGHT] = RotateRight(node->children[RIGHT]);
            return RotateLeft(node);
        }
            
    return node;
}

static avl_node_t *GetMin(avl_node_t *node)
{
    while(node->children[LEFT] != NULL)
    {
        node = node->children[LEFT];
    }
    return node;
}

static int MultiFind(avl_node_t *node, void *param, avl_is_match_t IsMatch, dll_t *list)
{
    int left_count=0, right_count=0, add=0;
    if(NULL == node)
        return 0;

    if(1 == IsMatch(node->data, param))
    {
        if(NULL == DLLInsert(DLLBegin(list), node->data, list))
        {
            DLLDestroy(list);
            return -1;
        }
        add = 1;
    }

    left_count = MultiFind(node->children[LEFT], param, IsMatch, list);
    {
        if (-1 == left_count)
            return -1;
    }
    right_count = MultiFind(node->children[RIGHT], param, IsMatch, list);
    {
        if (-1 == right_count)
            return -1;
    }

    return (left_count+right_count+add);
}

static int MultiRemove(avl_node_t *node, void *param, avl_is_match_t IsMatch, dll_t *list, compare_func_t compare_func, dll_iterator_t iter)
{
    if(DLLNext(iter) == NULL)
        return 0;
    
    if(DLLGetData(iter) == node->data)
        Remove(node, node->data, compare_func);

    MultiRemove(node->children[LEFT], node->children[LEFT]->data, IsMatch, list, compare_func, DLLNext(iter));
    MultiRemove(node->children[RIGHT], node->children[RIGHT]->data, IsMatch, list, compare_func, DLLNext(iter));
}