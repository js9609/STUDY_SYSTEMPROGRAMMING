#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void sig_handler(int signo);

int main()
{
	int  i = 0;
	sigset_t newmask, oldmask;

	printf("Program Start\n");

	if(signal(SIGHUP, (void*)sig_handler)==SIG_ERR)
	{
		perror("signal set error ");
		exit(0);
	}
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGHUP);
	if(sigprocmask(SIG_UNBLOCK, &newmask, &oldmask)<0)
	{
		perror("sigmask error : ");
		exit(0);
	}
	while(1)
	{
		printf("%d\n",i);
		i++;
		sleep(1);

	}
	return 1;
}

void sig_handler(int signo)
{
	execl("./sig_hup2", "./sig_hup2",NULL);
}


