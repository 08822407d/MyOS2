/*
 * libc/stdio/printf.c
 */

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

	#define SZ_4K 4096

int printf(const char * fmt, ...)
{
	va_list ap;
	char buf[SZ_4K];
	int rv;

	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);

	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
	return rv;
}