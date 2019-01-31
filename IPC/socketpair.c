#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	int sv[2];
	int pid;
	int mynum = 1;

	if(socketpair(AF_UNIX, SOCK_STREAM, 0 , sv) < 0)
	{
		perror("socketpair error");
		exit(0);
	}

	pid = fork();

	if(pid < 0)
	{
		perror("fork error");
		exit(0);
	}

	if(pid == 0)
	{
		dup2(sv[0],0);

		close(sv[1]);
		close(sv[0]);
		execl("/home/jinsun/Desktop/JinSun/SystemProgramming/IPC/pipe_cl", "pipe_cl", NULL);
	}
	else if(pid > 0)
	{
		close(sv[0]);
		while(1)
		{
			write(sv[1], (void *)&mynum, sizeof(mynum));
			sleep(1);
			read(sv[1], (void *)&mynum, sizeof(mynum));
			printf("num is %d\n", mynum);
		}
	}
}
