#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
using namespace std;
#include <iostream>
int main(int argc, char **argv)
{
	FILE *fp;
	char buf[80];
	char info[20];
	int my_pid;
	int pid;
	int mypipe[2];
	vector<string> proc_info;
	fp = fopen("config.cfg", "r");

	while(fgets(buf,80,fp)!=NULL)
	{
		buf[strlen(buf)-1] = 0x00;
		proc_info.push_back(buf);
	}
	fclose(fp);

	my_pid=getpid();
	pipe(mypipe);
	for(int i =0; i <proc_info.size(); i++)
	{
		if(getpid() != my_pid)
			exit(0);
		pid = fork();
		if(pid == 0)
		{
			dup2(mypipe[1], 1);
			close(mypipe[0]); // close read
			close(mypipe[1]); // close write
			execl(proc_info[i].c_str(), proc_info[i].c_str(), NULL);
		}
		read(mypipe[0], info, 20);
		cout << "exec " << info << endl;
	}
	pause();
}
