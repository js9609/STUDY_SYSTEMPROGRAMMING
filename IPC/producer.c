#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct msgbuf
{
	long msgtype;
	char mtext[256];
	char myname[16];
	int	 seq;
};

int main()
{
	key_t key_id;
	int i;
	struct msgbuf mybuf, rcvbuf;

	key_id = msgget((key_t)1234, IPC_CREAT|0666);
	if(key_id == -1)
	{
		perror("msgget error : ");
		exit(0);
	}

	printf("Key is %d\n", key_id);

	//Initializing the Struture of Message
	memset(mybuf.mtext, 0x00, 256);
	memset(mybuf.myname, 0x00, 16);
	memcpy(mybuf.mtext, "Hello World 4", 13);
	memcpy(mybuf.myname, "jinsun", 6);
	mybuf.seq = 0;
	i=0;

	while(1)
	{
		if(i % 2 == 0 )
			mybuf.msgtype = 4;
		else
			mybuf.msgtype = 3;
		mybuf.seq = i;

		if(msgsnd(key_id, (void *)&mybuf, sizeof(struct msgbuf), IPC_NOWAIT) ==-1)
		{
			perror("msgsnd error : ");
			exit(0);
		}
		printf("send %d\n",i);
		i++;
		sleep(1);
	}
	printf("%ld \n", rcvbuf.msgtype);
	printf("%s \n", rcvbuf.mtext);
	printf("%s \n", rcvbuf.myname);
	exit(0);
}
