#include "proc.h"
#include <iostream>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
Proc::Proc(){}
Proc::~Proc(){}

void Proc::MakeProcInfo()
{
	DIR *directory;
	struct dirent *entry = NULL;
	char proc_file[40];
	vector<string> procinfo;
	process_info lprocess_info;
	struct stat lstat;

	ProcList.clear();

	if((directory == opendir("/proc"))==NULL)
	{
		perror("opendir error : ");
		exit(0);
	}
	while((entry = readdir(directory))!=NULL)
	{
		if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0)
		{
			sprintf(proc_file, "/proc/%s/stat", entry->d_name);

			if(access(proc_file, F_OK)!=0)
				continue;

			if(IsDigit(entry->d_name))
			{
				struct passwd *upasswd;
				stat(proc_file, &lstat);
				lprocess_info.process = ProcParser(proc_file);
				upasswd = getpwuid(lstat.st_uid);
				strncpy(lprocess_info.username, upasswd->pw_name, 32);
				if(atoi(lprocess_info.process[0].c_str())==atoi(entry->d_name))
				{
					ProcList.push_back(lprocess_info);
				}
			}
			else{}

		
		}
	}
}

Plist Proc::GetProcList()
{
	return ProcList;
}

int Proc::ProcNum()
{
	return ProcList.size();
}

vector<string> Proc::ProcParser(char *proc_file)
{
	FILE *fp;
	char buf[512] = {0x00,};
	char field[80];
	int index = 0;
	unsigned int i,j = 0;
	vector<string> vproc;

	fp = fopen(proc_file, "r");
	if(fp == NULL)
	{
		perror("error : ");
		exit(0);
	}

	memset(field, 0x00 ,80);
	fgets(buf, 511 ,fp);
	for(i = 0; i<strlen(buf); i++)
	{
		if(buf[i] != ' ' && i != (strlen(buf)-1))
			field[j++] = buf[i];
		else{
			if(index == 1)
			{
				field[strlen(field)-1] = 0x00;
				vproc.push_back(field+1);
			}
			else
				vproc.push_back(field);
			memset(field, 0x00, 80);
			j=0;
			index ++;
		}
	}
	fclose(fp);
	return vproc;
}

int Proc::IsDigit(char *str)
{
	int i;
	for(i = 0; i < strlen(str); i++)
	{
		if(isdigit(str[i]) == 0)
			return 0;
	}
	return 1;
}
