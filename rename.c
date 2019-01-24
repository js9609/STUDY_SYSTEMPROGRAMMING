#include <stdio.h>
#include <errno.h>

int main(int argc , char **argv){
	int rtv;

	if(argc!=2){
		printf("Usage : ./rename filename\n");
		exit(0);
	}
	if(rename(argv[1], "renamedfile.txt")!=0){
		perror("file renamed error " );
		return 1;
	}
	return 0;
}
