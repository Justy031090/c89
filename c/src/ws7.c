/******************************************************************************
***********************REVIEWD BY *********************************************
******************************************************************************/

#include <stdio.h>             /*Inputs, outpus, printing, writing*/
#include <stdlib.h>            /*malloc*/
#include <string.h>            /*strlen, strncmp*/
#include "ws7.h"

#define CMD_ARR_SIZE (4)       /*Command array size for logger*/
#define MAX_LINE_SIZE (200)    /*largest string input from stdin for logger*/
#define PRINT_ARR_SIZE (10)    /*The size of the array for Printing Function*/


typedef int (*cmd_func)(FILE *file, char* filename);
typedef int (*compare_ptr)(const char*, const char*, size_t);
typedef void (*print_func_type)(int);

/******************************************************************************
***********************PRINT FUNCTION BLOCK************************************
******************************************************************************/

/*Structure for the Print function*/
struct print_me
{
	int x;
	print_func_type print_ptr;
};

/*Print function to be passed to the structure array*/
void Print(int x)
{
	printf("Printing integer %d\n", x);
}

/*Creating a pointer to the Print() function*/
print_func_type print_ptr = Print;

/*Creating an Array of print_me structures*/
struct print_me struct_prints[PRINT_ARR_SIZE];

/*Helper to fill the Array with data*/
void FillPrintArr()
{
	int i=0;
	while(i<PRINT_ARR_SIZE)
	{
		struct_prints[i].x = i;
		struct_prints[i].print_ptr = print_ptr;
		++i;
	}
}

/*Function Passed To Main*/
void PrintInMain()
{
	int i=0;
	FillPrintArr();
	while(i<PRINT_ARR_SIZE)
	{
		struct_prints[i].print_ptr(struct_prints[i].x);
		++i;
	}
	
}

/******************************************************************************
***********************LOGGER FUNCTION BLOCK***********************************
******************************************************************************/

/*Structure for special inputs*/
struct special_inputs
{
	char *str;
	compare_ptr comparison;
	cmd_func operation;
};

/*=============================================
         HELPER FUNCTIONS SECTION
=============================================*/



int OnExit(FILE *file, char* filename)
{
	(void)filename;
	return fclose(file);
}

int OnRemove(FILE *file, char* filename)
{
	fclose(file); 
	remove(filename);
	return 0;
}

int OnCount(FILE *file, char* filename)
{
	char c = 0;
	int count = 0;
	(void)filename;

	fseek(file, 0, SEEK_SET);
	
	while (c != EOF )
	{
		c = fgetc(file);
		if(c == '\n')
		{
			count++;
		}
	}
	
	if( 0 < count)
	{
		printf("Number of lines in the file is %d\n", count);
	}
	else
	{
		printf("The file is empty !\n");

	}
	return 1;
}

int OnLeftArrow(FILE *file, char* filename)
{
	char *buffer = NULL;
	int arrow_placement = 0;
	int length = 0;
		
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	buffer = (char *)malloc(length);
	if(NULL == buffer)
	{
		return 0;
	}
	
	
	while(arrow_placement != '<')
	{
		arrow_placement = fgetc(file);
	}
	
	arrow_placement = ftell(file);
	fseek(file, 0, SEEK_SET);
	fread(buffer, sizeof(char), length, file);
	
	freopen(filename, "w+", file);
	
	fwrite(buffer+arrow_placement, sizeof(char), length-arrow_placement ,file);
	
	fclose(file);
	file = fopen(filename, "a");
	
	fwrite(buffer, sizeof(char), arrow_placement-1 ,file);
	
	fclose(file);
	
	free(buffer);
	buffer = NULL;

	return 1;
}


int CheckCommand(char *command, struct special_inputs array[CMD_ARR_SIZE], FILE *file, char* filename)
{
	int result = 2;
	size_t i = 0;

	while(i < CMD_ARR_SIZE)
	{
		result = array[i].comparison(command, array[i].str, strlen(array[i].str));
		if(0 == result)
		{
			return array[i].operation(file, filename);
		}
		++i;
	}
	return 1;
}


struct special_inputs array_of_inputs[CMD_ARR_SIZE];

compare_ptr compare = strncmp;
cmd_func ext = OnExit;
cmd_func rm = OnRemove;
cmd_func count = OnCount;
cmd_func write_on_top = OnLeftArrow;

void FillSpecialInputs(struct special_inputs array[CMD_ARR_SIZE])
{
	array[0].str = "-count\n";
	array[0].comparison = compare;
	array[0].operation = count;
	
	array[1].str = "-remove\n";
	array[1].comparison = compare;
	array[1].operation = rm;
	
	array[2].str = "-exit\n";
	array[2].comparison = compare;
	array[2].operation = ext;
	
	array[3].str = "<";
	array[3].comparison = compare;
	array[3].operation = write_on_top;
}

int Logger(char *filename)
{	
	int cmd_flag = 1;
	char line_buffer[MAX_LINE_SIZE];
	char *input_string = NULL;
	FILE *fp = fopen(filename, "a+");
	
	FillSpecialInputs(array_of_inputs);
	
	if(NULL == fp)
		return -1;
		
	printf("Let's write a book!\n");
	
	while(1 == cmd_flag)
	{
		input_string = fgets(line_buffer, MAX_LINE_SIZE, stdin);
		if(0 == compare(input_string, "-", 1))
		{
			cmd_flag = CheckCommand(input_string, array_of_inputs, fp, filename);
		}
		else if(0 == compare(input_string, "<", 1))
		{
			fwrite(line_buffer, sizeof(char), strlen(input_string), fp);
			cmd_flag = CheckCommand(input_string, array_of_inputs, fp, filename);
		}
		else
		{
			fwrite(line_buffer, sizeof(char), strlen(input_string), fp);
		}
	}	
	return 1;
}





