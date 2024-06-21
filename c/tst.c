#include <stdio.h>

/*When y is a Negative value - the behavior will be undefined*/
long Pow2(unsigned int x, unsigned int y)
{	
	return 0 == y ? x : x*(2<<(y-1));
}

long IsPow2Loop(unsigned int n)
{
	while(n > 3 && n%2 == 0){
		n>>=1;
	}
	return 2 == n ? 1 : 0;
}

long IsPow2(unsigned int n)
{
	/*If bits are different, then the result is 1, else 0*/
	return (n+2 == (n^2)) && (n % 2 == 0) ? 1 : 0;
}

long AddOne(int n )
{
	if(n%2==0){
		return n = n | 1;}
		
		 n = n ^(~n &~1);
	return n;
	
}



int main()
{
	printf("Answer %ld\n", AddOne(3));
	printf("Answer %ld\n", AddOne(4));
	printf("Answer %ld\n", AddOne(64));
	printf("Answer %ld\n", AddOne(33));
	printf("Answer %ld\n", AddOne(512));
	printf("Answer %ld\n", AddOne(514));
	return 0;
}
