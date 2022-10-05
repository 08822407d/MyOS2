#include <linux/kernel/stddef.h>
#include <linux/block/blk_types.h>
#include <linux/fs/namei.h>
#include <linux/fs/dcache.h>
#include <linux/lib/string.h>

#include <obsolete/proto.h>
#include <linux/device/tty.h>

#include "include/device.h"

List_hdr_s cdev_lhdr;

void init_intr(void);

void enum_char_dev(void);

void init_char_dev(void);
void init_block_dev(void);

dentry_s * creat_append_devdirent(const char * name, dentry_s * parent);


void devices_init()
{
	init_intr();

	enum_char_dev();

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
		cdev_s * cd_p = container_of(cd_lp, cdev_s, list);
		dentry_s * cddrnt = creat_append_devdirent(cd_p->kobj.name, dev_dir);
		inode_s * cdino = cddrnt->d_inode;
		cdino->i_fop = cd_p->ops;
	}
}

dentry_s * creat_append_devdirent(const char * name, dentry_s * parent)
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

void init_intr()
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
	list_hdr_append(&cdev_lhdr, &find_tty("tty0")->list);
}