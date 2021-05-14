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

#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <stdarg.h>
#include <lib/font.h>

#include "../arch/amd64/include/exclusive.h"

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

#define is_digit(c)	((c) >= '0' && (c) <= '9')

#define WHITE 	0x00ffffff		//白
#define BLACK 	0x00000000		//黑
#define RED		0x00ff0000		//红
#define ORANGE	0x00ff8000		//橙
#define YELLOW	0x00ffff00		//黄
#define GREEN	0x0000ff00		//绿
#define BLUE	0x000000ff		//蓝
#define INDIGO	0x0000ffff		//靛
#define PURPLE	0x008000ff		//紫

/*

*/
typedef struct
{
	unsigned int	XResolution;
	unsigned int	YResolution;
	
	unsigned int	XPosition;
	unsigned int	YPosition;
	
	unsigned int	XCharSize;
	unsigned int	YCharSize;

	unsigned int *	FB_addr;
	unsigned long	FB_length;

	spinlock_T		printk_lock;
} position_t;

/*

*/
void putchar(unsigned int * fb,int Xsize,int x,int y,unsigned int FRcolor,unsigned int BKcolor,unsigned char font);

/*

*/
int skip_atoi(const char **s);

int do_div(long *,int);
/*

*/
static char * number(char * str, long num, int base, int size, int precision ,int type);

/*

*/
int vsprintf(char * buf,const char *fmt, va_list args);

/*

*/
int color_printk(unsigned int FRcolor,unsigned int BKcolor,const char * fmt,...);

#endif

