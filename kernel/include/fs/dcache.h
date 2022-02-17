#ifndef _LINUX_DCACHE_H_
#define _LINUX_DCACHE_H_

#include <stddef.h>
#include <lib/utils.h>

	struct dentry;
	typedef struct dentry dentry_s;

	struct inode;
	typedef struct inode inode_s;

	struct dirent_ops;
	typedef struct dirent_ops dirent_ops_s;

	/*
	* "quick string" -- eases parameter passing, but more importantly
	* saves "metadata" about the string (ie length and the hash).
	*
	* hash comes first so it snuggles against d_parent in the
	* dentry.
	*/
	typedef struct qstr {
		size_t len;
		const unsigned char *name;
	} qstr_s;

	typedef struct dentry
	{
		List_s		dirent_list;

		char *		name;
		int			name_length;

		dentry_s *	parent;
		List_hdr_s	childdir_lhdr;

		inode_s *		dir_inode;
		dirent_ops_s *	dir_ops;
	} dentry_s;

	dentry_s * __d_lookup(dentry_s * parent, qstr_s * name);
	dentry_s * __d_alloc(qstr_s * name);

#endif /* _LINUX_DCACHE_H_ */