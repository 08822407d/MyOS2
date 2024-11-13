#include <stdio.h>
#include <unistd.h>
#include <sched.h>


int normal_boot(int argc, const char *argv[]);

int main(int argc, const char *argv[])
{
	freopen("/dev/console", "r", stdin);
	freopen("/dev/console", "w", stdout);
	freopen("/dev/console", "w", stderr);


	printf("Welcome to MyOS2\n\n");

	return normal_boot(argc, argv);
}


int normal_boot(int argc, const char *argv[])
{
	char prog_name[] = "/boot/sh";
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };

	int rv = fork();
	
	if (rv == 0) {
		pid_t pid = getpid();
		// printf("child task, %d\n", rv);
		execve(prog_name, args, envs);
	}
	else {
		pid_t pid = getpid();
		// printf("parent task, %d\n", rv);
		while (1) {
			sched_yield();
		}
	}
}