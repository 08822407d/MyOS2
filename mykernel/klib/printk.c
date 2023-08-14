/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/
#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/kernel.h>
#include <linux/lib/string.h>
#include <linux/lib/stdarg.h>
#include <linux/device/tty.h>

// #include <klib/stdio.h>
#include <klib/font.h>

#include <obsolete/glo.h>
#include <obsolete/printk.h>


int snprintf(char *buf, size_t size, const char *fmt, ...)
{
	int i = 0;
	va_list args;
	va_start(args, fmt);

	i = vsnprintf(buf, size, fmt, args);

	va_end(args);

	return i;
}

/*

*/
int color_printk(unsigned int FRcolor, unsigned int BKcolor, const char *fmt, ...)
{
	static char buf[4096] = {0};
	int i = 0;
	int count = 0;
	int line = 0;
	va_list args;
	va_start(args, fmt);

	i = vsnprintf(buf, PAGE_SIZE, fmt, args);

	va_end(args);

	myos_tty_write_color(buf, i, FRcolor, BKcolor);

	return i;
}