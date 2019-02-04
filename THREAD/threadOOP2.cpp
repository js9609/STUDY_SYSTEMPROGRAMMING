#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <map>

using namespace std;

#define MAXLEN 256

map<int, pthread_t> Cinfo;

class tClass{
	private:
		pthread_t pt;
		int sockfd;
		char *buf;

		static void* callThread(void *arg);

		static void clean_up(void *arg){
			delete(tClass *)arg;
		}
	public:
		void tFunc(void)
		{
			int readn;
			buf = (char *)malloc(MAXLEN);
			pthread_cleanup_push(clean_up, (void*)this);
			while(1)
			{
				memset(buf, 0x00, MAXLEN);
				readn = read(sockfd, buf, MAXLEN);
				if(readn <= 0)
				{
					perror("Socket Read Error: ");
					break;
				}
				write(sockfd, buf, strlen(buf));
			}
			pthread_exit((void*)NULL);

			pthread_cleanup_pop(0);
		}
		
		tClass(int sfd){
			sockfd = sfd;
			buf = NULL;
			pt =0;
			pthread_attr_t attr;

			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			//이 attr을 이용하여 생성된 스레드는 detach상태로 생성됨을 의미
			pthread_create(&pt, &attr, &callThread, this);
		}

		pthread_t tClassId(){
			return pt;
		}

		~tClass(void){
			map<int, pthread_t>::iterator mi;
			if(buf)//buf가 존재한다면
				free(buf);
			if(sockfd){
				mi = Cinfo.find(sockfd);
				if(mi!=Cinfo.end())
					Cinfo.erase(mi);
				close(sockfd);
			}
			printf("Thread Close\n");
		}
};

void* tClass::callThread(void* arg)
{
	tClass* tp;
	tp = (tClass*)arg;
	tp->tFunc();
}
int main(int argc, char **argv)
{
	pthread_t pthread;
	struct sockaddr_in clientaddr, serveraddr, myaddr;
	int maxfd = 0;
	fd_set fd_w;
	struct timeval timeout;
	
	class tClass *ClientClass;

	int server_sockfd, client_sockfd, client_len;

	if(argc != 2)
	{
		printf("Usage : %s [PORT]\n", argv[0]);
		return 1;
	}

	if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket error: ");
		return 1;
	}

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
	{
		perror("bind error : ");
		return 1;
	}
	if(listen(server_sockfd, 5)==-1)
	{
		perror("listen error : ");
		return 1;
	}
	client_len = sizeof(clientaddr);

	char buf[MAXLEN];

	while(1){
		FD_ZERO(&fd_w);
		FD_SET(0, &fd_w);
		FD_SET(server_sockfd, &fd_w);

		select(server_sockfd+1, &fd_w, (fd_set*)0, (fd_set*)0, NULL);
		if(FD_ISSET(server_sockfd, &fd_w)) // 서버로 입력 들어온것
		{
			client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, (socklen_t*)&client_len);
			ClientClass = new tClass(client_sockfd);
			Cinfo[client_sockfd] = ClientClass->tClassId();
		}
		else if(FD_ISSET(0, &fd_w)) // 표준 입력으로 입력 들어온것
			{
			memset(buf, 0x00, MAXLEN);
			read(0,buf,MAXLEN);
			if(strncmp(buf, "show", 4)==0) // show
			{
				map<int,pthread_t>::iterator mi;
				mi = Cinfo.begin();
				printf("NUM\tThread ID\tClient Address\n");
				while(mi != Cinfo.end())
				{
					getsockname(mi->first, (struct sockaddr *)&myaddr, (socklen_t *)&client_len);
					printf("%d\t%lu\t%s\n", mi->first, mi->second, inet_ntoa(myaddr.sin_addr));
					*mi++;				
				}
				printf("====================\n");
			}
			if(strncmp(buf, "kill", 4) == 0){
				map<int, pthread_t>::iterator mi;
				mi = Cinfo.find(atoi(buf+5));
				if(mi == Cinfo.end())
				{
					printf("Not Found Thread\n");
				}
				else
				{
					printf("Kill Thread %lu\n", mi->second);
					pthread_cancel(mi->second);
				}
			}
				
		}
		else
		{
			printf("Unknown\n");
		}
	
	
	
	}
}
