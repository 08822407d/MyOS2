/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_STAT_H
#define _LINUX_STAT_H

	#include <uapi/asm-generic/stat.h>
	#include <uapi/kernel/stat.h>

	#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
	#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
	#define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)
	#define S_IWUGO		(S_IWUSR|S_IWGRP|S_IWOTH)
	#define S_IXUGO		(S_IXUSR|S_IXGRP|S_IXOTH)

	#define UTIME_NOW	((1l << 30) - 1l)
	#define UTIME_OMIT	((1l << 30) - 2l)

	#include <linux/kernel/types.h>
	#include <linux/kernel/time64.h>
	#include <linux/kernel/uidgid.h>

	typedef struct kstat {
		u32			result_mask;	/* What fields the user got */
		umode_t		mode;
		u32			blksize;		/* Preferred I/O size */
		u64			attributes;
		u64			attributes_mask;
		unsigned int	nlink;
	#define KSTAT_ATTR_FS_IOC_FLAGS				\
				(								\
					STATX_ATTR_COMPRESSED |		\
					STATX_ATTR_IMMUTABLE |		\
					STATX_ATTR_APPEND |			\
					STATX_ATTR_NODUMP |			\
					STATX_ATTR_ENCRYPTED |		\
					STATX_ATTR_VERITY			\
				)				/* Attrs corresponding to FS_*_FL flags */
		u64			ino;
		dev_t		dev;
		dev_t		rdev;
		kuid_t		uid;
		kgid_t		gid;
		loff_t		size;
		u64			blocks;
		u64			mnt_id;
		timespec64_s	atime;
		timespec64_s	mtime;
		timespec64_s	ctime;
		timespec64_s	btime;	/* File creation time */
	} kstat_s;

#endif