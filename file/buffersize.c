#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

int wtest(int cycle, int block_size);
int rtest(int cycle, int block_size, char *file);
int main(int argc, char **argv)
{
	int fd;
	int i = 1;
	int cycle =0;
	int total_size = 1024*1024*256;
	int minblock =16;
	int maxblock = 4096;
	int defaultblock = minblock;

	printf("========= Write Perf\n");
	while(minblock = minblock *2)
	{
		if(minblock > maxblock)
		{
			break;
		}
		wtest(total_size/minblock, minblock);
	}

	printf("============Read Perf\n");
	minblock = defaultblock;
	while(minblock = minblock * 2)
	{
		if(minblock > maxblock)
		{
			break;
		}
		rtest(total_size/minblock, minblock, "temp.1");
	}
	close(fd);
}
int wtest(int cycle, int block_size)
{
	int i;
	int fd;
	clock_t sclock, eclock;
	char *wdata;
	struct timeval stime, etime, rtime;

	unlink("temp.1");
	wdata = (char *)malloc(block_size);
	fd = open("temp.1", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
	if(fd < 0)
	{
		perror("file open error");
		return 1;
	}
	gettimeofday(&stime, NULL);
	sclock = clock();
	for(i = 0; i < cycle; i++)
	{
		write(fd, (void *)&wdata, block_size);
	}
	eclock = clock();
	gettimeofday(&etime, NULL);
	timersub(&etime, &stime, &rtime);
	printf("block=%d size=%d time=%d.%d clock=%.3fs\n", block_size, cycle*block_size, rtime.tv_sec, rtime.tv_usec, (double)(eclock-sclock)/CLOCKS_PER_SEC);

	close(fd);
}

int rtest(int cycle, int block_size, char *file)
{

	char *buff;
	int fd;
	clock_t sclock, eclock;
	int readn;
	struct timeval stime, etime, rtime;
	buff = (char *)malloc(block_size);
	fd = open(file, O_RDONLY);
	if( fd < 0)
	{
		perror("file open error");
		return 0;
	}
	gettimeofday(&stime, NULL);
	sclock = clock();
	while(readn = read(fd , buff, block_size))
	{}

	eclock = clock();
	gettimeofday(&etime, NULL);
	timersub(&etime, &stime, &rtime);
	printf("block = %d size =%d time %d.%d clock=%.3fs\n", block_size, cycle*block_size, rtime.tv_sec, rtime.tv_usec, (double)(eclock-sclock)/CLOCKS_PER_SEC);
	return 1;
}



















		
