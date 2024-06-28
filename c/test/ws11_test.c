#include "ws11.h"


int main ()
{
	char *buffer = "Hello World OK !";
	char *buffer2[17];
	FILE *file;
	file = fopen("test.bin","wb+");
	fwrite(&buffer,strlen(buffer),1,file);
	fclose(file);
	
	file = fopen("test.bin", "rb");
	fread(buffer2,1,sizeof(buffer2), file);
	fclose(file);
	
	printf("%s\n", *buffer2);
	
	
	
	return 0;
}

