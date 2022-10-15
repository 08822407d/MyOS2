#include <linux/kernel/stddef.h>
#include <linux/block/blk_types.h>
#include <linux/fs/namei.h>
#include <linux/fs/dcache.h>
#include <linux/lib/string.h>

#include <obsolete/proto.h>
#include <linux/device/tty.h>

#include "include/device.h"

void init_intr(void);

void init_cdev_intr(void);
void init_bdev_intr(void);


void myos_devices_init()
{
	init_intr();

	init_cdev_intr();
	init_bdev_intr();
}

void init_intr()
{
	HPET_init();
}

void init_cdev_intr()
{
	init_keyboard();
}

void init_bdev_intr()
{
	init_disk();
}