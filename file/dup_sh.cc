#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <algorithm>
#include <fcntl.h>
#include <string.h>
using namespace std;

vector<string> makearg(string str);

int main()
{

	char *buf;
	vector<string> arg;
	int pid = 0;
	int fd;
	int redirect = 0;

	signal(SIGCHLD, SIG_IGN);
	buf = (char *)malloc(sizeof(char)*255);
	while(1)
	{
	
		redirect =0;
		memset(buf, 0x00, 255);

		write(1, "# ", 3);
		if(read(0,buf,255)>1)
		{
			buf[strlen(buf) -1] = 0x00;
			if(strncmp(buf, "exit", 4) ==0)
				exit(0);

			if(strstr(buf, "<") != NULL)
			{
				arg = makearg(buf);
				fd = open(arg[1].c_str(), O_RDONLY);
				if(fd < 0 )
				{
					fprintf(stderr, "file not found : %s\n", arg[1].c_str());
					exit(0);
				}
				redirect = 1;
				strcpy(buf, arg[0].c_str());
			}
		
		pid = fork();
		if( pid == 0)
		{
			if(redirect)
				dup2(fd, 0);

			execlp(buf, buf, NULL);
		}
		else if(pid > 0)
		{
			if(redirect)
				close(fd);
			wait(0);
		}
		}
	}
}

vector<string> makearg(string str)
{
	int num;
	vector<string> return_str;
	string::iterator mi;
	mi = str.begin();
	while(mi != str.end())
	{
		if(*mi == ' ')
			str.erase(mi);
		*mi++;
	}
	num = str.find("<");
	return_str.push_back(str.substr(0,num));
	return_str.push_back(str.substr(num+1));

	return return_str;
}
