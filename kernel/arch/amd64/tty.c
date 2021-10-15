#include <lib/string.h>

#include "include/tty.h"
#include "include/keyboard.h"
#include "include/device.h"

#include "../../include/vfs.h"
#include "../../include/proto.h"

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
long keyboard_open(inode_s * inode, file_s * fp)
{
	fp->private_data = p_kb;

	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;
	memset(p_kb->buf, 0, KB_BUF_SIZE);

	return 1;
}

long keyboard_close(inode_s * inode,file_s * fp)
{
	fp->private_data = NULL;

	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;
	memset(p_kb->buf, 0, KB_BUF_SIZE);

	return 1;
}

#define	KEY_CMD_RESET_BUFFER	0

long keyboard_ioctl(inode_s * inode, file_s* fp, unsigned long cmd, unsigned long arg)
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

long keyboard_read(file_s * fp, char * buf, unsigned long count, long * position)
{
	long counter  = 0;
	unsigned char * tail = NULL;
	

	// if(p_kb->count == 0)
	// 	sleep_on(&keyboard_wait_queue);

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

long keyboard_write(file_s * fp, char * buf, unsigned long count, long * position)
{
	return 0;
}


file_ops_s tty_fops = 
{
	.open = keyboard_open,
	.close = keyboard_close,
	.ioctl = keyboard_ioctl,
	.read = keyboard_read,
	.write = NULL,
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