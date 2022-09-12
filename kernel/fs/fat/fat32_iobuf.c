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

static void free_cluster_chain(List_hdr_s *clus_lhdrp)
{
	list_hdr_dump(clus_lhdrp);
	kfree(clus_lhdrp);
}

List_hdr_s *get_cluster_chain(inode_s *inode)
{
	FAT32_inode_info_s * finode = inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = inode->i_sb->private_sb_info;
	unsigned long cluster = finode->first_cluster;
	List_hdr_s *clus_lhdrp = kmalloc(sizeof(List_hdr_s));
	if (clus_lhdrp == NULL)
		return ERR_PTR(-ENOMEM);
	list_hdr_init(clus_lhdrp);

	do
	{
		clus_list_s *clus_sp = kmalloc(sizeof(clus_list_s));
		if (clus_sp == NULL)
		{
			free_cluster_chain(clus_lhdrp);
			return ERR_PTR(-ENOMEM);
		}
		list_init(&clus_sp->list, clus_sp);

		clus_sp->cluster = cluster;
		list_hdr_enqueue(clus_lhdrp, &clus_sp->list);
	} while ((cluster = FAT32_read_FAT_Entry(fsbi, cluster)) <= MAX_FAT32);

	if (cluster == BAD_FAT32)
	{
		free_cluster_chain(clus_lhdrp);
		clus_lhdrp = ERR_PTR(-EIO);
	}
	return clus_lhdrp;
}

int FAT32_iobuf_iter_init(FAT32_iobuf_s *iobuf)
{
	iobuf->iter_cursor = 0;
	return -ENOERR;
}

const msdos_dirent_s *FAT32_iobuf_readent(FAT32_iobuf_s *iobuf, loff_t off)
{
	loff_t	bufend = iobuf->buf_nr * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
	{
		char *buf = kmalloc(iobuf->bufsize);
		if (buf == NULL)
			return ERR_PTR(-ENOMEM);

		ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
				FAT32_clus_to_blknr(iobuf->fsbi, iobuf->clusters[0]),
				iobuf->fsbi->sector_per_cluster, buf);
		iobuf->buffers[bufidx] = buf;
	}

	return (msdos_dirent_s *)&(iobuf->buffers[bufidx][bufoff]);
}

int FAT32_iobuf_write(FAT32_iobuf_s *iobuf, loff_t off,
		char *content, size_t size)
{
	loff_t	bufend = iobuf->buf_nr * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return -ENOENT;
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
	{
		char *buf = kmalloc(iobuf->bufsize);
		if (buf == NULL)
			return -ENOMEM;

		ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
				FAT32_clus_to_blknr(iobuf->fsbi, iobuf->clusters[0]),
				iobuf->fsbi->sector_per_cluster, buf);
		iobuf->buffers[bufidx] = buf;
	}

	for (size_t i = 0; i < size; i++)
	{
		iobuf->buffers[bufidx][bufoff + i] = content[i];
		iobuf->flags[bufidx] |= FAT32_IOBUF_DIRTY;
	}
	
	return -ENOERR;
}

const msdos_dirent_s *FAT32_iobuf_iter_next(FAT32_iobuf_s *iobuf)
{
	loff_t	bufend = iobuf->buf_nr * iobuf->bufsize;
	const msdos_dirent_s *ret_val = FAT32_iobuf_readent(iobuf, iobuf->iter_cursor);

	iobuf->iter_cursor += sizeof(msdos_dirent_s);
	if (iobuf->iter_cursor >= bufend)
		iobuf->iter_cursor = -1;

	return ret_val;
}

FAT32_iobuf_s *FAT32_iobuf_init(inode_s *dir)
{
	int error = 0;
	FAT32_inode_info_s * finode = dir->private_idx_info;
	FAT32_SBinfo_s * fsbi = dir->i_sb->private_sb_info;
	size_t bufsize = fsbi->bytes_per_cluster;

	List_hdr_s *clus_lhdrp = get_cluster_chain(dir);
	if (clus_lhdrp == NULL)
		return ERR_PTR(-ENOMEM);

	int count = clus_lhdrp->count;
	FAT32_iobuf_s *iobuf = kmalloc(sizeof(FAT32_iobuf_s));
	if (iobuf == NULL)
	{
		error = -ENOMEM;
		goto alloc_iobuf_fail;
	}
	memset(iobuf, 0, sizeof(FAT32_iobuf_s));
	iobuf->buf_nr = count;
	iobuf->bufsize = bufsize;
	iobuf->buffers = kmalloc(count * sizeof(char *));
	if (iobuf->buffers == NULL)
	{
		error = -ENOMEM;
		goto alloc_iobuf_bufps_fail;
	}
	iobuf->clusters = kmalloc(count * sizeof(*iobuf->clusters));
	if (iobuf->clusters == NULL)
	{
		error = -ENOMEM;
		goto alloc_iobuf_clusters_fail;
	}
	iobuf->flags = kmalloc(count * sizeof(*iobuf->flags));
	if (iobuf->flags == NULL)
	{
		error = -ENOMEM;
		goto alloc_iobuf_flags_fail;
	}
	memset(iobuf->buffers, 0, count * sizeof(char *));
	memset(iobuf->clusters, 0, count * sizeof(*iobuf->clusters));
	memset(iobuf->flags, 0, count * sizeof(*iobuf->flags));

	int i = 0;
	while (clus_lhdrp->count > 0)
	{
		List_s *lp = list_hdr_dequeue(clus_lhdrp);
		clus_list_s *clus_lp = container_of(lp, clus_list_s, list);
		iobuf->clusters[i] = clus_lp->cluster;
		iobuf->flags[i] = 0;
		kfree(clus_lp);
		i++;
	}
	iobuf->fsbi = fsbi;
	iobuf->iter_cursor = -1;
	iobuf->iter_init = FAT32_iobuf_iter_init;
	iobuf->next = FAT32_iobuf_iter_next;
	return iobuf;

alloc_iobuf_flags_fail:
	kfree(iobuf->clusters);
alloc_iobuf_clusters_fail:
	kfree(iobuf->buffers);
alloc_iobuf_bufps_fail:
	kfree(iobuf);
alloc_iobuf_fail:
	free_cluster_chain(clus_lhdrp);
	return ERR_PTR(error);
}

void FAT32_iobuf_release(FAT32_iobuf_s *iobuf)
{
	if (iobuf->buf_nr > 0)
	{
		for (int i = 0; i < iobuf->buf_nr; i++)
		{
			u32 cluster = iobuf->clusters[i];
			u32 flags = iobuf->flags[i];
			char *bufp = iobuf->buffers[i];
			if (flags == FAT32_IOBUF_DIRTY &&
				bufp != NULL)
			{
				ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD,
					FAT32_clus_to_blknr(iobuf->fsbi, cluster),
					iobuf->bufsize / iobuf->fsbi->sector_per_cluster, bufp);
			}
			if (bufp != NULL)
				kfree(iobuf->buffers[i]);
		}
		kfree(iobuf->clusters);
		kfree(iobuf->flags);
	}
	kfree(iobuf);
}