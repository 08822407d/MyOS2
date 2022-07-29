#include <linux/kernel/stddef.h>
#include <linux/lib/string.h>

#include <klib/font.h>

#include <obsolete/proto.h>
#include <obsolete/wait_queue.h>
#include <obsolete/printk.h>
#include <linux/fs/fs.h>

#include "include/tty.h"
#include "include/keyboard.h"
#include "include/device.h"
#include "include/mutex.h"

position_t Pos;
wait_queue_hdr_s kbd_wqhdr;

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
int tty_open(inode_s * inode, file_s * fp)
{
	fp->private_data = p_kb;

	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;
	memset(p_kb->buf, 0, KB_BUF_SIZE);

	return 1;
}

int tty_close(inode_s * inode,file_s * fp)
{
	fp->private_data = NULL;

	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;
	memset(p_kb->buf, 0, KB_BUF_SIZE);

	return 1;
}

#define	KEY_CMD_RESET_BUFFER	0

int tty_ioctl(inode_s * inode, file_s* fp, unsigned long cmd, unsigned long arg)
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

ssize_t tty_read(file_s *fp, char *buf, size_t count, loff_t *position)
{
	long counter  = 0;
	char *tmpbuf = kmalloc(count);
	memset(tmpbuf, 0, count);
	while (counter < count)
	{
		char key = kbd_parse_scancode();
		if (key != 0)
		{
			tmpbuf[counter] = key;
			counter++;
		}
	}
	copy_to_user(buf, tmpbuf, counter);
	kfree(tmpbuf);

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

void tty_write_color(const char *buf, size_t length, unsigned int FRcolor, unsigned int BKcolor)
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

ssize_t tty_write(file_s * filp, const char *buf, size_t length, loff_t *position)
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

cdev_s * find_tty(char *name)
{
	cdev_s * cd_tty = kmalloc(sizeof(cdev_s));
	list_init(&cd_tty->cdev_list, cd_tty);
	cd_tty->dev_name = name;
	cd_tty->dev_nr = 0;
	cd_tty->f_op = &tty_fops;
}