/******************************************************************************
***********************REVIEWD BY TAMIR****************************************
******************************************************************************/

#include <stdio.h>             /*Inputs, outpus, printing, writing*/
#include <stdlib.h>            /*malloc*/
#include <string.h>            /*strlen, strncmp*/
#include "ws7.h"

#define CMD_ARR_SIZE (4)       /*Command array size for logger*/
#define MAX_LINE_SIZE (200)    /*largest string input from stdin for logger*/
#define PRINT_ARR_SIZE (10)    /*The size of the array for Printing Function*/


typedef int (*cmd_func_t)(char* filename);
typedef int (*compare_ptr_t)(const char*, const char*, size_t);
typedef void (*print_func_t)(int);
typedef enum LoggerReturn { Error = -1, Exit , Continue, Success} logger_return_t;

/******************************************************************************
***********************PRINT FUNCTION SECTION**********************************
******************************************************************************/

/*Structure for the Print function*/
struct print_me
{
	int x;
	print_func_t print_ptr;
};

/*Print function to be passed to the structure array*/
void Print(int x)
{
	printf("Printing integer %d\n", x);
}

/*Creating a pointer to the Print() function*/
print_func_t print_ptr = Print;

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
***********************LOGGER FUNCTION SECTION*********************************
******************************************************************************/

/*Structure for special inputs*/
struct special_inputs
{
	char *str;
	compare_ptr_t comparison;
	cmd_func_t operation;
};

/*=============================================
         HELPER FUNCTIONS BLOCK
=============================================*/

int GetFileLength(FILE *file)
{	
	int length = 0;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}

/*Exits from the program*/
logger_return_t OnExit(char* filename)
{
	(void)filename;
	return Exit;
}

/*Deletes the file*/
logger_return_t OnRemove(char* filename)
{
	remove(filename);
	return Exit;
}

/*Returns the number of lines in the file, based on counting the newline character*/
logger_return_t OnCount(char* filename)
{
	char c = 0;
	int count = 0;
	
	FILE *file = fopen(filename, "r");
	if(NULL == file)
	{
		return Error;
	}
	
	/*Moving the pointer to the beginning of the file*/
	fseek(file, 0, SEEK_SET);
	while (c != EOF )
	{
		c = fgetc(file);
		if('\n'== c)
		{
			count++;
		}
	}
	fclose(file);
	
	if( 0  <  count)
	{
		printf("Number of lines in the file is %d\n", count);
	}
	else
	{
		printf("The file is empty !\n");

	}
	return Continue;
}

logger_return_t OnLeftArrow(char* filename)
{
	char *buffer = NULL;
	int arrow_placement = 0;
	int length = 0;
	FILE *file = fopen(filename, "a+");
	
	if(NULL == file)
	{
		return Error;
	}
		
	length = GetFileLength(file);
	
	buffer = (char *)malloc(length);
	if(NULL == buffer)
	{
		return Error;
	}
	
	
	while(arrow_placement != '<')
	{
		arrow_placement = fgetc(file);
	}
	
	arrow_placement = ftell(file);
	fseek(file, 0, SEEK_SET);
	fread(buffer, sizeof(char), length, file);
	
	freopen(filename, "w+", file);
	if(NULL == file)
	{
		return Error;
	}
	fwrite(buffer+arrow_placement, sizeof(char), length-arrow_placement ,file);
	fclose(file);
	
	file = fopen(filename, "a");
	if(NULL == file)
	{
		return Error;
	}
	fwrite(buffer, sizeof(char), arrow_placement-1 ,file);
	fclose(file);
	
	free(buffer);
	buffer = NULL;

	return Continue;
}

/*=============================================
         Initialization Block
=============================================*/

	/*initialize the function pointers*/
	compare_ptr_t compare = strncmp;
	cmd_func_t ext = OnExit;
	cmd_func_t rm = OnRemove;
	cmd_func_t count = OnCount;
	cmd_func_t write_on_top = OnLeftArrow;
	
	struct special_inputs array_of_inputs[CMD_ARR_SIZE];
	
/*Filling the array previously created with its related functions & commands*/
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
/*
	The functions checks which comnnad input the user sent,
	by chain of responsibility type-of (loops over every untill one dealing 
	with it)
*/
logger_return_t CheckCommand(char *input_string, struct special_inputs array[CMD_ARR_SIZE], char* filename)
{
	int result = Continue;
	size_t i = 0;

	while(i < CMD_ARR_SIZE)
	{
		/*Checking if its the actual command (if it equals to the string completely)*/
		result = array[i].comparison(input_string, array[i].str, strlen(array[i].str));
		
		/*in case it is the same - it triggers the operation in that struct*/
		if(Exit == result)
		{
			return array[i].operation(filename);
		}
		++i;
	}
	return Continue;
}

logger_return_t GetInput(char *filename, struct special_inputs array_of_inputs[CMD_ARR_SIZE])
{	
	char line_buffer[MAX_LINE_SIZE];
	char *input_string = NULL;
	int is_continue = Continue;
	FILE *file;
	
	input_string = fgets(line_buffer, MAX_LINE_SIZE, stdin);
		
	if(0 == compare(input_string, "-", 1))
	{
		is_continue = CheckCommand(input_string, array_of_inputs, filename);
		return is_continue;
	}
	else if(0 == compare(input_string, "<", 1))
	{
		file = fopen(filename, "a+");
		
		if(NULL == file)
		{
			return Error;
		}
		fwrite(line_buffer, sizeof(char), strlen(input_string), file);
		fclose(file);
		is_continue = CheckCommand(input_string, array_of_inputs, filename);

		
		return is_continue;
	}
	else
	{
		file = fopen(filename, "a+");
		if(NULL == file)
		{
			return Error;
		}
		fwrite(line_buffer, sizeof(char), strlen(input_string), file);
		fclose(file);
		return Continue;
	}
	
	return is_continue;
}

/*=============================================
         Logger Function Block
=============================================*/
int Logger(char *filename)
{	
	int is_running = Continue;
	
	FillSpecialInputs(array_of_inputs);
		
	printf("Let's write a book!\n");
	
	while(Continue == is_running)
	{
		is_running = GetInput(filename, array_of_inputs);
	}	
	return Success;
}





