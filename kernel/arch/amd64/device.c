#include <linux/kernel/stddef.h>
#include <linux/block/blk_types.h>
#include <linux/fs/namei.h>
#include <linux/fs/dcache.h>
#include <linux/lib/string.h>

#include <include/proto.h>

#include "include/device.h"
#include "include/tty.h"

List_hdr_s cdev_lhdr;
List_hdr_s bdev_lhdr;

void init_intr_dev(void);

void enum_char_dev(void);
void enum_block_dev(void);

void init_char_dev(void);
void init_block_dev(void);

dentry_s * creat_append_devdirent(char * name, dentry_s * parent);


void devices_init()
{
	init_intr_dev();

	enum_char_dev();
	enum_block_dev();

	init_char_dev();
	init_block_dev();
}

void creat_dev_file()
{
	path_s dev_path;
	kern_path("/dev", O_DIRECTORY, &dev_path);
	dentry_s * dev_dir = dev_path.dentry;

	List_s * cd_lp;
	for (cd_lp = cdev_lhdr.header.next; cd_lp != &cdev_lhdr.header; cd_lp = cd_lp->next)
	{
		cdev_s * cd_p = container_of(cd_lp, cdev_s, cdev_list);
		dentry_s * cddrnt = creat_append_devdirent(cd_p->dev_name, dev_dir);
		inode_s * cdino = cddrnt->d_inode;
		cdino->i_fop = cd_p->f_op;
	}

	List_s * bd_lp;
	for (bd_lp = bdev_lhdr.header.next; bd_lp != &bdev_lhdr.header; bd_lp = bd_lp->next)
	{
		block_device_s * bd_p = container_of(bd_lp, block_device_s, bdev_list);
		dentry_s * bddrnt = creat_append_devdirent(bd_p->dev_name, dev_dir);
		inode_s * bdino = bddrnt->d_inode;
		bdino->i_fop = bd_p->f_op;
	}
}

dentry_s * creat_append_devdirent(char * name, dentry_s * parent)
{
	dentry_s * dir = kmalloc(sizeof(dentry_s));
	list_init(&dir->d_child, dir);
	list_hdr_init(&dir->d_subdirs);
	dir->d_name.name = name;
	dir->d_name.len = strlen(dir->d_name.name);
	dir->d_parent = parent;
	dir->d_op = NULL;
	inode_s * ino = kmalloc(sizeof(inode_s));
	memset(ino, 0, sizeof(inode_s));
	dir->d_inode = ino;

	list_hdr_append(&parent->d_subdirs, &dir->d_child);

	return dir;
}

void init_intr_dev()
{
	HPET_init();
}

void init_char_dev()
{
	init_keyboard();
}

void init_block_dev()
{
	init_disk();
}

void enum_char_dev()
{
	list_hdr_init(&cdev_lhdr);
	list_hdr_append(&cdev_lhdr, &find_tty("tty0")->cdev_list);
	list_hdr_append(&cdev_lhdr, &find_tty("test_getdents64_verylong_name")->cdev_list);
}

void enum_block_dev()
{
	list_hdr_init(&bdev_lhdr);
}