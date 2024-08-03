/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Wrapper functions for accessing the file_fd_s array.
 */

#ifndef __LINUX_FILE_H_
#define __LINUX_FILE_H_

	#include <linux/compiler/compiler.h>
	#include <linux/kernel/types.h>
	#include <linux/kernel/fdtable.h>
	#include <linux/fs/fs.h>
	#include <uapi/linux/posix_types.h>
	#include <linux/lib/errno.h>

	extern void fput(file_s *file);

	extern file_s *
	alloc_file_pseudo(inode_s *, vfsmount_s *, const char *,
			int flags, const file_ops_s *);
	extern file_s *
	alloc_file_clone(file_s *, int flags, const file_ops_s *);

	static inline void
	fput_light(file_s *file, int fput_needed) {
		if (fput_needed) fput(file);
	}

	typedef struct fd {
		file_s			*file;
		unsigned int	flags;
	} fd_s;
	// #define FDPUT_FPUT       1
	// #define FDPUT_POS_UNLOCK 2

	// static inline void fdput(fd_s fd)
	// {
	// 	if (fd.flags & FDPUT_FPUT)
	// 		fput(fd.file);
	// }

	extern file_s *fget(unsigned int fd);
	// extern file_s *fget_many(unsigned int fd, unsigned int refs);
	// extern file_s *fget_raw(unsigned int fd);
	// extern file_s *fget_task(task_s *task, unsigned int fd);
	// extern unsigned long __fdget(unsigned int fd);
	// extern unsigned long __fdget_raw(unsigned int fd);
	// extern unsigned long __fdget_pos(unsigned int fd);
	// extern void __f_unlock_pos(file_s *);

	static inline fd_s
	__to_fd(ulong v) {
		files_struct_s *files = current->files;
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
		return (fd_s){file, 3};
	}

	static inline fd_s
	fdget(uint fd) {
		// return __to_fd(__fdget(fd));
		return __to_fd(fd);
	}

	static inline fd_s
	fdget_raw(uint fd) {
		// return __to_fd(__fdget_raw(fd));
		return __to_fd(fd);
	}

	static inline fd_s
	fdget_pos(int fd) {
		// return __to_fd(__fdget_pos(fd));
		return __to_fd(fd);
	}

	static inline void
	fdput_pos(fd_s f) {
		// if (f.flags & FDPUT_POS_UNLOCK)
		// 	__f_unlock_pos(f.file);
		// fdput(f);
	}

	// extern int f_dupfd(unsigned int from, file_s *file, unsigned flags);
	// extern int replace_fd(unsigned fd, file_s *file, unsigned flags);
	// extern void set_close_on_exec(unsigned int fd, int flag);
	// extern bool get_close_on_exec(unsigned int fd);
	// extern int __get_unused_fd_flags(unsigned flags, unsigned long nofile);
	extern int get_unused_fd_flags(unsigned flags);
	// extern void put_unused_fd(unsigned int fd);

	extern void fd_install(unsigned int fd, file_s *file);

	// extern int __receive_fd(file_s *file, int __user *ufd,
	// 			unsigned int o_flags);

	// extern int receive_fd(file_s *file, unsigned int o_flags);

	// static inline int receive_fd_user(file_s *file, int __user *ufd,
	// 				unsigned int o_flags)
	// {
	// 	if (ufd == NULL)
	// 		return -EFAULT;
	// 	return __receive_fd(file, ufd, o_flags);
	// }
	// int receive_fd_replace(int new_fd, file_s *file, unsigned int o_flags);

	// extern void flush_delayed_fput(void);
	// extern void __fput_sync(file_s *);

	// extern unsigned int sysctl_nr_open_min, sysctl_nr_open_max;


	extern fd_s myos_fdget_pos(int fd);

#endif /* __LINUX_FILE_H */