#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

	#define PATH_MAX        4096	/* # chars in a path name including nul */

void file_io_test(void);
void malloc_free_test(void);
void dirtest(void);

	char *	getcwd(char *, size_t);
	int		chdir(const char *);
	int		reboot(int);
	ssize_t	write(int, const void *, size_t);

ulong rdgsbase(void) {
	ulong gsbase;
	asm volatile(	"rdgsbase	%0		\t\n"
				:	"=r" (gsbase)
				:
				:	"memory");
	return gsbase;
}

ulong rdfsbase(void) {
	ulong fsbase;
	asm volatile(	"rdfsbase	%0		\t\n"
				:	"=r" (fsbase)
				:
				:	"memory");
	return fsbase;
}


int main(int argc, const char *argv[])
{
	printf("Welcome to MyOS2\n\n");

	// while (1);

	char prog_name[] = "/boot/sh";
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };


	ulong gsbase = rdgsbase();
	printf("gsbase in user space: %p\n", gsbase);

	int rv = fork();
	if (rv != 0)
	{
		// printf("parent task, %d\n", rv);
		while (1);
	}
	else
	{
		// printf("child task, %d\n", rv);
		execve(prog_name, args, envs);
		// malloc_free_test();
		// file_io_test();
		// dirtest();
	}
}

void file_io_test()
{
	char buf[512];

	// memset(buf, 0, 512);
	// int test_fd1 = open("/EFI/BOOT/BOOTX64.EFI", O_RDONLY);
	// if (test_fd1 >= 0)
	// {
	// 	read(test_fd1, buf, 512);
	// }
	// close(test_fd1);
	// printf("Read test 1 : %s\n", buf);

	memset(buf, 0, 512);
	int test_fd2 = open("/EFI/test/test.txt", O_RDONLY, 0);
	if (test_fd2 >= 0)
	{
		read(test_fd2, buf, 512);
	}
	close(test_fd2);
	printf("Read test 2 : %s\n", buf);

	char buf2[] = {"write test success."};
	int test_fd3 = open("/EFI/test/test.txt", O_RDWR | O_APPEND, 0);
	write(test_fd3, buf2, strlen(buf2));
	close(test_fd3);

	// int tty_fd = open("/dev/tty0", O_RDWR);
	// char buf3[10];
	// char buf4[] = "tty_write() test.\n";
	// write(tty_fd, buf4, strlen(buf4));
	// printf("tty_read() test, waiting kbd input :");
	// read(tty_fd, buf3, 9);
	// printf(" %s \n", buf3);
	// close(tty_fd);
}

void malloc_free_test()
{
	unsigned char * test1 = (char *)malloc(0x100);
	unsigned char * test2 = (char *)malloc(0x1000);
	unsigned char * test3 = (char *)malloc(0x10000);
	unsigned char * test4 = (char *)malloc(0x100000);

	free(test2);
	free(test3);
	free(test4);
	unsigned char * test5 = (char *)malloc(0x1);
	unsigned char * test6 = (char *)malloc(0x10);
	free(test1);
	free(test6);
	unsigned char * test7 = (char *)malloc(0x10);
	unsigned char * test8 = (char *)malloc(0x1);
	free(test5);
	free(test7);
	free(test8);
}

void dirtest()
{
	char pwd[PATH_MAX];
	getcwd(pwd, PATH_MAX - 1);
	printf("%s\n", pwd);
	chdir("/EFI/BOOT");
	getcwd(pwd, PATH_MAX - 1);
	printf("%s\n", pwd);

	DIR *dir = opendir("/EFI");
	// for (; ; )
	// {
	// 	struct dirent *dirent = readdir(dir);
	// 	if (dirent == NULL)
	// 		break;

	// 	printf("%s\n",dirent->d_name);
	// }
	closedir(dir);

	chdir("/");
}