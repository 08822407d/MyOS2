// SPDX-License-Identifier: GPL-2.0-or-later
/* Provide a way to create a superblock configuration context within the kernel
 * that allows a superblock to be set up prior to mounting.
 *
 * Copyright (C) 2017 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
// #include <linux/module.h>
#include <linux/fs/fs_context.h>
// #include <linux/fs_parser.h>
#include <linux/fs/fs.h>
#include <linux/kernel/mount.h>
// #include <linux/nsproxy.h>
// #include <linux/slab.h>
// #include <linux/magic.h>
// #include <linux/security.h>
// #include <linux/mnt_namespace.h>
// #include <linux/pid_namespace.h>
// #include <linux/user_namespace.h>
// #include <net/net_namespace.h>
// #include <asm/sections.h>
#include <linux/fs/mount.h>
#include <linux/fs/internal.h>


#include <linux/fs/fs.h>
#include <linux/lib/string.h>
#include <obsolete/proto.h>

enum legacy_fs_param {
	LEGACY_FS_UNSET_PARAMS,
	LEGACY_FS_MONOLITHIC_PARAMS,
	LEGACY_FS_INDIVIDUAL_PARAMS,
};

typedef struct legacy_fs_context {
	char			*legacy_data;	/* Data page for legacy filesystems */
	size_t			data_size;
	enum legacy_fs_param	param_type;
} legacy_fs_ctx_s;


static int legacy_init_fs_context(fs_ctxt_s *fc);

/**
 * alloc_fs_context - Create a filesystem context.
 * @fs_type: The filesystem type.
 * @reference: The dentry from which this one derives (or NULL)
 * @sb_flags: Filesystem/superblock flags (SB_*)
 * @sb_flags_mask: Applicable members of @sb_flags
 * @purpose: The purpose that this configuration shall be used for.
 *
 * Open a filesystem and create a mount context.  The mount context is
 * initialised with the supplied flags and, if a submount/automount from
 * another superblock (referred to by @reference) is supplied, may have
 * parameters such as namespaces copied across from that superblock.
 */
static fs_ctxt_s *alloc_fs_context(fs_type_s *fs_type, dentry_s *reference,
				unsigned int sb_flags, unsigned int sb_flags_mask,
				enum fs_context_purpose purpose)
{
	int (*init_fs_context)(fs_ctxt_s *);
	fs_ctxt_s *fc;
	int ret = -ENOMEM;

	fc = kzalloc(sizeof(fs_ctxt_s));
	if (fc == NULL)
		return ERR_PTR(-ENOMEM);

	fc->purpose			= purpose;
	fc->sb_flags		= sb_flags;
	fc->sb_flags_mask	= sb_flags_mask;
	fc->fs_type			= fs_type;

	switch (purpose) {
	case FS_CONTEXT_FOR_MOUNT:
		// fc->user_ns = get_user_ns(fc->cred->user_ns);
		break;
	case FS_CONTEXT_FOR_SUBMOUNT:
		// fc->user_ns = get_user_ns(reference->d_sb->s_user_ns);
		break;
	case FS_CONTEXT_FOR_RECONFIGURE:
		// fc->user_ns = get_user_ns(reference->d_sb->s_user_ns);
		fc->root = dget(reference);
		break;
	}

	/* TODO: Make all filesystems support this unconditionally */
	init_fs_context = fc->fs_type->init_fs_context;
	if (init_fs_context == NULL)
		init_fs_context = legacy_init_fs_context;

	ret = init_fs_context(fc);
	if (ret < 0)
		goto err_fc;
	fc->need_free = true;
	return fc;

err_fc:
	put_fs_context(fc);
	return ERR_PTR(ret);
}

/**
 * put_fs_context - Dispose of a superblock configuration context.
 * @fc: The context to dispose of.
 */
void put_fs_context(fs_ctxt_s *fc)
{
	super_block_s *sb;

	if (fc->root) {
		sb = fc->root->d_sb;
		dput(fc->root);
		fc->root = NULL;
		// deactivate_super(sb);
	}

	if (fc->need_free && fc->ops && fc->ops->free)
		fc->ops->free(fc);

	put_filesystem(fc->fs_type);
	kfree((void *)fc->source);
	kfree(fc);
}


/*
 * Get a mountable root with the legacy mount command.
 */
static int legacy_get_tree(struct fs_context *fc)
{
	legacy_fs_ctx_s *ctx = fc->fs_private;
	dentry_s *root;

	root = fc->fs_type->mount(fc->fs_type, fc->sb_flags,
				      fc->source, ctx->legacy_data);
	if (IS_ERR(root))
		return PTR_ERR(root);

	fc->root = root;
	return 0;
}

const fs_ctxt_ops_s legacy_fs_context_ops = {
	// .free				= legacy_fs_context_free,
	// .dup				= legacy_fs_context_dup,
	// .parse_param		= legacy_parse_param,
	// .parse_monolithic	= legacy_parse_monolithic,
	.get_tree			= legacy_get_tree,
	// .reconfigure		= legacy_reconfigure,
};

/*
 * Initialise a legacy context for a filesystem that doesn't support
 * fs_context.
 */
static int legacy_init_fs_context(fs_ctxt_s *fc)
{
	fc->fs_private = kmalloc(sizeof(legacy_fs_ctx_s));
	if (fc->fs_private == NULL)
		return -ENOMEM;
	fc->ops = &legacy_fs_context_ops;
	return 0;
}

fs_ctxt_s *fs_context_for_mount(fs_type_s *fs_type,
				unsigned int sb_flags)
{
	return alloc_fs_context(fs_type, NULL, sb_flags, 0,
					FS_CONTEXT_FOR_MOUNT);
}