#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int fd_in, fd_out;
	int n;
	char line[255];

	if(argc != 3)
	{
		printf("Usage : ./echo_file [원본파일이름] [복사파일이름]\n");
		exit(0);
	}

	if (access(argv[1], F_OK)!=0)
	{
		printf("파일 존재 X :%s\b", argv[1]);
		exit(0);
	}
	if (access(argv[2], F_OK) == 0)
	{
		printf("파일 이미 존재 : %s \n", argv[2]);
	}
	
	if((fd_in=open(argv[1], O_RDONLY))<0)
	{
		perror("file open error");
		exit(0);
	}
	if((fd_out=open(argv[2], O_WRONLY|O_CREAT))<0)
	{
		perror("file open error");
		exit(0);
	}
	memset(line, '\0', 255);
	while( (n = read(fd_in, line, 255)) >0)
	{
		write(fd_out, line, n);
		memset(line, '\0', 255);
	}
	close(fd_in);
	close(fd_out);
	
}
