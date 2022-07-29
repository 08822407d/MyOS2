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

// #include <linux/kobject.h>
// #include <linux/kobj_map.h>
#include <linux/fs/cdev.h>
// #include <linux/mutex.h>
// #include <linux/backing-dev.h>
// #include <linux/tty.h>

#include <linux/fs/internal.h>


#include <linux/fs/vfs_s_defs.h>
#include <obsolete/proto.h>

/*
 * Called every time a character special file is opened
 */
static int chrdev_open(inode_s *inode, file_s *filp)
{
// 	const struct file_operations *fops;
// 	struct cdev *p;
// 	struct cdev *new = NULL;
// 	int ret = 0;

// 	spin_lock(&cdev_lock);
// 	p = inode->i_cdev;
// 	if (!p) {
// 		struct kobject *kobj;
// 		int idx;
// 		spin_unlock(&cdev_lock);
// 		kobj = kobj_lookup(cdev_map, inode->i_rdev, &idx);
// 		if (!kobj)
// 			return -ENXIO;
// 		new = container_of(kobj, struct cdev, kobj);
// 		spin_lock(&cdev_lock);
// 		/* Check i_cdev again in case somebody beat us to it while
// 		   we dropped the lock. */
// 		p = inode->i_cdev;
// 		if (!p) {
// 			inode->i_cdev = p = new;
// 			list_add(&inode->i_devices, &p->list);
// 			new = NULL;
// 		} else if (!cdev_get(p))
// 			ret = -ENXIO;
// 	} else if (!cdev_get(p))
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