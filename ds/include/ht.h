#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h>

#include "dll.h"


typedef int (*compare_t)(const void*, const void*);
typedef int (*hash_func_t)(void *data);
typedef struct hash_table hash_t;

hash_t *HASHCreate(hash_func_t hash_func, compare_t cmp_func, size_t capacity);
/*
time complexity: O(1)
arguments: hash_t hash_func (hash function)
           compare_t cmp_func (compare function)
           size_t capacity (the size of the hash array)
return value: hash_t * (the new hash table)
              NULL if malloc failed
*/

void HASHDestroy(hash_t *hash);
/*
time complexity: O(1)
arguments: hash_t * (hash table)
return value: void
*/

int HASHInsert(hash_t *hash, void *val);
/*
time complexity: O(1)
arguments: hash_t * (hash table)
           void *val (value to insert)
return value: int (0 for success / -1 for malloc fail)
*/

void HASHRemove(hash_t *hash, void *val);
/*
time complexity: average O(1), worst O(n)
arguments: hash_t * (hash table)
           void *val (value to remove)
return value: void
*/

void *HASHFind(hash_t *hash, void *val);
/*
time complexity: O(n)
arguments: hash_t * (hash table)
           void *val (value to insert)
return value: int (0 for success / -1 for malloc fail)
*/

size_t HASHSize(const hash_t *hash);
/*
time complexity: O(n)
arguments: const hash_t * (hash table)
return value: size_t (size of hash table)
*/

int HASHIsEmpty(const hash_t *hash);
/*
time complexity: O(n)
arguments: arguments: const hash_t * (hash table)
return value: int (1 for empty / 0 for not empty)
*/

int HASHForEach(const hash_t *hash, action_t action_func, void *param);
/*
time complexity: O(n)
arguments: const hash_t * (hash table)
           action_t action_func (action function)
           void *param (a parameter for the use of the action function)
return value: int (number of action function successes or failures, depends on sll implementation)
*/


#endif /*__HASH_H__*/