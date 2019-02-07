#include <map>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <iostream>

using namespace std; 
int main(int argc, char **argv)
{
	FILE *fp;
	char buf[80];
	char info[20];
	char sub_pids[20];
	int sub_pid;
	int my_pid;
	int pid;
	int mypipe[2];
	map<int, string> mpid;
	vector<string> proc_info;

	fp = fopen("config.cfg", "r");

	while(fgets(buf,80,fp)!=NULL)
	{
		buf[strlen(buf) - 1] = 0x00;
		proc_info.push_back(buf);
	}
	fclose(fp);

	my_pid = getpid();
	pipe(mypipe);
	for(int i = 0; i<proc_info.size(); i++)
	{
		if(getpid()!=my_pid)
			exit(0);
		pid = fork();
		if(pid == 0)
		{
			dup2(mypipe[1], 1);
			close(mypipe[0]);
			close(mypipe[1]);
			execl(proc_info[i].c_str(), proc_info[i].c_str(),NULL);
		}
		read(mypipe[0], (void*)&sub_pids, 20);
		sub_pid = atoi(sub_pids);
		mpid[sub_pid] = proc_info[i];
		printf("subpid %d\n", sub_pid);
	}
	char proc_path[20];
	char exec_path[80];
	while(1)
	{
		memset(proc_path, 0x00, 20);
		memset(exec_path, 0x00, 80);
		map<int, string>::iterator mi = mpid.begin();
		while(mi != mpid.end())
		{
			sprintf(proc_path, "/proc/%d/exe", mi->first);
//			printf("/proc/%d/exe", mi->first);
			if(readlink(proc_path, exec_path, 80) > 0)
			{
				if(mi->second == exec_path)
					cout << "ok Process " << " : " << mi->first << endl;
				else
				{
					cout << mi->second << " Process Error "<< endl;
					mpid.erase(mi);
				}
			}
			else
			{
				cout <<mi->second<<"Process error"<<endl;
				mpid.erase(mi);
			}
			*mi++;
		}

		cout << " ============= " << endl;
		sleep(1);
	}
	pause();
}
