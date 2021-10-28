#include <sys/wait.h>
#include <sys/types.h>
#include<sys/_null.h>

pid_t waitpid(pid_t pid, int *status, int options)
{
	return wait4(pid, status, options, NULL);
}