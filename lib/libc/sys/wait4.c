#include <sys/wait.h>

int waitpid(unsigned long pid, int *status, int options)
{
	wait4(pid, status, options);
}