#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fd_lock(int fd);
int fd_unlock(int fd);
int fd_isopen(int fd);

int main()
{
	int fd;
	int n_read;
	char buf[11];
	char wbuf[11];

	fd = open("counter.txt", O_CREAT|O_RDWR);
	if(fd == -1)
	{
		perror("file open error : ");
		exit(0);
	}
	if(fd_isopen(fd) == -1)
	{
		perror("file is lock");
		exit(0);
	}
	printf(" I get file lock \n");
	memset(buf, 0x000, 11);
	memset(wbuf, 0x00, 11);

	if((n_read = read(fd, buf, 11))>0)
	{
		printf("%s\n", buf);
	}
	lseek(fd, 0, SEEK_SET);
	sprintf(wbuf, "%d", atoi(buf) +1);
	write(fd, wbuf, 11);

	ftruncate(fd, strlen(wbuf));

	sleep(10);

	if(fd_unlock(fd)==-1)
	{
		perror("file unlock error ");
	}
	printf("file unlock success\n");
	sleep(5);

	close(fd);
}

int fd_isopen(int fd)
{
	struct flock lock;

	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;

	return fcntl(fd, F_SETLKW, &lock);

}

int fd_unlock(int fd)
{
	struct flock lock;
	
	lock.l_type = F_UNLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;

	return fcntl(fd, F_SETLK, &lock);
}
