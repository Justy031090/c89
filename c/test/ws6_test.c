#include <stdlib.h>
#include <stdio.h>
#include "ws6.h"

/******************************************************************************
***********************REVIEWD BY HIS MAJESTY, OR KOL**************************
******************************************************************************/

int main()
{
	
	system("stty -icanon -echo");
	/*PressKeyIf();
	PressKeySwitch();
		*/
	PressKeyLut();
	system("stty icanon echo");
		
	return 0;
}
