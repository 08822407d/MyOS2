#include <linux/kernel/syscalls.h>
#include <linux/lib/errno.h>
#include <linux/kernel/fcntl.h>
#include <linux/fs/file.h>

// #include <asm/unistd_64.h>
// #include <asm/syscalls_64.h>


// #define __NR_read			0
MYOS_SYSCALL_DEFINE3(read, unsigned int, fd,
		char *,buf, size_t, count) {
	return ksys_read(fd, buf, count);
}

// #define __NR_write			1
MYOS_SYSCALL_DEFINE3(write, unsigned int, fd,
		const char *, buf, size_t, count)
{
	// task_s * curr = current;
	// file_s * fp = NULL;
	// ulong ret = 0;

	// if(fd < 0 || fd >= curr->files->fd_count)
	// 	return -EBADF;
	// if(count < 0)
	// 	return -EINVAL;

	// fp = curr->files->fd_array[fd];
	// if(fp->f_op && fp->f_op->write)
	// 	ret = fp->f_op->write(fp, buf, count, &fp->f_pos);
	// return ret;

	return ksys_write(fd, buf, count);
}

// #define __NR_open			2
MYOS_SYSCALL_DEFINE3(open, const char *, filename,
		int, flags, umode_t, mode)
{
	return do_sys_open(0, filename, flags, mode);
}

// #define __NR_close			3
MYOS_SYSCALL_DEFINE1(close, unsigned int, fd)
{
	task_s * curr = current;

	file_s * fp = NULL;

	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;

	fp = curr->files->fd_array[fd];
	// if(fp->f_op && fp->f_op->close)
	// 	fp->f_op->close(fp->f_path.dentry->d_inode, fp);

	kfree(fp);
	curr->files->fd_array[fd] = NULL;

	// int retval = close_fd(fd);

	// /* can't restart close syscall because file table entry was cleared */
	// if (unlikely(retval == -ERESTARTSYS ||
	// 	     retval == -ERESTARTNOINTR ||
	// 	     retval == -ERESTARTNOHAND ||
	// 	     retval == -ERESTART_RESTARTBLOCK))
	// 	retval = -EINTR;

	return 0;
}


// #define __NR_lseek			8
MYOS_SYSCALL_DEFINE3(lseek, unsigned int, fd,
		off_t, offset, unsigned int, whence)
{
	task_s * curr = current;
	file_s * fp = NULL;
	ulong ret = 0;

	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;

	fp = current->files->fd_array[fd];
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
	return ret;
}


// #define __NR_writev			20
MYOS_SYSCALL_DEFINE3(writev, unsigned long, fd,
		const iov_s __user *, vec, unsigned long, vlen)
{
	return do_writev(fd, vec, vlen, 0);
}


// #define __NR_dup			32
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

// #define __NR_dup2			33
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


// #define __NR_fcntl			72
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
