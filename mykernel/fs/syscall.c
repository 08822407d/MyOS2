#include <linux/kernel/fcntl.h>
#include <linux/kernel/syscalls.h>
#include <linux/fs/file.h>
#include <linux/lib/errno.h>


MYOS_SYSCALL_DEFINE3(fcntl, unsigned int, fd,
        unsigned int, cmd, unsigned long, arg)
{	
	fd_s f = fdget_raw(fd);
	long err = -EBADF;

	if (!f.file)
		goto out;

	if (unlikely(f.file->f_mode & FMODE_PATH)) {
		if (!check_fcntl_cmd(cmd))
			goto out1;
	}

	// err = security_file_fcntl(f.file, cmd, arg);
	// if (!err)
		err = do_fcntl(fd, cmd, arg, f.file);

out1:
 	fdput(f);
out:
	return err;
}

MYOS_SYSCALL_DEFINE2(dup2, unsigned int, oldfd, unsigned int, newfd)
{
	ALERT_DUMMY_SYSCALL(dup2, IF_ALERT_DUMMY_SYSCALL);

	// if (unlikely(newfd == oldfd)) { /* corner case */
	// 	files_struct_s *files = current->files;
	// 	int retval = oldfd;

	// 	rcu_read_lock();
	// 	if (!files_lookup_fd_rcu(files, oldfd))
	// 		retval = -EBADF;
	// 	rcu_read_unlock();
	// 	return retval;
	// }
	// return ksys_dup3(oldfd, newfd, 0);
}

MYOS_SYSCALL_DEFINE1(dup, unsigned int, fildes)
{
	ALERT_DUMMY_SYSCALL(dup, IF_ALERT_DUMMY_SYSCALL);

	// int ret = -EBADF;
	// file_s *file = fget_raw(fildes);

	// if (file) {
	// 	ret = get_unused_fd_flags(0);
	// 	if (ret >= 0)
	// 		fd_install(ret, file);
	// 	else
	// 		fput(file);
	// }
	// return ret;
}