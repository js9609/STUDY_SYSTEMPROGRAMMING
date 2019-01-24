#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int n, fd[2];
	char buf[255];
	int pid;

	if(pipe(fd)<0)
	{
		perror("pipe error : ");
		exit(0);
	}

	if ((pid = fork())<0)
	{
		perror("fork error : ");
		exit(0);
	}

	//CHILD PROCESS
	else if(pid == 0)
	{
		close(fd[0]); //자식은 쓰기만 할 것이기 때문에 읽은 fd[0] 은 닫아버림
		while(1)
		{
			memset(buf,0x00, 255);
			sprintf(buf, "Hello Mother Process. My name is %d\n", getpid());
			write(fd[1], buf, strlen(buf));
			sleep(1);
		}
	}
	else
	{
		close(fd[1]); //부모는 읽기만 할 것이기 때문에 쓰는 fd[0]은 닫아버림
		while(1)
		{
			memset(buf, 0x00, 255);
			n = read(fd[0], buf, 255);
			fprintf(stderr, "%s", buf);
		}
	}
}
