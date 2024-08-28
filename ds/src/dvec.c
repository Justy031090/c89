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
	if(NULL == vector) return NULL;

	vector->d_vector_arr = (char *)malloc(size_of_element * capacity);
	if(NULL == vector->d_vector_arr)
	{
		free(vector);
		vector = NULL;
		return NULL;
	}
	
	vector->capacity = (capacity < MIN_VECTOR_SIZE) ? MIN_VECTOR_SIZE : capacity;
	vector->size_of_element = size_of_element;
	vector->size = 0;
	
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
	if(idx >= d_vector->size) return NULL;
	return (void *)(d_vector->d_vector_arr + (idx * d_vector->size_of_element));
}

void DVectorDestroy(dvector_t *d_vector)
{
	assert(NULL !=d_vector);
	free(d_vector->d_vector_arr);
	free(d_vector);
}

int DVectorPushBack(dvector_t *d_vector, const void *data)
{	
	assert(NULL !=d_vector);
	
	if((d_vector->size) == d_vector->capacity)
	{
        if (DVectorReserve(d_vector, d_vector->capacity * GROWTH_FACTOR) == MEM_FAIL)
            return FAIL;
	}
	
	memcpy(d_vector->d_vector_arr + (d_vector->size * d_vector->size_of_element), data, d_vector->size_of_element);
    d_vector->size++;

	return SUCCESS;
}

int DVectorPopBack(dvector_t *d_vector)
{
    assert(NULL != d_vector);

    if (d_vector->size == 0) return FAIL;

    d_vector->size--;

    if (d_vector->size < d_vector->capacity / GROWTH_FACTOR && d_vector->capacity > MIN_VECTOR_SIZE)
    {
        if (DVectorShrink(d_vector) == MEM_FAIL) return MEM_FAIL;
    }

    return SUCCESS;
}

int DVectorShrink(dvector_t *d_vector)
{
    size_t new_capacity = d_vector->capacity;
    if (d_vector->size < new_capacity / GROWTH_FACTOR)
    {
        new_capacity = d_vector->size * GROWTH_FACTOR;
        if (new_capacity < MIN_VECTOR_SIZE)
        {
            new_capacity = MIN_VECTOR_SIZE;
        }

        return Resize(d_vector, new_capacity);
    }
    return SUCCESS;
}

int DVectorReserve(dvector_t *d_vector, size_t new_capacity)
{
	size_t new_size = new_capacity * d_vector->size_of_element;

	assert(NULL !=d_vector);

	if(new_capacity <= d_vector->capacity) return FAIL;

	return Resize(d_vector, new_size);
}

static int Resize(dvector_t *d_vector, size_t new_capacity)
{
	size_t new_size = new_capacity * d_vector->size_of_element;
	char *arr = (char *)realloc(d_vector->d_vector_arr, new_size);
	if(NULL == arr) return MEM_FAIL;

	d_vector->d_vector_arr = arr;
	d_vector->capacity = new_capacity;

	return SUCCESS;
}






