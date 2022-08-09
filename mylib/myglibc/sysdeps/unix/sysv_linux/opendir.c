/* Copyright (C) 1991-2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>	/* For BUFSIZ.  */
#include <sys/param.h>	/* For MIN and MAX.  */

#include <not-cancel.h>


#include <stdlib.h>
#include <stdbool.h>


enum {
	opendir_oflags = O_RDONLY|O_NDELAY|O_DIRECTORY|O_LARGEFILE|O_CLOEXEC
};

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