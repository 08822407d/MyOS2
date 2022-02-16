#ifndef _LINUX_DCACHE_H_
#define _LINUX_DCACHE_H_

#include <include/fs/vfs.h>

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

	dentry_s * __d_lookup(dentry_s * parent, qstr_s * name);
	dentry_s * __d_alloc(qstr_s * name);

#endif /* _LINUX_DCACHE_H_ */