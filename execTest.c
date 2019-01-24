#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
	printf("START\n");
	int ret = 	execl("/bin/ls", "ls", "-al", NULL);
	printf("%d\n", ret);
	printf("END\n");
}
