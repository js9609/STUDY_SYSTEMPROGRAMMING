#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
	setenv("TEST", "JINSUN", 1);
	execl("/bin/bash", "bash", NULL);
}
