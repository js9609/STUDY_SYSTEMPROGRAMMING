#include <unistd.h>
#include <stdio.h>

int main()
{
	int pid;
	pid = fork();
	if(pid == 0)
	{
		printf("I am Child \n");
		execlp("/bin/date", "/bin/date", (char*)0);
	}
	else
		printf("I am Parent\n");

	return 0;
}
