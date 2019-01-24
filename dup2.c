#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int main(int argc, char **argv){
	int fd;
	fd = open("test.log", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	fd = dup2(fd, STDOUT);
	printf("Hello World %d\n", fd);
}
