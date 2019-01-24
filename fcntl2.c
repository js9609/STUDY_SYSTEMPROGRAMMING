#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void set_fl(int fd, int flags){
	int val;

	if((val = fcntl(fd, F_GETFL, 0))<0){
		printf("fcntl F_GETFL error");
		exit(1);
	}

	val |= flags;

	if(fcntl(fd, F_SETFL, val)<0){
		printf("fcntl F_SETFL error");
		exit(1);
	}
}
