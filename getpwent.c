#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char **argv){
	char *name;
	if (argc != 2){
		printf("Usage : %s username\n", argv[0]);
		return 1;
	}
	name = argv[1];

	struct passwd *pass_info = NULL;

	while((pass_info = getpwent())!= NULL){
		if(strncmp(name ,pass_info->pw_name, strlen(name)) == 0){
			printf("%12s uid(%d) gid(%d) home(%s)\n", 
					pass_info->pw_name,
					pass_info->pw_uid,
					pass_info->pw_gid,
					pass_info->pw_dir);
			return 0;
		}
	}
	printf("Can not find user : %s \n", name);

}
