#ifndef __DLL_H__
#define __DLL_H__

#include <stddef.h> /* size_t */

/* type of the dll management struct */
typedef struct dll dll_t;
/* type of node struct */
typedef struct node node_t;
/* type of iterator */
typedef node_t *dll_iterator_t;


/* type of action_t functions:
Description - action function implemented by the user return count of errors.
Params:
	void *data - data from the ssl.
	void *param - parameter from the user
return value - size_t - number of errors.
*/
typedef size_t (*action_t)(void *data, void *param);


/* type of is_match_t function:
Description - comparison func the user should implemnt if equal return 1, else return 0.
Params:
	void *data - data from the ssl.
	void *param - parameter from the user
return value - int - boolean vlue.
time complexity - O(n)
space complexity - O(1)
*/
typedef int (*is_match_t)(void *data, void *param);



/* DLLCreate:
Description - create dll managemet structure and the dummy node
Params: no params
return value - dll_t *
time complexity - O(1)
space complexity - O(1)
*/
dll_t *DLLCreate(void);


/* DLLDestroy:
Description - delete dll
Params:
	dll_t *dll - management struct.
return value - NONE
time complexity - O(1)
space complexity - O(1)
*/
void DLLDestroy(dll_t *dll);


/* DLLInsert:
Description - insert new valu to dll.
Params:
	dll_iterator_t iter - iterator.
	const void *data - data pointer.
	dll_t *dll - management struct.
return value - dll_iterator_t
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLInsert(dll_iterator_t iter, const void *data, dll_t *dll);


/* DLLSetData:
Description - change a value in the list.
Params:
	dll_iterator_t iter - iterator.
	const void *data - data pointer.
return value - NONE.
time complexity - O(1)
space complexity - O(1)
*/
void DLLSetData(dll_iterator_t iter, const void *data);


/* DLLGetData:
Description - get a value from the list.
Params:
	dll_iterator_t iter - iterator.
return value - void *.
time complexity - O(1)
space complexity - O(1)
*/
void *DLLGetData(const dll_iterator_t iter);


/* DLLFind:
Description - find a value in the list.
Params:
	const dll_iterator_t from - iterator to iterate from.
	const dll_iterator_t to - iterator to iterate to (exclusive).
	is_match_t is_match - 
	void *param - comparison prameter.
return value - dll_iterator_t - return NULL if didn't find the value.
time complexity - O(n)
space complexity - O(1)
*/
dll_iterator_t DLLFind(const dll_iterator_t from, const dll_iterator_t to, is_match_t is_match, void *param);


/* DLLSize:
Description - return dll size.
Params:
	dll_t *dll - management struct.
return value - size_t.
time complexity - O(n)
space complexity - O(1)
*/
size_t DLLSize(const dll_t *dll);


/* DLLIsEmpty:
Description - is the dll empty.
Params:
	dll_t *dll - management struct.
return value - int - 1 if empty else 0.
time complexity - O(1)
space complexity - O(1)
*/
int DLLIsEmpty(const dll_t *dll);


/* DLLRemove:
Description - remove value from dll.
Params:
	dll_iterator_t iter - iterator.
	dll_t *dll - management struct.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLRemove(dll_iterator_t iter, dll_t *dll);


/* DLLBegin:
Description - move iterator to head.
Params:
	dll_t *dll - management struct.
return value - dll_iterator_t - first valid node (not dummy)
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLBegin(const dll_t *dll);


/* DLLEnd:
Description - move iterator to tail.
Params:
	dll_t *dll - management struct.
return value - dll_iterator_t - tail dummy node.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLEnd(const dll_t *dll);


/* DLLNext:
Description - move iterator to next node.
Params:
	dll_iterator_t iter - iterator pointer.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLNext(dll_iterator_t iter);


/* DLLPrev:
Description - move iterator to previous node.
Params:
	dll_iterator_t iter - iterator pointer.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLPrev(dll_iterator_t iter);

/* DLLIsEqual:
Description - compare two nodes.
Params:
	const dll_iterator_t iter1 - iterator.
	const dll_iterator_t iter2 - iterator.
return value - int - boolean.
time complexity - O(1)
space complexity - O(1)
*/
int DLLIsEqual(const dll_iterator_t iter1, const dll_iterator_t iter2);


/* DLLForEach:
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
size_t DLLForEach(const dll_iterator_t from, const dll_iterator_t to, action_t action_func, const void *param);


/* DLLPushFront:
Description - push value to head of list.
Params:
	dll_t *dll - management struct.
	void *data - data to add.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLPushFront(dll_t *dll, void *data);


/* DLLPushBack:
Description - push value as last item in the list.
Params:
	dll_t *dll - management struct.
	void *data - data to add.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLPushBack(dll_t *dll, void *data);


/* DLLPopFront:
Description - pop value from the head of the list.
Params:
	dll_t *dll - management struct.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLPopFront(dll_t *dll);


/* DLLPopBack:
Description - pop value from the last item in the list.
Params:
	dll_t *dll - management struct.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLPopBack(dll_t *dll);


/* DLLMultiFilnd:
Description - pop value from the last item in the list.
Params:
	const dll_iterator_t from - iterator to iterate from.
	const dll_iterator_t to - iterator to iterate to (exclusive).
	is_match_t is_match.
	void *param - comparison prameter.
	dll_t *dest - list to add all values found.
return value - dll_iterator_t - last value found.
time complexity - O(n)
space complexity - O(n)
*/
dll_iterator_t DLLMultiFilnd(const dll_iterator_t from, const dll_iterator_t to, is_match_t is_match, void *param, dll_t *dest);


/* DLLSpliced:
Description - move a section of a list to another list.
Params:
	dll_iterator_t from - iterator to iterate from.
	const dll_iterator_t to - iterator to iterate to (exclusive).
	dll_iterator_t dest - where to add the new section.
return value - dll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/
dll_iterator_t DLLSpliced(dll_iterator_t from, const dll_iterator_t to, dll_iterator_t dest);

#endif /*__DLL_H__*/

