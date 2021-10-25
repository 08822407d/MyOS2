#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>

extern int main(int, const char **);

__attribute__((section(".crt1.text"))) void _start(int argc, const char * argv[])
{
	stdin;
	stdout;
	stderr;
	exit(main(argc, argv));
}