#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	char buf[20];
	memset(buf, 0x00, 20);
	sprintf(buf, "%d",getpid());
	write(1,buf,20);
	while(1)
	{
		sleep(1);
	}
}
