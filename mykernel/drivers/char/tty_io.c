// SPDX-License-Identifier: GPL-2.0
/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * 'tty_io.c' gives an orthogonal feeling to tty's, be they consoles
 * or rs-channels. It also implements echoing, cooked mode etc.
 *
 * Kill-line thanks to John T Kohl, who also corrected VMIN = VTIME = 0.
 *
 * Modified by Theodore Ts'o, 9/14/92, to dynamically allocate the
 * tty_struct and tty_queue structures.  Previously there was an array
 * of 256 tty_struct's which was statically allocated, and the
 * tty_queue structures were allocated at boot time.  Both are now
 * dynamically allocated only when the tty is open.
 *
 * Also restructured routines so that there is more of a separation
 * between the high-level tty routines (tty_io.c and tty_ioctl.c) and
 * the low-level tty routines (serial.c, pty.c, console.c).  This
 * makes for cleaner and more compact code.  -TYT, 9/17/92
 *
 * Modified by Fred N. van Kempen, 01/29/93, to add line disciplines
 * which can be dynamically activated and de-activated by the line
 * discipline handling modules (like SLIP).
 *
 * NOTE: pay no attention to the line discipline code (yet); its
 * interface is still subject to change in this version...
 * -- TYT, 1/31/92
 *
 * Added functionality to the OPOST tty handling.  No delays, but all
 * other bits should be there.
 *	-- Nick Holloway <alfie@dcs.warwick.ac.uk>, 27th May 1993.
 *
 * Rewrote canonical mode and added more termios flags.
 *	-- julian@uhunix.uhcc.hawaii.edu (J. Cowley), 13Jan94
 *
 * Reorganized FASYNC support so mouse code can share it.
 *	-- ctm@ardi.com, 9Sep95
 *
 * New TIOCLINUX variants added.
 *	-- mj@k332.feld.cvut.cz, 19-Nov-95
 *
 * Restrict vt switching via ioctl()
 *      -- grif@cs.ucr.edu, 5-Dec-95
 *
 * Move console and virtual terminal code to more appropriate files,
 * implement CONFIG_VT and generalize console device interface.
 *	-- Marko Kohtala <Marko.Kohtala@hut.fi>, March 97
 *
 * Rewrote tty_init_dev and tty_release_dev to eliminate races.
 *	-- Bill Hawes <whawes@star.net>, June 97
 *
 * Added devfs support.
 *      -- C. Scott Ananian <cananian@alumni.princeton.edu>, 13-Jan-1998
 *
 * Added support for a Unix98-style ptmx device.
 *      -- C. Scott Ananian <cananian@alumni.princeton.edu>, 14-Jan-1998
 *
 * Reduced memory usage for older ARM systems
 *      -- Russell King <rmk@arm.linux.org.uk>
 *
 * Move do_SAK() into process context.  Less stack use in devfs functions.
 * alloc_tty_struct() always uses kmalloc()
 *			 -- Andrew Morton <andrewm@uow.edu.eu> 17Mar01
 */

#include <linux/kernel/types.h>
#include <uapi/linux/major.h>
#include <linux/lib/errno.h>
#include <linux/kernel/signal.h>
#include <linux/kernel/fcntl.h>
#include <linux/sched/signal.h>
#include <linux/device/tty.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
#include <linux/device/console.h>
#include <linux/kernel/ctype.h>
#include <linux/kernel/mm.h>
#include <linux/lib/string.h>
#include <linux/init/init.h>
#include <linux/device/device.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/uaccess.h>
#include <linux/device/tty.h>


#include <obsolete/printk.h>

class_s *tty_class;

static const file_ops_s tty_fops = {
	.read				= myos_tty_read,
	.write				= myos_tty_write,
	.open				= myos_tty_open,
	.close				= myos_tty_close,
	.ioctl				= myos_tty_ioctl,

	// .llseek			= no_llseek,
	// .read_iter		= tty_read,
	// .write_iter		= tty_write,
	// .splice_read	= generic_file_splice_read,
	// .splice_write	= iter_file_splice_write,
	// .poll			= tty_poll,
	// .unlocked_ioctl	= tty_ioctl,
	// .compat_ioctl	= tty_compat_ioctl,
	// .open			= tty_open,
	// .release		= tty_release,
	// .fasync			= tty_fasync,
	// .show_fdinfo	= tty_show_fdinfo,
};

static const file_ops_s console_fops = {
	.read				= myos_tty_read,
	.write				= myos_tty_write,
	.open				= myos_tty_open,
	.close				= myos_tty_close,
	.ioctl				= myos_tty_ioctl,

	// .llseek			= no_llseek,
	// .read_iter		= tty_read,
	// .write_iter		= redirected_tty_write,
	// .splice_read	= generic_file_splice_read,
	// .splice_write	= iter_file_splice_write,
	// .poll			= tty_poll,
	// .unlocked_ioctl	= tty_ioctl,
	// .compat_ioctl	= tty_compat_ioctl,
	// .open			= tty_open,
	// .release		= tty_release,
	// .fasync			= tty_fasync,
};


// static char *tty_devnode(device_s *dev, umode_t *mode)
// {
// 	if (!mode)
// 		return NULL;
// 	if (dev->devt == MKDEV(TTYAUX_MAJOR, 0) ||
// 	    dev->devt == MKDEV(TTYAUX_MAJOR, 2))
// 		*mode = 0666;
// 	return NULL;
// }

int tty_class_init(void)
{
	tty_class = class_create("tty");
	if (IS_ERR(tty_class))
		return PTR_ERR(tty_class);
	// tty_class->devnode = tty_devnode;
	return 0;
}


static cdev_s tty_cdev, console_cdev;
/*
 * Ok, now we can initialize the rest of the tty devices and can count
 * on memory allocations, interrupts etc..
 */
int tty_init(void)
{
	cdev_init(&tty_cdev, &tty_fops);
	tty_cdev.kobj.name = "tty";
	if (cdev_add(&tty_cdev, MKDEV(TTYAUX_MAJOR, 0), 1))
		color_printk(RED, BLACK, "Couldn't register /dev/tty driver\n");
	else
		myos_device_create(tty_class, MKDEV(TTYAUX_MAJOR, 0), "tty");

	cdev_init(&console_cdev, &console_fops);
	console_cdev.kobj.name = "console";
	if (cdev_add(&console_cdev, MKDEV(TTYAUX_MAJOR, 1), 1))
		color_printk(RED, BLACK, "Couldn't register /dev/console driver\n");
	else
		myos_device_create(tty_class, MKDEV(TTYAUX_MAJOR, 1), "console");

// #ifdef CONFIG_VT
	// vty_init(&console_fops);
// #endif
	return 0;
}



#include <obsolete/ktypes.h>
#include <obsolete/printk.h>

extern framebuffer_s	framebuffer;
extern position_t Pos;

void myos_init_video()
{	
	Pos.FB_addr = (unsigned int *)framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
	Pos.YResolution = framebuffer.Y_Resolution;

	Pos.XPosition = 0;
	Pos.YPosition = 0;
	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	spin_lock_init(&Pos.lock);
	// clean screen
	memset((void *)framebuffer.FB_virbase, 0, framebuffer.FB_size);

	char linebuf[4096] = {0};
	int i;
	for (i = 0; i < Pos.XResolution / Pos.XCharSize ; i++)
		linebuf[i] = ' ';
	color_printk(BLACK, GREEN, "%s", linebuf);
	color_printk(BLACK, GREEN, "\n");

	pr_info("MyOS Templete Console Initiated.");
}

// map VBE frame_buffer, this part should not be
// add into any memory manage unit
void myos_init_VBE_mapping()
{
	u64 start, end;
	start = PFN_PHYS(PFN_DOWN(framebuffer.FB_phybase));
	myos_ioremap(start, PFN_PHYS(PFN_UP(framebuffer.FB_size)));
	flush_tlb_local();
}