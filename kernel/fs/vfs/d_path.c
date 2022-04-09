#include <uapi/limits.h>

#include <errno.h>

#include <include/proto.h>
#include <include/task.h>
#include <include/fs/vfs.h>

/*
 * NOTE! The user-level library version returns a
 * character pointer. The kernel system call just
 * returns the length of the buffer filled (which
 * includes the ending '\0' character), or a negative
 * error value. So libc would do something like
 *
 *	char *getcwd(char * buf, size_t size)
 *	{
 *		int retval;
 *
 *		retval = sys_getcwd(buf, size);
 *		if (retval >= 0)
 *			return buf;
 *		errno = -retval;
 *		return NULL;
 *	}
 */
long sys_getcwd(char *buf, unsigned long size)
{
	int		error;
	task_s *curr = curr_tsk;
	path_s	pwd = curr->fs->pwd,
			root = curr->fs->root;


	// if (unlikely(d_unlinked(pwd.dentry))) {
	// 	rcu_read_unlock();
	// 	error = -ENOENT;
	// } else {
		unsigned len;
	// 	DECLARE_BUFFER(b, page, PATH_MAX);
		char *b = kmalloc(PATH_MAX);

	// 	prepend(&b, "", 1);
		// if (prepend_path(&pwd, &root, &b) > 0)
		// 	prepend(&b, "(unreachable)", 13);
	// 	rcu_read_unlock();

		// len = PATH_MAX - b.len;
		// if (len > PATH_MAX)
		// 	error = -ENAMETOOLONG;
		// else if (len > size)
		// 	error = -ERANGE;
		// else if (copy_to_user(buf, b.buf, len))
		// 	error = -EFAULT;
		// else
		// 	error = len;
	// }
	// __putname(page);
	return error;
}