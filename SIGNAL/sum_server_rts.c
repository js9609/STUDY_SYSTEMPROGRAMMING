#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <fcntl.h>

//Data communication With Client
struct data{
	int a;
	int b;
	int sum;
};

int setup_sigio(int fd)
{
	if(fcntl(fd, F_SETFL, O_RDWR|O_NONBLOCK|O_ASYNC)<0)
	{
		printf("Couldn't setup nonblocking io %d\n", fd);
		return -1;
	}
	if(fcntl(fd, F_SETSIG, SIGRTMIN)<0)
	{
		printf("Couldn't set signal %d on %d\n", SIGRTMIN, fd);
		return -1;
	}
	if(fcntl(fd, F_SETOWN, getpid())<0)
	{
		printf("Couldn't set owner %d on %d\n", getpid(), fd);
		return -1;
	}
	return 0;
}

void setup_sigio_listeners(int fd)
{
	if(setup_sigio(fd)!=0)
	{
		printf("setup_sigio_listeners error : %d\n", fd);
		exit(0);
	}
	else
	{}
}

int get_listeners_fd(int port)
{
	int sockfd;
	int clilen;
	int state;
	struct sockaddr_in serveraddr;

	clilen = sizeof(serveraddr);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		printf("Socket Create Error \n");
		exit(0);
	}
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	state = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(state == -1)
	{
		perror("bind error : ");
		exit(0);
	}
	return sockfd;
}

int main()
{
	sigset_t set;
	siginfo_t si;
	int ret;
	int resockfd;
	int sockfd1, sockfd2;
	struct data add_data;
	struct sockaddr_in clientaddr;
	int clilen;

	sigemptyset(&set);
	sigaddset(&set, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &set, NULL);

	sockfd1 = get_listeners_fd(1234);
	setup_sigio_listeners(sockfd1);

	sockfd2 = get_listeners_fd(1235);
	setup_sigio_listeners(sockfd2);

	while(1)
	{
		clilen = sizeof(clientaddr);

		printf("SIG WAIT\n");
		ret = sigwaitinfo(&set, &si);

		if(ret == SIGRTMIN)
		{
			printf("====================================\n");
			printf("RTS I/O socket %d\n", si.si_fd);
			printf("RTS I/O revents %d\n", si.si_band);
			printf("===================================\n");
			resockfd = si.si_fd;
			recvfrom(resockfd, (void *)&add_data, sizeof(add_data),0,(struct sockaddr *)&clientaddr, &clilen);
			add_data.sum = add_data.a+add_data.b;
			sendto(resockfd, (void *)&add_data, sizeof(add_data), 0, (struct sockaddr *)&clientaddr, clilen);
		}
			
	
	
	}


}
