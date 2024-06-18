#include <stdio.h>
#include <stdlib.h>


	int add (int y, int z)
	{
		return y+z;
	}

	typedef int my_int;
	typedef char my_char;
	typedef float my_float;
	typedef char arr_ptr[][10];
	typedef int (*func_ptr)(int, int);
	
void TypeDefTests (void)
{
	my_int i = 3;
	my_char c = 97;
	my_float floaty = 2.5;
	arr_ptr test_arr = {{1,2}, {3,4}, {5.6}};
	func_ptr AddFunc = add;
	
	printf("%d\n%c\n%f\n%i\n%d\n", i, c, floaty, test_arr[0][1], AddFunc(2,2));
}
	
	
void PressKeyIf(void)
{
	int t = 1;
	while(t)
	{
		char c = getc(stdin);
		if(c == 116 || c == 84)
		{
			printf("\"T\" was pressed\n");
		}
		if(c == 97 || c == 65)
		{
			printf("\"A\" was pressed\n");
		}
		if(c == 27)
		{
			t = 0;
		}
	}
}

void PressKeySwitch(void)
{
	int t = 1;
	
	while(0 != t)
	{
		switch(getc(stdin))
		{
		case 116:
		case 84:
			printf("\"T\" was pressed\n");
			break;
		case 97:
		case 65:
			printf("\"A\" was pressed\n");
			break;
		case 27:
			t = 0;
			break;
		}
	}
}

void PressKeyLut(void)
{
}



int main()
{
	system("stty -icanon -echo");
	SwitchPresKey();
	system("stty icanon echo");
	return 0;
}






