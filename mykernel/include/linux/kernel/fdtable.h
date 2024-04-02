/* SPDX-License-Identifier: GPL-2.0 */
/*
 * descriptor table internals; you almost certainly want file.h instead.
 */

#ifndef __LINUX_FDTABLE_H
#define __LINUX_FDTABLE_H

	#include <uapi/linux/posix_types.h>
	#include <linux/compiler/compiler.h>
	#include <linux/kernel/lock_ipc.h>
	#include <linux/kernel/types.h>
	#include <linux/init/init.h>
	#include <linux/fs/fs.h>


	/*
	 * The default fd array needs to be at least BITS_PER_LONG,
	 * as this is the granularity returned by copy_fdset().
	 */
	// #define NR_OPEN_DEFAULT BITS_PER_LONG
	#define NR_OPEN_DEFAULT	256
	// #define NR_OPEN_MAX ~0U
	#define NR_OPEN_MAX		NR_OPEN_DEFAULT

	// typedef struct fdtable {
	// 	unsigned int max_fds;
	// 	file_s __rcu **fd;      /* current fd array */
	// 	unsigned long *close_on_exec;
	// 	unsigned long *open_fds;
	// 	unsigned long *full_fds_bits;
	// 	struct rcu_head rcu;
	// } fdtable_s;

	// static inline bool close_on_exec(unsigned int fd, const struct fdtable *fdt)
	// {
	// 	return test_bit(fd, fdt->close_on_exec);
	// }

	// static inline bool fd_is_open(unsigned int fd, const struct fdtable *fdt)
	// {
	// 	return test_bit(fd, fdt->open_fds);
	// }

	/*
	 * Open file table structure
	 */
	typedef struct files_struct {
		/*
		 * read mostly part
		 */
		atomic_t		refcount;
		// bool resize_in_progress;
		// wait_queue_head_t resize_wait;

		// fdtable_s __rcu	*fdt;
		// fdtable_s fdtab;
		/*
		 * written part on a separate cache line in SMP
		 */
		// spinlock_t file_lock ____cacheline_aligned_in_smp;
		spinlock_t		file_lock;
		// unsigned int	next_fd;
		// unsigned long	close_on_exec_init[1];
		// unsigned long	open_fds_init[1];
		// unsigned long	full_fds_bits_init[1];
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;

	// struct file_operations;
	// struct vfsmount;
	// struct dentry;

	// #define rcu_dereference_check_fdtable(files, fdtfd) \
	// 	rcu_dereference_check((fdtfd), lockdep_is_held(&(files)->file_lock))

	// #define files_fdtable(files) \
	// 	rcu_dereference_check_fdtable((files), (files)->fdt)

	// /*
	// * The caller must ensure that fd table isn't shared or hold rcu or file lock
	// */
	// static inline file_s *files_lookup_fd_raw(files_struct_s *files, unsigned int fd)
	// {
	// 	struct fdtable *fdt = rcu_dereference_raw(files->fdt);

	// 	if (fd < fdt->max_fds) {
	// 		fd = array_index_nospec(fd, fdt->max_fds);
	// 		return rcu_dereference_raw(fdt->fd[fd]);
	// 	}
	// 	return NULL;
	// }

	// static inline file_s *files_lookup_fd_locked(files_struct_s *files, unsigned int fd)
	// {
	// 	RCU_LOCKDEP_WARN(!lockdep_is_held(&files->file_lock),
	// 			"suspicious rcu_dereference_check() usage");
	// 	return files_lookup_fd_raw(files, fd);
	// }

	// static inline file_s *files_lookup_fd_rcu(files_struct_s *files, unsigned int fd)
	// {
	// 	RCU_LOCKDEP_WARN(!rcu_read_lock_held(),
	// 			"suspicious rcu_dereference_check() usage");
	// 	return files_lookup_fd_raw(files, fd);
	// }

	// static inline file_s *lookup_fd_rcu(unsigned int fd)
	// {
	// 	return files_lookup_fd_rcu(current->files, fd);
	// }

	// file_s *task_lookup_fd_rcu(task_s *task, unsigned int fd);
	// file_s *task_lookup_next_fd_rcu(task_s *task, unsigned int *fd);

	// struct task_struct;

	void put_files_struct(files_struct_s *fs);
	int unshare_files(void);
	files_struct_s *dup_fd(files_struct_s *, unsigned, int *) __latent_entropy;
	// void do_close_on_exec(files_struct_s *);
	// int iterate_fd(files_struct_s *, unsigned,
	// 		int (*)(const void *, file_s *, unsigned),
	// 		const void *);

	// extern int close_fd(unsigned int fd);
	// extern int __close_range(unsigned int fd, unsigned int max_fd, unsigned int flags);
	// extern int close_fd_get_file(unsigned int fd, file_s **res);
	// extern int unshare_fd(unsigned long unshare_flags, unsigned int max_fds,
	// 			files_struct_s **new_fdp);

#endif /* __LINUX_FDTABLE_H */
