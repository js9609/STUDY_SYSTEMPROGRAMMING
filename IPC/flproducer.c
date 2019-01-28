#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 10

struct data
{
	char name[80];
};

struct flock lock, unlock;

int lock_open(int fd, int index)
{
	lock.l_start = index;
	lock.l_type = F_WRLCK;
	lock.l_len = 1;
	lock.l_whence = SEEK_SET;
	return fcntl(fd, F_SETLKW, &lock);
}
int lock_close(int fd, int index)
{
	unlock.l_start = index;
	unlock.l_type = F_UNLCK;
	unlock.l_len = 1;
	unlock.l_whence = SEEK_SET;
	return fcntl(fd, F_SETLK, &unlock);
}
void lock_init()
{
	lock.l_start = 0;
	lock.l_type = F_WRLCK;
	lock.l_len = 1;
	lock.l_whence = SEEK_SET;
}

void unlock_init()
{
	unlock.l_start = 0;
	unlock.l_type =F_UNLCK;
	unlock.l_len =1;
	unlock.l_whence = SEEK_SET;
}

int main()
{
	int shmid;
	int i =0;
	int offset = 0;

	struct data *cal_num;
	void *shared_memory;
	struct data ldata;
	int fd;

	lock_init();
	unlock_init();

	if((fd = open("shm_lock", O_CREAT | O_RDWR))<0)
	{
		perror("file open error " );
		exit(0);
	}
	write(fd, (void *)'\0', sizeof(char)*QUEUE_SIZE);

	shmid = shmget((key_t)1234, sizeof(ldata)*QUEUE_SIZE, 0666|IPC_CREAT);
	if(shmid == -1)
	{
		perror("shmget failed : ");
		exit(0);
	}
	
	shared_memory = (void *)malloc(sizeof(ldata)*QUEUE_SIZE);
	shared_memory = shmat(shmid, (void *)0, 0);

	if(shared_memory == (void *)-1)
	{
		perror("shmat failed : ");
		exit(0);
	}

	while(1)
	{
		sprintf(ldata.name, "write data : %d\n", i);

		printf("%d %s", (i==0) ? QUEUE_SIZE -1:i-1, ldata.name);

		if(lock_open(fd, i)<0)
		{
			perror("lock error");
		}
		if(lock_close(fd, (i==0)?QUEUE_SIZE-1:i-1)<0)
		{
			perror("flock error");
		}

		memcpy((void *)shared_memory+offset, (void *)&ldata, sizeof(ldata));
		sleep(1);
		offset += sizeof(ldata);
		i++;

		if(i==QUEUE_SIZE)
		{
			i = 0;
			offset = 0;
		}
	}
}
