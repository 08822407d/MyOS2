#include <string.h>
#include <errno.h>

#include <uapi/dirent.h>

#include <include/proto.h>
#include <linux/fs/vfs.h>
#include <linux/fs/file.h>

#include <arch/amd64/include/archconst.h>
#include <arch/amd64/include/arch_proto.h>

// long sys_getdents64(unsigned int fd,
// 		linux_dirent64_s *dirent, unsigned int count)
// {
// 	fd_s f;
// 	struct getdents_callback64 buf = {
// 		.ctx.actor = filldir64,
// 		.count = count,
// 		.current_dir = dirent
// 	};
// 	int error;

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
// }

long sys_getdents64(unsigned int fd,
		linux_dirent64_s *dirent, unsigned int count)
{
	long ret_val = 0;
	fd_s f = fdget_pos(fd);
	if (f.file->f_ops != NULL &&
		f.file->f_ops->readdir != NULL)
		ret_val = f.file->f_ops->readdir(f.file, dirent, fill_dentry);

	return ret_val;
}

int fill_dentry(void *buf, char *name, long namelen, long type, long offset)
{
	linux_dirent64_s *dent = (linux_dirent64_s *)buf;
	
	if((unsigned long)buf < USERADDR_LIMIT &&
		!verify_area(buf,sizeof(linux_dirent64_s) + namelen))
		return -EFAULT;

	copy_to_user(dent->d_name, name, namelen);
	// memcpy(dent->d_name, name, namelen);
	dent->d_reclen = namelen;
	dent->d_type = type;
	dent->d_off = offset;
	return sizeof(linux_dirent64_s) + namelen;
}