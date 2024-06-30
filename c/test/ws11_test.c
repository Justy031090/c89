
#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
#include "ws11.h"


int main ()
{	
	char *filename = "test.bin";
	Student Michael = {
		"Michael",
		"Bar",
		{
			99.5,
			{55.2, 59.3, 58.4},
			{55.2, 59.3, 58.4}
		}
	};
	size_t msize = sizeof(Michael);
	Student *NotMichael =  malloc(msize);
	if(NULL == NotMichael)
	{
		return MEM_FAIL;
	}

	WriteToBin(filename, Michael);
	ReadFromBin(filename, NotMichael);
	
	printf("%s\n", NotMichael->firstname);
	printf("%s\n", NotMichael->lastname);
	printf("%.3f\n", NotMichael->grades.sports);
	printf("%.3f\n", NotMichael->grades.r_g.chemistry);
	printf("%.3f\n", NotMichael->grades.r_g.physics);
	printf("%.3f\n", NotMichael->grades.r_g.math);
	printf("%.3f\n", NotMichael->grades.h_g.sociology);
	printf("%.3f\n", NotMichael->grades.h_g.psychology);
	printf("%.3f\n", NotMichael->grades.h_g.literature);
	
	free(NotMichael);
	NotMichael = NULL;
	
	return 0;
}

