#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

int main()
{
	int fd;
	int val;
	fd = open("exec_copy.txt", O_CREAT);

	val = fcntl(fd, F_GETFD, 0);
	if(val & FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");

	val |= FD_CLOEXEC;
	if(val & FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");
	fcntl(fd, F_SETFD, val);
	
	execl("/home/jinsun/Desktop/JinSun/SystemProgramming/file/loop", "./loop", 0);

}
