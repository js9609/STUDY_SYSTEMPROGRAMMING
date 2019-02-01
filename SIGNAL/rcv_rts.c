#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void sighandler(int signo, siginfo_t *si)
{
	if(si->si_code == SI_QUEUE)
	{
		printf("User RTS Signal %d\n", si->si_pid);
		printf("Sig Number %d\n", si->si_signo);
		printf("User Data is %d\n", si->si_value.sival_int);

		getchar();
	}
	else
	{
		printf("Get None realtime signal %d\n", signo);
	}
}

int main()
{
	struct sigaction sigact;

	printf("My pid %d\n", getpid());

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_restorer = NULL;
	sigact.sa_sigaction = (void *)sighandler;

	if(sigaction(SIGRTMIN, &sigact, 0) ==1 )
	{
		printf("signal error\n");
		exit(0);
	}
	while(1)
		sleep(1);
}
