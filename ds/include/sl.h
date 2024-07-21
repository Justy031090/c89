#ifndef __SL_H__
#define __SL_H__

#include <stddef.h> /* size_t */
#include "dll.h"

/* type of the sorted list management struct */
typedef struct sl sl_t;

struct iterator
{
    dll_iterator_t dll_iter;
    sl_t *sl;
};

typedef struct iterator sl_iterator_t


/* type of action_t functions:
Description - action function implemented by the user return count of errors.
Params:
	void *data - data from the list.
	void *param - parameter from the user
return value - size_t - number of errors.
*/
typedef size_t (*action_t)(void *data, void *param);


/* type of is_match_t function:
Description - comparison func the user should implemnt if equal return 1, else return 0.
Params:
	void *data - data from the list.
	void *param - parameter from the user
return value - int - boolean vlue.
time complexity - O(n)
space complexity - O(1)
*/
typedef int (*is_match_t)(void *data, void *param);


/* type of compare_func_t function:
Description - comparison func the user should implemnt as sort criteria.
Params:
	void *data - data from the list.
	void *param - parameter from the user
return value - int.
time complexity - O(n)
space complexity - O(1)
*/
typedef int (*compare_func_t)(const void *data, const void *param);


/* SLCreate:
Description - create dll managemet structure and the dummy node.
Params:
	compare_func_t compare_func - sort criteria.
return value - sl_t *
time complexity - O(n)
space complexity - O(1)
*/
sl_t *SLCreate(compare_func_t compare_func);


/* SLDestroy:
Description - delete list.
Params:
	sl_t *sl - management struct.
return value - NONE
time complexity - O(n)
space complexity - O(1)
*/
void SLDestroy(sl_t *sl);


/* SLInsert:
Description - insert new value to list.
Params:
	const void *data - data pointer.
	sl_t *sl - management struct.
return value - sl_iterator_t
time complexity - O(n)
space complexity - O(1)
*/
sl_iterator_t SLInsert(const void *data, sl_t *sl);


/* DLLRemove:
Description - remove value from list.
Params:
	sl_iterator_t iter - iterator.
	dll_t *dll - management struct.
return value - sl_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
sl_iterator_t SLRemove(sl_iterator_t iter);


/* SLGetData:
Description - get a value from the list.
Params:
	sl_iterator_t iter - iterator.
return value - void *.
time complexity - O(1)
space complexity - O(1)
*/
void *SLGetData(const sl_iterator_t iter);


/* SLSize:
Description - return list size.
Params:
	sl_t *sl - management struct.
return value - size_t.
time complexity - O(n)
space complexity - O(1)
*/
size_t SLSize(const sl_t *sl);


/* SLIsEmpty:
Description - is the list empty.
Params:
	const sl_t *sl - management struct.
return value - int - 1 if empty else 0.
time complexity - O(1)
space complexity - O(1)
*/
int SLIsEmpty(const sl_t *sl);


/* SLBegin:
Description - move iterator to head.
Params:
	sl_t *sl - management struct.
return value - sl_iterator_t - first valid node (not dummy)
time complexity - O(1)
space complexity - O(1)
*/
sl_iterator_t SLBegin(const sl_t *sl);


/* SLEnd:
Description - move iterator to tail.
Params:
	sl_t *sl - management struct.
return value - sl_iterator_t - tail dummy node.
time complexity - O(1)
space complexity - O(1)
*/
sl_iterator_t SLEnd(const sl_t *sl);


/* SLNext:
Description - move iterator to next node.
Params:
	sl_iterator_t iter - iterator pointer.
return value - sl_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
sl_iterator_t SLNext(sl_iterator_t iter);


/* SLPrev:
Description - move iterator to previous node.
Params:
	sl_iterator_t iter - iterator pointer.
return value - sl_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
sl_iterator_t SLPrev(sl_iterator_t iter);


/* SLIsEqual:
Description - compare two nodes.
Params:
	const sl_iterator_t iter1 - iterator.
	const sl_iterator_t iter2 - iterator.
return value - int - boolean.
time complexity - O(1)
space complexity - O(1)
*/
int SLIsEqual(const sl_iterator_t iter1, const sl_iterator_t iter2);


/* SLPopFront:
Description - pop value from the head of the list.
Params:
	sl_t *sl - management struct.
return value - sl_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
sl_iterator_t SLPopFront(sl_t *sl);


/* SLPopBack:
Description - pop value from the last item in the list.
Params:
	sl_t *sl - management struct.
return value - sl_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
sl_iterator_t SLPopBack(sl_t *sl);


/* SLFind:
Description - find a value in the list using sort criteria.
Params:
	const sl_iterator_t from - iterator to iterate from.
	const sl_iterator_t to - iterator to iterate to (exclusive).
	is_match_t is_match - 
	void *param - comparison prameter.
return value - sl_iterator_t - return NULL if didn't find the value.
time complexity - O(n)
space complexity - O(1)
*/
sl_iterator_t SLFind(const sl_iterator_t from, const sl_iterator_t to, void *param, sl_t *sl);


/* SLFind:
Description - find a value in the list.
Params:
	const sl_iterator_t from - iterator to iterate from.
	const sl_iterator_t to - iterator to iterate to (exclusive).
	is_match_t is_match - 
	void *param - comparison prameter.
return value - sl_iterator_t - return NULL if didn't find the value.
time complexity - O(n)
space complexity - O(1)
*/
sl_iterator_t SLFindCustom(const sl_iterator_t from, const sl_iterator_t to, is_match_t is_match, void *param);

/* SLForEach:
Description - perform a function between two variables.
Params:
	const dll_iterator_t from - iterator to iterate from.
	const dll_iterator_t to - iterator to iterate to (exclusive).
	is_match_t is_match
	void *param - comparison prameter.
return value - size_t.
time complexity - O(n)
space complexity - O(1)
*/
size_t SLForEach(const dll_iterator_t from, const dll_iterator_t to, action_t action_func, const void *param);


/* SLMerge:
Description - Merge two list with same sort criteria only.
Params:
	sl_t *sl_source.
	sl_t *sl_dest.
return value - sl_iterator_t.
time complexity - O(n)
space complexity - O(1)
*/
sl_iterator_t SLMerge(sl_t *sl_source, sl_t *sl_dest);



#endif /*__DLL_H__*/













