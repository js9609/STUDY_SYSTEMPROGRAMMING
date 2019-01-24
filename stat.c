#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char **argv)
{
	int return_stat;
	char *file_name;
	struct stat file_info;

	mode_t file_mode;

	if (argc != 2){
		printf("Usage : ./file_info [file_name]\n");
		exit(0);
	}
	file_name = argv[1];

	if ((return_stat = stat(file_name, &file_info)) == -1)
	{
		perror("Error : ");
		exit(0);
	}

	file_mode = file_info.st_mode;
	printf("�����̸� : %s \n", file_name);
	printf("====================================\n");
	printf("���� Ÿ�� : ");
	if (S_ISREG(file_mode))
	{
		printf("��������\n");
	}
	else if(S_ISLNK(file_mode))
	{
		printf("�ɺ��� ��ũ\n");
	}
	else if(S_ISDIR(file_mode))
	{
		printf("���丮\n");
	}
	else if(S_ISCHR(file_mode))
	{
		printf("���� ����̽�\n");
	}
	else if(S_ISBLK(file_mode))
	{
		printf("�� ����̽�\n");
	}
	else if(S_ISFIFO(file_mode))
	{
		printf("FIFO\n");
	}
	else if(S_ISSOCK(file_mode))
	{
		printf("����\n");
	}
	printf("OWNDER			: %d\n", file_info.st_uid);
	printf("GROUP			: %d\n", file_info.st_gid);
	printf("dev				: %d\n", file_info.st_dev);
	printf("inode			: %d\n", file_info.st_ino);
	printf("FILE SIZE IS	: %d\n", file_info.st_size);
	printf("������ ���� �ð�: %d\n", file_info.st_atime);
	printf("������ ����		: %d\n", file_info.st_mtime);
	printf("�ϵ帵ũ�� ���� �� : %d\n", file_info.st_nlink);
}
//
