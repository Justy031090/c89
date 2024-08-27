/*					Dynamic Vector Implementation.
(\.../)		.. Authored by Michael Bar 08/07/2024
(=';'=) .. code reviewd by Oran.S 09/07/2024..
(")-("))	..
*/

#include <stddef.h> /* size_t */
#include <stdlib.h> /*malloc, realloc*/
#include <string.h> /*memcpy*/
#include <assert.h> /*assert*/

#include "dvec.h"

#define GROWTH_FACTOR (1.5)
#define MIN_VECTOR_SIZE (2)

enum status
{
	MEM_FAIL = -1, FAIL, SUCCESS
};
static int Resize(dvector_t *d_vector, size_t new_capacity);



struct dvector
{
    char *d_vector_arr;
    size_t size;
    size_t size_of_element;
    size_t capacity;
};

dvector_t *DVectorCreate(size_t capacity, size_t size_of_element)
{
	
	dvector_t *vector = (dvector_t *)malloc(sizeof(dvector_t));
	assert(size_of_element > 0);
	if(NULL == vector)
	{
		return NULL;
	}
	vector->d_vector_arr = (char *)malloc(size_of_element * capacity);
	if(NULL == vector->d_vector_arr)
	{
		free(vector);
		vector = NULL;
		return NULL;
	}
	
	vector->capacity = capacity;
	vector->size_of_element = size_of_element;
	vector->size = 0;
	
	if(vector->capacity < MIN_VECTOR_SIZE)
	{
		vector->capacity = MIN_VECTOR_SIZE;
	}
	
	return vector;
}

size_t DVectorCapacity(const dvector_t *d_vector)
{
	assert(NULL !=d_vector);
	return d_vector->capacity;
}
size_t DVectorSize(const dvector_t *d_vector)
{
	assert(NULL !=d_vector);
	return d_vector->size;
}

void *DVectorGet(const dvector_t *d_vector, size_t idx)
{
	if(idx < d_vector->size) return NULL;
	return d_vector ? (void *)d_vector->d_vector_arr[idx] : NULL;
}

void DVectorDestroy(dvector_t *d_vector)
{
	assert(NULL !=d_vector);
	free(d_vector->d_vector_arr);
	free(d_vector);
}

int DVectorPushBack(dvector_t *d_vector, const void *data)
{
	int reserve = -1;
	size_t size_to_check = d_vector->size +1;
	char *new_data = (char *)data;
	char *dest = NULL;
	
	assert(NULL !=d_vector);
	
	if((size_to_check) == (d_vector->capacity))
	{
		reserve = DVectorReserve(d_vector, d_vector->capacity*GROWTH_FACTOR);
		if(0 == reserve)
		{
			return MEM_FAIL;
		}
	}
	dest = (char *)d_vector->d_vector_arr + (d_vector->size);
	memcpy(dest, new_data ,d_vector->size_of_element);
	d_vector->size = d_vector->size +1;

	
	return SUCCESS;
}

int DVectorPopBack(dvector_t *d_vector)
{
	char *to_pop = d_vector->d_vector_arr + d_vector->size;
	int shrink = 0;
	assert(NULL !=d_vector);
	
	if(0 == d_vector->size)
	{
		return FAIL;
	}
	to_pop = NULL;
	d_vector-> size = d_vector->size -1;
	shrink = DVectorShrink(d_vector);
	
	return shrink == 0 ? MEM_FAIL : SUCCESS;
	
}

int DVectorShrink(dvector_t *d_vector)
{
	size_t to_allocate = 0;
	assert(NULL !=d_vector);
	if((d_vector->size * MIN_VECTOR_SIZE) < (d_vector->capacity))
	{
		to_allocate = d_vector->size_of_element * d_vector->size * GROWTH_FACTOR;
		return Resize(d_vector, to_allocate);
	}
	return SUCCESS;
}

int DVectorReserve(dvector_t *d_vector, size_t new_capacity)
{
	size_t to_allocate = new_capacity * d_vector->size_of_element;
	assert(NULL !=d_vector);
	if(new_capacity < d_vector->capacity)
	{
		return FAIL;
	}

	return Resize(d_vector, to_allocate);
}

static int Resize(dvector_t *d_vector, size_t new_capacity)
{
	char *arr = NULL;
	arr = (char *)realloc(d_vector->d_vector_arr, new_capacity);
	if(NULL == arr)
	{
		return MEM_FAIL;
	}
	d_vector->d_vector_arr = arr;
	d_vector->capacity = new_capacity;
	return SUCCESS;
}






