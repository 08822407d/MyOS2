// SPDX-License-Identifier: GPL-2.0
/*
 * Routines that mimic syscalls, but don't use the user address space or file
 * descriptors.  Only for init/ and related early init code.
 */
#include <linux/init/init.h>
#include <linux/kernel/mount.h>
#include <linux/fs/namei.h>
#include <linux/fs/fs.h>
#include <linux/fs/file.h>
#include <linux/fs/internal.h>


#include <linux/kernel/sched.h>

int init_mount(const char *dev_name, const char *dir_name,
				const char *type_page, unsigned long flags)
{
	struct path path;
	int ret;

	ret = kern_path(dir_name, LOOKUP_FOLLOW, &path);
	if (ret)
		return ret;
	ret = path_mount(dev_name, &path, type_page, flags);
	path_put(&path);
	return ret;
}

int init_umount(const char *name, int flags)
{
	int lookup_flags = LOOKUP_MOUNTPOINT;
	struct path path;
	int ret;

	if (!(flags & UMOUNT_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
	ret = kern_path(name, lookup_flags, &path);
	if (ret)
		return ret;
	return path_umount(&path, flags);
}

int init_chdir(const char *filename)
{
	struct path path;
	int error;

	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (error)
		return error;
	// error = path_permission(&path, MAY_EXEC | MAY_CHDIR);
	if (!error)
		set_fs_pwd(current->fs, &path);
	path_put(&path);
	return error;
}

int init_chroot(const char *filename)
{
	struct path path;
	int error;

	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (error)
		return error;
	// error = path_permission(&path, MAY_EXEC | MAY_CHDIR);
	// if (error)
	// 	goto dput_and_out;
	// error = -EPERM;
	// if (!ns_capable(current_user_ns(), CAP_SYS_CHROOT))
	// 	goto dput_and_out;
	// error = security_path_chroot(&path);
	// if (error)
	// 	goto dput_and_out;
	set_fs_root(current->fs, &path);
dput_and_out:
	path_put(&path);
	return error;
}

int init_chown(const char *filename, uid_t user, gid_t group, int flags)
{
	int lookup_flags = (flags & AT_SYMLINK_NOFOLLOW) ? 0 : LOOKUP_FOLLOW;
	struct path path;
	int error;

	error = kern_path(filename, lookup_flags, &path);
	if (error)
		return error;
	error = mnt_want_write(path.mnt);
	if (!error) {
		error = chown_common(&path, user, group);
		mnt_drop_write(path.mnt);
	}
	path_put(&path);
	return error;
}

int init_chmod(const char *filename, umode_t mode)
{
	struct path path;
	int error;

	error = kern_path(filename, LOOKUP_FOLLOW, &path);
	if (error)
		return error;
	error = chmod_common(&path, mode);
	path_put(&path);
	return error;
}

int init_mkdir(const char *pathname, umode_t mode)
{
	dentry_s *dentry;
	path_s path;
	int error;

	dentry = kern_path_create(AT_FDCWD, pathname, &path, LOOKUP_DIRECTORY);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);
	error = vfs_mkdir(path.dentry->d_inode, dentry, mode);
	done_path_create(&path, dentry);
	return error;
}

void do_name(void)
{
	init_mkdir("dev", 16877);
	init_mkdir("root", 16832);
	init_mkdir(".", 16877);
	init_mkdir("kernel", 16877);
	init_mkdir("conf", 16877);
	init_mkdir("etc", 16877);
	init_mkdir("usr", 16877);
}