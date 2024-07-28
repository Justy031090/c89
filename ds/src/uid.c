#include "uid.h"
#include <unistd.h>


const my_uid_t bad_uid = {0,0,0,0};

my_uid_t UIDGenerate()
{
	static size_t counter = 1;
	time_t sec = 0;
	my_uid_t new_uid;
	new_uid.counter = counter;
	new_uid.timestamp = time(&sec);
	new_uid.pid = getpid();
	new_uid.ip = gethostid();
	
	++counter;
	
	return new_uid;
}

int UIDIsEqual(my_uid_t a, my_uid_t b)
{
	int count = a.counter == b.counter;
	int timestamp = a.timestamp == b.timestamp;
	int pidt = a.pid == b.pid;
	int ip = a.ip == b.ip;
	
	if(!count || !timestamp || !pidt || !ip)
		return 0;
	
	return 1;
}

