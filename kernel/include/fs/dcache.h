#ifndef _LINUX_DCACHE_H_
#define _LINUX_DCACHE_H_

#include <include/fs/vfs.h>
#include <include/fs/namei.h>

	dirent_s * __d_lookup(nameidata_s * nd);
	dirent_s * __d_alloc(const char * name, size_t name_len);

#endif /* _LINUX_DCACHE_H_ */