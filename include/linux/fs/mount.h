/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/kernel/mount.h>
// #include <linux/seq_file.h>
// #include <linux/poll.h>
// #include <linux/ns_common.h>
// #include <linux/fs_pin.h>

#include <linux/fs/vfs_s_defs.h>

// struct mnt_namespace {
// 	struct ns_common	ns;
// 	struct mount *	root;
// 	/*
// 	 * Traversal and modification of .list is protected by either
// 	 * - taking namespace_sem for write, OR
// 	 * - taking namespace_sem for read AND taking .ns_lock.
// 	 */
// 	struct list_head	list;
// 	spinlock_t		ns_lock;
// 	struct user_namespace	*user_ns;
// 	struct ucounts		*ucounts;
// 	u64			seq;	/* Sequence number to prevent loops */
// 	wait_queue_head_t poll;
// 	u64 event;
// 	unsigned int		mounts; /* # of mounts in the namespace */
// 	unsigned int		pending_mounts;
// } __randomize_layout;

// struct mnt_pcp {
// 	int mnt_count;
// 	int mnt_writers;
// };

typedef struct mountpoint {
	// hlist_node_s m_hash;
	dentry_s *m_dentry;
	// hlist_head_s m_list;
	int m_count;
} mountpoint_s;

typedef struct mount {
	// hlist_node_s mnt_hash;
	mount_s		*mnt_parent;
	dentry_s	*mnt_mountpoint;
	vfsmount_s	mnt;
	// union {
	// 	struct rcu_head mnt_rcu;
	// 	struct llist_node mnt_llist;
	// };
#ifdef CONFIG_SMP
	struct mnt_pcp __percpu *mnt_pcp;
#else
	int			mnt_count;
	int			mnt_writers;
#endif
	List_hdr_s	mnt_mounts;		/* list of children, anchored here */
	List_s		mnt_child;		/* and going through their mnt_child */
	// list_head_s	mnt_instance;	/* mount instance on sb->s_mounts */
	const char	*mnt_devname;	/* Name of device e.g. /dev/dsk/hda1 */
	// list_head_s mnt_list;
	// list_head_s mnt_expire;		/* link in fs-specific expiry list */
	// list_head_s mnt_share;		/* circular list of shared mounts */
	// list_head_s mnt_slave_list;	/* list of slave mounts */
	// list_head_s mnt_slave;		/* slave list entry */
	mount_s		*mnt_master;		/* slave is on master->mnt_slave_list */
	// mnt_namespace_s *mnt_ns;		/* containing namespace */
	mountpoint_s	*mnt_mp;	/* where is it mounted */
	// union {
	// 	hlist_node_s mnt_mp_list;	/* list mounts with the same mountpoint */
	// 	hlist_node_s mnt_umount;
	// };
	// list_head_s mnt_umounting; 	/* list entry for umount propagation */
#ifdef CONFIG_FSNOTIFY
	// struct fsnotify_mark_connector __rcu *mnt_fsnotify_marks;
	// __u32 mnt_fsnotify_mask;
#endif
	int mnt_id;					/* mount identifier */
	int mnt_group_id;			/* peer group identifier */
	int mnt_expiry_mark;		/* true if marked for expiry */
	// hlist_head_s mnt_pins;
	// hlist_head_s mnt_stuck_children;
} mount_s;

#define MNT_NS_INTERNAL ERR_PTR(-EINVAL) /* distinct from any mnt_namespace */

static inline mount_s *real_mount(vfsmount_s *mnt)
{
	return container_of(mnt, struct mount, mnt);
}

static inline int mnt_has_parent(mount_s *mnt)
{
	return mnt != mnt->mnt_parent;
}

// static inline int is_mounted(vfsmount_s *mnt)
// {
// 	/* neither detached nor internal? */
// 	return !IS_ERR_OR_NULL(real_mount(mnt)->mnt_ns);
// }

// extern mount_s *__lookup_mnt(vfsmount_s *, dentry_s *);

// extern int __legitimize_mnt(vfsmount_s *, unsigned);
// extern bool legitimize_mnt(vfsmount_s *, unsigned);

// static inline bool __path_is_mountpoint(const path_s *path)
// {
// 	mount_s *m = __lookup_mnt(path->mnt, path->dentry);
// 	return m && !(m->mnt.mnt_flags & MNT_SYNC_UMOUNT);
// }

// extern void __detach_mounts(dentry_s *dentry);

// static inline void detach_mounts(dentry_s *dentry)
// {
// 	if (!d_mountpoint(dentry))
// 		return;
// 	__detach_mounts(dentry);
// }

// static inline void get_mnt_ns(mnt_namespace_s *ns)
// {
// 	refcount_inc(&ns->ns.count);
// }

// extern seqlock_t mount_lock;

// struct proc_mounts {
// 	mnt_namespace_s *ns;
// 	path_s root;
// 	int (*show)(seq_file_s *, vfsmount_s *);
// 	mount_s cursor;
// };

// extern const seq_operations_s mounts_op;

// extern bool __is_local_mountpoint(dentry_s *dentry);
// static inline bool is_local_mountpoint(dentry_s *dentry)
// {
// 	if (!d_mountpoint(dentry))
// 		return false;

// 	return __is_local_mountpoint(dentry);
// }

// static inline bool is_anon_ns(mnt_namespace_s *ns)
// {
// 	return ns->seq == 0;
// }

// extern void mnt_cursor_del(mnt_namespace_s *ns, mount_s *cursor);