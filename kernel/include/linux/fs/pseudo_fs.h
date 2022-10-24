#ifndef __LINUX_PSEUDO_FS__
#define __LINUX_PSEUDO_FS__

	#include <linux/fs/fs_context.h>

	typedef struct pseudo_fs_context {
		const super_ops_s *ops;
		// const struct xattr_handler **xattr;
		const dentry_ops_s *dops;
		unsigned long magic;
	} pseudo_fs_ctxt_s;

	pseudo_fs_ctxt_s *init_pseudo(fs_ctxt_s *fc, unsigned long magic);

#endif