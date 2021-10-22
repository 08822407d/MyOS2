#include <sys/types.h>
#include <sys/cdefs.h>

#include <string.h>

#include "include/arch_proto.h"
#include "include/archconst.h"

#include "../../include/printk.h"

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