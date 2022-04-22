#include <linux/kernel/err.h>

#include <string.h>
#include <errno.h>

#include <include/proto.h>
#include <linux/fs/vfs.h>

/* Find an unused file structure and return a pointer to it.
 * Returns an error pointer if some error happend e.g. we over file
 * structures limit, run out of memory or operation is not permitted.
 *
 * Be very careful using this.  You are responsible for
 * getting write access to any mount that you might assign
 * to this filp, if it is opened for write.  If this is not
 * done, you will imbalance int the mount's writer count
 * and a warning at __fput() time.
 */
file_s *alloc_empty_file(int flags)
{
	static long old_max;
	file_s *f;

	// static struct file *__alloc_file(int flags, const struct cred *cred)
	// {
		f = kmalloc(sizeof(file_s));
		if (!f)
			return ERR_PTR(-ENOMEM);

		memset(f, 0, sizeof(file_s));
		f->f_flags = flags;
		f->f_mode = OPEN_FMODE(flags);
		/* f->f_version: 0 */
	// }
	return f;
}

void fput(file_s *file)
{
	kfree(file);
}