#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
void sig_int();
void sig_usr();

int main()
{
	char buf[255];
	int i = 0;

	if((signal(SIGINT, sig_int)) == SIG_ERR)
	{
		perror("signal setting error : ");
		exit(1);
	}
	if((signal(SIGUSR2, sig_usr)) == SIG_ERR)
	{
		perror("signal setting error : ");
		exit(1);
	}

	while(1)
	{
		printf("%d\n", i);
		sleep(1);
		i++;
	}
}
void sig_int()
{
	fprintf(stderr, "SIGINT !\n");
	sleep(5);
}
void sig_usr()
{
	fprintf(stderr, "SIGUSR!\n");
}
