#include <uapi/dirent.h>

#include <include/fs/file.h>

long sys_getdents64(unsigned int fd,
		linux_dirent64_s *dirent, unsigned int count)
{
	fd_s f;
// 	struct getdents_callback64 buf = {
// 		.ctx.actor = filldir64,
// 		.count = count,
// 		.current_dir = dirent
// 	};
	int error;

// 	f = fdget_pos(fd);
// 	if (!f.file)
// 		return -EBADF;

// 	error = iterate_dir(f.file, &buf.ctx);
// 	if (error >= 0)
// 		error = buf.error;
// 	if (buf.prev_reclen) {
// 		struct linux_dirent64 __user * lastdirent;
// 		typeof(lastdirent->d_off) d_off = buf.ctx.pos;

// 		lastdirent = (void __user *) buf.current_dir - buf.prev_reclen;
// 		if (put_user(d_off, &lastdirent->d_off))
// 			error = -EFAULT;
// 		else
// 			error = count - buf.count;
// 	}
// 	fdput_pos(f);
// 	return error;
}