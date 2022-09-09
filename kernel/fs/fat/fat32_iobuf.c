#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/msdos_fs.h>

#include <linux/fs/fat32.h>
#include <obsolete/printk.h>
#include <obsolete/proto.h>
#include "../../arch/amd64/include/device.h"
#include "../../arch/amd64/include/ide.h"

List_hdr_s *get_cluster_chain(inode_s *inode)
{
	FAT32_inode_info_s * finode = inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = inode->i_sb->private_sb_info;
	unsigned long cluster = finode->first_cluster;
	List_hdr_s *clus_lhdrp = kmalloc(sizeof(List_hdr_s));
	if (clus_lhdrp == NULL)
		return -ENOMEM;
	list_hdr_init(clus_lhdrp);

	do
	{
		clus_list_s *clus_sp = kmalloc(sizeof(clus_list_s));
		list_init(&clus_sp->list, clus_sp);

		clus_sp->cluster = cluster;
		list_hdr_enqueue(clus_lhdrp, &clus_sp->list);
	} while ((cluster = FAT32_read_FAT_Entry(fsbi, cluster)) <= MAX_FAT32);

	if (cluster == BAD_FAT32)
	{
		list_hdr_dump(clus_lhdrp);
		kfree(clus_lhdrp);
		clus_lhdrp = NULL;
	}

	return clus_lhdrp;
}

int FAT32_iobuf_init(inode_s *dir, FAT32_iobuf_s *iobuf)
{
	FAT32_inode_info_s * finode = dir->private_idx_info;
	FAT32_SBinfo_s * fsbi = dir->i_sb->private_sb_info;

	List_hdr_s *clus_lhdrp = get_cluster_chain(dir);
	if (clus_lhdrp == NULL)
		return -EIO;
}