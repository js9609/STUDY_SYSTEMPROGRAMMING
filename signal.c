#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int pid;
	int sig_num;

	if(argc != 3)
	{
		printf("usage %s [pid] [signum]\n", argv[0]);
		return 1;
	}
	pid = atoi(argv[1]);
	sig_num = atoi(argv[2]);
	if(!kill(pid, sig_num))
	{
		perror("Signal Send error ");
		return 1;
	}
	return 0;
}
