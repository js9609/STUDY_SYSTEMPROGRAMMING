#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	int fd;
	int val;
	fd = open("exec_copy.txt", O_CREAT);

	execl("~/Desktop/JinSun/SystemProgramming", "./a.out",0);
	// 다른 프로그램을 실행하고 자신은 종료
	printf("ERROR");
}
