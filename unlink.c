#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void){
	if (open("tempfile",O_CREAT|O_RDWR)<0){
		printf("open error\n");
		exit(1);
	}
	if(unlink("tempfile")<0){
		printf("unlink error\n");
		exit(1);
	}
	printf("file unlinked\n");
	sleep(15);
	printf("done\n");
	exit(0);
}

