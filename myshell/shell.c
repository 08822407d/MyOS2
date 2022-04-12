#include <sys/fcntl.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <dirent.h>

#include <uapi/limits.h>

#include "externs.h"

typedef struct builtincmd
{
	char *name;
	int (*function)(int,char**);
} builtincmd_s;

int cd_command(int argc, char **argv);
int ls_command(int argc, char **argv);
int pwd_command(int argc, char **argv);

builtincmd_s shell_internal_cmd[] = 
{
	{"cd",		cd_command},
	{"ls",		ls_command},
	{"pwd",		pwd_command},
	// {"cat",		cat_command},
	// {"touch",	touch_command},
	// {"rm",		rm_command},
	// {"mkdir",	mkdir_command},
	// {"rmdir",	rmdir_command},
	// {"exec",	exec_command},
	// {"reboot",	reboot_command},
};

char current_dir[PATH_MAX];

int read_line(char *buf);
int parse_command(char * buf,int * argc,char ***argv);
void run_cmd(int index, int argc, char **argv);

int main(int argc, const char *argv[])
{
	printf("Welcome to MyOS2!\n");
	printf("This is MyShell v0.01.\n");
	printf("You are login as root.\n\n");

	// reader_loop();

	memset(current_dir, 0, PATH_MAX);
	getcwd(current_dir, PATH_MAX);

	char buf[SZ_1K];
	int index = -1;

	while (1)
	{
		printf("$ ");
		memset(buf, 0, SZ_1K);
		read_line(buf);
		printf(" \n");

		int __argc = 0;
		char ** __argv = NULL;
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
		else
			run_cmd(index, __argc, __argv);	//argc,argv
	}
}

int read_line(char *buf)
{
	char key = 0;
	int count = 0;

	while(1)
	{
		key = fgetc(stdin);
		if(key == '\n')
		{
			return count;
		}
		else if(key)
		{
			buf[count++] = key;
			printf("%c", key);
		}			
		else
			continue;
	}
}

void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}

int find_cmd(char *cmd)
{
	int i = 0;
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
		if(!strcmp(cmd,shell_internal_cmd[i].name))
			return i;
	return -1;
}

int parse_command(char * buf,int * argc,char ***argv)
{
	int i = 0;
	int j = 0;

	while(buf[j] == ' ')
		j++;

	for(i = j;i<256;i++)
	{
		if(!buf[i])
			break;
		if(buf[i] != ' ' && (buf[i+1] == ' ' || buf[i+1] == '\0'))
			(*argc)++;
	}
	if(!*argc)
		return -1;

	*argv = (char **)malloc(sizeof(char **) * (*argc + 1));
	memset(*argv,0,sizeof(char **) * (*argc + 1));
	for(i = 0; i < *argc && j < 256; i++)
	{
		*((*argv)+i) = &buf[j];

		while(buf[j] && buf[j] != ' ')
			j++;
		buf[j++] = '\0';
		while(buf[j] == ' ')
			j++;
	}

	return find_cmd(**argv);
}

int cd_command(int argc, char **argv)
{
	char *path = NULL;
	int len = 0;
	int i = 0;

	/////.
	if(!strcmp(".", argv[1]))
		return 1;

	i = chdir(argv[1]);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
	}
	else
		printf("cd: %s: %s\n", strerror(i), argv[1]);
	return 1;
}

int ls_command(int argc, char **argv)
{
	DIR* dir = NULL;
	linux_dirent64_s * buf = NULL;

	dir = opendir(current_dir);

	buf = (linux_dirent64_s *)malloc(256);
	while(1)
	{
		buf = readdir(dir);
		if(buf == NULL)
			break;
		if (strcmp(buf->d_name, ".") != 0 &&
			strcmp(buf->d_name, "..") != 0)
			printf("%s\t", buf->d_name);
	}
	printf("\n");
	closedir(dir);
	return 1;
}

int pwd_command(int argc, char **argv)
{
	if(current_dir)
		printf("%s\n", current_dir);
	return 1;
}