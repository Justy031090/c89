#include <stdio.h>
#include "ws6.h"


static func_ptr table[127];
table[84] = PKL;
table[116] = PKL;
table[97] = PKL;
table[65] = PKL;
table[26] = PKL;

func_ptr PKL = PressKeyLut;
void PressKeyLut(func_ptr arr[])
{
	char c = getc(stdin);
	arr[c];
}

void PressKeyIf()
{
	char c = 0;
	while(27 != c)
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
	}
}

void PressKeySwitch()
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



