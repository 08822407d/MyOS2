/* SPDX-License-Identifier: GPL-2.0 */
// #include <linux/syscalls.h>
// #include <linux/export.h>
// #include <linux/uaccess.h>
// #include <linux/fs_struct.h>
#include <linux/fs/fs.h>
#include <linux/kernel/slab.h>
// #include <linux/prefetch.h>
#include <linux/fs/mount.h>


#include <linux/kernel/sched.h>
#include <linux/lib/string.h>
#include <linux/lib/errno.h>
#include <obsolete/proto.h>

typedef struct prepend_buffer
{
	char *buf;
	int len;
} prpndbuf_s;


/**
 * prepend_name - prepend a pathname in front of current buffer pointer
 * @buffer: buffer pointer
 * @buflen: allocated length of the buffer
 * @name:   name string and length qstr structure
 *
 * With RCU path tracing, it may race with d_move(). Use READ_ONCE() to
 * make sure that either the old or the new name pointer and length are
 * fetched. However, there may be mismatch between length and pointer.
 * The length cannot be trusted, we need to copy it byte-by-byte until
 * the length is reached or a null byte is found. It also prepends "/" at
 * the beginning of the name. The sequence number check at the caller will
 * retry it again when a d_move() does happen. So any garbage in the buffer
 * due to mismatched pointer and length will be discarded.
 *
 * Load acquire is needed to make sure that we see that terminating NUL.
 */
// Linux function proto:
// static bool prepend_name(struct prepend_buffer *p, const struct qstr *name)
static bool prepend_name(prpndbuf_s *p, const qstr_s *name)
{
	const char *dname = (const char *)name->name; /* ^^^ */
	uint32_t dlen = name->len;
	char *s;

	p->len -= dlen + 1;
	if (p->len < 0)
		return false;
	s = p->buf -= dlen + 1;
	*s++ = '/';
	while (dlen--) {
		char c = *dname++;
		if (c == '\0')
			break;
		*s++ = c;
	}
	return true;
}

// Linux function proto:
// static void prepend(struct prepend_buffer *p, const char *str, int namelen)
static void prepend(prpndbuf_s *p, const char *str, int namelen)
{
	p->len -= namelen;
	if (p->len >= 0) {
		p->buf -= namelen;
		memcpy(p->buf, str, namelen);
	}
}

// Linux function proto:
// static int __prepend_path(const struct dentry *dentry, const struct mount *mnt,
// 			  const struct path *root, struct prepend_buffer *p)
static int __prepend_path(const dentry_s *dentry, const mount_s *mnt,
			  const path_s *root, prpndbuf_s *p)
{
	while (dentry != root->dentry || &mnt->mnt != root->mnt) {
		const dentry_s *parent = dentry->d_parent;

		if (dentry == mnt->mnt.mnt_root) {
			mount_s *m = mnt->mnt_parent;

			if (mnt != m) {
				dentry = mnt->mnt_mountpoint;
				mnt = m;
				continue;
			}

			return 2;	// detached or not attached yet
		}

		if (dentry == parent)
			/* Escaped? */
			return 3;

		if (!prepend_name(p, &dentry->d_name))
			break;
		dentry = parent;
	}
	return 0;
}

/**
 * prepend_path - Prepend path string to a buffer
 * @path: the dentry/vfsmount to report
 * @root: root vfsmnt/dentry
 * @buffer: pointer to the end of the buffer
 * @buflen: pointer to buffer length
 *
 * The function will first try to write out the pathname without taking any
 * lock other than the RCU read lock to make sure that dentries won't go away.
 * It only checks the sequence number of the global rename_lock as any change
 * in the dentry's d_seq will be preceded by changes in the rename_lock
 * sequence number. If the sequence number had been changed, it will restart
 * the whole pathname back-tracing sequence again by taking the rename_lock.
 * In this case, there is no need to take the RCU read lock as the recursive
 * parent pointer references will keep the dentry chain alive as long as no
 * rename operation is performed.
 */
// Linux function proto:
// static int prepend_path(const struct path *path,
// 			const struct path *root,
// 			struct prepend_buffer *p)
static int prepend_path(const path_s *path, const path_s *root,
			prpndbuf_s *p)
{
	unsigned seq, m_seq = 0;
	prpndbuf_s b;
	int error;

restart:
	b = *p;
	error = __prepend_path(path->dentry, real_mount(path->mnt), root, &b);

	if (error == 3)
		b = *p;

	if (b.len == p->len)
		prepend(&b, "/", 1);

	*p = b;
	return error;
}

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
	prpndbuf_s b;
	char *bufhead;

	// if (unlikely(d_unlinked(pwd.dentry))) {
	// 	rcu_read_unlock();
	// 	error = -ENOENT;
	// } else {
		unsigned len;
		b.len = PATH_MAX;
		bufhead = kzalloc(b.len, GFP_KERNEL);
		b.buf = bufhead + PATH_MAX;

		prepend(&b, "", 1);
		if (prepend_path(&pwd, &root, &b) > 0)
			prepend(&b, "(unreachable)", 13);

		len = PATH_MAX - b.len;
		if (len > PATH_MAX)
			error = -ENAMETOOLONG;
		else if (len > size)
			error = -ERANGE;
		else if (copy_to_user(buf, b.buf, len))
			error = -EFAULT;
		else
			error = (long)buf;
	// }
	// __putname(page);
	kfree(bufhead);
	return error;
}