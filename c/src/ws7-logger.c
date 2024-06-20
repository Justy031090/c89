#include <stdio.h> /*prints, streams */
#include <stdlib.h>
#include <string.h> /*strncmp*/

#define ARRAY_SIZE (4)



typedef int (*fun)(FILE *file, char* filename);
typedef int (*compare_ptr)(const char*, const char*, size_t);

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
	int count = 0;
	int c = getc(file);
	(void)filename;
	
	if( 0 < count)
	{
		printf("Number of lines in the file is %d\n", count);
		return 0;
	}
	else
	{
		printf("The file is empty !\n");
		return 1;
	}
}

int OnLeftArrow(FILE *file, char* filename)
{
	char buffer[1024];
	int size = 0;
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fread(buffer, 1, size, file);
	printf("%s\n", buffer);
	freopen(filename, "w", file);
	buffer[0] = '^';
	fwrite(buffer,1, size, file);
	
	/*create buffer to hold prev info*/
	/*write first new line, then flush buffer inside*/
	(void)filename;

	return 1;
}

	compare_ptr compare = strncmp;
	fun ext = OnExit;
	fun rm = OnRemove;
	fun count = OnCount;
	fun write_on_top = OnLeftArrow;
	
struct special_inputs
{
	char *str;
	compare_ptr comparison;
	fun operation;
};

struct special_inputs array_of_inputs[ARRAY_SIZE];


void FillArr(struct special_inputs array[ARRAY_SIZE])
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

int CheckCommand(char *command, struct special_inputs array[], size_t array_size, FILE *file, char* filename)
{
	int result = 2;
	size_t i = 0;
	while(i<array_size)
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
	
void EnterStrings(char *filename)
{	

	int result = 1;
	char buffer[200];
	char *test = NULL;
	FILE *fp = fopen(filename, "a+");
	
	FillArr(array_of_inputs);
	
	if(fp == NULL)
		return;
		
	printf("Let's write a book!\n");
	
	while(result == 1)
	{
		text = fgets(buffer, 199, stdin);
		if(compare(test, "<", 1) == 0 || compare(test, "-", 1) == 0)
		{
			result = CheckCommand(text, array_of_inputs, ARRAY_SIZE, fp, filename);
		}
		fwrite(buffer,1, strlen(text), fp);
	}
	
	
	exit(0);
}
