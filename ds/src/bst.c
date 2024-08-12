#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "bst.h"

#define EQUAL (0)
#define SMALLER (-1)
#define BIGGER (1)
#define STACK_SIZE (200)
struct bst
{
    node_t root_dummy;
    compare_func_t cmp_func;
};

static bst_iter_t FindMin(bst_iter_t iter);
static bst_iter_t FindMax(bst_iter_t iter);
static void Free(bst_iter_t iter);
static size_t CountNodes(bst_iter_t iter);

bst_t *BSTCreate(compare_func_t cmp_func)
{
    bst_t *new_bst = malloc(sizeof(bst_t));
    if(NULL == new_bst)
        return NULL;

    new_bst->root_dummy.data = NULL;
    new_bst->root_dummy.parent = NULL;
    new_bst->root_dummy.child_node[LEFT] = NULL;
    new_bst->root_dummy.child_node[RIGHT] = NULL;
    new_bst->cmp_func = cmp_func;

    return new_bst;
}

void BSTDestroy(bst_t *bst)
{
    assert(NULL!=bst);
    Free(bst->root_dummy.child_node[LEFT]);
    free(bst);
}

bst_iter_t Insert(bst_t *bst, const void *data)
{
    bst_iter_t to_insert = malloc(sizeof(node_t));
    bst_iter_t current = NULL;
    bst_iter_t parrent = NULL;
    int temp = 0;

    if(NULL == to_insert || NULL == bst || NULL == data)
        return NULL;

    to_insert->data = (void *)data;
    to_insert->child_node[LEFT] = NULL;
    to_insert->child_node[RIGHT] = NULL;
    to_insert->parent = NULL;

    if(bst->root_dummy.child_node[LEFT] == NULL)
    {
        bst->root_dummy.child_node[LEFT] = to_insert;
        to_insert->parent = &bst->root_dummy;
        return to_insert;
    }

    current = bst->root_dummy.child_node[LEFT];
    while (NULL != current)
    {
        parrent = current;
        temp = bst->cmp_func(current->data, data);

        if(temp == EQUAL)
        {
            free(to_insert);
            return NULL;
        }
        current = (temp == SMALLER) ? current->child_node[LEFT] : current->child_node[RIGHT];

    }
    
    temp = bst->cmp_func(parrent->data, data);
    if(temp == SMALLER)
    {
        parrent->child_node[LEFT] = to_insert;
    }
    else
    {
        parrent->child_node[RIGHT] = to_insert;
    }

    to_insert->parent = parrent;
    return to_insert;
}

bst_iter_t BSTRemove(bst_iter_t iter) {
    bst_iter_t next = NULL;
    bst_iter_t successor = NULL;
    
    if (NULL != iter->child_node[LEFT] && NULL != iter->child_node[RIGHT]) {
        successor = FindMin(iter->child_node[RIGHT]);
        iter->data = successor->data;
        return BSTRemove(successor);
    }

    if (NULL != iter->child_node[LEFT]) 
    {
        next = iter->child_node[LEFT];
    } 
    else 
    {
        next = iter->child_node[RIGHT];
    }

    if (NULL != iter->parent) 
    {
        if (iter == iter->parent->child_node[LEFT]) 
        {
            iter->parent->child_node[LEFT] = next;
        } 
        else 
        {
            iter->parent->child_node[RIGHT] = next;
        }
        if (NULL != next) 
        {
            next->parent = iter->parent;
        }
    } 
    else 
    {
        if (NULL != next) 
        {
            next->parent = NULL;
        }
    }

    free(iter);
    return next;
}

bst_iter_t BSTFind(const bst_t *bst, const void *data)
{
    bst_iter_t node = bst->root_dummy.child_node[LEFT];
    int temp = 0;
    while(NULL != node)
    {
        temp = bst->cmp_func(node->data, data);
        if(temp == EQUAL)
        {
            return node;
        }
        node = temp == SMALLER ? node->child_node[LEFT] : node->child_node[RIGHT];
    }
    return NULL;
}

int BSTIsEmpty(const bst_t *bst)
{
    assert(bst != NULL);
    return bst->root_dummy.child_node[LEFT] == NULL;
}

void *BSTGetData(bst_iter_t iter)
{
    return iter ? iter->data : NULL;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    bst_iter_t parent = NULL;

    assert(NULL != iter);
    
    parent = iter->parent;

    if(NULL != iter->child_node[RIGHT])
    {
        return FindMin(iter->child_node[RIGHT]);
    }

    while(iter->parent->data != NULL && iter == parent->child_node[RIGHT])
    {
        iter = iter->parent;
    }
    if(NULL != iter->parent->data && 0 == BSTIsEqual(iter, iter->parent->child_node[RIGHT]))
        return iter->parent;
    else 
        return NULL;
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    bst_iter_t parent = NULL;
    
    assert(NULL != iter);

    if(NULL == iter->parent) return NULL;

    if(NULL != iter->child_node[LEFT])
    {
        return iter->child_node[LEFT];
    }
    while(NULL != iter->parent && iter == iter->parent->child_node[LEFT])
    {
        iter = iter->parent;
    }
    return iter->parent;
}

bst_iter_t BSTBegin(const bst_t *bst)
{
    bst_iter_t childL = NULL;
    assert(NULL != bst);
    childL = bst->root_dummy.child_node[LEFT];
    return FindMin(childL);
}

bst_iter_t BSTEnd(const bst_t *bst)
{
    return (bst_iter_t)&bst->root_dummy;
}

int BSTIsEqual(bst_iter_t iter1, bst_iter_t iter2)
{
    return iter1 == iter2;
}

int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func, const void *param)
{
    
    int counter = 0;
    bst_iter_t runner = from;
    
    assert(NULL != from && NULL != to);

    while(runner != to && NULL != runner)
    {
        if (0 == action_func(runner->data, (void *)param))
        {
            ++counter;
        }
        runner = BSTNext(runner);
    }
    return counter;
}

size_t BSTSize(const bst_t *bst)
{   
    assert(bst);
    return CountNodes(bst->root_dummy.child_node[LEFT]);
    
}

static bst_iter_t FindMin(bst_iter_t iter)
{
    if(iter == NULL) return NULL;
    while(iter->child_node[LEFT]!= NULL)
    {
        iter = iter->child_node[LEFT];
    }
    return iter;
}

static bst_iter_t FindMax(bst_iter_t iter)
{
    if(iter == NULL) return NULL;
    while(iter->child_node[RIGHT]!= NULL)
    {
        iter = iter->child_node[RIGHT];
    }
    return iter;
}

static void Free(bst_iter_t iter)
{
    bst_iter_t current = iter;
    bst_iter_t stack[STACK_SIZE];
    bst_iter_t childR = NULL;
    size_t stack_size = 0;
    if(iter == NULL) return;
    while(NULL != current || stack_size > 0)
    {
            while(NULL != current)
            {
                stack[stack_size++] = current;
                current = current->child_node[LEFT];
            }

            current = stack[--stack_size];
            childR = current->child_node[RIGHT];
            free(current);
            current = childR;

    }
}

static size_t CountNodes(bst_iter_t iter) 
{
    size_t count = 0;
    bst_iter_t current = iter;
    bst_iter_t stack[STACK_SIZE];
    size_t stack_size = 0;

    while (current != NULL || stack_size > 0) 
    {
        while (current != NULL) 
        {
            stack[stack_size++] = current;
            current = current->child_node[LEFT];
        }

        current = stack[--stack_size];
        ++count;
        current = current->child_node[RIGHT];
    }

    return count;
}