void PrintFloatBits(float f)
{
	int *np = (int *)&f;
	int i = 31;
	for ( ; i > 0 ; i--)
	{
		printf("%d", (*np >> i) & 1);
	}
	printf("\n");
}



int main()
{
	float x = 5.75;
	float *a = &x;
	
	PrintFloatBits(x);
}
