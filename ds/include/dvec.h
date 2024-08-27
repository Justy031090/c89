#ifndef __DVECTOR_H__
#define __DVECTOR_H__
#include <stddef.h> /* size_t */

typedef struct dvector dvector_t;

dvector_t *DVectorCreate(size_t capacity, size_t size_of_element);

void DVectorDestroy(dvector_t *d_vector);

void *DVectorGet(const dvector_t *d_vector, size_t idx);

int DVectorPushBack(dvector_t *d_vector, const void *data);

int DVectorPopBack(dvector_t *d_vector);

size_t DVectorSize(const dvector_t *d_vector);

size_t DVectorCapacity(const dvector_t *d_vector);

int DVectorShrink(dvector_t *d_vector);

int DVectorReserve(dvector_t *d_vector, size_t new_capacity);

#endif /*__DVECTOR_H__*/



