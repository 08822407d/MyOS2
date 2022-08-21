/* Read a directory.  Linux LFS version.
   Copyright (C) 1997-2022 Free Software Foundation, Inc.
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

/* When _DIRENT_MATCHES_DIRENT64 is defined we can alias 'readdir64' to
   'readdir'.  However the function signatures are not equal due
   different return types, so we need to suppress {__}readdir so weak
   and strong alias do not throw conflicting types errors.  */
#include <dirent.h>

#include <limits.h>
#include <stddef.h>

/* Read a directory entry from DIRP.  */
struct dirent64 *
readdir(DIR *dirp)
{
	struct dirent64 *dp;
// 	int saved_errno = errno;

	// do
	// {
		size_t reclen;

		if (dirp->offset >= dirp->size)
		{
			/* We've emptied out our buffer.  Refill it.  */

			size_t maxread = dirp->allocation;
			ssize_t bytes;

			// bytes = __getdents64(dirp->fd, dirp->data, maxread);
			if (maxread > INT_MAX)
				maxread = INT_MAX;
			bytes = getdents64(dirp->fd, dirp->data, maxread);
			if (bytes <= 0)
			{
				// /* On some systems getdents fails with ENOENT when the
				// open directory has been rmdir'd already.  POSIX.1
				// requires that we treat this condition like normal EOF.  */
				// if (bytes < 0 && errno == ENOENT)
				// 	bytes = 0;

				// /* Don't modifiy errno when reaching EOF.  */
				// if (bytes == 0)
				// 	__set_errno(saved_errno);
				// dp = NULL;
				// break;
				return NULL;
			}
			dirp->size = (size_t)bytes;

			/* Reset the offset into the buffer.  */
			dirp->offset = 0;
		}

		dp = (struct dirent64 *)&dirp->data[dirp->offset];

		reclen = dp->d_reclen;

		dirp->offset += reclen;

		dirp->filepos = dp->d_off;

	// 	/* Skip deleted files.  */
	// } while (dp->d_ino == 0);

	return dp;
}
// __weak_alias(__readdir64, readdir)