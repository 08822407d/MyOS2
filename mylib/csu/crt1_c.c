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
	// void *malloctest = malloc(0x20000);

	exit(main(argc, argv));
}