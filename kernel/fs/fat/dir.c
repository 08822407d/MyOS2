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

char *FAT32_parse_short(int *namelen, const msdos_dirent_s *de)
{
	int x;
	*namelen = 0;
	char *ret_val = kmalloc(15);
	memset(ret_val, 0, 15);

	//short file/dir base name compare
	for(x=0; x < 8; x++)
	{
		if(de->name[x] == ' ')
			break;
		if(de->lcase & LOWERCASE_BASE)
			ret_val[(*namelen)++] = de->name[x] + 32;
		else
			ret_val[(*namelen)++] = de->name[x];
	}

	if(!(de->attr & ATTR_DIRECTORY))
	{
		ret_val[(*namelen)++] = '.';

		//short file ext name compare
		for(x=8; x < 11; x++)
		{
			if(de->name[x] == ' ')
				break;
			if(de->lcase & LOWERCASE_EXT)
				ret_val[(*namelen)++] = de->name[x] + 32;
			else
				ret_val[(*namelen)++] = de->name[x];
		}
		if(x == 8)
			ret_val[--(*namelen)] = 0;
	}

	return ret_val;
}

char *FAT32_parse_long(int *namelen, FAT32_iobuf_s *iobuf)
{
	int i = 0, x, y;
	char *ret_val;
	int entlen = sizeof(msdos_dirent_s);
	loff_t cursor = iobuf->iter_cursor - entlen * 2;
	msdos_dirslot_s *lde_tmp = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
	while(lde_tmp->attr == ATTR_LONG_NAME &&
		!FAT32_ent_empty((msdos_dirent_s *)lde_tmp) &&
		cursor >= 0)
	{
		i++;
		if(lde_tmp->id & 0x40)
			break;
		cursor -= entlen;
		lde_tmp = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
	}

	//long file/dir name read
	ret_val = kmalloc(i * 13 + 1);
	memset(ret_val, 0, i * 13 + 1);
	cursor = iobuf->iter_cursor - entlen * 2;
	msdos_dirslot_s *lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
	for(x = 0; x < i; x++, cursor -= entlen, lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor))
	{
		for(y = 0; y < 10; y += 2)
			if(*(u16 *)(&lde->name0_4[y]) != 0xffff &&
				*(u16 *)(&lde->name0_4[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name0_4[y];

		for(y = 0; y < 12; y += 2)
			if(*(u16 *)(&lde->name5_10[y]) != 0xffff &&
				*(u16 *)(&lde->name5_10[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name5_10[y];

		for(y = 0; y < 4; y += 2)
			if(*(u16 *)(&lde->name11_12[y]) != 0xffff &&
				*(u16 *)(&lde->name11_12[y]) != 0x0000)
				ret_val[(*namelen)++] = (char)lde->name11_12[y];
	}

	return ret_val;
}

/* See if directory is empty */
int FAT32_dir_empty(inode_s *dir)
{
	const msdos_dirent_s *de;
	int result = ENOERR;
	size_t pos = 0;
	size_t bufsize = 0;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(dir);

	iobuf->iter_init(iobuf);
	while (!IS_ERR(de = FAT32_get_full_ent(iobuf)))
	{
		if (!FAT32_ent_empty(de) &&
			strncmp(de->name, MSDOS_DOT   , MSDOS_NAME) &&
			strncmp(de->name, MSDOS_DOTDOT, MSDOS_NAME))
		{
			result = -ENOTEMPTY;
			break;
		}
		pos += sizeof(msdos_dirent_s);
	}
	FAT32_iobuf_release(iobuf);
	return result;
}

u32 FAT32_alloc_new_dir(inode_s *dir)
{
	u32 cluster;
	sector_t sector;
	FAT32_SBinfo_s *fsbi = (FAT32_SBinfo_s *)dir->i_sb->private_sb_info;
	FAT32_inode_info_s *finode = (FAT32_inode_info_s *)dir->private_idx_info;
	char *buf;
	size_t bufsize = fsbi->bytes_per_sector;
	msdos_dirent_s *de;

	buf = kmalloc(bufsize);
	if (buf == NULL)
		return -(ENOMEM);
	cluster = FAT32_find_available_cluster(fsbi);
	if (cluster < 2)
		return cluster;
	sector = FAT32_clus_to_blknr(fsbi, cluster);
	
	memset(buf, 0, bufsize);

	de = (msdos_dirent_s *)buf;
	/* filling the new directory slots ("." and ".." entries) */
	memcpy(de[0].name, MSDOS_DOT, MSDOS_NAME);
	memcpy(de[1].name, MSDOS_DOTDOT, MSDOS_NAME);
	de->attr = de[1].attr = ATTR_DIR;
	de[0].lcase = de[1].lcase = 0;
	de[0].size = de[1].size = 0;
	fat_set_start(&de[0], cluster);
	fat_set_start(&de[1], finode->first_cluster);

	if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD,
			sector, 1, (unsigned char *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
		return -EIO;
	}

	FAT32_write_FAT_Entry(fsbi, cluster, FAT_ENT_EOF);
	return cluster;
}

static int fat_add_new_entries(inode_s *dir, void *slots, int nr_slots,
				int *nr_cluster, msdos_dirent_s **de, loff_t *i_pos)
{
// 	struct super_block *sb = dir->i_sb;
// 	struct msdos_sb_info *sbi = MSDOS_SB(sb);
// 	struct buffer_head *bhs[MAX_BUF_PER_PAGE];
// 	sector_t blknr, start_blknr, last_blknr;
// 	unsigned long size, copy;
// 	int err, i, n, offset, cluster[2];

// 	/*
// 	 * The minimum cluster size is 512bytes, and maximum entry
// 	 * size is 32*slots (672bytes).  So, iff the cluster size is
// 	 * 512bytes, we may need two clusters.
// 	 */
// 	size = nr_slots * sizeof(struct msdos_dir_entry);
// 	*nr_cluster = (size + (sbi->cluster_size - 1)) >> sbi->cluster_bits;
// 	BUG_ON(*nr_cluster > 2);

// 	err = fat_alloc_clusters(dir, cluster, *nr_cluster);
// 	if (err)
// 		goto error;

// 	/*
// 	 * First stage: Fill the directory entry.  NOTE: This cluster
// 	 * is not referenced from any inode yet, so updates order is
// 	 * not important.
// 	 */
// 	i = n = copy = 0;
// 	do {
// 		start_blknr = blknr = fat_clus_to_blknr(sbi, cluster[i]);
// 		last_blknr = start_blknr + sbi->sec_per_clus;
// 		while (blknr < last_blknr) {
// 			bhs[n] = sb_getblk(sb, blknr);
// 			if (!bhs[n]) {
// 				err = -ENOMEM;
// 				goto error_nomem;
// 			}

// 			/* fill the directory entry */
// 			copy = min(size, sb->s_blocksize);
// 			/* Avoid race with userspace read via bdev */
// 			lock_buffer(bhs[n]);
// 			memcpy(bhs[n]->b_data, slots, copy);
// 			set_buffer_uptodate(bhs[n]);
// 			unlock_buffer(bhs[n]);
// 			mark_buffer_dirty_inode(bhs[n], dir);
// 			slots += copy;
// 			size -= copy;
// 			if (!size)
// 				break;
// 			n++;
// 			blknr++;
// 		}
// 	} while (++i < *nr_cluster);

// 	memset(bhs[n]->b_data + copy, 0, sb->s_blocksize - copy);
// 	offset = copy - sizeof(struct msdos_dir_entry);
// 	get_bh(bhs[n]);
// 	*bh = bhs[n];
// 	*de = (struct msdos_dir_entry *)((*bh)->b_data + offset);
// 	*i_pos = fat_make_i_pos(sb, *bh, *de);

// 	/* Second stage: clear the rest of cluster, and write outs */
// 	err = fat_zeroed_cluster(dir, start_blknr, ++n, bhs, MAX_BUF_PER_PAGE);
// 	if (err)
// 		goto error_free;

// 	return cluster[0];

// error_free:
// 	brelse(*bh);
// 	*bh = NULL;
// 	n = 0;
// error_nomem:
// 	for (i = 0; i < n; i++)
// 		bforget(bhs[i]);
// 	fat_free_clusters(dir, cluster[0]);
// error:
// 	return err;
}

int fat_add_entries(inode_s *dir, void *slots, int nr_slots)
{
	// 	struct super_block *sb = dir->i_sb;
	// 	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	// 	struct buffer_head *bh, *prev, *bhs[3]; /* 32*slots (672bytes) */
	// 	int err, free_slots, i, nr_bhs;
	// 	loff_t pos, i_pos;

	const msdos_dirent_s *de;
	const msdos_dirent_s *empty_start;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(dir);
	iobuf->iter_init(iobuf);

	do
	{
		de = iobuf->next(iobuf);
		if (iobuf->iter_reach_end)
		{
			FAT32_iobuf_expand(iobuf, 1);
			de = iobuf->next(iobuf);
		}
		if (FAT32_ent_empty(de))
		{

		}
	}
	while (iobuf->iter_cursor < FAT_MAX_DIR_SIZE);


	// 	sinfo->nr_slots = nr_slots;

	// 	/* First stage: search free directory entries */
	// 	free_slots = nr_bhs = 0;
	// 	bh = prev = NULL;
	// 	pos = 0;
	// 	err = -ENOSPC;
	// 	while (fat_get_entry(dir, &pos, &bh, &de) > -1) {
	// 		/* check the maximum size of directory */
	// 		if (pos >= FAT_MAX_DIR_SIZE)
	// 			goto error;

	// 		if (IS_FREE(de->name)) {
	// 			if (prev != bh) {
	// 				get_bh(bh);
	// 				bhs[nr_bhs] = prev = bh;
	// 				nr_bhs++;
	// 			}
	// 			free_slots++;
	// 			if (free_slots == nr_slots)
	// 				goto found;
	// 		} else {
	// 			for (i = 0; i < nr_bhs; i++)
	// 				brelse(bhs[i]);
	// 			prev = NULL;
	// 			free_slots = nr_bhs = 0;
	// 		}
	// 	}
	// 	if (dir->i_ino == MSDOS_ROOT_INO) {
	// 		if (!is_fat32(sbi))
	// 			goto error;
	// 	} else if (MSDOS_I(dir)->i_start == 0) {
	// 		fat_msg(sb, KERN_ERR, "Corrupted directory (i_pos %lld)",
	// 		       MSDOS_I(dir)->i_pos);
	// 		err = -EIO;
	// 		goto error;
	// 	}

	// found:
	// 	err = 0;
	// 	pos -= free_slots * sizeof(*de);
	// 	nr_slots -= free_slots;
	// 	if (free_slots) {
	// 		/*
	// 		 * Second stage: filling the free entries with new entries.
	// 		 * NOTE: If this slots has shortname, first, we write
	// 		 * the long name slots, then write the short name.
	// 		 */
	// 		int size = free_slots * sizeof(*de);
	// 		int offset = pos & (sb->s_blocksize - 1);
	// 		int long_bhs = nr_bhs - (nr_slots == 0);

	// 		/* Fill the long name slots. */
	// 		for (i = 0; i < long_bhs; i++) {
	// 			int copy = min_t(int, sb->s_blocksize - offset, size);
	// 			memcpy(bhs[i]->b_data + offset, slots, copy);
	// 			mark_buffer_dirty_inode(bhs[i], dir);
	// 			offset = 0;
	// 			slots += copy;
	// 			size -= copy;
	// 		}
	// 		if (long_bhs && IS_DIRSYNC(dir))
	// 			err = fat_sync_bhs(bhs, long_bhs);
	// 		if (!err && i < nr_bhs) {
	// 			/* Fill the short name slot. */
	// 			int copy = min_t(int, sb->s_blocksize - offset, size);
	// 			memcpy(bhs[i]->b_data + offset, slots, copy);
	// 			mark_buffer_dirty_inode(bhs[i], dir);
	// 			if (IS_DIRSYNC(dir))
	// 				err = sync_dirty_buffer(bhs[i]);
	// 		}
	// 		for (i = 0; i < nr_bhs; i++)
	// 			brelse(bhs[i]);
	// 		if (err)
	// 			goto error_remove;
	// 	}

	// 	if (nr_slots) {
	// 		int cluster, nr_cluster;

	// 		/*
	// 		 * Third stage: allocate the cluster for new entries.
	// 		 * And initialize the cluster with new entries, then
	// 		 * add the cluster to dir.
	// 		 */
	// 		cluster = fat_add_new_entries(dir, slots, nr_slots, &nr_cluster,
	// 					      &de, &bh, &i_pos);
	// 		if (cluster < 0) {
	// 			err = cluster;
	// 			goto error_remove;
	// 		}
	// 		err = fat_chain_add(dir, cluster, nr_cluster);
	// 		if (err) {
	// 			fat_free_clusters(dir, cluster);
	// 			goto error_remove;
	// 		}
	// 		if (dir->i_size & (sbi->cluster_size - 1)) {
	// 			fat_fs_error(sb, "Odd directory size");
	// 			dir->i_size = (dir->i_size + sbi->cluster_size - 1)
	// 				& ~((loff_t)sbi->cluster_size - 1);
	// 		}
	// 		dir->i_size += nr_cluster << sbi->cluster_bits;
	// 		MSDOS_I(dir)->mmu_private += nr_cluster << sbi->cluster_bits;
	// 	}
	// 	sinfo->slot_off = pos;
	// 	sinfo->de = de;
	// 	sinfo->bh = bh;
	// 	sinfo->i_pos = fat_make_i_pos(sb, sinfo->bh, sinfo->de);

	// 	return 0;

	// error:
	// 	brelse(bh);
	// 	for (i = 0; i < nr_bhs; i++)
	// 		brelse(bhs[i]);
	// 	return err;

	// error_remove:
	// 	brelse(bh);
	// 	if (free_slots)
	// 		__fat_remove_entries(dir, pos, free_slots);
	// 	return err;
}