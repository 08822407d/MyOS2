/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NAMEI_H
#define _LINUX_NAMEI_H

	#define ND_ROOT_PRESET	1
	#define ND_ROOT_GRABBED	2
	#define ND_JUMPED		4

	#define MAXSYMLINKS		40
	enum {WALK_TRAILING = 1, WALK_MORE = 2, WALK_NOFOLLOW = 4};

#include <linux/fs/fs.h>
#include <linux/fs/path.h>
// #include <linux/kernel.h>
#include <linux/kernel/fcntl.h>

#include <errno.h>

	enum { MAX_NESTED_LINKS = 8 };

	#define MAXSYMLINKS 40
	/*
	* Type of the last component on LOOKUP_PARENT
	*/
	enum {LAST_NORM, LAST_ROOT, LAST_DOT, LAST_DOTDOT};

	/* pathwalk mode */
	#define LOOKUP_FOLLOW			0x0001	/* follow links at the end */
	#define LOOKUP_DIRECTORY		0x0002	/* require a directory */
	#define LOOKUP_AUTOMOUNT		0x0004  /* force terminal automount */
	#define LOOKUP_EMPTY			0x4000	/* accept empty path [user_... only] */
	#define LOOKUP_DOWN				0x8000	/* follow mounts in the starting point */
	#define LOOKUP_MOUNTPOINT		0x0080	/* follow mounts in the end */

	#define LOOKUP_REVAL			0x0020	/* tell ->d_revalidate() to trust no cache */
	#define LOOKUP_RCU				0x0040	/* RCU pathwalk mode; semi-internal */

	/* These tell filesystem methods that we are dealing with the final component... */
	#define LOOKUP_OPEN				0x0100	/* ... in open */
	#define LOOKUP_CREATE			0x0200	/* ... in object creation */
	#define LOOKUP_EXCL				0x0400	/* ... in exclusive creation */
	#define LOOKUP_RENAME_TARGET	0x0800	/* ... in destination of rename() */

	/* internal use only */
	#define LOOKUP_PARENT			0x0010

	/* Scoping flags for lookup. */
	#define LOOKUP_NO_SYMLINKS		0x010000 /* No symlink crossing. */
	#define LOOKUP_NO_MAGICLINKS	0x020000 /* No nd_jump_link() crossing. */
	#define LOOKUP_NO_XDEV			0x040000 /* No mountpoint crossing. */
	#define LOOKUP_BENEATH			0x080000 /* No escaping from starting point. */
	#define LOOKUP_IN_ROOT			0x100000 /* Treat dirfd as fs root. */
	#define LOOKUP_CACHED			0x200000 /* Only do cached lookup */
	/* LOOKUP_* flags which do scope-related checks based on the dirfd. */
	#define LOOKUP_IS_SCOPED (LOOKUP_BENEATH | LOOKUP_IN_ROOT)	

	// extern int path_pts(path_s *path);

	extern int user_path_at_empty(int dfd, const char *name, unsigned flags, path_s *);
	static inline int user_path_at(int dfd, const char *name, unsigned flags, path_s *path)
	{
		return user_path_at_empty(dfd, name, flags, path);
	}
	int kern_path(const char * name, unsigned int flags, path_s * path);

	extern dentry_s *kern_path_create(int, const char *, path_s *, unsigned int);
	extern dentry_s *user_path_create(int, const char *, path_s *, unsigned int);
	// extern void done_path_create(path_s *, dentry_s *);
	// extern dentry_s *kern_path_locked(const char *, path_s *);

	// extern dentry_s *try_lookup_one_len(const char *, dentry_s *, int);
	// extern dentry_s *lookup_one_len(const char *, dentry_s *, int);
	// extern dentry_s *lookup_one_len_unlocked(const char *, dentry_s *, int);
	// extern dentry_s *lookup_positive_unlocked(const char *, dentry_s *, int);
	// dentry_s *lookup_one(user_namespace_s *, const char *, dentry_s *, int);

	// extern int follow_down_one(path_s *);
	// extern int follow_down(path_s *);
	// extern int follow_up(path_s *);

	// extern dentry_s *lock_rename(dentry_s *, dentry_s *);
	// extern void unlock_rename(dentry_s *, dentry_s *);

	// extern int nd_jump_link(path_s *path);

	// static inline void nd_terminate_link(void *name, size_t len, size_t maxlen)
	// {
	// 	((char *) name)[min(len, maxlen)] = '\0';
	// }

	// /**
	//  * retry_estale - determine whether the caller should retry an operation
	//  * @error: the error that would currently be returned
	//  * @flags: flags being used for next lookup attempt
	//  *
	//  * Check to see if the error code was -ESTALE, and then determine whether
	//  * to retry the call based on whether "flags" already has LOOKUP_REVAL set.
	//  *
	//  * Returns true if the caller should try the operation again.
	//  */
	// static inline bool
	// retry_estale(const long error, const unsigned int flags)
	// {
	// 	return error == -ESTALE && !(flags & LOOKUP_REVAL);
	// }

#endif /* _LINUX_NAMEI_H */