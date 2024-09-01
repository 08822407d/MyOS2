#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

typedef unsigned long ulong;


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
	freopen("/dev/console", "r", stdin);
	freopen("/dev/console", "w", stdout);
	freopen("/dev/console", "w", stderr);

	printf("Welcome to MyOS2\n\n");

	char prog_name[] = "/boot/sh";
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };


	ulong gsbase = rdgsbase();
	printf("gsbase in user space: %p\n", (void *)gsbase);

	int rv = fork();
	
	if (rv != 0) {
		printf("parent task, %d\n", rv);
		// while (1);
	} else {
		printf("child task, %d\n", rv);
		execve(prog_name, args, envs);
		// while (1);
	}
	while (1);
}