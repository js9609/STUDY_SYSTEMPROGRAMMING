#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STDIN 0

int main()
{
	int mode, fd , value;
	char buf[255];

	memset(buf, 0x00, 255);
	
	read(STDIN, buf, 255);
	printf("-> %s\n", buf);
	memset(buf, 0x00, 255);

	value = fcntl(STDIN, F_GETFL, 0);
	value |= O_NONBLOCK;
	
	fcntl(STDIN, F_SETFL, value);
	printf("NON BLOCKING MODE 로 변경\n");

	sleep(2);

	while(1)
	{
		read(STDIN, buf, 255);
		printf("-> %s\n", buf);
	}
}
