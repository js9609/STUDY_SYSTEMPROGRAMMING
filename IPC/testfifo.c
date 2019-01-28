#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	FILE *fp;

	char buf[255];

	if((fp = fopen("/home/jinsun/Desktop/JinSun/SystemProgramming/IPC/testfifo", "rw")) == NULL)
	{
		perror("open error : ");
		exit(0);
	}

	memset(buf, 0x00, 255);

	while(1)
	{
		while(fgets(buf, 255, fp)!=NULL)
		{
			printf("%s", buf);
			memset(buf, 0x00, 255);
		}
	}
}
