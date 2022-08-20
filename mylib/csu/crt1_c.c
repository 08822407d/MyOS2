#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

extern int main(int, const char **);

extern void init_uslab(void);

FILE *stdin;
FILE *stdout;
FILE *stderr;

__attribute__((section(".crt1.text"))) void _start(int argc, const char * argv[])
{
	stdin = fopen("/dev/tty0", "r");
	stdout = fopen("/dev/tty0", "w");
	stderr = fopen("/dev/tty0", "w");
	
	exit(main(argc, argv));
}