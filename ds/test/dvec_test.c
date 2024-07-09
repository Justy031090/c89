#include <stdio.h>
#include "dvec.h"
int main()
{
	int num = 10;
	int num2 = 5;
	int *ptr = &num;
	int *ptr2 = &num2;
	
	
	dvector_t *myVector = DVectorCreate(10, 8);
	printf("Capacity %lu\n", DVectorCapacity(myVector));
	printf("Size %lu\n", DVectorSize(myVector));
	DVectorPushBack(myVector, ptr2);
	printf("Capacity %lu\n", DVectorCapacity(myVector));
	printf("Size %lu\n", DVectorSize(myVector));
	DVectorPushBack(myVector, ptr2);
	printf("Capacity %lu\n", DVectorCapacity(myVector));
	DVectorPushBack(myVector, ptr2);
	printf("Capacity %lu\n", DVectorCapacity(myVector));
	DVectorPushBack(myVector, ptr2);
	printf("Capacity %lu\n", DVectorCapacity(myVector));
	DVectorPushBack(myVector, ptr2);
	printf("Capacity %lu\n", DVectorCapacity(myVector));
	DVectorDestroy(myVector);
	return 0;
}

/*
void DVectorDestroy(dvector_t *d_vector);
int DVectorShrink(dvector_t *d_vector);
*/
