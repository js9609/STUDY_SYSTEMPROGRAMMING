#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define ERRMSG "Device Not Zero\n"

int main(){
	int a,b;
	char buf[80];

	read(STDIN, buf, sizeof(buf));
	a = atoi(buf);

	
	read(STDIN, buf, sizeof(buf));
	b = atoi(buf);

	if( b == 0 ){
		write(STDERR, ERRMSG, strlen(ERRMSG));
		return 1;
	}

	sprintf(buf, "%d / %d = %d\n", a,b,(int)(a/b));

	write(STDOUT, buf, strlen(buf));
	return 0;
}
