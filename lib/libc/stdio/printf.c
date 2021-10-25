/*
 * libc/stdio/printf.c
 */

#include <stdarg.h>
#include <limits.h>
#include <stdio.h>

int printf(const char * fmt, ...)
{
	va_list ap;
	char buf[_POSIX_ARG_MAX];
	int rv;

	va_start(ap, fmt);
	rv = vsnprintf(buf, _POSIX_ARG_MAX, fmt, ap);
	va_end(ap);

	rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	fflush(stdout);
	return rv;
}