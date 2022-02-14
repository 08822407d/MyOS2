#include <stdio.h>

#include <include/printk.h>
#include <include/fs/vfs.h>
#include <include/fs/ext2.h>

#include "arch/amd64/include/device.h"
#include "arch/amd64/include/ide.h"

superblock_s * read_ext2_superblock(GPT_PE_s * DPTE, void * buf)
{

}

fs_type_s EXT2_fs_type=
{
	.name = "EXT2",
	.fs_flags = 0,
	.read_superblock = read_ext2_superblock,
	.next = NULL,
};

void init_EXT2_FS()
{
	register_filesystem(&EXT2_fs_type);
} 