#include <stdlib.h>
#include <stdio.h>
#include "ws6.h"


int main()
{
	system("stty -icanon -echo");
	PressKeyIf();
	PressKeySwitch();
	PressKeyLut();
	system("stty icanon echo");
		
	return 0;
}
