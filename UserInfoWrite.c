#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

struct userInfo
{
  char name[28];
  int age;
  int sex;
  char hobby[28];
};

void makeUserInfo(struct userInfo *uinfo,
  char *name, // �̸�
  int age,    // ����
  int sex,    // �� (��: 0, ��: 1)
  char *hobby) // ���
{
  memset((void *)uinfo, 0x00, sizeof(struct userInfo));
  strcpy(uinfo->name, name);
  uinfo->age = age;
  uinfo->sex = sex;
  strcpy(uinfo->hobby, hobby);
}

int main()
{
  int fd;
  struct userInfo myAddrBook;
  fd = open("hello.txt", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
  if (fd < 0)
  {
    perror("file open error");
    return 1;
  }

  makeUserInfo((void *)&myAddrBook, "yundream", 19, 0, "���α׷���");
  write(fd, (void *)&myAddrBook, sizeof(myAddrBook));

  makeUserInfo((void *)&myAddrBook, "hello", 22, 1, "����");
  write(fd, (void *)&myAddrBook, sizeof(myAddrBook));

  makeUserInfo((void *)&myAddrBook, "�����", 33, 1, "���");
  write(fd, (void *)&myAddrBook, sizeof(myAddrBook));

  close(fd);
  return 0;
}
