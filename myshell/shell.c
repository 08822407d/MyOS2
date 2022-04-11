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

int main(/* int argc, const char *argv[] */)
{
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

		int argc = 0;
		char ** argv = NULL;
		index = parse_command(buf, &argc, &argv);
		if(index < 0)
			printf("Input Error,No Command Found!\n");
		else
			run_cmd(index, argc, argv);	//argc,argv
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
	// printf("run_command %s\n", shell_internal_cmd[index].name);
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
	// printf("parse_command argc:%d\n",*argc);
	if(!*argc)
		return -1;

	*argv = (char **)malloc(sizeof(char **) * (*argc + 1));
	memset(*argv,0,sizeof(char **) * (*argc + 1));
	// printf("parse_command argv\n");	
	for(i = 0; i < *argc && j < 256; i++)
	{
		*((*argv)+i) = &buf[j];

		while(buf[j] && buf[j] != ' ')
			j++;
		buf[j++] = '\0';
		while(buf[j] == ' ')
			j++;
		// printf("%s\n",(*argv)[i]);
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

	////..
	// if(!strcmp("..", argv[1]))
	// {
	// 	if(!strcmp("/", current_dir))
	// 		return 1;
	// 	for(i = len-1;i > 1;i--)
	// 		if(current_dir[i] == '/')
	// 			break;
	// 	current_dir[i] = '\0';
	// 	// printf("pwd switch to %s\n", current_dir);
	// 	return 1;
	// }

	////others
	// i = len + strlen(argv[1]);
	// path = malloc(i + 2);
	// memset(path,0,i + 2);
	// strcpy(path,current_dir);
	// if(len > 1)	
	// 	path[len] = '/';
	// strcat(path,argv[1]);
	// printf("cd_command :%s\n", path);

	i = chdir(argv[1]);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
	}
	else
		printf("Can`t Goto Dir %s\n",argv[1]);
	// printf("pwd switch to %s\n",current_dir);
	return 1;
}

int ls_command(int argc, char **argv)
{
	// struct DIR* dir = NULL;
	// struct dirent * buf = NULL;

	// dir = opendir(current_dir);
	// printf("ls_command opendir:%d\n",dir->fd);

	// buf = (struct dirent *)malloc(256);
	// while(1)
	// {
	// 	buf = readdir(dir);
	// 	if(buf == NULL)
	// 		break;
	// 	printf("ls_command readdir len:%d,name:%s\n",buf->d_namelen,buf->d_name);
	// }
	// closedir(dir);
	// return 1;
}

int pwd_command(int argc, char **argv)
{
	if(current_dir)
		printf("%s\n", current_dir);
	return 1;
}