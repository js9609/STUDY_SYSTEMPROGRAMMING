#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sig_handler(int signo);

int main()
{
	int i = 0;
	printf("Program Start\n");
	if(signal(SIGHUP, (void*)sig_handler) == SIG_ERR)
	{
		perror("signal set error : ");
		exit(0);
	}
	while(1)
	{
		printf("%d\n", i);
		i++;
		sleep(1);
	}
	return 1;
}
void sig_handler(int signo)
{
	execl("./sig_hup","./sig_hup", NULL);
}
