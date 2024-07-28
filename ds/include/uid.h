#ifndef __UID_H__
#define __UID_H__

#include <stddef.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct uid
{
	size_t counter;
	time_t timestamp;
	pid_t pid;
	long ip;
} my_uid_t;

extern my_uid_t bad_uid; 

my_uid_t UIDGenerate();

int UIDIsEqual(my_uid_t a, my_uid_t b);

#endif
