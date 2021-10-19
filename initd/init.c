#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/stddef.h>

#include <lib/fcntl.h>
#include <lib/unistd.h>

int main(int argc, const char *argv[])
{
	int ret_val = 0;
	int testi = 0x12345678;
	int rv = 0;

	rv = printf("Message from init.bin.\n");

	// char buf[512];
	// memset(buf, 0, 512);
	// int test_fd1 = open("/EFI/BOOT/BOOTX64.EFI", O_RDONLY);
	// if (test_fd1 >= 0)
	// {
	// 	read(test_fd1, buf, 512);
	// }
	// printf("Read test 1 : %s\n", buf);
	// close(test_fd1);

	// memset(buf, 0, 512);
	// int test_fd2 = open("/EFI/BOOT/test.txt", O_RDONLY);
	// if (test_fd2 >= 0)
	// {
	// 	read(test_fd2, buf, 512);
	// }
	// printf("Read test 2 : %s\n", buf);
	// close(test_fd2);

	// char buf2[] = {"write test success."};
	// int test_fd3 = open("/EFI/BOOT/test.txt", O_RDWR | O_APPEND);
	// write(test_fd3, buf2, strlen(buf2));
	// close(test_fd3);

	// char *av[] = {"initd", "called by init()", NULL};
	// execve("/init.bin", av, NULL);

	// int pid = getpid();

	// char buf3[10];
	// int tty_fd = open("/dev/tty0", O_RDONLY);
	// read(tty_fd, buf3, 9);
	// printf("Read kbd : %s \n", buf3);
	// close(tty_fd);

	rv = fork();
	printf("message after fork, ret_val = %d, self:%d\n", rv, getpid());
	rv = fork();
	printf("message after fork, ret_val = %d, self:%d\n", rv, getpid());
	// if (rv != 0)
	// 	while (1);
	// else
	// {
	// 	rv = fork();
	// 	if ( rv == 0)
	// 		while (1);
	// }

	return ret_val;
}