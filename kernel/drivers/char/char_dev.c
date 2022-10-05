// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/char_dev.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

// #include <linux/init.h>
#include <linux/fs/fs.h>
#include <linux/kernel/kdev_t.h>
// #include <linux/slab.h>
#include <linux/lib/string.h>

#include <uapi/major.h>
// #include <linux/module.h>
// #include <linux/seq_file.h>

#include <linux/kernel/kobject.h>
#include <linux/kernel/kobj_map.h>
#include <linux/device/cdev.h>
// #include <linux/mutex.h>
// #include <linux/backing-dev.h>
// #include <linux/tty.h>

#include <linux/fs/internal.h>


#include <linux/lib/list.h>
#include <linux/kernel/stddef.h>
#include <linux/fs/vfs_s_defs.h>
#include <obsolete/proto.h>

static kobj_map_s *cdev_map;

/*
 * Called every time a character special file is opened
 */
static int chrdev_open(inode_s *inode, file_s *filp)
{
	const file_ops_s *fops;
	cdev_s *p;
	cdev_s *new = NULL;
	int ret = 0;

	// spin_lock(&cdev_lock);
	p = inode->i_cdev;
	if (!p) {
		kobj_s *kobj;
		int idx;
		// spin_unlock(&cdev_lock);
		kobj = kobj_lookup(cdev_map, inode->i_rdev, &idx);
		if (!kobj)
			return -ENXIO;
		new = container_of(kobj, cdev_s, kobj);
		// spin_lock(&cdev_lock);
		/* Check i_cdev again in case somebody beat us to it while
		   we dropped the lock. */
		p = inode->i_cdev;
		if (!p) {
			inode->i_cdev = p = new;
			list_hdr_push(&inode->i_devices, &p->list);
			new = NULL;
		}
		// else if (!cdev_get(p))
		// 	ret = -ENXIO;
	}
// 	else if (!cdev_get(p))
// 		ret = -ENXIO;
// 	spin_unlock(&cdev_lock);
// 	cdev_put(new);
// 	if (ret)
// 		return ret;

// 	ret = -ENXIO;
// 	fops = fops_get(p->ops);
// 	if (!fops)
// 		goto out_cdev_put;

// 	replace_fops(filp, fops);
// 	if (filp->f_op->open) {
// 		ret = filp->f_op->open(inode, filp);
// 		if (ret)
// 			goto out_cdev_put;
// 	}

// 	return 0;

//  out_cdev_put:
// 	cdev_put(p);
// 	return ret;
}

/*
 * Dummy default file-operations: the only thing this does
 * is contain the open that then fills in the correct operations
 * depending on the special file...
 */
const file_ops_s def_chr_fops = {
	.open = chrdev_open,
	.llseek = noop_llseek,
};

static kobj_s *exact_match(dev_t dev, int *part, void *data)
{
	cdev_s *p = data;
	return &p->kobj;
}

/**
 * cdev_add() - add a char device to the system
 * @p: the cdev structure for the device
 * @dev: the first device number for which this device is responsible
 * @count: the number of consecutive minor numbers corresponding to this
 *         device
 *
 * cdev_add() adds the device represented by @p to the system, making it
 * live immediately.  A negative error code is returned on failure.
 */
int cdev_add(cdev_s *p, dev_t dev, unsigned count)
{
	int error;

	p->dev = dev;
	p->count = count;

	if (dev == WHITEOUT_DEV)
		return -EBUSY;

	error = kobj_map(cdev_map, dev, count, exact_match, p);
	if (error)
		return error;

	return 0;
}

/**
 * cdev_del() - remove a cdev from the system
 * @p: the cdev structure to be removed
 *
 * cdev_del() removes @p from the system, possibly freeing the structure
 * itself.
 *
 * NOTE: This guarantees that cdev device will no longer be able to be
 * opened, however any cdevs already open will remain and their fops will
 * still be callable even after cdev_del returns.
 */
void cdev_del(cdev_s *p)
{
	// cdev_unmap(p->dev, p->count);
}

/**
 * cdev_alloc() - allocate a cdev structure
 *
 * Allocates and returns a cdev structure, or NULL on failure.
 */
cdev_s *cdev_alloc(void)
{
	cdev_s *p = kmalloc(sizeof(cdev_s));
	if (p)
		list_init(&p->list, p);

	return p;
}

/**
 * cdev_init() - initialize a cdev structure
 * @cdev: the structure to initialize
 * @fops: the file_operations for this device
 *
 * Initializes @cdev, remembering @fops, making it ready to add to the
 * system with cdev_add().
 */
void cdev_init(cdev_s *cdev, const file_ops_s *fops)
{
	memset(cdev, 0, sizeof(cdev_s));
	list_init(&cdev->list, cdev);
	cdev->ops = fops;
}

static kobj_s *base_probe(dev_t dev, int *part, void *data)
{
	// if (request_module("char-major-%d-%d", MAJOR(dev), MINOR(dev)) > 0)
	// 	/* Make old-style 2.4 aliases work */
	// 	request_module("char-major-%d", MAJOR(dev));
	return NULL;
}

void chrdev_init(void)
{
	cdev_map = kobj_map_init(base_probe);
}