#include <unistd.h>
#include <utmp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

int get_current_pid(int login_pid);
char *get_current_procname(int proc_num);

int main(int argc, char **argv){
	struct utmp *utmpfp;
	struct utmp *myutmp;

	char *tty_name = NULL;
	char *host_ip;

	time_t the_time;
	struct tm *tm_ptr;

	setutent();

	// getutent()를 이용해서 utmp 정보를 얻어온다.

	while((utmpfp = getutent())!=NULL)
	{
		the_time = utmpfp->ut_time;
		tm_ptr = localtime(&the_time);
		sprintf(now_local_time, "%d/%02d/%02d %02d:%02d",
				tm_ptr->tm_year + 1900, tm_prt->tm_mon +1,
				tm_ptr->tm_mday, tm_ptr->tm_hour,
				tm_ptr->tm_min);
		host_ip = utmpfp->ut_host;
		if(strlen(host_ip_<1)
				host_ip = "-";
		if(utmpfp->ut_type == USER_PROCESS){
			printf("%-12 %-8s %-12s %s	%s\n",
					utmpfp->ut_user,
					utmpfp->ut_line,
					host_ip,
					now_local_time,
					get_current_procname(get_current_pid(utmpfp->ut_pid)));_
		}
		
	}
	return 1;


}
