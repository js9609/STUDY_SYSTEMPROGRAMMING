#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREAD_NUM 1
void *t_function(void *data)
{
	time_t current_time;
	struct tm *mytm;
	char *time_str;
	
	current_time = time((time_t*)NULL);
	mytm = localtime(&current_time);

	time_str = asctime(mytm);
	printf("Child Thread Start Time : %s ", time_str);
	sleep(5);
}

int main(int argc, char **argv)
{
	pthread_t p_thread[2];
	int thr_id;
	int status;
	int i = 0;
	time_t current_time;
	struct tm *mytm;
	char *time_str;

	current_time = time((time_t *)NULL);
	mytm = localtime(&current_time);
	time_str = asctime(mytm);
	printf("Main Thread Start Time 1 : %s", time_str);
	sleep(10);

	for(i = 0; i< MAX_THREAD_NUM; i++)
	{
		thr_id = pthread_create(&p_thread[i], NULL, &t_function, (void*)&i);
		if(thr_id < 0)
		{
			perror("thread create error :");
			exit(0);
		}
	}
	pthread_join(p_thread[0], NULL);
	printf("Main Thread Start Time 2 : %s", time_str);
	return 0;
}
