/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Wrapper functions for accessing the file_struct fd array.
 */

#ifndef __LINUX_FILE_H_
#define __LINUX_FILE_H_

#include <include/task.h>
#include <include/fs/vfs.h>

	typedef struct fd {
		file_s *file;
		unsigned int flags;
	} fd_s;

	int get_unused_fd_flags(unsigned flags);
	void fd_install(unsigned int fd, file_s *file);

#endif /* __LINUX_FILE_H */