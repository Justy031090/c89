/*			.. UID Generator Implementation ..
(\.../)		.. Authored by Michael Bar 28/07/2024 .. 
(=';'=) 	.. code reviewd by Yuval 28/07/2024..
(")-("))	.. The only hard day was yesterday ! ..
*/

#define _DEFAULT_SOURCE
#include "uid.h"
#include "stdatomic.h"

const my_uid_t bad_uid = {0,0,0,0};

my_uid_t UIDGenerate()
{
	my_uid_t new_uid;
	static atomic_size_t counter = 1; /*changed from static to be thread-safe*/
	time_t sec = time(&sec);
	if(0 > sec)
		return bad_uid;
	
	new_uid.counter = atomic_fetch_add(&counter, 1);;
	new_uid.timestamp = sec;
	new_uid.pid = getpid();
	new_uid.ip = gethostid();
	
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

