#ifndef _STRING_H_
#define _STRING_H_

#include <sys/types.h>
#include <sys/cdefs.h>

	void	*memchr(const void *, int, size_t len);
	int	 	memcmp(const void * ptr1, const void * ptr2, size_t len);
	void	*memcpy(void * __restrict dst, const void * __restrict src, size_t len);
	void	*memmove(void * dst, const void * src, size_t len);
	void	*memset(void * dst, char content, size_t len);

	int		strlen(char * str);
	int		strcmp(char * str1, char * str2);
	char *	strcpy(char * dst, char * src);
	char *	strncpy(char * dst, char * src, long count);

#endif /* _STRING_H_ */