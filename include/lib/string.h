#ifndef _STRING_H_
#define _STRING_H_

#include <sys/types.h>
#include <sys/cdefs.h>

void	*memchr(const void *, int, size_t);
int	 	memcmp(const void *, const void *, size_t);
void	*memcpy(void * __restrict, const void * __restrict, size_t);
void	*memmove(void *, const void *, size_t);
void	*memset(void *, char, size_t);

int strlen(char *);

#endif /* _STRING_H_ */