#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

//쓰레드 종료시 호출
void clean_up(void *);
//쓰레드 생성시 실행할 함수
void *thread_func(void *);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lmu = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	pthread_t pt;
	pthread_create(&pt, NULL, &thread_func, NULL);

	pthread_cancel(pt);

	sleep(5);
	pthread_cond_signal(&cond);

	pthread_join(pt, NULL);
	printf("exit\n");
	exit(1);
}
void clean_up(void *arg)
{
	printf("Thread Cancel Clean_up Funciton\n");
}
void *thread_func(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	//종료시 호출할 함수를 등록하는 함수
	pthread_cleanup_push(clean_up,(void*)NULL);

	pthread_mutex_lock(&lmu);
	printf("THREAD Cond Wait\n");
	pthread_cond_wait(&cond, &lmu);
	printf("NO WAIT COND\n");
	pthread_mutex_unlock(&lmu);

	printf("EXIT\n");
	pthread_cleanup_pop(0);

}
