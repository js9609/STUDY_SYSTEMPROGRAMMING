#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	int opt;
	if(argc != 4)
	{
		printf("Usage: ./link[s][h] [원본파일] [복사파일]\n");
		return -1;
	}

	if(access(argv[3], F_OK) ==0)
	{
		printf("이미 파일이 존재합니다\n");
		return -1;
	}
	while((opt == getopt(argc, argv, "sh"))!=-1)
	{
		switch(opt)
		{
			case 's':
				printf("심볼릭링크\n");
				symlink(argv[2], argv[3]);
				break;
		
			case 'h':
				printf("하드링크\n");
				link(argv[2], argv[3]);
				break;
		}
	}
}
