#include <stdio.h>
#include <unistd.h>

main(){
	int pid;
	pid = fork();
	if(pid == 0 )
		printf("I am Child\n");
	else
		printf("I am Parent\n");
}

