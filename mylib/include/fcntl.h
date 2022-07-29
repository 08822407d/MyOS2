/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_FCNTL_H
#define _UAPI_LINUX_FCNTL_H

// #include <uapi/asm-generic/fcntl.h>
#include "openat2.h"

	#define O_ACCMODE	00000003
	#define O_RDONLY	00000000
	#define O_WRONLY	00000001
	#define O_RDWR		00000002
	#ifndef O_CREAT
		#define O_CREAT		00000100	/* not fcntl */
	#endif
	#ifndef O_EXCL
		#define O_EXCL		00000200	/* not fcntl */
	#endif
	#ifndef O_NOCTTY
		#define O_NOCTTY	00000400	/* not fcntl */
	#endif
	#ifndef O_TRUNC
		#define O_TRUNC		00001000	/* not fcntl */
	#endif
	#ifndef O_APPEND
		#define O_APPEND	00002000
	#endif
	#ifndef O_NONBLOCK
		#define O_NONBLOCK	00004000
	#endif
	#ifndef O_DSYNC
		#define O_DSYNC		00010000	/* used to be O_SYNC, see below */
	#endif
	#ifndef FASYNC
		#define FASYNC		00020000	/* fcntl, for BSD compatibility */
	#endif
	#ifndef O_DIRECT
		#define O_DIRECT	00040000	/* direct disk access hint */
	#endif
	#ifndef O_LARGEFILE
		#define O_LARGEFILE	00100000
	#endif
	#ifndef O_DIRECTORY
		#define O_DIRECTORY	00200000	/* must be a directory */
	#endif
	#ifndef O_NOFOLLOW
		#define O_NOFOLLOW	00400000	/* don't follow links */
	#endif
	#ifndef O_NOATIME
		#define O_NOATIME	01000000
	#endif
	#ifndef O_CLOEXEC
		#define O_CLOEXEC	02000000	/* set close_on_exec */
	#endif

	/*
	* Before Linux 2.6.33 only O_DSYNC semantics were implemented, but using
	* the O_SYNC flag.  We continue to use the existing numerical value
	* for O_DSYNC semantics now, but using the correct symbolic name for it.
	* This new value is used to request true Posix O_SYNC semantics.  It is
	* defined in this strange way to make sure applications compiled against
	* new headers get at least O_DSYNC semantics on older kernels.
	*
	* This has the nice side-effect that we can simply test for O_DSYNC
	* wherever we do not care if O_DSYNC or O_SYNC is used.
	*
	* Note: __O_SYNC must never be used directly.
	*/
	#ifndef O_SYNC
		#define __O_SYNC	04000000
		#define O_SYNC		(__O_SYNC|O_DSYNC)
	#endif

	#ifndef O_PATH
		#define O_PATH		010000000
	#endif

	#ifndef __O_TMPFILE
		#define __O_TMPFILE	020000000
	#endif

	/* a horrid kludge trying to make sure that this will fail on old kernels */
	#define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
	#define O_TMPFILE_MASK (__O_TMPFILE | O_DIRECTORY | O_CREAT)      

	#ifndef O_NDELAY
		#define O_NDELAY	O_NONBLOCK
	#endif

	#define F_DUPFD			0	/* dup */
	#define F_GETFD			1	/* get close_on_exec */
	#define F_SETFD			2	/* set/clear close_on_exec */
	#define F_GETFL			3	/* get file->f_flags */
	#define F_SETFL			4	/* set file->f_flags */
	#ifndef F_GETLK
		#define F_GETLK			5
		#define F_SETLK			6
		#define F_SETLKW		7
	#endif
	#ifndef F_SETOWN
		#define F_SETOWN		8	/* for sockets. */
		#define F_GETOWN		9	/* for sockets. */
	#endif
	#ifndef F_SETSIG
		#define F_SETSIG		10	/* for sockets. */
		#define F_GETSIG		11	/* for sockets. */
	#endif

	#ifndef CONFIG_64BIT
		#ifndef F_GETLK64
			#define F_GETLK64		12	/*  using 'struct flock64' */
			#define F_SETLK64		13
			#define F_SETLKW64		14
		#endif
	#endif

	#ifndef F_SETOWN_EX
		#define F_SETOWN_EX		15
		#define F_GETOWN_EX		16
	#endif

	#ifndef F_GETOWNER_UIDS
		#define F_GETOWNER_UIDS		17
	#endif

	/*
	* Open File Description Locks
	*
	* Usually record locks held by a process are released on *any* close and are
	* not inherited across a fork().
	*
	* These cmd values will set locks that conflict with process-associated
	* record  locks, but are "owned" by the open file description, not the
	* process. This means that they are inherited across fork() like BSD (flock)
	* locks, and they are only released automatically when the last reference to
	* the the open file against which they were acquired is put.
	*/
	#define F_OFD_GETLK		36
	#define F_OFD_SETLK		37
	#define F_OFD_SETLKW	38

	#define F_OWNER_TID		0
	#define F_OWNER_PID		1
	#define F_OWNER_PGRP	2

	// struct f_owner_ex {
	// 	int	type;
	// 	__kernel_pid_t	pid;
	// };

	/* for F_[GET|SET]FL */
	#define FD_CLOEXEC		1	/* actually anything with low bit set goes */

	/* for posix fcntl() and lockf() */
	#ifndef F_RDLCK
		#define F_RDLCK			0
		#define F_WRLCK			1
		#define F_UNLCK			2
	#endif

	/* for old implementation of bsd flock () */
	#ifndef F_EXLCK
		#define F_EXLCK			4	/* or 3 */
		#define F_SHLCK			8	/* or 4 */
	#endif

	/* operations for bsd flock(), also used by the kernel implementation */
	#define LOCK_SH			1	/* shared lock */
	#define LOCK_EX			2	/* exclusive lock */
	#define LOCK_NB			4	/* or'd with one of the above to prevent
					blocking */
	#define LOCK_UN			8	/* remove lock */

	#define LOCK_MAND		32	/* This is a mandatory flock ... */
	#define LOCK_READ		64	/* which allows concurrent read operations */
	#define LOCK_WRITE		128	/* which allows concurrent write operations */
	#define LOCK_RW			192	/* which allows concurrent read & write ops */

	#define F_LINUX_SPECIFIC_BASE	1024


	#define F_SETLEASE		(F_LINUX_SPECIFIC_BASE + 0)
	#define F_GETLEASE		(F_LINUX_SPECIFIC_BASE + 1)

	/*
	* Cancel a blocking posix lock; internal use only until we expose an
	* asynchronous lock api to userspace:
	*/
	#define F_CANCELLK		(F_LINUX_SPECIFIC_BASE + 5)

	/* Create a file descriptor with FD_CLOEXEC set. */
	#define F_DUPFD_CLOEXEC	(F_LINUX_SPECIFIC_BASE + 6)

	/*
	* Request nofications on a directory.
	* See below for events that may be notified.
	*/
	#define F_NOTIFY		(F_LINUX_SPECIFIC_BASE+2)

	/*
	* Set and get of pipe page size array
	*/
	#define F_SETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 7)
	#define F_GETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 8)

	/*
	* Set/Get seals
	*/
	#define F_ADD_SEALS		(F_LINUX_SPECIFIC_BASE + 9)
	#define F_GET_SEALS		(F_LINUX_SPECIFIC_BASE + 10)

	/*
	* Types of seals
	*/
	#define F_SEAL_SEAL			0x0001	/* prevent further seals from being set */
	#define F_SEAL_SHRINK		0x0002	/* prevent file from shrinking */
	#define F_SEAL_GROW			0x0004	/* prevent file from growing */
	#define F_SEAL_WRITE		0x0008	/* prevent writes */
	#define F_SEAL_FUTURE_WRITE	0x0010  /* prevent future writes while mapped */
	/* (1U << 31) is reserved for signed error codes */

	/*
	* Set/Get write life time hints. {GET,SET}_RW_HINT operate on the
	* underlying inode, while {GET,SET}_FILE_RW_HINT operate only on
	* the specific file.
	*/
	#define F_GET_RW_HINT		(F_LINUX_SPECIFIC_BASE + 11)
	#define F_SET_RW_HINT		(F_LINUX_SPECIFIC_BASE + 12)
	#define F_GET_FILE_RW_HINT	(F_LINUX_SPECIFIC_BASE + 13)
	#define F_SET_FILE_RW_HINT	(F_LINUX_SPECIFIC_BASE + 14)

	/*
	* Valid hint values for F_{GET,SET}_RW_HINT. 0 is "not set", or can be
	* used to clear any hints previously set.
	*/
	#define RWH_WRITE_LIFE_NOT_SET	0
	#define RWH_WRITE_LIFE_NONE		1
	#define RWH_WRITE_LIFE_SHORT	2
	#define RWH_WRITE_LIFE_MEDIUM	3
	#define RWH_WRITE_LIFE_LONG		4
	#define RWH_WRITE_LIFE_EXTREME	5

	/*
	* The originally introduced spelling is remained from the first
	* versions of the patch set that introduced the feature, see commit
	* v4.13-rc1~212^2~51.
	*/
	#define RWF_WRITE_LIFE_NOT_SET	RWH_WRITE_LIFE_NOT_SET

	/*
	* Types of directory notifications that may be requested.
	*/
	#define DN_ACCESS		0x00000001	/* File accessed */
	#define DN_MODIFY		0x00000002	/* File modified */
	#define DN_CREATE		0x00000004	/* File created */
	#define DN_DELETE		0x00000008	/* File removed */
	#define DN_RENAME		0x00000010	/* File renamed */
	#define DN_ATTRIB		0x00000020	/* File changed attibutes */
	#define DN_MULTISHOT	0x80000000	/* Don't remove notifier */

	/*
	* The constants AT_REMOVEDIR and AT_EACCESS have the same value.  AT_EACCESS is
	* meaningful only to faccessat, while AT_REMOVEDIR is meaningful only to
	* unlinkat.  The two functions do completely different things and therefore,
	* the flags can be allowed to overlap.  For example, passing AT_REMOVEDIR to
	* faccessat would be undefined behavior and thus treating it equivalent to
	* AT_EACCESS is valid undefined behavior.
	*/
	#define AT_FDCWD				-100    /* Special value used to indicate
												openat should use the current
												working directory. */
	#define AT_SYMLINK_NOFOLLOW		0x100   /* Do not follow symbolic links.  */
	#define AT_EACCESS				0x200	/* Test access permitted for
												effective IDs, not real IDs.  */
	#define AT_REMOVEDIR			0x200   /* Remove directory instead of
												unlinking file.  */
	#define AT_SYMLINK_FOLLOW		0x400   /* Follow symbolic links.  */
	#define AT_NO_AUTOMOUNT			0x800	/* Suppress terminal automount traversal */
	#define AT_EMPTY_PATH			0x1000	/* Allow empty relative pathname */

	#define AT_STATX_SYNC_TYPE		0x6000	/* Type of synchronisation required from statx() */
	#define AT_STATX_SYNC_AS_STAT	0x0000	/* - Do whatever stat() does */
	#define AT_STATX_FORCE_SYNC		0x2000	/* - Force the attributes to be sync'd with the server */
	#define AT_STATX_DONT_SYNC		0x4000	/* - Don't sync attributes with the server */

	#define AT_RECURSIVE			0x8000	/* Apply to the entire subtree */

#endif /* _UAPI_LINUX_FCNTL_H */