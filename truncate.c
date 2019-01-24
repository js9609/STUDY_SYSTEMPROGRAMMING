#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(){
	int fd;
	fd = open("test4.txt", O_CREAT|O_RDWR);

	if (ftruncate(fd, 20) < 0){
		perror("error : ");
	}
	close(fd);

}
