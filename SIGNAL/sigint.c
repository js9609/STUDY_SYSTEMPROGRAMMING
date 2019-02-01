#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sig_handler(int signo);

int main()
{
	int i = 0;
	signal(SIGINT, (void *)sig_handler);

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
	printf("\a");
}
