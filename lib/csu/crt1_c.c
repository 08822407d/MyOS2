#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>

extern int main(int, const char **);

extern void init_uslab(void);

__attribute__((section(".crt1.text"))) void _start(int argc, const char * argv[])
{
	init_uslab();
	// stdin;
	// stdout;
	// stderr;
	
	exit(main(argc, argv));
}