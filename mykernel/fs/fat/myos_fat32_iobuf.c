// #include <linux/kernel/slab.h>
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/linux/msdos_fs.h>

#include <linux/fs/fat32.h>
#include <obsolete/printk.h>
#include <obsolete/device.h>
#include <obsolete/ide.h>

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
	List_hdr_s *clus_lhdrp = kmalloc(sizeof(List_hdr_s), GFP_KERNEL);
	if (clus_lhdrp == NULL)
		return ERR_PTR(-ENOMEM);
	list_hdr_init(clus_lhdrp);

	if (cluster > 0)
	{
		do
		{
			clus_list_s *clus_sp = kmalloc(sizeof(clus_list_s), GFP_KERNEL);
			if (clus_sp == NULL)
			{
				free_cluster_chain(clus_lhdrp);
				return ERR_PTR(-ENOMEM);
			}
			INIT_LIST_S(&clus_sp->list);

			clus_sp->cluster = cluster;
			list_hdr_enqueue(clus_lhdrp, &clus_sp->list);
			cluster = FAT32_read_FAT_Entry(fsbi, cluster);
		} while (cluster <= MAX_FAT32 && cluster >= FAT_START_ENT);

		if (cluster == FAT_ENT_BAD)
		{
			free_cluster_chain(clus_lhdrp);
			clus_lhdrp = ERR_PTR(-EIO);
		}
	}
	return clus_lhdrp;
}

int FAT32_iobuf_iter_init(FAT32_iobuf_s *iobuf)
{
	iobuf->iter_cursor = 0;
	iobuf->iter_reach_end = false;
	return -ENOERR;
}

const msdos_dirent_s *FAT32_iobuf_readent(FAT32_iobuf_s *iobuf, loff_t off)
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
	{
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
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
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return -ENOENT;
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
	{
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
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
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	const msdos_dirent_s *ret_val = FAT32_iobuf_readent(iobuf, iobuf->iter_cursor);

	iobuf->iter_cursor += sizeof(msdos_dirent_s);
	if (iobuf->iter_cursor >= bufend)
		iobuf->iter_reach_end = true;

	return ret_val;
}

int FAT32_iobuf_expand(FAT32_iobuf_s *iobuf, size_t nr)
{
	u32 cluster = 0;
	int tmp_nr = iobuf->nr_clus;
	while (nr >= 0 &&
		(cluster = FAT32_find_available_cluster(iobuf->fsbi)) > 1)
	{
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
		if (buf == NULL)
			break;
		
		iobuf->buffers[tmp_nr] = buf;
		iobuf->clusters[tmp_nr] = cluster;
		iobuf->flags[tmp_nr] = FAT32_IOBUF_NEWCLUS;
		tmp_nr++;
		nr--;
	}

	if (nr > 0)
	{
		while (tmp_nr > iobuf->nr_clus)
		{
			tmp_nr--;
			kfree(iobuf->buffers[tmp_nr]);
			iobuf->clusters[tmp_nr] = 0;
			iobuf->flags[tmp_nr] = 0;
		}

		return -EIO;
	}
	else
	{
		iobuf->nr_clus = tmp_nr;
		iobuf->iter_reach_end = false;
		return 0;
	}
}

FAT32_iobuf_s *FAT32_iobuf_init(inode_s *dir)
{
	int error = 0;
	FAT32_inode_info_s * finode = dir->private_idx_info;
	FAT32_SBinfo_s * fsbi = dir->i_sb->private_sb_info;
	size_t bufsize = fsbi->bytes_per_cluster;
	size_t max_nr = FAT32_MAX_FILE_SIZE / bufsize;

	List_hdr_s *clus_lhdrp = get_cluster_chain(dir);
	if (clus_lhdrp == NULL)
		return ERR_PTR(-ENOMEM);

	int count = clus_lhdrp->count;
	FAT32_iobuf_s *iobuf = kzalloc(sizeof(FAT32_iobuf_s), GFP_KERNEL);
	if (iobuf == NULL)
	{
		error = -ENOMEM;
		goto alloc_iobuf_fail;
	}
	iobuf->nr_clus = count;
	iobuf->max_nr_clus = max_nr;
	iobuf->bufsize = bufsize;
	if (count > 0)
	{
		iobuf->buffers = kzalloc(max_nr * sizeof(char *), GFP_KERNEL);
		if (iobuf->buffers == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_bufps_fail;
		}
		iobuf->clusters = kzalloc(max_nr * sizeof(*iobuf->clusters), GFP_KERNEL);
		if (iobuf->clusters == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_clusters_fail;
		}
		iobuf->flags = kzalloc(max_nr * sizeof(*iobuf->flags), GFP_KERNEL);
		if (iobuf->flags == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_flags_fail;
		}
	}

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
	if (iobuf->nr_clus > 0)
	{
		for (int i = iobuf->nr_clus - 1; i >= 0; i--)
		{
			bool clus_end_change = false;
			u32 cluster = iobuf->clusters[i];
			u32 flags = iobuf->flags[i];
			char *bufp = iobuf->buffers[i];
			if (bufp != NULL)
			{
				if (flags == FAT32_IOBUF_DIRTY)
					ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD,
						FAT32_clus_to_blknr(iobuf->fsbi, cluster),
						iobuf->bufsize / iobuf->fsbi->bytes_per_sector, bufp);
				kfree(iobuf->buffers[i]);
			}
			if (flags == FAT32_IOBUF_DELETE)
			{
				clus_end_change = true;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_FREE);
			}
			if (flags != FAT32_IOBUF_DELETE && clus_end_change)
			{
				clus_end_change = false;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_EOF);
			}
		}
		kfree(iobuf->clusters);
		kfree(iobuf->flags);
	}
	kfree(iobuf);
}