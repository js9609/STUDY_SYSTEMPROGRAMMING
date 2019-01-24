#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
	int pid;
	int i;
	pid = fork();
	if (pid == -1)
	{
			perror("fork error ");
			exit(0);

	}
	else if(pid == 0)
	{
		printf("자식: 내 PID는 %d\n", getpid());
		close(0);
		close(1);
		close(2);
		setsid(); // 그룹의 Leader일 경우 fork, 아닐 경우 exec같은 역할을 함
		while(1)
		{
			printf("-->%d\n", i);
			i++;
			sleep(1);
		}
	}
	else
	{
		printf("부모: 내가 낳은 자식의 PID는 %d\n", pid);
		sleep(1);
		printf("나 죽네~~\n");
		exit(0);
	}

}
