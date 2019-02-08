#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
void zombie_handler()
{
	int status;
	int spid;
	spid = wait(&status);
	printf("Child Process Wait Success \n");
	printf("===========================\n");
	printf("PID			:%d\n", spid);
	printf("EXIT Value	:%d\n", WEXITSTATUS(status));
	printf("Exit Stat	:%d\n", WIFEXITED(status));
}

int main()
{
	int pid;
	int status;
	int spid;
	int i;

	signal(SIGCHLD, (void *)zombie_handler);
	for(i = 0; i < 3; i++)
	{
		pid = fork();
		int random_value = (random()%5)+3;
		if(pid == 0)
		{
			printf("I will be back %d %d\n", random_value, getpid());
			sleep(random_value);
			return random_value;
		}
	}
	getchar();
}
