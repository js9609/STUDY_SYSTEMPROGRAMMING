#include "qps.h"
#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>

using namespace std;

int viewProc()
{
	Proc *mproc;
	unsigned i;
	int pcpu;
	int seconds = 0;

	Plist ProcList;
	mproc = new Proc;
	mproc->MakeProcInfo();
	
	ProcList = mproc->GetProcList();

	int total_cpu = 0;

	seconds = uptime();
	printf("%-10s %7s %7s %2s %16s %4s %9s\n", "USER", "PID", "PPID", "ST", "NAME", "CPU", "VMEM");
	printf("=====================================================\n");
	for( i = 0; i< mproc->ProcNum(); i++)
	{
		pcpu = getcputime(atol(ProcList[i].process[13].c_str()), atol(ProcList[i].process[14].c_str()), atol(ProcList[i].process[21].c_str()), seconds);

		 printf("%-10s %7s %7s %2s %16s %2d.%d %9s\n", ProcList[i].username, 
                                    ProcList[i].process[0].c_str(),
                                    ProcList[i].process[3].c_str(),
                                    ProcList[i].process[2].c_str(), 
                                    ProcList[i].process[1].c_str(), pcpu/10, pcpu%10,
                                    ProcList[i].process[22].c_str());
	
	}
	return 1;
}


int getcputime(ulong utime, ulong stime, ulong starttime, int seconds)
{
	unsigned long long total_time;
	int pcpu = 0 ;
	total_time = utime + stime;
	seconds = seconds - (int)(starttime/100.);

	if(seconds)
	{
		pcpu = (int)(total_time * 1000ULL/100.)/seconds;
	}
	return pcpu;
}

int uptime()
{
	FILE *fp;
	char buf[36];
	double stime;
	double idletime;

	if((fp = fopen("/proc/uptime", "r"))==NULL)
	{
		perror("fopen error : ");
		exit(0);
	}
	fgets(buf, 36, fp);
	sscanf(buf, "%lf %lf", &stime, &idletime);
	fclose(fp);

	return (int)stime;

}

