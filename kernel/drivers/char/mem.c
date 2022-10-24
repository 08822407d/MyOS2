// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/drivers/char/mem.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  Added devfs support.
 *    Jan-11-1998, C. Scott Ananian <cananian@alumni.princeton.edu>
 *  Shared /dev/zero mmapping support, Feb 2000, Kanoj Sarcar <kanoj@sgi.com>
 */

#include <linux/mm/mm.h>
// #include <linux/miscdevice.h>
#include <linux/kernel/slab.h>
// #include <linux/vmalloc.h>
// #include <linux/mman.h>
// #include <linux/random.h>
#include <linux/init/init.h>
#include <linux/device/tty.h>
// #include <linux/capability.h>
// #include <linux/ptrace.h>
#include <linux/device/device.h>
// #include <linux/highmem.h>
// #include <linux/backing-dev.h>
#include <linux/mm/shmem_fs.h>
// #include <linux/splice.h>
#include <linux/mm/pfn.h>
// #include <linux/export.h>
// #include <linux/io.h>
// #include <linux/uio.h>
// #include <linux/uaccess.h>
// #include <linux/security.h>


#include <linux/kernel/minmax.h>
#include <obsolete/printk.h>

#ifdef CONFIG_IA64
#include <linux/efi.h>
#endif

#define DEVMEM_MINOR 1
#define DEVPORT_MINOR 4

#define zero_lseek null_lseek
#define full_lseek null_lseek
#define write_zero write_null
#define write_iter_zero write_iter_null
#define open_mem open_port

/*
 * Special lseek() function for /dev/null and /dev/zero.  Most notably, you
 * can fopen() both devices with "a" now.  This was previously impossible.
 * -- SRB.
 */
static loff_t null_lseek(file_s *file,
				loff_t offset, int orig)
{
	return file->f_pos = 0;
}

static ssize_t read_null(file_s *file,
		char *buf, size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t write_null(file_s *file,
		const char *buf, size_t count, loff_t *ppos)
{
	return count;
}

// static ssize_t read_iter_null(struct kiocb *iocb, struct iov_iter *to)
// {
// 	return 0;
// }

// static ssize_t write_iter_null(struct kiocb *iocb, struct iov_iter *from)
// {
// 	size_t count = iov_iter_count(from);
// 	iov_iter_advance(from, count);
// 	return count;
// }

static ssize_t read_zero(file_s *file,
		char *buf, size_t count, loff_t *ppos)
{
	size_t cleared = 0;

	while (count) {
		size_t chunk = min_t(size_t, count, PAGE_SIZE);

		memset(buf + cleared, 0, chunk);
		count -= chunk;
		if (count == 0)
			break;
		
		cleared += chunk;

		// left = clear_user(buf + cleared, chunk);
		// if (left) {
		// 	cleared += (chunk - left);
		// 	if (!cleared)
		// 		return -EFAULT;
		// 	break;
		// }
		// count -= chunk;
		// cleared += chunk;
	}

	return cleared;
}

#define zero_lseek	null_lseek
#define full_lseek	null_lseek
#define write_zero	write_null
#define write_iter_zero	write_iter_null
#define open_mem	open_port

static const file_ops_s mem_fops = {
	// .llseek	= memory_lseek,
	// .read	= read_mem,
	// .write	= write_mem,
	// .mmap	= mmap_mem,
	// .open	= open_mem,
	// #ifndef CONFIG_MMU
	// 	.get_unmapped_area	= get_unmapped_area_mem,
	// 	.mmap_capabilities	= memory_mmap_capabilities,
	// #endif
};

static const file_ops_s null_fops = {
	.llseek		= null_lseek,
	.read		= read_null,
	.write		= write_null,
	// .read_iter	= read_iter_null,
	// .write_iter	= write_iter_null,
	// .splice_write	= splice_write_null,
};

static const file_ops_s port_fops = {
	// .llseek	= memory_lseek,
	// .read	= read_port,
	// .write	= write_port,
	// .open	= open_port,
};

static const file_ops_s zero_fops = {
		.llseek		= zero_lseek,
		.write		= write_zero,
	// 	.read_iter	= read_iter_zero,
		.read		= read_zero,
	// 	.write_iter	= write_iter_zero,
	// 	.mmap		= mmap_zero,
	// 	.get_unmapped_area	= get_unmapped_area_zero,
	// #ifndef CONFIG_MMU
	// 	.mmap_capabilities	= zero_mmap_capabilities,
	// #endif
};

static const file_ops_s full_fops = {
	// .llseek		= full_lseek,
	// .read_iter	= read_iter_zero,
	// .write		= write_full,
};

typedef struct memdev
{
	const char	*name;
	umode_t		mode;
	const file_ops_s	*fops;
	fmode_t		fmode;
	cdev_s		cdev;
} memdev_s;

memdev_s devlist[] = {
#ifdef CONFIG_DEVMEM
	[DEVMEM_MINOR] = {"mem", 0, &mem_fops, FMODE_UNSIGNED_OFFSET},
#endif
	[3] = {"null", 0666, &null_fops, FMODE_NOWAIT},
#ifdef CONFIG_DEVPORT
	[4] = {"port", 0, &port_fops, 0},
#endif
	[5] = {"zero", 0666, &zero_fops, FMODE_NOWAIT},
	[7] = {"full", 0666, &full_fops, 0},
	// 	[8] = {"random", 0666, &random_fops, 0},
	// 	[9] = {"urandom", 0666, &urandom_fops, 0},
	// #ifdef CONFIG_PRINTK
	// 	[11] = {"kmsg", 0644, &kmsg_fops, 0},
	// #endif
};

static class_s *mem_class;
int chr_dev_init(void)
{
	int minor;
	int arr_size = sizeof(devlist) / sizeof(memdev_s);

	mem_class = class_create("mem");

	for (minor = 0; minor < arr_size; minor++)
	{
		memdev_s memdev = devlist[minor];
		if (!memdev.name)
			continue;

		memdev.cdev.kobj.name = memdev.name;
		dev_t devt = MKDEV(MEM_MAJOR, minor);
		cdev_init(&memdev.cdev, memdev.fops);
		if (cdev_add(&memdev.cdev, devt, 1))
			color_printk(RED, BLACK, "Couldn't register %s driver\n", memdev.name);
		else
			myos_device_create(mem_class, devt, memdev.name);
	}

	return tty_init();
}