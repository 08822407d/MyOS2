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

	stdin = fopen("/dev/console", "r");
	stdout = fopen("/dev/console", "w");
	stderr = fopen("/dev/console", "w");

	printf("Cmd args - ");
	for (int i = 0; i < argc; i++)
		printf("[%d]:%s -> ", i, argv[i]);
	printf("\nEnv vars - ");
	for (int i = 0; envs[i] != NULL; i++)
		printf("[%d]:%s -> ", i, envs[i]);
	printf("\n");
	
	exit(main(argc, argv));
}