#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv){

	if(chmod(argv[1], S_IRUSR|S_IWUSR)<0){
		perror("chmod error :");
		exit(0);
	}
}
