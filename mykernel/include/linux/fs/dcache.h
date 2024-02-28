#ifndef _LINUX_DCACHE_H_
#define _LINUX_DCACHE_H_

	// #
	#include <linux/lib/list.h>
	#include <linux/kernel/math.h>
	// #include <linux/rculist.h>
	// #include <linux/rculist_bl.h>
	#include <linux/kernel/lock_ipc.h>
	// #include <linux/seqlock.h>
	#include <linux/kernel/cache.h>
	// #include <linux/rcupdate.h>
	// #include <linux/lockref.h>
	// #include <linux/stringhash.h>
	// #include <linux/wait.h>


	#include <linux/fs/vfs_s_defs.h>

	/*
	 * linux/include/linux/dcache.h
	 *
	 * Dirent cache data structures
	 *
	 * (C) Copyright 1997 Thomas Schoebel-Theuer,
	 * with heavy changes by Linus Torvalds
	 */

	// #define IS_ROOT(x) ((x) == (x)->d_parent)

	// /* The hash is always the low bits of hash_len */
	// #ifdef __LITTLE_ENDIAN
	// #	define HASH_LEN_DECLARE u32 hash; u32 len
	// #	define bytemask_from_count(cnt)	(~(~0ul << (cnt)*8))
	// #else
	// #	define HASH_LEN_DECLARE u32 len; u32 hash
	// #	define bytemask_from_count(cnt)	(~(~0ul >> (cnt)*8))
	// #endif

	/*
	 * "quick string" -- eases parameter passing, but more importantly
	 * saves "metadata" about the string (ie length and the hash).
	 *
	 * hash comes first so it snuggles against d_parent in the
	 * dentry.
	 */
	typedef struct qstr {
		size_t				len;
		const unsigned char	*name;
	} qstr_s;
	// #define QSTR_INIT(n,l) { { { .len = l } }, .name = n }

	// extern const qstr_s	empty_name;
	// extern const qstr_s	slash_name;
	// extern const qstr_s	dotdot_name;

	/*
	 * Try to keep dentry_s aligned on 64 byte cachelines (this will
	 * give reasonable cacheline footprint with larger lines without the
	 * large memory footprint increase).
	 */
	#	define DNAME_INLINE_LEN	32	/* 192 bytes */

	// #define d_lock	d_lockref.lock

	typedef struct dentry
	{
		/* RCU lookup touched fields */
		unsigned int 		d_flags;		/* protected by d_lock */
		// seqcount_spinlock_t d_seq;		/* per dentry seqlock */
		// hlist_bl_node_s d_hash;			/* lookup hash list */
		dentry_s			*d_parent;		/* parent directory */
		qstr_s				d_name;
		inode_s				*d_inode;		/* Where the name belongs to - NULL is
											* negative */
		// unsigned char		d_iname[DNAME_INLINE_LEN];	/* small names */

		/* Ref lookup also touches following */
		// lockref_s		d_lockref;			/* per-dentry lock and refcount */
		const dentry_ops_s	*d_op;
		super_block_s		*d_sb;			/* The root of the dentry tree */
		// unsigned long		d_time;			/* used by d_revalidate */
		// void				*d_fsdata;		/* fs-specific data */

		List_s				d_child;		/* child of parent list */
		List_hdr_s			d_subdirs;		/* our children */
	} dentry_s;

	typedef struct dentry_operations
	{
		int		(*d_revalidate)(dentry_s *, unsigned int);
		int		(*d_weak_revalidate)(dentry_s *, unsigned int);
		int		(*d_hash)(const dentry_s *, qstr_s *);
		int		(*d_compare)(const dentry_s *dir, unsigned int len,
						const char *str, const qstr_s * name);
		int		(*d_delete)(const dentry_s *);
		int		(*d_init)(dentry_s *);
		void	(*d_release)(dentry_s *);
		void	(*d_prune)(dentry_s *);
		void	(*d_iput)(dentry_s *, inode_s *);
		char	*(*d_dname)(dentry_s *, char *, int);
		int		(*d_manage)(const path_s *, bool);
		dentry_s	*(*d_real)(dentry_s *, const inode_s *);
		vfsmount_s	*(*d_automount)(path_s *);
	} dentry_ops_s;

	/*
	 * Locking rules for dentry_operations callbacks are to be found in
	 * Documentation/filesystems/locking.rst. Keep it updated!
	 *
	 * FUrther descriptions are found in Documentation/filesystems/vfs.rst.
	 * Keep it updated too!
	 */

	/* d_flags entries */
	#define DCACHE_OP_HASH				0x00000001
	#define DCACHE_OP_COMPARE			0x00000002
	#define DCACHE_OP_REVALIDATE		0x00000004
	#define DCACHE_OP_DELETE			0x00000008
	#define DCACHE_OP_PRUNE				0x00000010

	#define	DCACHE_DISCONNECTED			0x00000020
		/* This dentry is possibly not currently connected to the dcache tree, in
		* which case its parent will either be itself, or will have this flag as
		* well.  nfsd will not use a dentry with this bit set, but will first
		* endeavour to clear the bit either by discovering that it is connected,
		* or by performing lookup operations.   Any filesystem which supports
		* nfsd_operations MUST have a lookup function which, if it finds a
		* directory inode with a DCACHE_DISCONNECTED dentry, will d_move that
		* dentry into place and return that dentry rather than the passed one,
		* typically using d_splice_alias. */

	#define DCACHE_REFERENCED			0x00000040 /* Recently used, don't discard. */

	#define DCACHE_DONTCACHE			0x00000080 /* Purge from memory on final dput() */

	#define DCACHE_CANT_MOUNT			0x00000100
	#define DCACHE_GENOCIDE				0x00000200
	#define DCACHE_SHRINK_LIST			0x00000400

	#define DCACHE_OP_WEAK_REVALIDATE	0x00000800

	#define DCACHE_NFSFS_RENAMED		0x00001000
		/* this dentry has been "silly renamed" and has to be deleted on the last
		* dput() */
	#define DCACHE_COOKIE				0x00002000 /* For use by dcookie subsystem */
	#define DCACHE_FSNOTIFY_PARENT_WATCHED	0x00004000
		/* Parent inode is watched by some fsnotify listener */

	#define DCACHE_DENTRY_KILLED		0x00008000

	#define DCACHE_MOUNTED				0x00010000 /* is a mountpoint */
	#define DCACHE_NEED_AUTOMOUNT		0x00020000 /* handle automount on this dir */
	#define DCACHE_MANAGE_TRANSIT		0x00040000 /* manage transit from this dirent */
	#define DCACHE_MANAGED_DENTRY (							\
				DCACHE_MOUNTED | DCACHE_NEED_AUTOMOUNT |	\
				DCACHE_MANAGE_TRANSIT						\
			)

	#define DCACHE_LRU_LIST				0x00080000

	#define DCACHE_ENTRY_TYPE			0x00700000
	#define DCACHE_MISS_TYPE			0x00000000 /* Negative dentry (maybe fallthru to nowhere) */
	#define DCACHE_WHITEOUT_TYPE		0x00100000 /* Whiteout dentry (stop pathwalk) */
	#define DCACHE_DIRECTORY_TYPE		0x00200000 /* Normal directory */
	#define DCACHE_AUTODIR_TYPE			0x00300000 /* Lookupless directory (presumed automount) */
	#define DCACHE_REGULAR_TYPE			0x00400000 /* Regular file type (or fallthru to such) */
	#define DCACHE_SPECIAL_TYPE			0x00500000 /* Other file type (or fallthru to such) */
	#define DCACHE_SYMLINK_TYPE			0x00600000 /* Symlink (or fallthru to such) */

	#define DCACHE_MAY_FREE				0x00800000
	#define DCACHE_FALLTHRU				0x01000000 /* Fall through to lower layer */
	#define DCACHE_NOKEY_NAME			0x02000000 /* Encrypted name encoded without key */
	#define DCACHE_OP_REAL				0x04000000

	#define DCACHE_PAR_LOOKUP			0x10000000 /* being looked up (with parent locked shared) */
	#define DCACHE_DENTRY_CURSOR		0x20000000
	#define DCACHE_NORCU				0x40000000 /* No RCU delay for freeing */

	// extern seqlock_t rename_lock;

	/*
	 * These are the low-level FS interfaces to the dcache..
	 */
	extern void d_instantiate(dentry_s *, inode_s *);
	// extern void d_instantiate_new(dentry_s *, inode_s *);
	// extern dentry_s *d_instantiate_unique(dentry_s *, inode_s *);
	// extern dentry_s *d_instantiate_anon(dentry_s *, inode_s *);
	// extern void __d_drop(dentry_s *dentry);
	// extern void d_drop(dentry_s *dentry);
	// extern void d_delete(dentry_s *);
	extern void d_set_d_op(dentry_s *dentry, const dentry_ops_s *op);

	// /* allocate/de-allocate */
	extern dentry_s *d_alloc(dentry_s *, const qstr_s *);
	// extern dentry_s *d_alloc_anon(struct super_block *);
	// extern dentry_s *d_alloc_parallel(dentry_s *, const qstr_s *,
	// 					wait_queue_head_t *);
	// extern dentry_s *d_splice_alias(inode_s *, dentry_s *);
	// extern dentry_s *d_add_ci(dentry_s *, inode_s *, qstr_s *);
	// extern dentry_s *d_exact_alias(dentry_s *, inode_s *);
	// extern dentry_s *d_find_any_alias(inode_s *inode);
	// extern dentry_s *d_obtain_alias(inode_s *);
	// extern dentry_s *d_obtain_root(inode_s *);
	// extern void shrink_dcache_sb(struct super_block *);
	// extern void shrink_dcache_parent(dentry_s *);
	// extern void shrink_dcache_for_umount(struct super_block *);
	// extern void d_invalidate(dentry_s *);

	// /* only used at mount-time */
	extern dentry_s *d_make_root(inode_s *);

	// /* <clickety>-<click> the ramfs-type tree */
	// extern void d_genocide(dentry_s *);

	// extern void d_tmpfile(dentry_s *, inode_s *);

	// extern dentry_s *d_find_alias(inode_s *);
	// extern void d_prune_aliases(inode_s *);

	// extern dentry_s *d_find_alias_rcu(inode_s *);

	// /* test whether we have any submounts in a subdir tree */
	// extern int path_has_submounts(const struct path *);

	// /*
	// * This adds the entry to the hash queues.
	// */
	// extern void d_rehash(dentry_s *);
	
	// extern void d_add(dentry_s *, inode_s *);

	// /* used for rename() and baskets */
	// extern void d_move(dentry_s *, dentry_s *);
	// extern void d_exchange(dentry_s *, dentry_s *);
	// extern dentry_s *d_ancestor(dentry_s *, dentry_s *);

	// /* appendix may either be NULL or be used for transname suffixes */
	// extern dentry_s *d_lookup(const dentry_s *, const qstr_s *);
	// extern dentry_s *d_hash_and_lookup(dentry_s *, qstr_s *);
	extern dentry_s *__d_lookup(const dentry_s *, const qstr_s *);
	// extern dentry_s *__d_alloc(super_block_s *sb, const qstr_s * name);
	extern dentry_s *__myos_d_alloc(super_block_s *sb, const qstr_s * name);
	// extern dentry_s *__d_lookup_rcu(const dentry_s *parent,
	// 				const qstr_s *name, unsigned *seq);

	// static inline unsigned d_count(const dentry_s *dentry)
	// {
	// 	return dentry->d_lockref.count;
	// }

	// /*
	// * helper function for dentry_operations.d_dname() members
	// */
	// extern __printf(4, 5)
	// char *dynamic_dname(dentry_s *, char *, int, const char *, ...);

	// extern char *__d_path(const struct path *, const struct path *, char *, int);
	// extern char *d_absolute_path(const struct path *, char *, int);
	// extern char *d_path(const struct path *, char *, int);
	// extern char *dentry_path_raw(const dentry_s *, char *, int);
	// extern char *dentry_path(const dentry_s *, char *, int);

	// /* Allocation counts.. */

	// /**
	//  *	dget, dget_dlock -	get a reference to a dentry
	// *	@dentry: dentry to get a reference to
	// *
	// *	Given a dentry or %NULL pointer increment the reference count
	// *	if appropriate and return the dentry. A dentry will not be 
	// *	destroyed when it has references.
	// */
	// static inline dentry_s *dget_dlock(dentry_s *dentry)
	// {
	// 	if (dentry)
	// 		dentry->d_lockref.count++;
	// 	return dentry;
	// }

	static inline dentry_s *
	dget(dentry_s *dentry) {
		// if (dentry)
		// 	lockref_get(&dentry->d_lockref);
		return dentry;
	}

	// extern dentry_s *dget_parent(dentry_s *dentry);

	// /**
	//  *	d_unhashed -	is dentry hashed
	// *	@dentry: entry to check
	// *
	// *	Returns true if the dentry passed is not currently hashed.
	// */
	
	// static inline int d_unhashed(const dentry_s *dentry)
	// {
	// 	return hlist_bl_unhashed(&dentry->d_hash);
	// }

	// static inline int d_unlinked(const dentry_s *dentry)
	// {
	// 	return d_unhashed(dentry) && !IS_ROOT(dentry);
	// }

	// static inline int cant_mount(const dentry_s *dentry)
	// {
	// 	return (dentry->d_flags & DCACHE_CANT_MOUNT);
	// }

	// static inline void dont_mount(dentry_s *dentry)
	// {
	// 	spin_lock(&dentry->d_lock);
	// 	dentry->d_flags |= DCACHE_CANT_MOUNT;
	// 	spin_unlock(&dentry->d_lock);
	// }

	// extern void __d_lookup_done(dentry_s *);

	// static inline int d_in_lookup(const dentry_s *dentry)
	// {
	// 	return dentry->d_flags & DCACHE_PAR_LOOKUP;
	// }

	// static inline void d_lookup_done(dentry_s *dentry)
	// {
	// 	if (unlikely(d_in_lookup(dentry))) {
	// 		spin_lock(&dentry->d_lock);
	// 		__d_lookup_done(dentry);
	// 		spin_unlock(&dentry->d_lock);
	// 	}
	// }

	extern void dput(dentry_s *);

	// static inline bool d_managed(const dentry_s *dentry)
	// {
	// 	return dentry->d_flags & DCACHE_MANAGED_DENTRY;
	// }

	// static inline bool d_mountpoint(const dentry_s *dentry)
	// {
	// 	return dentry->d_flags & DCACHE_MOUNTED;
	// }

	/*
	 * Directory cache entry type accessor functions.
	 */
	static inline unsigned
	__d_entry_type(const dentry_s *dentry) {
		return dentry->d_flags & DCACHE_ENTRY_TYPE;
	}

	static inline bool
	d_is_miss(const dentry_s *dentry) {
		return __d_entry_type(dentry) == DCACHE_MISS_TYPE;
	}

	static inline bool
	d_is_whiteout(const dentry_s *dentry) {
		return __d_entry_type(dentry) == DCACHE_WHITEOUT_TYPE;
	}

	static inline bool
	d_can_lookup(const dentry_s *dentry) {
		return __d_entry_type(dentry) == DCACHE_DIRECTORY_TYPE;
	}

	static inline bool
	d_is_autodir(const dentry_s *dentry) {
		return __d_entry_type(dentry) == DCACHE_AUTODIR_TYPE;
	}

	static inline bool
	d_is_dir(const dentry_s *dentry) {
		return d_can_lookup(dentry) || d_is_autodir(dentry);
	}

	// static inline bool d_is_symlink(const dentry_s *dentry)
	// {
	// 	return __d_entry_type(dentry) == DCACHE_SYMLINK_TYPE;
	// }

	// static inline bool d_is_reg(const dentry_s *dentry)
	// {
	// 	return __d_entry_type(dentry) == DCACHE_REGULAR_TYPE;
	// }

	// static inline bool d_is_special(const dentry_s *dentry)
	// {
	// 	return __d_entry_type(dentry) == DCACHE_SPECIAL_TYPE;
	// }

	// static inline bool d_is_file(const dentry_s *dentry)
	// {
	// 	return d_is_reg(dentry) || d_is_special(dentry);
	// }

	// static inline bool d_is_negative(const dentry_s *dentry)
	// {
	// 	// TODO: check d_is_whiteout(dentry) also.
	// 	return d_is_miss(dentry);
	// }

	// static inline bool d_flags_negative(unsigned flags)
	// {
	// 	return (flags & DCACHE_ENTRY_TYPE) == DCACHE_MISS_TYPE;
	// }

	// static inline bool d_is_positive(const dentry_s *dentry)
	// {
	// 	return !d_is_negative(dentry);
	// }

#endif /* _LINUX_DCACHE_H_ */