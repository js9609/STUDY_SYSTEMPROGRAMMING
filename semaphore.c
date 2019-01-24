#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_THREAD 2

int count = 0;

void *myThreadFunc(void *data);

struct sembuf mysem_open = {0, -1, SEM_UNDO};
struct sembuf mysem_close = {0, 1, SEM_UNDO};

union snum
{
	int val;
};
static int semid;

int main(int argc, char **argv)
{
	int thr_id;
	int status;
	int i;
	union snum s_union;
	pthread_t pt[MAX_THREAD];

	semid = semget(2345, 1, 0600|IPC_CREAT);

	if(semid == - 1)
	{
		perror("semget error");
		return 1;
	}
	s_union.val = 1;
	if(semctl(semid, 0 , SETVAL, s_union)==-1)
	{
		return 1;
	}
	for(i = 0; i< MAX_THREAD; i++){
		thr_id = pthread_create(&pt[i], NULL, myThreadFunc, (void *)&i);
		if(thr_id < 0)
		{
			perror("Thread Create Error");
			return 1;
		}
		sleep(1);
	}
	for(i = 0; i< MAX_THREAD; i++)
	{
		pthread_join(pt[i], NULL);
	}
}

void *myThreadFunc(void *data)
{
	int thread_num =  *(int *)data;
	int lnum;
	printf("Thread Create %d\n", thread_num);
	while(1)
	{
//		semop(semid, &mysem_open, 1);
		lnum = count;
		sleep(1);
		lnum = lnum +1;
		count = lnum;
		printf("[%d] count : %d\n", thread_num, count);
		semop(semid, &mysem_close, 1);
	}
}
