#include <stdio.h>

#include <include/printk.h>
#include <linux/fs/fs.h>
#include <linux/fs/ext2.h>

#include "arch/amd64/include/device.h"
#include "arch/amd64/include/ide.h"

super_block_s * read_ext2_superblock(GPT_PE_s * DPTE, void * buf)
{

}

fs_type_s ext2_fs_type=
{
	.name = "EXT2",
	.fs_flags = 0,
	.read_superblock = read_ext2_superblock,
	.next = NULL,
};

void init_EXT2_FS()
{
	register_filesystem(&ext2_fs_type);
} 

int init_ext2_fs(void)
{
	return register_filesystem(&ext2_fs_type);
}

void exit_ext2_fs(void)
{
	unregister_filesystem(&ext2_fs_type);
}