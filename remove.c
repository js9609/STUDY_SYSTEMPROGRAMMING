#include <stdio.h>
#include <errno.h>

int main(int argc , char **argv){
	int rtv;

	if(argc != 2){
		printf("Usage : ./remove filename\n");
		exit(0);
	}
	if(remove(argv[1])!=0){
		perror("file remove error ");
		return 1;
	}
	return 0;
}
