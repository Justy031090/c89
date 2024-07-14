
#ifndef __SLL_H__
#define __SLL_H__

typedef struct node node_t;
typedef struct sll sll_t;
typedef node_t *sll_iterator_t;


/* type of action_t functions:
Description - action function implemented by the user return count of errors.
Params:
	void *data - data from the ssl.
	void *param - parameter from the user
return value - size_t - number of errors.
*/

typedef int (*action_t)(void *data, void *param);

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

/* SLLCreate:
Description - create sll managemet structure and the dummy node
Params: no params
return value - sll_t *
time complexity - O(1)
space complexity - O(1)
*/

sll_t *SLLCreate(void);


/* SLLDestroy:
Description - delete sll
Params:
	sll_t *sll - management struct.
return value - NONE
time complexity - O(1)
space complexity - O(1)
*/

void SLLDestroy(sll_t *sll);


/*Description - insert new value to sll.
Params:
	sll_iterator_t iter - iterator.
	const void *data - data pointer.
	sll_t *sll - management struct.
return value - sll_iterator_t
time complexity - O(1)
space complexity - O(1)
*/

sll_iterator_t SLLInsert(sll_iterator_t iter, const void *data, sll_t *sll);

/* SLLSetData:
Description - change a value in the list.
Params:
	sll_iterator_t iter - iterator.
	const void *data - data pointer.
return value - NONE.
time complexity - O(1)
space complexity - O(1)
*/

void SLLSetData(sll_iterator_t iter, const void *data);

/* SLLGetData:
Description - get a value from the list.
Params:
	sll_iterator_t iter - iterator.
return value - void *.
time complexity - O(1)
space complexity - O(1)
*/

void *SLLGetData(const sll_iterator_t iter);

/* SLLFind:
Description - find a value in the list.
Params:
	const sll_iterator_t from - iterator to iterate from.
	const sll_iterator_t to - iterator to iterate to (not included).
	is_match_t is_match - 
	void *param - comparison prameter.
return value - sll_iterator_t - return NULL if didn't find the value.
time complexity - O(n)
space complexity - O(1)
*/

sll_iterator_t SLLFind(const sll_iterator_t from, const sll_iterator_t to, is_match_t is_match, void *param);

/* SLLSize:
Description - return sll size.
Params:
	sll_t *sll - management struct.
return value - size_t.
time complexity - O(n)
space complexity - O(1)
*/

size_t SLLSize(const sll_t *sll);

/* SLLIsEmpty:
Description - is the sll empty.
Params:
	sll_t *sll - management struct.
return value - int - 1 if empty else 0.
time complexity - O(1)
space complexity - O(1)
*/

int SLLIsEmpty(const sll_t *sll);

/* SLLRemove:
Description - remove value from sll.
Params:
	sll_iterator_t iter - iterator.
	sll_t *sll - management struct.
return value - sll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/

sll_iterator_t SLLRemove(sll_iterator_t iter, sll_t *sll);

/* SLLBegin:
Description - move iterator to head.
Params:
	sll_t *sll - management struct.
return value - sll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/

sll_iterator_t SLLBegin(const sll_t *sll);


/* SLLEnd:
Description - move iterator to tail.
Params:
	sll_t *sll - management struct.
return value - sll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/

sll_iterator_t SLLEnd(const sll_t *sll);


/* SLLNext:
Description - move iterator to next node.
Params:
	sll_iterator_t iter - iterator pointer.
return value - sll_iterator_t.
time complexity - O(1)
space complexity - O(1)
*/

sll_iterator_t SLLNext(sll_iterator_t iter);


/* SLLIsEqual:
Description - compare two nodes.
Params:
	const sll_iterator_t iter1 - iterator.
	const sll_iterator_t iter2 - iterator.
return value - int - boolean.
time complexity - O(1)
space complexity - O(1)
*/

int SLLIsEqual(const sll_iterator_t iter1, const sll_iterator_t iter2);


/* SLLForEach:
Description - perform a function between two variables.
Params:
	const sll_iterator_t from - iterator to iterate from.
	const sll_iterator_t to - iterator to iterate to (not included).
	is_match_t is_match
	void *param - comparison prameter.
return value - size_t.
time complexity - O(n)
space complexity - O(1)
*/

int SLLForEach(const sll_iterator_t from, const sll_iterator_t to, action_t action_func, void *param);

/* SLL Exercise questions
space O(1), time O(n);
*/
sll_iterator_t FindIntersection(sll_iterator_t head_1, sll_iterator_t head_2);
int HasLoop(const sll_iterator_t head);
sll_iterator_t Flip(sll_t *sll);


#endif /*__SLL_H__*/
