#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
	int pid;
	int i;
	i = 1000;
	pid = fork();
	if (pid == -1)
	{
		perror("fork error ");
		exit(0);
	}
	else if(pid == 0)
	{
		printf("자식 : 내 PID는  %d\n", getpid());
		while(1)
		{
			printf("--> %d\n", i);
			i++;
			sleep(1);
		}
	}
	else
	{
		printf("부모: 내가 낳은 자식의 PID는 %d\n", pid);
		while(1)
		{
			printf("==>%d\n",i);
			i += 4;
			sleep(1);
		}
	}
}
