#ifndef __WS11_H__
#define __WS11_H__

#define MAX_NAME_LEN (10)

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
	char firstname[MAX_NAME_LEN];
	char lastname[MAX_NAME_LEN];
	float sports;
	humanistic_grades_t h_g;
	real_grades_t r_g;
} Student;


int WriteToBin(char *filename, Student);
int ReadFromBin(char *filename, Student*);


#endif /* __WS11_H__ */
