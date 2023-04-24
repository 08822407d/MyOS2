#include <linux/kernel/slab.h>
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/linux/msdos_fs.h>

#include <linux/fs/fat32.h>
#include <obsolete/printk.h>
#include <obsolete/device.h>
#include <obsolete/ide.h>

char *FAT32_parse_short(int *namelen, const msdos_dirent_s *de)
{
	int x;
	*namelen = 0;
	char *ret_val = kzalloc(15, GFP_KERNEL);

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
	ret_val = kzalloc(i * 13 + 1, GFP_KERNEL);
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

	buf = kzalloc(bufsize, GFP_KERNEL);
	if (buf == NULL)
		return -(ENOMEM);
	cluster = FAT32_find_available_cluster(fsbi);
	if (cluster < 2)
		return cluster;

	de = (msdos_dirent_s *)buf;
	/* filling the new directory slots ("." and ".." entries) */
	memcpy(de[0].name, MSDOS_DOT, MSDOS_NAME);
	memcpy(de[1].name, MSDOS_DOTDOT, MSDOS_NAME);
	de[0].attr = de[1].attr = ATTR_DIR;
	de[0].lcase = de[1].lcase = 0;
	de[0].size = de[1].size = 0;
	fat_set_start(&de[0], cluster);
	fat_set_start(&de[1], finode->first_cluster);

	sector = FAT32_clus_to_blknr(fsbi, cluster);
	if(ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD,
			sector, 1, (unsigned char *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
		return -EIO;
	}

	FAT32_write_FAT_Entry(fsbi, cluster, FAT_ENT_EOF);
	return cluster;
}

int fat_add_entries(inode_s *dir, void *slots, int nr_slots)
{
	int error = 0;
	loff_t empty_off = -1;
	int emtpy_count = 0;
	const msdos_dirent_s *de = NULL;
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
			if (empty_off == -1)
				empty_off = iobuf->iter_cursor - sizeof(msdos_dirent_s);
			else
				emtpy_count++;
		}
		else
		{
			empty_off = -1;
			emtpy_count = 0;
		}

		if (emtpy_count == nr_slots)
			break;
	}
	while (iobuf->iter_cursor < FAT_MAX_DIR_SIZE);

	if (emtpy_count == 0 && empty_off == -1)
	{
		error = -ENOENT;
		goto not_found;
	}
	else
	{
		FAT32_iobuf_write(iobuf, empty_off, slots,
				nr_slots * sizeof(msdos_dirent_s));
	}

not_found:
	FAT32_iobuf_release(iobuf);
	return 0;
}