#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BLOCK_SIZE = 16;

int fd_isopen(int fd, int data_index);
int fd_unlock(int fd, int data_index);
int main(int argc, char **argv)
{
	int fd;
	int n_read;
	int data_index;
	char buf[BLOCK_SIZE];
	char wbuf[BLOCK_SIZE];

	if (argc < 2)
	{
		printf("Usage :./fcntl_counter.c [데이터번호]\n");
		exit(0);
	}

	data_index = BLOCK_SIZE * (atoi(argv[1])-1);

	fd = open("b_counter", O_RDWR);
	if (fd == -1)
	{
		perror("file open error : ");
		exit(0);
	}

	if(fd_isopen(fd, data_index)== -1)
	{
		perror("file is lock");
		exit(0);
	}
	lseek(fd, data_index, SEEK_SET);
	memset(buf, 0x00, BLOCK_SIZE);
	if((n_read == read(fd, buf, BLOCK_SIZE))<0)
	{
		perror("read error : " );
		exit(0);
	}
	memset(wbuf, 0x00, BLOCK_SIZE);
	sprintf(wbuf, "%d", atoi(buf)+1);
	printf("count %s -> %s\n", buf, wbuf);
	
	lseek(fd, data_index, SEEK_SET);
	write(fd, wbuf, BLOCK_SIZE);

	sleep(10);

	if(fd_unlock(fd, data_index) == -1)
	{
		perror("file unlock error ");
		exit(0);
	}
	close(fd);
}

int fd_isopen(int fd, int data_index)
{
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = data_index;
	lock.l_whence = SEEK_SET;
	lock.l_len = 1;

	return fcntl(fd, F_SETLKW, &lock);	
}

int fd_unlock(int fd, int data_index)
{
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_start = data_index;
	lock.l_whence = SEEK_SET;
	lock.l_len = 1;

	return fcntl(fd, F_SETLK, &lock);
}
