
#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
#include "ws11.h"


int main ()
{	
	char *filename = "test.bin";
	Student Michael = {
		"Michael",
		"Bar",
		99.20,
		{55.2, 59.3, 58.4},
		{55.2, 59.3, 58.4}
	};
	
	Student *NotMichael = malloc(sizeof(Student));
	if(NULL == NotMichael)
	{
		return -1;
	}

	WriteToBin(filename, Michael);
	ReadFromBin(filename, NotMichael);
	
	printf("%s\n", NotMichael->firstname);
	printf("%s\n", NotMichael->lastname);
	printf("%.3f\n", NotMichael->sports);
	printf("%.3f\n", NotMichael->r_g.chemistry);
	printf("%.3f\n", NotMichael->r_g.physics);
	printf("%.3f\n", NotMichael->r_g.math);
	printf("%.3f\n", NotMichael->h_g.sociology);
	printf("%.3f\n", NotMichael->h_g.psychology);
	printf("%.3f\n", NotMichael->h_g.literature);
	
	free(NotMichael);
	NotMichael = NULL;
	
	return 0;
}

