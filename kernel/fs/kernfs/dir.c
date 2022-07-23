// SPDX-License-Identifier: GPL-2.0-only
/*
 * fs/kernfs/dir.c - kernfs directory implementation
 *
 * Copyright (c) 2001-3 Patrick Mochel
 * Copyright (c) 2007 SUSE Linux Products GmbH
 * Copyright (c) 2007, 2013 Tejun Heo <tj@kernel.org>
 */

// #include <linux/sched.h>
#include <linux/fs/fs.h>
#include <linux/fs/namei.h>
// #include <linux/idr.h>
// #include <linux/slab.h>
// #include <linux/security.h>
// #include <linux/hash.h>
#include <linux/fs/kernfs-internal.h>


#include <include/proto.h>

static kernfs_node_s *__kernfs_new_node(kernfs_root_s *root,
				kernfs_node_s *parent,
				const char *name, umode_t mode,
				kuid_t uid, kgid_t gid,
				unsigned flags)
{
	kernfs_node_s *kn;

	kn = kmalloc(sizeof(kernfs_node_s));
	if (kn == NULL)
	return ERR_PTR(-ENOMEM);

	kn->name = name;
	kn->mode = mode;
	kn->flags = flags;

	// if (!uid_eq(uid, GLOBAL_ROOT_UID) || !gid_eq(gid, GLOBAL_ROOT_GID)) {
	// 	struct iattr iattr = {
	// 		.ia_valid = ATTR_UID | ATTR_GID,
	// 		.ia_uid = uid,
	// 		.ia_gid = gid,
	// 	};

	// 	ret = __kernfs_setattr(kn, &iattr);
	// 	if (ret < 0)
	// 		goto err_out3;
	// }

	return kn;
}

/**
 * kernfs_create_root - create a new kernfs hierarchy
 * @scops: optional syscall operations for the hierarchy
 * @flags: KERNFS_ROOT_* flags
 * @priv: opaque data associated with the new directory
 *
 * Returns the root of the new hierarchy on success, ERR_PTR() value on
 * failure.
 */
kernfs_root_s *kernfs_create_root(kernfs_syscall_ops_s *scops,
				unsigned int flags, void *priv)
{
	kernfs_root_s *root;
	kernfs_node_s *kn;

	root = kmalloc(sizeof(kernfs_root_s));
	if (root == NULL)
		return ERR_PTR(-ENOMEM);

	kn = __kernfs_new_node(root, NULL, "",
					S_IFDIR | S_IRUGO | S_IXUGO,
					GLOBAL_ROOT_UID, GLOBAL_ROOT_GID,
					KERNFS_DIR);
	if (kn == NULL) {
		kfree(root);
		return ERR_PTR(-ENOMEM);
	}

	kn->priv = priv;
	kn->dir.root = root;

	root->syscall_ops = scops;
	root->flags = flags;
	root->kn = kn;

	// if (!(root->flags & KERNFS_ROOT_CREATE_DEACTIVATED))
	// 	kernfs_activate(kn);

	return root;
}