#ifndef _SYS_VFS_H_
#define _SYS_VFS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <arch/amd64/types.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

	#define VFS_MAX_PATH		(1024)
	#define	VFS_MAX_NAME		(256)
	#define VFS_MAX_FD			(256)
	#define VFS_NODE_HASH_SIZE	(256)

	#define O_RDONLY			(1 << 0)
	#define O_WRONLY			(1 << 1)
	#define O_RDWR				(O_RDONLY | O_WRONLY)
	#define O_ACCMODE			(O_RDWR)

	#define O_CREAT				(1 << 8)
	#define O_EXCL				(1 << 9)
	#define O_NOCTTY			(1 << 10)
	#define O_TRUNC				(1 << 11)
	#define O_APPEND			(1 << 12)
	#define O_DSYNC				(1 << 13)
	#define O_NONBLOCK			(1 << 14)
	#define O_SYNC				(1 << 15)

	int vfs_mount_count(void);
	int vfs_open(const char * path, uint32_t flags, uint32_t mode);
	int vfs_close(int fd);
	uint64_t vfs_read(int fd, const void * buf, uint64_t len);
	uint64_t vfs_write(int fd, const void * buf, uint64_t len);
	int64_t vfs_lseek(int fd, int64_t off, int whence);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_VFS_H_ */