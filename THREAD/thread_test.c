#include <pthread.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void *threadfunc(void *arg);

int main()
{
	int n,i,j;
	pthread_t threadid;

	if((n = pthread_create(&threadid, NULL, &threadfunc, NULL))!=0)
	{
		perror("Thread Create Error " );
		exit(0);
	}
	printf("Main Thread START \n");
	for(i=0;i<1000000;i++)
	{
		j = i*5;
	}
	printf("Main Thread END\n");
	pthread_join(threadid, NULL);
}

void *threadfunc(void *arg)
{
	int i,j;
	printf("Thread Start\n");

	for(i =0; i<1000000;i++)
	{
		j = i*5;
	}
	printf("Thread End \n");
	return NULL;

}
