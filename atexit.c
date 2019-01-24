#include <stdlib.h>
#include <stdio.h>

static void my_exit1(void);
static void my_exit2(void);

int main(void)
{
	if(atexit(my_exit2) != 0)
	{
		printf("cant register my_exit2");
		exit(1);
	}
	if(atexit(my_exit1) != 0)
	{
		printf("cant register my_exit1");
		exit(1);
	}
	if(atexit(my_exit1) != 0)
	{
		printf("cant register my_exit1");
		exit(1);
	}

	printf("main is done\n");
	return 0;

}

static void my_exit1(void)
{
	printf("first exit handler\n");
}
static void my_exit2(void)
{
	printf("second exit handler\n");

}
