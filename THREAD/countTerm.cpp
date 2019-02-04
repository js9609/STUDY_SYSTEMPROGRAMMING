#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

#include <fcntl.h>
#include <string.h>

#define ThreadNum 4

pthread_mutex_t mutex_lock;
pthread_cond_t sync_cond;

pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;

const char seps[] = "-.,()\";:{}'+@/<>[]!?#\n";
using namespace std;

struct Data
{
	int queuenum;
	int msgtype;
	int tid;
	char msg[256];
	int size;
	key_t key_id;
};

struct DocInfo
{
	int tnum;
	int start;
	int end;
	char fname[80];
	char regex[24];
};

void 
*Tfunction(void *data)
{
	struct Data lData;
	struct DocInfo *lDocInfo;
	FILE *fp;
	char line[256];
	int rtv;
	int bsize;
	int readn = 0;
	regex_t preg;
	char *tr;

	lData = *(struct Data *)(data);
	lDocInfo = (struct DocInfo *)lData.msg;
	
	bsize = lDocInfo->end - lDocInfo->start;
	printf("Debug Thread %d %d\n", lDocInfo->tnum, bsize);
	if((fp = fopen(lDocInfo->fname, "r"))==NULL)
	{
		perror("Fopen Error");
		exit(0);
	}

	lseek(fileno(fp), lDocInfo->start, SEEK_SET);
	rtv = regcomp(&preg, lDocInfo->regex, REG_EXTENDED|REG_NOSUB);

	pthread_mutex_lock(&mutex_lock);
	pthread_cond_signal(&sync_cond);
	pthread_mutex_unlock(&mutex_lock);

	if(rtv != 0)
	{
		regerror(rtv, &preg, lDocInfo->regex, REG_EXTENDED|REG_NOSUB);
		exit(0);
	}
	for(readn = 0; readn < bsize;)
	{
		if(fgets(line, 256, fp) == NULL)
			break;
		tr = strtok(line,seps);
		if(tr!=NULL)
		{
			if(regexec(&preg, tr, 0, NULL, 0) == 0)
			{
				lData.queuenum = 1;
				lData.msgtype = 1 <<1;
				lData.size = strlen(tr);
				sprintf(lData.msg, "%s", tr);

				if(msgsnd(lData.key_id, (void *)&lData, sizeof(lData), IPC_NOWAIT)<0)
				{
					perror("msg snd error");
					exit(0);
				}
			
			}
		}
		readn += strlen(line);
		sleep(1);
	}
}


int main(int argc, char **argv)
{
	struct Data lData;
	struct DocInfo lDocInfo;

	pthread_t p_thread[ThreadNum];

	int fd;
	char *fname;
	int fsize =0;
	int blocksize =0;
	struct stat fileinfo;
	char *regex;

	key_t key_id;
	int msgtype;

	fname = argv[1];
	regex = argv[2];
//메세지 큐 생성
	key_id = msgget((key_t)8888, IPC_CREAT|0666);
	if(key_id == -1)
	{
		perror("msgget error : ");
		exit(0);
	}
//파일 읽기
	fd = open(fname, O_RDONLY);
	if(fd < 0)
	{
		perror("Open file error ");
		return 1;
	}
	if(fstat(fd, &fileinfo)<0)
	{
		return 1;
	}
	fsize = fileinfo.st_size;
	printf("T Size is %d\n", fsize);

	blocksize = fsize/ThreadNum;

	for(int i =0; i< ThreadNum; i++)
	{
		lDocInfo.start = (i*blocksize);
		lDocInfo.end = lDocInfo.start+blocksize;
		sprintf(lDocInfo.fname, "%s", fname);
		sprintf(lDocInfo.regex, "%s", regex);
		lDocInfo.tnum = i;

		if(i == (ThreadNum -1))
			lDocInfo.end +=fsize%ThreadNum;
		lData.queuenum = 1;
		lData.msgtype - 1<<2;
		lData.size = sizeof(lDocInfo);
		lData.key_id = key_id;
		lData.tid = lDocInfo.tnum;
		memcpy((void*)&lData.msg, (void*)&lDocInfo, sizeof(lDocInfo));

		pthread_mutex_lock(&mutex_lock);
		pthread_create(&p_thread[i], NULL, &Tfunction, (void *)&lData);
		pthread_cond_wait(&sync_cond, &mutex_lock);
		pthread_mutex_unlock(&mutex_lock);
	}

	while(1)
	{
		if(msgrcv((int)key_id, (void*)&lData, sizeof(lData), (1>>1), 0) == -1)
			perror("msgrcv error: ");

		printf("Get Term %d %s\n", lData.tid, lData.msg);
	}
}
