#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ws11.h"

int WriteToBin(char *filename, Student Michael)
{	

	FILE *file = fopen(filename, "wb");
	if (NULL == file)
	{
        return -1;
    };

	fwrite(&Michael.firstname, sizeof(char),MAX_NAME_LEN , file);
	fwrite(&Michael.lastname, sizeof(char), MAX_NAME_LEN, file);
	fwrite(&Michael.sports, sizeof(float), 1, file);
	fwrite(&Michael.h_g.sociology,sizeof(float) , 1, file);
	fwrite(&Michael.h_g.psychology, sizeof(float), 1, file);
	fwrite(&Michael.h_g.literature, sizeof(float), 1, file);
	fwrite(&Michael.r_g.math, sizeof(float), 1, file);
	fwrite(&Michael.r_g.physics, sizeof(float), 1, file);
	fwrite(&Michael.r_g.chemistry, sizeof(float), 1, file);
	
	fclose(file);
	return 0;
}

int ReadFromBin(char *filename, Student *NotMichael)
{
	FILE *file = fopen(filename, "rb");
	if (NULL == file)
	{
        return -1;
    };
  
	
	fread(&NotMichael->firstname, sizeof(char), MAX_NAME_LEN, file);
	fread(&NotMichael->lastname, sizeof(char), MAX_NAME_LEN, file);
	fread(&NotMichael->sports, sizeof(float), 1, file);
	fread(&NotMichael->h_g.sociology, sizeof(float), 1, file);
	fread(&NotMichael->h_g.psychology, sizeof(float), 1, file);
	fread(&NotMichael->h_g.literature, sizeof(float), 1, file);
	fread(&NotMichael->r_g.math, sizeof(float), 1, file);
	fread(&NotMichael->r_g.physics, sizeof(float), 1, file);
	fread(&NotMichael->r_g.chemistry, sizeof(float), 1, file);
	
	fclose(file);
	
	return 0;
	
}


