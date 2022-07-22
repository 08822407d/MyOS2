#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <dirent.h>

#include <uapi/fcntl.h>

/* Open a directory stream on a file descriptor in Hurd internal form.
   We do no checking here on the descriptor.  */
DIR *_hurd_fd_opendir(struct hurd_fd *d)
{
	DIR *dirp;

	dirp = (DIR *)malloc(sizeof(DIR));
	if (dirp == NULL)
		return NULL;

	/* Set the descriptor to close on exec. */
	d->flags |= FD_CLOEXEC;

	dirp->__fd = d;
	dirp->__data = dirp->__ptr = NULL;
	dirp->__entry_data = dirp->__entry_ptr = 0;
	dirp->__allocation = 0;
	dirp->__size = 0;

	return dirp;
}

DIR *opendir(const char *name)
{
	int fd = 0;
	DIR *dir = NULL;

	fd = open(name, O_DIRECTORY, 0);

	if (fd >= 0)
	{
		dir = (DIR *)malloc(sizeof(DIR));
		if (dir == NULL)
		{
			close(fd);
			return (DIR *)-ENOMEM;
		}
		memset(dir, 0, sizeof(DIR));
		dir->fd = fd;
		dir->buf_pos = 0;
		dir->buf_end = 256;

		return dir;
	}
	else
		return NULL;

	if (name[0] == '\0')
	{
		/* POSIX.1-1990 says an empty name gets ENOENT;
		but `open' might like it fine.  */
		// __set_errno(ENOENT);
		return NULL;
	}

	int fd = open(name, O_RDONLY | O_NONBLOCK | O_DIRECTORY, 0);
	if (fd < 0)
		return NULL;

	/* Extract the pointer to the descriptor structure.  */
	DIR *dirp = _hurd_fd_opendir(_hurd_fd_get(fd));
	if (dirp == NULL)
		close(fd);

	return dirp;
}