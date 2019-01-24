#include <stdio.h>
#include <stdlib.h>

int main()
{
	char name[255];
	int i;
	mkstemp(name);
	printf("%s\n",name);
}
