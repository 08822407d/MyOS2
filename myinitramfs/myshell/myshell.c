#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "externs.h"

	#define PATH_MAX        4096	/* # chars in a path name including nul */
	#define SZ_1K			1024

#define	SYSTEM_REBOOT	(1UL << 0)
#define	SYSTEM_POWEROFF	(1UL << 1)

typedef struct builtincmd
{
	char *name;
	int (*function)(int,char**);
} builtincmd_s;

int cd_command(int argc, char **argv);
int ls_command(int argc, char **argv);
int pwd_command(int argc, char **argv);
int cat_command(int argc, char **argv);
int exec_command(int argc, char **argv);
int touch_command(int argc, char **argv);
int mkdir_command(int argc, char **argv);
int rmdir_command(int argc, char **argv);
int rm_command(int argc, char **argv);
int reboot_command(int argc, char **argv);
 
	char *	getcwd(char *, size_t);
	int		chdir(const char *);
	int		reboot(int);


builtincmd_s shell_internal_cmd[] = 
{
	{"cd",		cd_command},
	{"ls",		ls_command},
	{"pwd",		pwd_command},
	{"cat",		cat_command},
	{"touch",	touch_command},
	{"rm",		rm_command},
	{"mkdir",	mkdir_command},
	{"rmdir",	rmdir_command},
	{"exec",	exec_command},
	{"reboot",	reboot_command},
};

char *homedir = "/";
char current_dir[PATH_MAX];

int read_line(char *buf);
int parse_command(char * buf,int * argc,char ***argv);
void run_cmd(int index, int argc, char **argv);

int main(int argc, const char *argv[])
{
	printf("This is MyShell v0.01.\n");
	printf("You are login as root.\n");


	memset(current_dir, 0, PATH_MAX);
	getcwd(current_dir, PATH_MAX);

	char buf[SZ_1K];
	int index = -1;

	while (1)
	{
		printf("\n");
		printf("$ ");
		memset(buf, 0, SZ_1K);
		read_line(buf);
		printf("\n");

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

		switch (key)
		{
		case 0:
			continue;
		
		case '\n':
			return count;
		
		case '\b':
			if (count > 0)
			{
				buf[--count] = 0;
				printf("\b");
			}
			break;

		default:
			buf[count++] = key;
			printf("%c", key);
			break;
		}
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

int parse_command(char *buf, int *argc, char ***argv)
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
	memset(*argv, 0, sizeof(char **) * (*argc + 1));
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

	if (argc <= 1)
		path = homedir;
	else
		path = argv[1];

	/////.
	if(!strcmp(".", path))
		return 1;

	i = chdir(path);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
		getcwd(current_dir, PATH_MAX);
	}
	else
		printf("cd: %s: %s\n", strerror(errno), path);
	return 1;
}

int ls_command(int argc, char **argv)
{
	DIR* dir = NULL;
	struct dirent* buf = NULL;
	char *path = NULL;
	if (argc <=1)
		path = ".";
	else
		path = argv[1];

	dir = opendir(path);
	if (dir == NULL)
	{
		printf("ls: %s: %s\n", strerror(errno), path);
		return -1;
	}

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

int cat_command(int argc, char **argv)
{
	int len = 0;
	char *filename = NULL;
	int fd = 0;
	char *buf = NULL;
	int i = 0;

	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
	// printf("cat_command filename:%s\n", filename);

	fd = open(filename, O_RDONLY, 0);	
	if (fd < 0)
	{
		printf("ls: %s: %s\n", strerror(errno), filename);
		return -1;
	}
	i = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	buf = malloc(i + 1);
	memset(buf, 0, i + 1);
	len = read(fd, buf, i);
	// printf("length:%d\t%s\n", len, buf);
	printf("%s\n", buf);

	close(fd);
	return 1;
}

int exec_command(int argc, char **argv)
{
	// int err = 0;
	// int retval = 0;
	// int len = 0;
	// char * filename = NULL;
	// int i = 0;

	// err = fork();
	// if(err == 0)
	// {
	// 	printf("child process\n");
	// 	len = strlen(current_dir);
	// 	i = len + strlen(argv[1]);
	// 	filename = malloc(i+2);
	// 	memset(filename, 0, i + 2);
	// 	strcpy(filename, current_dir);
	// 	if(len > 1)
	// 		filename[len] = '/';
	// 	strcat(filename, argv[1]);

	// 	printf("exec_command filename:%s\n", filename);
	// 	for(i = 0; i < argc; i++)
	// 		printf("argv[%d]:%s\n", i, argv[i]);

	// 	execve(filename, argv, NULL);
	// 	exit(0);
	// }
	// else
	// {
	// 	printf("parent process childpid:%#d\n", err);
	// 	waitpid(errno, &retval, 0);
	// 	printf("parent process waitpid:%#018lx\n", retval);
	// }
	// return 1;
}

int touch_command(int argc, char **argv)
{
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;

	nmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	err = creat(argv[1], nmode);

	return 0;
}

int mkdir_command(int argc, char **argv)
{
	int err = 0, nmode = 0;
	if (argc != 2)
		return -1;

	nmode = (S_IRWXU | S_IRWXG | S_IRWXO);
	err = mkdir(argv[1], nmode);

	return 0;
}

int rmdir_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;
	
	char *dirname = argv[1];
	err = rmdir(dirname);
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);

	return err;
}

int rm_command(int argc, char **argv)
{
	int err = 0;
	if (argc != 2)
		return -1;

	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

	return err;
}

int reboot_command(int argc, char **argv)
{
	reboot(SYSTEM_REBOOT);
	return 1;
}