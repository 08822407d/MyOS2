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

	#define VFS_SEEK_SET		(0)
	#define VFS_SEEK_CUR		(1)
	#define VFS_SEEK_END		(2)

	struct vfs_stat_t;
	struct vfs_dirent_t;
	struct vfs_node_t;
	struct vfs_mount_t;
	struct filesystem_t;

	struct vfs_stat_t {
		uint64_t st_ino;
		int64_t st_size;
		uint32_t st_mode;
		uint64_t st_dev;
		uint32_t st_uid;
		uint32_t st_gid;
		uint64_t st_ctime;
		uint64_t st_atime;
		uint64_t st_mtime;
	};

	enum vfs_dirent_type_t {
		VDT_UNK,
		VDT_DIR,
		VDT_CHR,
		VDT_BLK,
		VDT_REG,
		VDT_LNK,
		VDT_FIFO,
		VDT_SOCK,
	};

	struct vfs_dirent_t {
		uint64_t d_off;
		uint32_t d_reclen;
		enum vfs_dirent_type_t d_type;
		char d_name[VFS_MAX_NAME];
	};

	enum vfs_node_flag_t {
		VNF_NONE,
		VNF_ROOT,
	};

	enum vfs_node_type_t {
		VNT_UNK,
		VNT_DIR,
		VNT_CHR,
		VNT_BLK,
		VNT_REG,
		VNT_LNK,
		VNT_FIFO,
		VNT_SOCK,
	};

	struct vfs_node_t {
		// struct list_head v_link;
		struct vfs_mount_t * v_mount;
		atomic_t v_refcnt;
		char v_path[VFS_MAX_PATH];
		enum vfs_node_flag_t v_flags;
		enum vfs_node_type_t v_type;
		// struct mutex_t v_lock;
		uint64_t v_ctime;
		uint64_t v_atime;
		uint64_t v_mtime;
		uint32_t v_mode;
		int64_t v_size;
		void * v_data;
	};

	enum {
		MOUNT_RW	= (0x0 << 0),
		MOUNT_RO	= (0x1 << 0),
		MOUNT_MASK	= (0x1 << 0),
	};

	struct vfs_mount_t {
		// struct list_head m_link;
		struct filesystem_t * m_fs;
		void * m_dev;
		char m_path[VFS_MAX_PATH];
		uint32_t m_flags;
		atomic_t m_refcnt;
		struct vfs_node_t * m_root;
		struct vfs_node_t * m_covered;
		// struct mutex_t m_lock;
		void * m_data;
	};

	struct filesystem_t {
		struct kobj_t * kobj;
		// struct list_head list;
		const char * name;

		int (*mount)(struct vfs_mount_t *, const char *);
		int (*unmount)(struct vfs_mount_t *);
		int (*msync)(struct vfs_mount_t *);
		int (*vget)(struct vfs_mount_t *, struct vfs_node_t *);
		int (*vput)(struct vfs_mount_t *, struct vfs_node_t *);

		uint64_t (*read)(struct vfs_node_t *, int64_t, void *, uint64_t);
		uint64_t (*write)(struct vfs_node_t *, int64_t, void *, uint64_t);
		int (*truncate)(struct vfs_node_t *, int64_t);
		int (*sync)(struct vfs_node_t *);
		int (*readdir)(struct vfs_node_t *, int64_t, struct vfs_dirent_t *);
		int (*lookup)(struct vfs_node_t *, const char *, struct vfs_node_t *);
		int (*create)(struct vfs_node_t *, const char *, uint32_t);
		int (*remove)(struct vfs_node_t *, struct vfs_node_t *, const char *);
		int (*rename)(struct vfs_node_t *, const char *, struct vfs_node_t *, struct vfs_node_t *, const char *);
		int (*mkdir)(struct vfs_node_t *, const char *, uint32_t);
		int (*rmdir)(struct vfs_node_t *, struct vfs_node_t *, const char *);
		int (*chmod)(struct vfs_node_t *, uint32_t);
	};

	extern struct list_head __filesystem_list;

	struct filesystem_t * search_filesystem(const char * name);
	bool register_filesystem(struct filesystem_t * fs);
	bool unregister_filesystem(struct filesystem_t * fs);

	void vfs_force_unmount(struct vfs_mount_t * m);
	int vfs_mount(const char * dev, const char * dir, const char * fsname, uint32_t flags);
	int vfs_unmount(const char * path);
	int vfs_sync(void);
	struct vfs_mount_t * vfs_mount_get(int index);
	int vfs_mount_count(void);
	int vfs_open(const char * path, uint32_t flags, uint32_t mode);
	int vfs_close(int fd);
	uint64_t vfs_read(int fd, const void * buf, uint64_t len);
	uint64_t vfs_write(int fd, const void * buf, uint64_t len);
	int64_t vfs_lseek(int fd, int64_t off, int whence);
	int vfs_fsync(int fd);
	int vfs_fchmod(int fd, uint32_t mode);
	int vfs_fstat(int fd, struct vfs_stat_t * st);
	int vfs_opendir(const char * name);
	int vfs_closedir(int fd);
	int vfs_readdir(int fd, struct vfs_dirent_t * dir);
	int vfs_rewinddir(int fd);
	int vfs_mkdir(const char * path, uint32_t mode);
	int vfs_rmdir(const char * path);
	int vfs_rename(const char * src, const char * dst);
	int vfs_unlink(const char * path);
	int vfs_access(const char * path, uint32_t mode);
	int vfs_chmod(const char * path, uint32_t mode);
	int vfs_stat(const char * path, struct vfs_stat_t * st);

	void do_init_vfs(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_VFS_H_ */