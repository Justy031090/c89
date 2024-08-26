#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc, free*/
#include <assert.h>

#include "dll.h"
#include "ht.h"


struct hash_table
{
    dll_t **hash_arr ;
    compare_t cmp_func;
    hash_func_t hash_func;
    size_t capacity;
};


static void CleanHash(hash_t *ht, size_t capacity);
static size_t GetIdx(const hash_t *hash, void *val);
static dll_t *GetListIdx(const hash_t *hash, size_t index);


hash_t *HASHCreate(hash_func_t hash_func, compare_t cmp_func, size_t capacity)
{
    size_t i = 0;
    hash_t *ht = malloc(sizeof(hash_t));
    dll_t **hash_arr = calloc(capacity, sizeof(dll_t *));
    if(NULL == ht) return NULL;
    if(NULL == hash_arr)
    {
        free(ht);
        return NULL;
    }

    for(i=0; i< capacity; ++i)
    {
        hash_arr[i] = DLLCreate();
        if(NULL == hash_arr[i])
        {
            CleanHash(ht, i);
            return NULL;
        }
    }

    ht->hash_arr = hash_arr;
    ht->hash_func = hash_func;
    ht->cmp_func = cmp_func;
    ht->capacity = capacity;

    return ht;
}

void HASHDestroy(hash_t *hash)
{
    assert(NULL != hash);
    CleanHash(hash, hash->capacity);
    free(hash);
}

int HASHInsert(hash_t *hash, void *val)
{
    size_t index = 0;
    dll_t *list = NULL;
    assert(NULL != hash);
    assert(NULL != val);

    index = GetIdx(hash, val);
    list = GetListIdx(hash, index);
    if(NULL == list) return -1;

    DLLPushFront(list, val);

    return 1;
}

void HASHRemove(hash_t *hash, void *val)
{
    size_t index = 0;
    dll_t *list = NULL;
    dll_iterator_t iter = NULL;
    assert(NULL != hash);
    assert(NULL != val);

    index = GetIdx(hash, val);
    list = GetListIdx(hash, index);
    if(NULL == list) return;

    iter = DLLBegin(list);
    while(iter != DLLEnd(list))
    {
        if(1 == hash->cmp_func(DLLGetData(iter), val))
        {
            DLLRemove(iter, list);
            return;
        }
        iter = DLLNext(iter);
    }
}

void *HASHFind(hash_t *hash, void *val)
{
    size_t index = 0;
    dll_t *list = NULL;
    dll_iterator_t iter = NULL;

    assert(NULL != hash);
    assert(NULL != val);

    
    index = GetIdx(hash, val);   
    list = GetListIdx(hash, index);
    if(NULL == list) return NULL;
    iter = DLLBegin(list);
    while(iter != DLLEnd(list))
    {
                
        if(1 == hash->cmp_func(DLLGetData(iter), val))
            return DLLGetData(iter);


        iter = DLLNext(iter);
    }

    return NULL;
}

size_t HASHSize(const hash_t *hash)
{
    size_t i = 0;
    size_t count = 0;
    assert(NULL != hash);
    for(i = 0 ; i< hash->capacity; ++i)
    {
        count += DLLSize(hash->hash_arr[i]);
    }
    return count;
}

int HASHIsEmpty(const hash_t *hash)
{
    size_t i = 0;
    assert(NULL != hash);
    
    for(i = 0; i< hash->capacity; ++i)
    {
        if(!DLLIsEmpty(hash->hash_arr[i]))
            return 0;
    }
    return 1;
}

int HASHForEach(const hash_t *hash, action_t action_func, void *param)
{
    size_t i = 0;
    int count = 0;
    assert(NULL != hash);
    for(i = 0; i< hash->capacity; ++i)
    {
        count += DLLForEach(DLLBegin(hash->hash_arr[i]), DLLEnd(hash->hash_arr[i]), action_func, param);
    }
    return count;
}




/*Helpers*/
static void CleanHash(hash_t *ht, size_t capacity)
{
    size_t i = 0;
    if (ht->hash_arr)
    {
        for (i = 0; i < capacity; ++i)
        {
            if (ht->hash_arr[i])
            {
                DLLDestroy(ht->hash_arr[i]);
            }
        }
        free(ht->hash_arr);
    }
}

static size_t GetIdx(const hash_t *hash, void *val)
{
    return hash->hash_func(val) % hash->capacity;
}

static dll_t *GetListIdx(const hash_t *hash, size_t index)
{
    return (index < hash->capacity) ? hash->hash_arr[index] : NULL;
}