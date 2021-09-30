#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/fcntl.h>

#include <lib/unistd.h>

int main(int argc,char *argv[])
{
	int ret_val = 123;
	int testi = 0x12345678;

	int rv = printf("Message from init.bin.\n");

	char buf[512];
	int test_fd1 = open("/EFI/BOOT/BOOTX64.EFI", O_RDONLY);
	if (test_fd1 >= 0)
	{
		read(test_fd1, buf, 512);
	}
	int test_fd2 = open("/EFI/BOOT/test.txt", O_RDONLY);

	while (1);

	return ret_val;
}