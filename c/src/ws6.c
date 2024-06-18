

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

