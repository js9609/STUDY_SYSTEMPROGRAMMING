#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* fp;
	char buf[256];

	fp = fopen("test.txt", "a+");
	if(fp == NULL)
	{
		perror("fopen error : " );
		exit(0);
	}

	printf("%d\n\n", ftell(fp));

	fseek(fp, 3, SEEK_SET);
	fgets(buf, 256, fp);
	printf("%s", buf);

	fseek(fp, 0 , SEEK_SET);
	fgets(buf, 256, fp);
	printf("%s", buf);

	fclose(fp);
}
