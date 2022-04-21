/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_STAT_H
#define _LINUX_STAT_H

// #include <asm/stat.h>
#include <uapi/stat.h>

	#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
	#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
	#define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)
	#define S_IWUGO		(S_IWUSR|S_IWGRP|S_IWOTH)
	#define S_IXUGO		(S_IXUSR|S_IXGRP|S_IXOTH)

	#define UTIME_NOW	((1l << 30) - 1l)
	#define UTIME_OMIT	((1l << 30) - 2l)

#include <sys/types.h>
#include <sys/time64.h>
#include <sys/uidgid.h>

	typedef struct kstat {
		uint32_t		result_mask;	/* What fields the user got */
		umode_t			mode;
		unsigned int	nlink;
		uint32_t		blksize;/* Preferred I/O size */
		uint64_t		attributes;
		uint64_t		attributes_mask;
	#define KSTAT_ATTR_FS_IOC_FLAGS				\
				(STATX_ATTR_COMPRESSED |		\
				STATX_ATTR_IMMUTABLE |			\
				STATX_ATTR_APPEND |				\
				STATX_ATTR_NODUMP |				\
				STATX_ATTR_ENCRYPTED |			\
				STATX_ATTR_VERITY				\
				)				/* Attrs corresponding to FS_*_FL flags */
		uint64_t		ino;
		dev_t			dev;
		dev_t			rdev;
		kuid_t			uid;
		kgid_t			gid;
		loff_t			size;
		timespec64_s	atime;
		timespec64_s	mtime;
		timespec64_s	ctime;
		timespec64_s	btime;	/* File creation time */
		uint64_t		blocks;
		uint64_t		mnt_id;
	} kstat_s;

#endif