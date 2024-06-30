#ifndef __WS11_H__
#define __WS11_H__

#define MAX_NAME_LEN (10)
#define TOTAL_STRUCT_SIZE (27)

typedef enum e_c {
	MEM_FAIL = -4,
	FAILED_TO_OPEN,
	FAILED_TO_WRITE,
	FAILED_TO_READ,
	SUCCESS
} exit_code;


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

typedef struct grades {
	float sports;
	humanistic_grades_t h_g;
	real_grades_t r_g;
} grades;


typedef struct student {
	char firstname[MAX_NAME_LEN];
	char lastname[MAX_NAME_LEN];
	grades grades;
	
} Student;


exit_code WriteToBin(char *filename, Student);
exit_code ReadFromBin(char *filename, Student*);


#endif /* __WS11_H__ */
