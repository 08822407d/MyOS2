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

#include <stdarg.h>

#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/stddef.h>
#include <lib/string.h>
#include <lib/printf.h>
#include <lib/font.h>

#include "../include/glo.h"
#include "../include/printk.h"
#include "../include/param.h"


static char buf[4096] = {0};
position_t Pos;

/*

*/
void putchar(unsigned int *fb, int Xresol, int x, int y, unsigned int FRcolor, unsigned int BKcolor, unsigned char font)
{
	int i = 0, j = 0;
	unsigned int *addr = NULL;
	unsigned char *fontp = NULL;
	int testval = 0;
	fontp = font_ascii[font];

	for (i = 0; i < 16; i++)
	{
		addr = fb + Xresol * (y + i) + x;
		testval = 0x100;
		for (j = 0; j < 8; j++)
		{
			testval = testval >> 1;
			if (*fontp & testval)
				*addr = FRcolor;
			else
				*addr = BKcolor;
			addr++;
		}
		fontp++;
	}
}


/*

*/
int color_printk(unsigned int FRcolor, unsigned int BKcolor, const char *fmt, ...)
{
	lock_spin_lock(&Pos.printk_lock);

	int i = 0;
	int count = 0;
	int line = 0;
	va_list args;
	va_start(args, fmt);

	i = vsprintf(buf, fmt, args);

	va_end(args);

	for (count = 0; count < i || line; count++)
	{
		////	add \n \b \t
		if (line > 0)
		{
			count--;
			goto Label_tab;
		}
		if ((unsigned char)*(buf + count) == '\n')
		{
			Pos.YPosition++;
			Pos.XPosition = 0;
		}
		else if ((unsigned char)*(buf + count) == '\b')
		{
			Pos.XPosition--;
			if (Pos.XPosition < 0)
			{
				Pos.XPosition = Pos.XResolution / Pos.XCharSize - 1;
				Pos.YPosition--;
				if (Pos.YPosition < 0)
					Pos.YPosition = Pos.YResolution / Pos.YCharSize - 1;
			}
			putchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize, Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, ' ');
		}
		else if ((unsigned char)*(buf + count) == '\t')
		{
			line = ((Pos.XPosition + 8) & ~(8 - 1)) - Pos.XPosition;

		Label_tab:
			line--;
			putchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize, Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, ' ');
			Pos.XPosition++;
		}
		else
		{
			putchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize, Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, (unsigned char)*(buf + count));
			Pos.XPosition++;
		}

		if (Pos.XPosition >= (Pos.XResolution / Pos.XCharSize))
		{
			Pos.YPosition++;
			Pos.XPosition = 0;
		}
		if (Pos.YPosition >= (Pos.YResolution / Pos.YCharSize))
		{
			Pos.YPosition = 0;
		}
	}

	unlock_spin_lock(&Pos.printk_lock);

	return i;
}