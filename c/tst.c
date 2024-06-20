#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	size_t test = 0;
	int arrow_placement = 0;
	int length = 0;
	(void)filename;
		
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
	
	fread(buffer, 1, length, file);
	
	freopen(filename, "w+", file);
	
	printf("%s\n", buffer+arrow_placement);
	
	fwrite(buffer+arrow_placement, 1, length-arrow_placement ,file);
	
	fclose(file);
	file = fopen(filename, "a");
	
	fwrite(buffer, 1, arrow_placement-1 ,file);

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
	/* have to check the <*/
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
		test = fgets(buffer, 199, stdin);
		if(compare(test, "-", 1) == 0)
		{
			result = CheckCommand(test, array_of_inputs, 4, fp, filename);
		}
		else if(compare(test, "<", 1) == 0)
		{
			fwrite(buffer,1, strlen(test), fp);
			result = CheckCommand(test, array_of_inputs, 4, fp, filename);
		}
		else
		{
			fwrite(buffer,1, strlen(test), fp);
		}
	}	
	exit(0);
}


int main()
{	
	EnterStrings("tst.txt");
	return 0;
}
