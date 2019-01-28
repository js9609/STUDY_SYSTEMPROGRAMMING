#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define chop(x) x[strlen(x) -1] = 0x00

int main()
{
	int fp_w;
	int fp_r;

	int n;
	char buf_r[11];
	char buf_w[11];

	if((fp_r = open("/home/jinsun/Desktop/JinSun/SystemProgramming/IPC/fifo_c", O_RDONLY))<0)
	{
		perror("open error : ");
		exit(0);
	}
	if((fp_w = open("/home/jinsun/Desktop/JinSun/SystemProgramming/IPC/fifo_s", O_WRONLY))<0)
	{
		perror("open error :");
		exit(0);
	}
	memset(buf_r, 0x00, 11);
	memset(buf_w, 0x00, 11);

	while((n=read(fp_r, buf_r, 11))>0)
	{
		printf("%s", buf_r);
		chop(buf_r);
		sprintf(buf_w, "%d\n", atoi(buf_r) * atoi(buf_r));

		write(fp_w, buf_w, 11);
		
		memset(buf_r, 0x00, 11);
		memset(buf_w, 0x00, 11);
	}
	exit(1);
}
