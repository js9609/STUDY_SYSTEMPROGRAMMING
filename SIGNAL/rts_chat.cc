#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <map>
#include <string>
#include <iostream>
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <fcntl.h>
#define SOCK_MAX_NUM 1024

using namespace std;

static int listen_sockfd;

map<int, struct sockadddr_in> msoinfo;
map<int, struct sockaddr_in>::iterator mi;

void do_sigio(int signo)
{
	printf("SIGIO : RTS signal queue overflow\n");
}

int init_signal_handler()
{
	struct sigaction sigact;

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_restorer = NULL;
	sigact.sa_handler = do_sigio;
	if(sigaction(SIGIO, &sigact, NULL)<0)
	{
		printf("sigio handler error\n");
		exit(0);
	}
}

int setup_sigio(int fd)
{
	if(fcntl(fd, F_SETFL, O_RDWR|O_NONBLOCK|O_ASYNC)<0)
	{
		printf("Nonblocking error \n");
		return -1;
	}
	if(fcntl(fd, F_SETSIG, SIGRTMIN)<0)
	{
		printf("Set signal error %d on %d\n", SIGRTMIN, fd);
		return -1;
	}
	if(fcntl(fd, F_SETOWN, getpid())<0)
	{
		printf("Owner Setting error %d on %d\n", getpid(), fd);
		return -1;
	}
	return 0;
}

void setup_sigio_listeners(int fd)
{
	if(setup_sigio(fd)!=0)
	{
		printf("setup sigio error : %d\n", fd);
		exit(0);
	}
}
int get_listener_fd()
{
	int clien;
	int state;
	struct sockaddr_in serveraddr;
	clien = sizeof(serveraddr);
	listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sockfd<0)
	{
		printf("Socket create error\n");
		return -1;
	}
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(1234);

	state = bind(listen_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	if(state < 0 )
	{
		printf("Bind Error \n");
		return -1;
	}
	state = listen(listen_sockfd, 5);
	if(state < 0)
	{
		printf("Listen Error\n");
		return -1;
	}
	printf("Listen Socket Create %d\n", listen_sockfd);
	return setup_sigio(listen_sockfd);

}
int get_connect_fd()
{
	int sockfd;
	socklen_t clien;
	struct sockaddr_in clientaddr;
	clien = sizeof(clientaddr);
	sockfd = accept(listen_sockfd, (struct sockaddr *)&clientaddr, &clien);
	if(sockfd < 0)
	{
		printf("Accept error \n");
		return -1;
	}
	msoinfo[sockfd] = clientaddr;
	setup_sigio_listeners(sockfd);
	printf("Accept %d\n", sockfd);
}
int chat_close(int fd)
{
	char buf[256] = {0x00,};
	if(read(fd, buf, 255)<=0)
	{
		mi = msoinfo.begin();
		while(mi!=msoinfo.end())
		{
			sprintf(buf, "Disconnect : %s\n", inet_ntoa(mi->second.sin_addr));
			if(mi->first != fd)
				write(mi->first, buf, 256);
			*mi++;
		}
		msoinfo.erase(msoinfo.find(fd));
		close(fd);
	}
}
void chat_info(int fd)
{
	char buf[256] = {0x00, };
	int i = 1;
	mi = msoinfo.begin();

	while(*mi != *msoinfo.end())
	{
		sprintf(buf, "%d : %s (%d)\n", i, inet_ntoa(mi->second.sin_addr),mi->second.sin_port);
		write(fd, buf, 256);
		i++;
		*mi++;
	}
}
int send_chat_msg(int fd)
{
	char buf[256] = {0x00,};
	if(read(fd,buf,255)<=0)
	{
		chat_close(fd);
	}
	else
	{
		if(strncasecmp(buf, "/quit", 5)==0)
		{
			chat_close(fd);
		}
		else if(strncasecmp(buf, "/info", 5) == 0){
			chat_info(fd);
		}
		else
		{
			mi = msoinfo.begin();
			while(mi != msoinfo.end())
			{
				if(mi->first != fd)
					write(mi->first, buf, 256);
				*mi++;
			}
		}
	}
}

int main()
{
	siginfo_t si;
	sigset_t set;
	int clilen;
	int ret;

	sigemptyset(&set);
	sigaddset(&set, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &set, NULL);

	init_signal_handler();

	if(get_listener_fd() < 0)
	{
		printf("Socket Create Error\n");
		exit(0);
	}

	while(1)
	{
		printf("sigwait\n");

		ret = sigwaitinfo(&set, &si);
		if(ret == SIGRTMIN)
		{
			printf("ok signal %d : %d\n", si.si_fd, si.si_code);
			
			if(si.si_fd == listen_sockfd)
			{
				get_connect_fd();
			}
			else
			{
				send_chat_msg(si.si_fd);
			}
		
		}
	}
}
