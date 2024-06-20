#include <stdio.h>
#include "ws6.h"

/*better too define a macro for the ascii umbers of Tt,Aa, ESC*/

void PressedOnT()
{
	printf("\"T\" was pressed\n");
}

void PressedOnA()
{
	printf("\"A\" was pressed\n");
}

void PressedOnEsc()
{
	system("stty icanon echo");
}


void Dummy()
{
 ;
}


void FillTable ()
{	

	int i = 0;
	func_ptr POA = PressedOnA;
	func_ptr POT = PressedOnT;
	func_ptr DUM = Dummy;
	func_ptr POESC = PressedOnEsc;
	while(i<256)
	{
		table[i] = DUM;
		i++;
	}
	
	table[116] = POT;
	table[84] = POT;
	table[65] = POA;
	table[97] = POA;
	table[27] = POESC;
}




void PressKeyLut()
{	
	char c = 0;
	FillTable();
	while(c != 27)
	{
		c = getc(stdin);
		system("stty -icanon -echo");
		table[c]();
	}
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



