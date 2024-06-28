#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

typedef struct student {
	char *firstname;
	char *lastname;
	float sports;
	humanistic_grades_t *h_g;
	real_grades_t *r_g;
} Student;
struct h_g michael_hg = {99.8, 60.00, 13.5};
struct r_g michael_rg = {99.9, 98.00, 97.7};
Student Michael = {"Michael", "Bar", 99.20, &michael_hg, &michael_rg};

int WriteToBin()
{	
	int length = 0;
	Student *notmichael;
	FILE *file;
	file = fopen("test.bin", "a+b");
	fwrite(&Michael,1000,1,file);
	fclose(file);
	
	file = fopen("test.bin", "rb");
	notmichael =malloc(sizeof(Student));
	
	fread(&notmichael->firstname, sizeof(notmichael->firstname), 1, file);
	fread(&notmichael->lastname, sizeof(notmichael->lastname), 1, file);
	fread(&notmichael->sports, sizeof(notmichael->sports), 1, file);
	fread(&notmichael->h_g->sociology, sizeof(notmichael->h_g->sociology), 1, file);
	fread(&notmichael->h_g->sociology, sizeof(notmichael->h_g->sociology), 1, file);
	fread(&notmichael->h_g->sociology, sizeof(notmichael->h_g->sociology), 1, file);
	fread(&notmichael->r_g->math, sizeof(notmichael->r_g->math), 1, file);
	fread(&notmichael->r_g->math, sizeof(notmichael->r_g->math), 1, file);
	fread(&notmichael->r_g->math, sizeof(notmichael->r_g->math), 1, file);
	
	printf("%s\n", notmichael.firstname);
	
	fclose(file);
	
	return 0;
	
}

int main()
{
	WriteToBin();
	/*printf("%s\n%s\n%.3f\n%.3f\n", Michael.firstname, Michael.lastname, Michael.sports, Michael.h_g->sociology);*/
	
	return 0;
}

