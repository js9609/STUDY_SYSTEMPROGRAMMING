#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	int fd;
	char *file = NULL;
	struct stat sb;
	char buf[80] = {0x00,};
	int flag = PROT_WRITE | PROT_READ;

	if(argc < 2)
	{
		fprintf(stderr, "Usage: input \n");
		exit(1);
	}
	if((fd = open(argv[1], O_RDWR | O_CREAT)) <0)
	{
		perror("File Open Error ");
		exit(1);
	}
	
	//파일의 유형을 확인하는것
	if(fstat(fd, &sb)<0)
	{
		perror("fstat error ");
		exit(0);
	}

	file = (char *)malloc(40);

	if((file = (char *)mmap(0,40,flag, MAP_SHARED, fd ,0)) == -1)
	{
		perror("mmap error");
		exit(1);
	}

	printf("%s\n", file);
	memset(file, 0x00, 40);
	close(fd);
}
