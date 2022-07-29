#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/fcntl.h>
#include <linux/lib/string.h>
#include <linux/lib/errno.h>

#include <include/obsolete/proto.h>
#include <include/obsolete/printk.h>
#include <linux/fs/fs.h>

#include "include/arch_proto.h"
#include "include/archconst.h"

inline __always_inline bool verify_area(unsigned char *addr, unsigned long size)
{
	if(((unsigned long)addr + size) <= (unsigned long)USERADDR_LIMIT)
		return true;
	else
		return false;
}

inline __always_inline long copy_from_user(void *to, void *from, unsigned long size)
{
	unsigned long d0, d1;
	if(!verify_area(from, size))
		return -EFAULT;
	memcpy(to, from, size);
	return size;
}

inline __always_inline long copy_to_user(void *to, void *from, unsigned long size)
{
	unsigned long d0, d1;
	if(!verify_area(to, size))
		return -EFAULT;
	memcpy(to, from, size);
	return size;
}

inline __always_inline long strncpy_from_user(void *to, void *from, unsigned long size)
{
	if(!verify_area(from, size))
		return -EFAULT;

	strncpy(to, from, size);
	return	size;
}

inline __always_inline long strlen_user(void *src)
{
	unsigned long size = strlen(src);
	if(!verify_area(src, size))
		return -EFAULT;

	return size;
}

inline __always_inline long strnlen_user(void *src, unsigned long maxlen)
{
	unsigned long size = strlen(src);
	if(!verify_area(src, size))
		return -EFAULT;

	return size <= maxlen ? size : maxlen;
}