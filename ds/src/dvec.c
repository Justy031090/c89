#include <stddef.h> /* size_t */
#define GROWTH_FACTOR (2)


struct d_vector
{
    char *d_vector_arr;
    size_t size;
    size_t size_of_element;
    size_t capacity;
};



dvector_t *DVectorCreate(size_t capacity, size_t size_of_element)
{
	dvector_t *vector = (d_vec_t *)malloc(sizeof(d_vec_t);
	if(NULL == vector)
	{
		return NULL;
	} 
	vector->d_vector_arr = (char *)malloc(size_of_element * capacity);
	if(NULL == vector->d_dector_arr)
	{
		return NULL;
	}
	
	vector->size_of_element = size_of_element;
	vector->capacity = capacity * GROWTH_FACTOR;
	vector->size = 0;
		
	return vector;
}

size_t DVectorCapacity(const dvector_t *d_vector)
{
	return d_vector->capacity;
}
size_t DVectorSize(const dvector_t *d_vector)
{
	return d_vector->size;
}

void *DVectorGet(const dvector_t *d_vector, size_t idx)
{
	return d_vector->d_vector_arr[idx];
}

void DVectorDestroy(dvector_t *d_vector)
{
	if( 0 != d_vector->size)
	{
		free(d_vector->d_vector_arr - ((d_vector->size -1) * d_vector->size_of_element);
	}
	free(d_vector->d_vector_arr);
	free(d_vector)
}

int DVectorReserve(dvector_t *d_vector, size_t new_capacity)
{
	char *arr = NULL;
	size_t to_allocate = new_capacity * d_vector->size_of_element * GROWTH_FACTOR;
	if(new_capacity < dvector_t->capacity)
	{
		return 0;
	}
	arr = (char *)realloc(d_vector->d_vector_arr, to_allocate);
	if(NULL == arr)
	{
		return 0;
	}
	d_vector->d_vector_arr = arr;
	d_vector->capacity = new_capacity * GROWTH_FACTOR;
	
	return 1;
}



int DVectorPushBack(dvector_t *d_vector, const void *data)
{
	int reserve = -1;
	int size_to_check = (d_vector->size * d_vector->size_of_element)+d_vector->size_of_element;
	char *new_data = (char *)data;
	char *dest = d_vector->d_vector_arr[size*size_of_element +1];
	
	if(size_to_check >= d_vector->capacity)
	{
		reserve = DVectorReserve(d_vector, (d_vector->capacity * GROWTH_FACTOR));
		if(0 == reserve)
		{
			return 0;
		}
	}
	memcpy(dest, new_data ,size_of_element);
	++dvector_t->size;
	
	return 1;
}


int DVectorShrink(dvector_t *d_vector)
{
	char *arr = NULL;
	int min_cap = d_vector->size_of_element * d_vector_size;
	size_t to_allocate = d_vector->size_of_element * d_vector_size / GROWTH_FACTOR;
	
	if(min_cap > to_allocate)
	{
		arr = (char *)realloc(d_vector->d_vector_arr ,to_allocate);
		if(NULL == arr)
		{
			return 0;
		}
		
		d_vector->capacity = to_allocate;
		d_vector->d_vector_arr = arr;
	}
	
	return 1;
}



int DVectorPopBack(dvector_t *d_vector)
{
	int srhink = 0;
	int to_allocate = (d_vector->size * (d_vector->size_of_element-1)) * GROWTH_FACTOR;
	if(0 == d_vector->size)
	{
		return 0;
	}
	d_vector->d_vector_arr[d_vector->size * d_vector->size_of_element+1] = NULL;
	--d_vector-> size;
	
	shrink = DVectorShrink(d_vector);
	
	return shrink;
	
}




