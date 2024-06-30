#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ws11.h"

exit_code WriteToBin(char *filename, Student Michael)
{	

	FILE *file = fopen(filename, "wb");
	int num_of_writes = 0 ;
	if (NULL == file)
	{
        return FAILED_TO_OPEN;
    };
    
	num_of_writes += fwrite(&Michael.firstname, sizeof(char),MAX_NAME_LEN , file);
	num_of_writes += fwrite(&Michael.lastname, sizeof(char), MAX_NAME_LEN, file);
	num_of_writes += fwrite(&Michael.grades.sports, sizeof(float), 1, file);
	num_of_writes += fwrite(&Michael.grades.h_g.sociology,sizeof(float) , 1, file);
	num_of_writes += fwrite(&Michael.grades.h_g.psychology, sizeof(float), 1, file);
	num_of_writes += fwrite(&Michael.grades.h_g.literature, sizeof(float), 1, file);
	num_of_writes += fwrite(&Michael.grades.r_g.math, sizeof(float), 1, file);
	num_of_writes += fwrite(&Michael.grades.r_g.physics, sizeof(float), 1, file);
	num_of_writes += fwrite(&Michael.grades.r_g.chemistry, sizeof(float), 1, file);
	
	fclose(file);
	
	if (TOTAL_STRUCT_SIZE != num_of_writes)
	{	printf("failed in fwrite\n, %d\n", num_of_writes);
		remove(filename);
		return FAILED_TO_WRITE;
	}
	
	return SUCCESS;
}

exit_code ReadFromBin(char *filename, Student *NotMichael)
{
	FILE *file = fopen(filename, "rb");
	int num_of_reads = 0 ;
	if (NULL == file)
	{
        return FAILED_TO_OPEN;
    };
  
	num_of_reads += fread(&NotMichael->firstname, sizeof(char), MAX_NAME_LEN, file);
	num_of_reads += fread(&NotMichael->lastname, sizeof(char), MAX_NAME_LEN, file);
	num_of_reads += fread(&NotMichael->grades.sports, sizeof(float), 1, file);
	num_of_reads += fread(&NotMichael->grades.h_g.sociology, sizeof(float), 1, file);
	num_of_reads += fread(&NotMichael->grades.h_g.psychology, sizeof(float), 1, file);
	num_of_reads += fread(&NotMichael->grades.h_g.literature, sizeof(float), 1, file);
	num_of_reads += fread(&NotMichael->grades.r_g.math, sizeof(float), 1, file);
	num_of_reads += fread(&NotMichael->grades.r_g.physics, sizeof(float), 1, file);
	num_of_reads += fread(&NotMichael->grades.r_g.chemistry, sizeof(float), 1, file);
	
	fclose(file);
	
	if (TOTAL_STRUCT_SIZE != num_of_reads)
	{
		printf("failed in fread\n");
		return FAILED_TO_READ;
	}
	
	return SUCCESS;
	
}


