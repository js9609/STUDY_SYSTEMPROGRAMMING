#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	char buf[256];

	printf("Instruction Execute Shell : %s\n", getenv("SHELL"));
	fp = popen("ls -al", "r");

	while(fgets(buf, 256,fp))
		printf("%s\n", buf);
	pclose(fp);
}
