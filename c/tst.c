#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*fun)(FILE *file, char* filename);
typedef int (*compare_ptr)(const char*, const char*);

int OnExit(FILE *file, char* filename)
{
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
	
	printf("%c\n", c);
	
	for(; c != EOF; c = getc(file))
	{
		if('\n' == c)
		{
			count++;
		}
	}
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
	/*Add to the beginning of the file*/
	/*Set Point Back to the end*/
	/*close the file*/
	return 1;
}

	compare_ptr compare = strcmp;
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

struct special_inputs array_of_inputs[4];

void FillArr(struct special_inputs array[])
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
		result = array[i].comparison(command, array[i].str);
		if(0 == result)
		{
			array[i].operation(file, filename);
			break;
		}
		++i;
	}
	return 0;
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
		test = fgets(buffer, 199, stdin);
		if(strncmp(test, "<", 1) == 0 || strncmp(test, "-", 1) == 0)
		{
			result = CheckCommand(test, array_of_inputs, 4, fp, filename);
		}
		
		fwrite(buffer,1, strlen(test), fp);
	}
	
	exit(0);
}


int main()
{	
	EnterStrings("tst.txt");
	return 0;
}
