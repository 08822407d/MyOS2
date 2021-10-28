#include <sys/cdefs.h>
#include <sys/fcntl.h>

#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

#include <include/vfs.h>
#include <include/proto.h>
#include <include/printk.h>

#include "include/arch_proto.h"
#include "include/archconst.h"

unsigned long kopen(char *filename, int flags)
{
	task_s * curr = curr_tsk;
	long error = 0;
	dirent_s * dentry = NULL;
	file_s * fp = NULL;
	file_s ** f = NULL;
	int fd = -1;
	int i;

	dentry = path_walk(filename, 0);

	if(dentry == NULL)
		return -ENOENT;

	if((flags & O_DIRECTORY) && (dentry->dir_inode->attribute != FS_ATTR_DIR))
		return -ENOTDIR;
	if(!(flags & O_DIRECTORY) && (dentry->dir_inode->attribute == FS_ATTR_DIR))
		return -EISDIR;

	fp = (file_s *)kmalloc(sizeof(file_s));
	memset(fp, 0, sizeof(struct file));
	fp->dentry = dentry;
	fp->mode = flags;

	fp->f_ops = dentry->dir_inode->f_ops;
	if(fp->f_ops && fp->f_ops->open)
		error = fp->f_ops->open(dentry->dir_inode, fp);
	if(error != 1)
	{
		kfree(fp);
		return -EFAULT;
	}

	if(fp->mode & O_TRUNC)
	{
		fp->dentry->dir_inode->file_size = 0;
	}
	if(fp->mode & O_APPEND)
	{
		fp->position = fp->dentry->dir_inode->file_size;
	}

	f = curr_tsk->fps;
	for(i = 0;i < MAX_FILE_NR;i++)
		if(f[i] == NULL)
		{
			fd = i;
			break;
		}
	if(i == MAX_FILE_NR)
	{
		kfree(fp);
		// reclaim struct index_node & struct dir_entry
		return -EMFILE;
	}
	f[fd] = fp;

	return fd;
}

inline __always_inline long verify_area(unsigned char* addr, unsigned long size)
{
	if(((unsigned long)addr + size) <= (unsigned long)USERADDR_LIMIT)
		return 1;
	else
		return 0;
}

inline __always_inline long copy_from_user(void * from, void * to, unsigned long size)
{
	unsigned long d0,d1;
	if(!verify_area(from,size))
		return 0;
	__asm__ __volatile__(	"rep			\n\t"
							"movsq			\n\t"
							"movq	%3,	%0	\n\t"
							"rep			\n\t"
							"movsb			\n\t"
						:	"=&c"(size), "=&D"(d0), "=&S"(d1)
						:	"r"(size & 7), "0"(size / 8), "1"(to), "2"(from)
						:	"memory"
						);
	return size;
}

inline __always_inline long copy_to_user(void * to, void * from, unsigned long size)
{
	unsigned long d0,d1;
	if(!verify_area(to,size))
		return 0;
	__asm__ __volatile__(	"rep			\n\t"
							"movsq			\n\t"
							"movq	%3,	%0	\n\t"
							"rep			\n\t"
							"movsb			\n\t"
						:	"=&c"(size), "=&D"(d0), "=&S"(d1)
						:	"r"(size & 7), "0"(size / 8), "1"(to), "2"(from)
						:	"memory"
						);
	return size;
}

inline __always_inline long strncpy_from_user(void * from, void * to, unsigned long size)
{
	if(!verify_area(from, size))
		return 0;

	strncpy(to, from, size);
	return	size;
}

inline __always_inline long strnlen_user(void * src, unsigned long maxlen)
{
	unsigned long size = strlen(src);
	if(!verify_area(src, size))
		return 0;

	return size <= maxlen ? size : maxlen;
}