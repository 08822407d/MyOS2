#include <linux/kernel/types.h>
#include <linux/lib/string.h>
#include <linux/lib/errno.h>
#include <linux/kernel/uaccess.h>

#include <obsolete/arch_proto.h>

inline __always_inline long
strncpy_from_user(void *to, void *from, ulong size) {
	if (!access_ok(from, size))
		return -EFAULT;

	strncpy(to, from, size);
	return	size;
}

inline __always_inline long
strlen_user(void *src) {
	ulong size = strlen(src);
	if (!access_ok(src, size))
		return -EFAULT;

	return size;
}