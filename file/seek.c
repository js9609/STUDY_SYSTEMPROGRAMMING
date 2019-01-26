#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int fd;
	char buff[255];
	memset(buff, 0x00, 255);

	fd = open("test.txt", O_RDONLY);
	if(fd < 0 )
	{
		perror("error : ");
		exit(0);
	}

	lseek(fd , -3, SEEK_END);
	read(fd, buff, 255);
	printf("%s", buff);

	close(fd);

}
