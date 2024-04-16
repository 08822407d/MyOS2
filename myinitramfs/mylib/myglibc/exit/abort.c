/*
 * libc/exit/abort.c
 */

#include <stdlib.h>

void __attribute__((__noreturn__)) abort(void)
{
	 while(1);
}