/*
 * libc/errno/strerror.c
 */
#include <errno.h>

const char *const strerror(int e)
{
	unsigned int uerr;
	uerr = (e >= 0) ? e : -e;

	if (e >= _sys_nerr)
		return "Invalid error number";
	else
		return _sys_errlist[uerr];
}