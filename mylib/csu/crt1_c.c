#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

extern int main(int, const char *[]);

extern void init_uslab(void);

FILE *stdin;
FILE *stdout;
FILE *stderr;
const char **envs;

__attribute__((section(".crt1.text"))) void _start(int argc, const char **argv)
{
	while (argv[argc] != NULL);
	envs = argv + argc + 1;
	
	// void *malloctest = malloc(0x20000);
	stdin = fopen("/dev/console", "r");
	stdout = fopen("/dev/console", "w");
	stderr = fopen("/dev/console", "w");
	
	exit(main(argc, argv));
}