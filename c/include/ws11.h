#ifndef __WS11_H__
#define __WS11_H__

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


#endif /* __WS11_H__ */
