#include <sys/_null.h>

#include <string.h>
#include <stddef.h>

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

dirent_s * creat_append_devdirent(char * name, dirent_s * parent);


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
	dirent_s * root_dir = path_walk("/", 0);
	dirent_s * dev_dir = creat_append_devdirent("dev", root_dir);

	List_s * cd_lp;
	for (cd_lp = cdev_lhdr.header.next; cd_lp != &cdev_lhdr.header; cd_lp = cd_lp->next)
	{
		cdev_s * cd_p = container_of(cd_lp, cdev_s, cdev_list);
		dirent_s * cddrnt = creat_append_devdirent(cd_p->dev_name, dev_dir);
		inode_s * cdino = cddrnt->dir_inode;
		cdino->f_ops = cd_p->f_ops;
	}

	List_s * bd_lp;
	for (bd_lp = bdev_lhdr.header.next; bd_lp != &bdev_lhdr.header; bd_lp = bd_lp->next)
	{
		bdev_s * bd_p = container_of(bd_lp, bdev_s, bdev_list);
		dirent_s * bddrnt = creat_append_devdirent(bd_p->dev_name, dev_dir);
		inode_s * bdino = bddrnt->dir_inode;
		bdino->f_ops = bd_p->f_ops;
	}
}
dirent_s * creat_append_devdirent(char * name, dirent_s * parent)
{
	dirent_s * dir = kmalloc(sizeof(dirent_s));
	list_init(&dir->dirent_list, dir);
	list_hdr_init(&dir->childdir_lhdr);
	dir->name = name;
	dir->name_length = strlen(dir->name);
	dir->parent = parent;
	dir->dir_ops = NULL;
	inode_s * ino = kmalloc(sizeof(inode_s));
	memset(ino, 0, sizeof(inode_s));
	dir->dir_inode = ino;

	list_hdr_append(&parent->childdir_lhdr, &dir->dirent_list);

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
	list_hdr_append(&cdev_lhdr, &find_tty()->cdev_list);
}

void enum_block_dev()
{
	list_hdr_init(&bdev_lhdr);
}