#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(){

	int fd;
	int i;
	int wdata = 0;
	int wsize = 0;
	fd = open("data.txt", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	if(fd < 0){
		perror("file open error");
		return 1;
	}

	for (i = 0; i< 100; i++){
		wdata =i*2;
		wsize = write(fd , (void*)&wdata, sizeof(int));
		printf("Write %d (%d byte)\n", i*2, wsize);
	}
	close(fd);

}
