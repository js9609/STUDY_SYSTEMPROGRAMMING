#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

int main()
{
	int pid;
	int stauts;
	pid = fork();

	if(pid == 0)
	{
		sleep(5);
		printf("I Will Be Back %d\n", getpid());
		exit(0);
	}
	else if (pid > 0)
	{
		printf(" Im parent %d\n", getpid());
		printf("Press any key\n");
		getchar();
	}
	else
	{
		perror("fork error : ");
	}


}
