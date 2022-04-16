#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

extern int main(int, const char **);

extern void init_uslab(void);

__attribute__((section(".crt1.text"))) void _start(int argc, const char * argv[])
{
	open("/dev/tty0", O_RDONLY, 0);
	open("/dev/tty0", O_WRONLY, 0);
	open("/dev/tty0", O_WRONLY, 0);

	stdin;
	stdout;
	stderr;
	
	exit(main(argc, argv));
}