#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include <dirent.h>

#include <fcntl.h>

DIR *__alloc_dir(int fd, bool close_fd, int flags)
{
	/* The st_blksize value of the directory is used as a hint for the
	   size of the buffer which receives struct dirent values from the
	   kernel.  st_blksize is limited to max_buffer_size, in case the
	   file system provides a bogus value.  */
	size_t allocation = 1048576;

	DIR *dirp = (DIR *)malloc(sizeof(DIR) + allocation);
	if (dirp == NULL)
	{
		if (close_fd)
			close(fd);
		return NULL;
	}

	dirp->fd = fd;
	// dirp->allocation = allocation;
	// dirp->size = 0;
	// dirp->offset = 0;
	// dirp->filepos = 0;
	// dirp->errcode = 0;

	return dirp;
}

DIR *opendir(const char *name)
{
	// int fd = 0;
	// DIR *dir = NULL;

	// fd = open(name, O_DIRECTORY, 0);

	// if (fd >= 0)
	// {
	// 	dir = (DIR *)malloc(sizeof(DIR));
	// 	if (dir == NULL)
	// 	{
	// 		close(fd);
	// 		return (DIR *)-ENOMEM;
	// 	}
	// 	memset(dir, 0, sizeof(DIR));
	// 	dir->fd = fd;
	// 	dir->buf_pos = 0;
	// 	dir->buf_end = 256;

	// 	return dir;
	// }
	// else
	// 	return NULL;

	return __alloc_dir(open(name, O_DIRECTORY, 0), true, 0);
}