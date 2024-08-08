

int x =2;
char *string_literal = "hello world";
int *ptr_to_malloc = malloc(sizeof(int));
*ptr_to_malloc = x;
int global_var = 9;
static int global_static = 92;

static int Add(int num1, int num2)
{
	return num1+num2;
};

extern int Mult(int num1 int num2)
{
	return num1*num2;
};

void variable_check()
{
	int local = 2;
	static int stat_local = 4;
	
}

