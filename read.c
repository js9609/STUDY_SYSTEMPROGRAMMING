#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


int main(){
	int fd;
	int readn = 0;
	int buf;
	fd = open("data.txt", O_RDONLY);
	if(fd < 0 ){
		perror("file open error:");
		return 1;
	}
	while( (readn = read(fd,(void*)&buf,sizeof(buf)))>0)
	{
		printf("Read Data :%d\n", buf);
	}
 
}
