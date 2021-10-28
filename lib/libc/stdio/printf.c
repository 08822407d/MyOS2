/*
 * libc/stdio/printf.c
 */

#include <stdarg.h>
#include <stdio.h>

#include <sizes.h>

int printf(const char * fmt, ...)
{
	va_list ap;
	char buf[SZ_4K];
	int rv;

	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);

	// buffered IO has unsolved bug, so derectly use the write() function
	stdout->write(stdout, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
	return rv;
}