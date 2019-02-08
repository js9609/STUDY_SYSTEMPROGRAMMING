#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
int main()
{
	pid_t pid;
	if((pid = fork()) < 0)
		exit(0);
	else if(pid != 0)
		exit(0);

	chdir("/");

	setsid();
	while(1)
		sleep(1);
}
