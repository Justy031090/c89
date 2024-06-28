#include <stdio.h>
#include <string.h>

#include "ws11.h"

typedef struct h_g {
	float sociology;
	float psychology;
	float literature;
} humanistic_grades_t;
				
typedef struct r_g {
	float math;
	float physics;
	float chemistry;
} real_grades_t;

struct student {
	char *firstname;
	char *lastname;
	float sports;
	humanistic_grades_t *h_g;
	real_grades_t *r_g;
};

struct h_g michael_hg = {99.8, 60.00, 13.5};
struct r_g michael_rg = {99.9, 98.00, 97.7};
struct student Michael = {"Michael", "Bar", 99.20, &michael_hg, &michael_rg};

int main ()
{
	char *buffer = "Hello World OK !";
	char *buffer2[17];
	FILE *file;
	file = fopen("test.bin","wb+");
	fwrite(&buffer,strlen(buffer),1,file);
	fclose(file);
	
	file = fopen("test.bin", "rb");
	fread(buffer2,1,sizeof(buffer2), file);
	fclose(file);
	
	printf("%s\n", *buffer2);
	
	
	
	return 0;
}


