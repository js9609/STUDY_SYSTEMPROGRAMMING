#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int fd;
	int val;
	fd = open("exec_copy.txt", O_CREAT);

	execl("/home/jinsun/Desktop/SystemProgramming/file/loop", "./loop", NULL);
}
