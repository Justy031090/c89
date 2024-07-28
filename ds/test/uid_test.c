#include <stdio.h>

#include "uid.h"

int main ()
{
	
	my_uid_t x = UIDGenerate();
	my_uid_t y = UIDGenerate();
	my_uid_t z;
	int b = UIDIsEqual(x,y);
	int d = UIDIsEqual(x,x);
	sleep(10);
	z = UIDGenerate();
	
	printf("%d\n",x.counter); 
	printf("%d\n",y.counter); 
	printf("%d\n",x.timestamp); 
	printf("%d\n",y.timestamp);
	printf("Time Stamp After Sleep %d\n",z.timestamp); 
	printf("%d\n",x.pid); 
	printf("%d\n",y.pid);
	printf("%d\n",x.ip); 
	printf("%d\n",y.ip);
	printf("EQUAL %d\n", d);
	printf("NOT EQUAL %d\n", b);
	
	
	
	return 0;
}
