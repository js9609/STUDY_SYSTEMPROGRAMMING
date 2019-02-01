#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	union sigval sv;
	sv.sival_int = atoi(argv[2]);
	sigqueue(atoi(argv[1]), SIGRTMIN, sv);
}
