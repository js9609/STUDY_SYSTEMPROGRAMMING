#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(){

	int fd;

	if((fd = creat("file.hole", FILE_MODE)) <0){
		printf("creat error");
		exit(1);
	}
	if(write(fd, buf1, 10) != 10){
		printf("bif1 write error");
		exit(1);
	}
	if(lseek(fd, 16384, SEEK_SET) == -1){
		printf("lseek error");
		exit(1);
	}
	if(write(fd, buf2, 10)!=10){
		printf("buf2 write error");
		exit(1);
	}
	exit(0);
}
