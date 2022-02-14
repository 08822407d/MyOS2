#include <include/task.h>
#include <include/vfs.h>

/*
 * allocate a file descriptor, mark it busy.
 */
static int alloc_fd(unsigned start, unsigned end, unsigned flags)
{
	task_s * curr = curr_tsk;
	file_s ** f = NULL;

	int fd = -1;
	f = curr->fps;
	for(int i = start; i < end; i++)
		if(f[i] == NULL)
		{
			fd = i;
			break;
		}

	return fd;
}

int get_unused_fd_flags(unsigned flags)
{
	return alloc_fd(0, MAX_FILE_NR, flags);
}