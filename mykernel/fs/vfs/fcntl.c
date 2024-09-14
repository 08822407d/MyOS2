#include <linux/kernel/fcntl.h>
#include <linux/fs/file.h>
#include <linux/lib/errno.h>


long
do_fcntl(int fd, uint cmd, ulong arg, file_s *filp)
{
	void __user *argp = (void __user *)arg;
	int argi = (int)arg;
	flock_s flock;
	long err = -EINVAL;

	switch (cmd) {
	case F_DUPFD:
		err = f_dupfd(argi, filp, 0);
		break;
	case F_DUPFD_CLOEXEC:
		err = f_dupfd(argi, filp, O_CLOEXEC);
		break;
	// case F_GETFD:
	// 	err = get_close_on_exec(fd) ? FD_CLOEXEC : 0;
	// 	break;
	// case F_SETFD:
	// 	err = 0;
	// 	set_close_on_exec(fd, argi & FD_CLOEXEC);
	// 	break;
	case F_GETFL:
		err = filp->f_flags;
		break;
	// case F_SETFL:
	// 	err = setfl(fd, filp, argi);
	// 	break;
	// /* 32-bit arches must use fcntl64() */
	// case F_OFD_GETLK:
	// case F_GETLK:
	// 	if (copy_from_user(&flock, argp, sizeof(flock)))
	// 		return -EFAULT;
	// 	err = fcntl_getlk(filp, cmd, &flock);
	// 	if (!err && copy_to_user(argp, &flock, sizeof(flock)))
	// 		return -EFAULT;
	// 	break;
	// /* 32-bit arches must use fcntl64() */
	// case F_OFD_SETLK:
	// case F_OFD_SETLKW:
	// 	fallthrough;
	// case F_SETLK:
	// case F_SETLKW:
	// 	if (copy_from_user(&flock, argp, sizeof(flock)))
	// 		return -EFAULT;
	// 	err = fcntl_setlk(fd, filp, cmd, &flock);
	// 	break;
	// case F_GETOWN:
	// 	/*
	// 	 * XXX If f_owner is a process group, the
	// 	 * negative return value will get converted
	// 	 * into an error.  Oops.  If we keep the
	// 	 * current syscall conventions, the only way
	// 	 * to fix this will be in libc.
	// 	 */
	// 	err = f_getown(filp);
	// 	force_successful_syscall_return();
	// 	break;
	// case F_SETOWN:
	// 	err = f_setown(filp, argi, 1);
	// 	break;
	// case F_GETOWN_EX:
	// 	err = f_getown_ex(filp, arg);
	// 	break;
	// case F_SETOWN_EX:
	// 	err = f_setown_ex(filp, arg);
	// 	break;
	// case F_GETOWNER_UIDS:
	// 	err = f_getowner_uids(filp, arg);
	// 	break;
	// case F_GETSIG:
	// 	err = filp->f_owner.signum;
	// 	break;
	// case F_SETSIG:
	// 	/* arg == 0 restores default behaviour. */
	// 	if (!valid_signal(argi)) {
	// 		break;
	// 	}
	// 	err = 0;
	// 	filp->f_owner.signum = argi;
	// 	break;
	// case F_GETLEASE:
	// 	err = fcntl_getlease(filp);
	// 	break;
	// case F_SETLEASE:
	// 	err = fcntl_setlease(fd, filp, argi);
	// 	break;
	// case F_NOTIFY:
	// 	err = fcntl_dirnotify(fd, filp, argi);
	// 	break;
	// case F_SETPIPE_SZ:
	// case F_GETPIPE_SZ:
	// 	err = pipe_fcntl(filp, cmd, argi);
	// 	break;
	// case F_ADD_SEALS:
	// case F_GET_SEALS:
	// 	err = memfd_fcntl(filp, cmd, argi);
	// 	break;
	// case F_GET_RW_HINT:
	// case F_SET_RW_HINT:
	// 	err = fcntl_rw_hint(filp, cmd, arg);
	// 	break;
	default:
		pr_alert("  Syscall \'fcntl\' unsupported flag: %d\n", cmd);
		break;
	}
	return err;
}

int
check_fcntl_cmd(uint cmd)
{
	switch (cmd) {
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_GETFD:
	case F_SETFD:
	case F_GETFL:
		return 1;
	}
	return 0;
}
