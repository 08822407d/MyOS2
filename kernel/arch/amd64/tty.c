#include <lib/string.h>
#include <lib/font.h>

#include "include/tty.h"
#include "include/keyboard.h"
#include "include/device.h"
#include "include/mutex.h"

#include "../../include/vfs.h"
#include "../../include/proto.h"
#include "../../include/wait_queue.h"
#include "../../include/printk.h"

position_t Pos;
wait_queue_hdr_s kbd_wqhdr;

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
long tty_open(inode_s * inode, file_s * fp)
{
	fp->private_data = p_kb;

	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;
	memset(p_kb->buf, 0, KB_BUF_SIZE);

	return 1;
}

long tty_close(inode_s * inode,file_s * fp)
{
	fp->private_data = NULL;

	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;
	memset(p_kb->buf, 0, KB_BUF_SIZE);

	return 1;
}

#define	KEY_CMD_RESET_BUFFER	0

long tty_ioctl(inode_s * inode, file_s* fp, unsigned long cmd, unsigned long arg)
{
	switch(cmd)
	{

		case KEY_CMD_RESET_BUFFER:
			p_kb->p_head = p_kb->buf;
			p_kb->p_tail = p_kb->buf;
			p_kb->count  = 0;
			memset(p_kb->buf,0,KB_BUF_SIZE);
		break;

		default:
		break;
	}

	return 0;
}

long tty_read(file_s * fp, char * buf, unsigned long count, long * position)
{
	long counter  = 0;
	unsigned char * tail = NULL;

	if(p_kb->count == 0)
		wq_sleep_on(&kbd_wqhdr);

	counter = p_kb->count >= count? count:p_kb->count;
	tail = p_kb->p_tail;
	
	if(counter <= (p_kb->buf + KB_BUF_SIZE - tail))
	{
		copy_to_user(tail,buf,counter);
		p_kb->p_tail += counter;
	}
	else
	{
		copy_to_user(tail,buf,(p_kb->buf + KB_BUF_SIZE - tail));
		copy_to_user(p_kb->p_head,buf,counter - (p_kb->buf + KB_BUF_SIZE - tail));
		p_kb->p_tail = p_kb->p_head + (counter - (p_kb->buf + KB_BUF_SIZE - tail));
	}
	p_kb->count -= counter;

	return counter;	
}

void kputchar(unsigned int *fb, int Xresol, int x, int y, unsigned int FRcolor, unsigned int BKcolor, unsigned char font)
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

void tty_write_color(char * buf, unsigned long length, unsigned int FRcolor, unsigned int BKcolor)
{
	int count = 0;
	int line = 0;

	lock_spin_lock(&Pos.printk_lock);
	for (count = 0; count < length || line; count++)
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
			kputchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize, Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, ' ');
		}
		else if ((unsigned char)*(buf + count) == '\t')
		{
			line = ((Pos.XPosition + 8) & ~(8 - 1)) - Pos.XPosition;

		Label_tab:
			line--;
			kputchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize, Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, ' ');
			Pos.XPosition++;
		}
		else
		{
			kputchar(Pos.FB_addr, Pos.XResolution, Pos.XPosition * Pos.XCharSize, Pos.YPosition * Pos.YCharSize, FRcolor, BKcolor, (unsigned char)*(buf + count));
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
}

long tty_write(file_s * filp, char * buf, unsigned long length, long * position)
{
	long ret_val = 0;
	tty_write_color(buf, length, GREEN, BLACK);
	return ret_val;
}

file_ops_s tty_fops = 
{
	.open = tty_open,
	.close = tty_close,
	.ioctl = tty_ioctl,
	.read = tty_read,
	.write = tty_write,
};

cdev_s * find_tty()
{
	cdev_s * cd_tty = kmalloc(sizeof(cdev_s));
	list_init(&cd_tty->cdev_list, cd_tty);
	cd_tty->dev_name = "tty0";
	cd_tty->dev_nr.main = 0;
	cd_tty->dev_nr.sub = 0;
	cd_tty->f_ops = &tty_fops;
}