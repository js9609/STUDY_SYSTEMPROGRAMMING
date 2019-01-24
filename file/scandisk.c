#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



long int total_size = 0;
int indent= 0;

void dir_parser(char *wd)
{
	struct dirent **items;
	int nitems, i, j;
	struct stat fstat;
	char per;

	if(chdir(wd)<0)
	{
		perror("chdir ");
		exit(1);
	}

	nitems = scandir(".", &items, NULL, alphasort);
	
	for(i =0; i<nitems; i++)
	{
		struct stat fstat;

		if((!strcmp(items[i]->d_name, "."))|| (!strcmp(items[i]->d_name, "..")))
		{
			continue;
		}

		lstat(items[i]->d_name, &fstat);

		for(j=0;j<indent;j++)
		{
			printf("\t");
		}
		printf("%s\t%d\n", items[i]->d_name, fstat.st_size);
		total_size += fstat.st_size;

		if(S_ISDIR(fstat.st_mode) && S_ISLNK(fstat.st_mode))
		{
			indent++;
			dir_parser(items[i]->d_name);
		}
	}
	indent --;
	chdir("..");

}

int main(int argc, char **argv)
{
	//memset(, 0x00, 255);
	dir_parser(argv[1]);
	printf("size is %d byte\n", total_size);

}
