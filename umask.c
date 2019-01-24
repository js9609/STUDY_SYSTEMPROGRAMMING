#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd;

	umask(022);
	fd = open("test.txt", O_CREAT|O_WRONLY, 0666);
	close(fd);

}
