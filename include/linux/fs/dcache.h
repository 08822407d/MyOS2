#ifndef _LINUX_DCACHE_H_
#define _LINUX_DCACHE_H_

#include <stddef.h>
#include <lib/utils.h>

// #include "vfs_s_defs.h"

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
		List_s			dirent_list;

		unsigned int	d_flags;		/* protected by d_lock */
		qstr_s			d_name;

		dentry_s *		d_parent;
		List_hdr_s		childdir_lhdr;

		inode_s *		d_inode;
		dentry_ops_s *	dir_ops;
	} dentry_s;

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
	#define DCACHE_MANAGED_DENTRY \
		(DCACHE_MOUNTED|DCACHE_NEED_AUTOMOUNT|DCACHE_MANAGE_TRANSIT)

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


	dentry_s * __d_lookup(dentry_s * parent, qstr_s * name);
	dentry_s * __d_alloc(qstr_s * name);


	/*
	* Directory cache entry type accessor functions.
	*/
	static inline unsigned __d_entry_type(const dentry_s *dentry)
	{
		return dentry->d_flags & DCACHE_ENTRY_TYPE;
	}

	static inline bool d_is_miss(const dentry_s *dentry)
	{
		return __d_entry_type(dentry) == DCACHE_MISS_TYPE;
	}

	static inline bool d_is_whiteout(const dentry_s *dentry)
	{
		return __d_entry_type(dentry) == DCACHE_WHITEOUT_TYPE;
	}

	static inline bool d_can_lookup(const dentry_s *dentry)
	{
		return __d_entry_type(dentry) == DCACHE_DIRECTORY_TYPE;
	}

	static inline bool d_is_autodir(const dentry_s *dentry)
	{
		return __d_entry_type(dentry) == DCACHE_AUTODIR_TYPE;
	}

	static inline bool d_is_dir(const dentry_s *dentry)
	{
		return d_can_lookup(dentry) || d_is_autodir(dentry);
	}

	static inline bool d_is_symlink(const dentry_s *dentry)
	{
		return __d_entry_type(dentry) == DCACHE_SYMLINK_TYPE;
	}

	static inline bool d_is_reg(const dentry_s *dentry)
	{
		return __d_entry_type(dentry) == DCACHE_REGULAR_TYPE;
	}

	static inline bool d_is_special(const dentry_s *dentry)
	{
		return __d_entry_type(dentry) == DCACHE_SPECIAL_TYPE;
	}

	static inline bool d_is_file(const dentry_s *dentry)
	{
		return d_is_reg(dentry) || d_is_special(dentry);
	}

	static inline bool d_is_negative(const dentry_s *dentry)
	{
		// TODO: check d_is_whiteout(dentry) also.
		return d_is_miss(dentry);
	}

	static inline bool d_flags_negative(unsigned flags)
	{
		return (flags & DCACHE_ENTRY_TYPE) == DCACHE_MISS_TYPE;
	}

	static inline bool d_is_positive(const dentry_s *dentry)
	{
		return !d_is_negative(dentry);
	}

#endif /* _LINUX_DCACHE_H_ */