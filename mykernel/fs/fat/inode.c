// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/fs/fat/inode.c
 *
 *  Written 1992,1993 by Werner Almesberger
 *  VFAT extensions by Gordon Chaffee, merged with msdos fs by Henrik Storner
 *  Rewritten for the constant inumbers support by Al Viro
 *
 *  Fixes:
 *
 *	Max Cohan: Fixed invalid FSINFO offset when info_sector is 0
 */

// #include <linux/module.h>
#include <linux/mm/pagemap.h>
// #include <linux/mpage.h>
// #include <linux/vfs.h>
// #include <linux/seq_file.h>
// #include <linux/parser.h>
// #include <linux/uio.h>
#include <linux/block/blkdev.h>
// #include <linux/backing-dev.h>
// #include <asm/unaligned.h>
// #include <linux/random.h>
// #include <linux/iversion.h>
#include <linux/fs/fat.h>


#include <linux/kernel/asm-generic/bitops.h>
#include <linux/kernel/slab.h>
#include <uapi/linux/magic.h>

extern file_ops_s FAT32_file_ops;


#define KB_IN_SECTORS 2

/* DOS dates from 1980/1/1 through 2107/12/31 */
#define FAT_DATE_MIN (0<<9 | 1<<5 | 1)
#define FAT_DATE_MAX (127<<9 | 12<<5 | 31)
#define FAT_TIME_MAX (23<<11 | 59<<5 | 29)

/*
 * A deserialized copy of the on-disk structure laid out in struct
 * fat_boot_sector.
 */
typedef struct fat_bios_param_block {
	uint16_t	fat_sector_size;
	uint8_t		fat_sec_per_clus;
	uint16_t	fat_reserved;
	uint8_t		fat_fats;
	uint16_t	fat_dir_entries;
	uint16_t	fat_sectors;
	uint16_t	fat_fat_length;
	uint32_t	fat_total_sect;

	uint8_t		fat16_state;
	uint32_t	fat16_vol_id;

	uint32_t	fat32_length;
	uint32_t	fat32_root_cluster;
	uint16_t	fat32_info_sector;
	uint8_t		fat32_state;
	uint32_t	fat32_vol_id;
} fat_bios_param_block_s;


void fat_attach(inode_s *inode, loff_t i_pos)
{

}

void fat_detach(inode_s *inode)
{

}

static int is_exec(unsigned char *extension)
{
	unsigned char exe_extensions[] = "EXECOMBAT", *walk;

	for (walk = exe_extensions; *walk; walk += 3)
		if (strncmp(extension, walk, 3) == 0)
			return 1;
	return 0;
}

static int fat_calc_dir_size(inode_s *inode)
{
	// msdos_sb_info_s *sbi = MSDOS_SB(inode->i_sb);
	// int ret, fclus, dclus;

	// inode->i_size = 0;
	// if (MSDOS_I(inode)->i_start == 0)
	// 	return 0;

	// ret = fat_get_cluster(inode, FAT_ENT_EOF, &fclus, &dclus);
	// if (ret < 0)
	// 	return ret;
	// inode->i_size = (fclus + 1) << sbi->cluster_bits;

	// return 0;
}

static int fat_validate_dir(inode_s *dir)
{
	// super_block_s *sb = dir->i_sb;

	// if (dir->i_nlink < 2) {
	// 	/* Directory should have "."/".." entries at least. */
	// 	fat_fs_error(sb, "corrupted directory (invalid entries)");
	// 	return -EIO;
	// }
	// if (MSDOS_I(dir)->i_start == 0 ||
	//     MSDOS_I(dir)->i_start == MSDOS_SB(sb)->root_cluster) {
	// 	/* Directory should point valid cluster. */
	// 	fat_fs_error(sb, "corrupted directory (invalid i_start)");
	// 	return -EIO;
	// }
	// return 0;
}

/* doesn't deal with root inode */
int fat_fill_inode(inode_s *inode, msdos_dirent_s *de)
{
	// msdos_sb_info_s *sbi = MSDOS_SB(inode->i_sb);
	int error;

	inode->i_mode = 0;
	// MSDOS_I(inode)->i_pos = 0;
	// inode->i_uid = sbi->options.fs_uid;
	// inode->i_gid = sbi->options.fs_gid;
	// inode_inc_iversion(inode);

	if ((de->attr & ATTR_DIR) && !FAT32_IS_FREE(de->name)) {
		// inode->i_mode = fat_make_mode(sbi, de->attr, S_IRWXUGO);
		inode->i_mode	= S_IFDIR;
		// inode->i_op = sbi->dir_ops;
		// inode->i_fop = &fat_dir_operations;
		inode->i_fop	= &FAT32_file_ops;
		inode->i_op		= &vfat_dir_inode_operations;

		// MSDOS_I(inode)->i_start = fat_get_start(sbi, de);
		// MSDOS_I(inode)->i_logstart = MSDOS_I(inode)->i_start;
		// error = fat_calc_dir_size(inode);
		// if (error < 0)
		// 	return error;
		// error = fat_validate_dir(inode);
		// if (error < 0)
		// 	return error;
	} else { /* not a directory */
		// inode->i_mode = fat_make_mode(sbi, de->attr,
		// 		((sbi->options.showexec && !is_exec(de->name + 8))
		// 		? S_IRUGO|S_IWUGO : S_IRWXUGO));
		inode->i_mode	= S_IFREG;
		if (is_exec(de->name + 8))
			inode->i_mode |= S_IRWXUGO;
		else
			inode->i_mode |= S_IRUGO | S_IWUGO;
		// MSDOS_I(inode)->i_start = fat_get_start(sbi, de);

		// MSDOS_I(inode)->i_logstart = MSDOS_I(inode)->i_start;
		inode->i_size = de->size;
		// inode->i_op = &fat_file_inode_operations;
		// inode->i_fop = &fat_file_operations;
		inode->i_fop	= &FAT32_file_ops;
		inode->i_op		= &vfat_dir_inode_operations;
	}
	// if (de->attr & ATTR_SYS) {
	// 	if (sbi->options.sys_immutable)
	// 		inode->i_flags |= S_IMMUTABLE;
	// }
	// fat_save_attrs(inode, de->attr);

	// inode->i_blocks = ((inode->i_size + (sbi->cluster_size - 1))
	// 				& ~((loff_t)sbi->cluster_size - 1)) >> 9;

	// fat_time_fat2unix(sbi, &inode->i_mtime, de->time, de->date, 0);
	// if (sbi->options.isvfat) {
	// 	fat_time_fat2unix(sbi, &inode->i_ctime, de->ctime,
	// 			  de->cdate, de->ctime_cs);
	// 	fat_time_fat2unix(sbi, &inode->i_atime, 0, de->adate, 0);
	// } else
	// 	fat_truncate_time(inode, &inode->i_mtime, S_ATIME|S_CTIME);

	return 0;
}

inode_s *fat_build_inode(super_block_s *sb,
				msdos_dirent_s *de, loff_t i_pos)
{
	inode_s *inode;
	int err;

	inode = new_inode(sb);
	if (inode == NULL) {
		inode = ERR_PTR(-ENOMEM);
		goto out;
	}
	// inode->i_ino = iunique(sb, MSDOS_ROOT_INO);
	err = fat_fill_inode(inode, de);
	if (err) {
		iput(inode);
		inode = ERR_PTR(err);
		goto out;
	}
	// fat_attach(inode, i_pos);
out:
	return inode;
}


static void fat_put_super(super_block_s *sb)
{
	msdos_sb_info_s *sbi = MSDOS_SB(sb);

	// fat_set_state(sb, 0, 0);
	iput(sbi->fsinfo_inode);
	iput(sbi->fat_inode);
}

static inode_s *fat_alloc_inode(super_block_s *sb)
{
	msdos_inode_info_s *ei;
	ei = kmalloc(sizeof(msdos_inode_info_s), GFP_KERNEL);
	if (ei == NULL)
		return NULL;

	/* Zeroing to allow iput() even if partial initialized inode. */
	ei->mmu_private = 0;
	ei->i_start = 0;
	ei->i_logstart = 0;
	ei->i_attrs = 0;
	ei->i_pos = 0;

	return &ei->vfs_inode;
}

static void fat_free_inode(inode_s *inode)
{
	kfree(MSDOS_I(inode));
}

static int fat_write_inode(inode_s *inode)
{
	int err;

	// static int __fat_write_inode(inode_s *inode)
	// {
	// 	super_block_s *sb = inode->i_sb;
	// 	msdos_sb_info_s *sbi = MSDOS_SB(sb);
	// 	// buffer_head_s *bh;
	// 	msdos_dir_entry_s *raw_entry;
	// 	loff_t i_pos;
	// 	sector_t blocknr;
	// 	int err, offset;

	// 	if (inode->i_ino == MSDOS_ROOT_INO)
	// 		return 0;

	// retry:
	// 	i_pos = fat_i_pos_read(sbi, inode);
	// 	if (!i_pos)
	// 		return 0;

	// 	fat_get_blknr_offset(sbi, i_pos, &blocknr, &offset);
	// 	bh = sb_bread(sb, blocknr);
	// 	if (!bh) {
	// 		return -EIO;
	// 	}
	// 	if (i_pos != MSDOS_I(inode)->i_pos) {
	// 		goto retry;
	// 	}

	// 	raw_entry = &((msdos_dir_entry_s *) (bh->b_data))[offset];
	// 	if (S_ISDIR(inode->i_mode))
	// 		raw_entry->size = 0;
	// 	else
	// 		raw_entry->size = inode->i_size;
	// 	raw_entry->attr = fat_make_attrs(inode);
	// 	fat_set_start(raw_entry, MSDOS_I(inode)->i_logstart);
	// 	mark_buffer_dirty(bh);
	// 	err = 0;
	// 	brelse(bh);
	// }
	return err;
}

static const super_ops_s fat_sops = {
	.alloc_inode	= fat_alloc_inode,
	.free_inode		= fat_free_inode,
	.write_inode	= fat_write_inode,
	// .evict_inode	= fat_evict_inode,
	.put_super		= fat_put_super,
	// .statfs			= fat_statfs,
	// .remount_fs		= fat_remount,

	// .show_options	= fat_show_options,
};

static int fat_read_root(inode_s *inode)
{
	msdos_sb_info_s *sbi = MSDOS_SB(inode->i_sb);
	int error;

	MSDOS_I(inode)->i_pos = MSDOS_ROOT_INO;
	// inode->i_uid = sbi->options.fs_uid;
	// inode->i_gid = sbi->options.fs_gid;
	inode->i_mode = fat_make_mode(sbi, ATTR_DIR, S_IRWXUGO);
	inode->i_op = (inode_ops_s *)sbi->dir_ops;
	inode->i_fop = (file_ops_s *)&fat_dir_operations;
	if (is_fat32(sbi)) {
		MSDOS_I(inode)->i_start = sbi->root_cluster;
		error = fat_calc_dir_size(inode);
		if (error < 0)
			return error;
	} else {
		MSDOS_I(inode)->i_start = 0;
		inode->i_size = sbi->dir_entries * sizeof(msdos_dirent_s);
	}
	inode->i_blocks = ((inode->i_size + (sbi->cluster_size - 1))
			   & ~((loff_t)sbi->cluster_size - 1)) >> 9;
	MSDOS_I(inode)->i_logstart = 0;
	MSDOS_I(inode)->mmu_private = inode->i_size;

	fat_save_attrs(inode, ATTR_DIR);

	return 0;
}

static int fat_read_bpb(fat_boot_sector_s *b,
		fat_bios_param_block_s *bpb)
{
	int error = -EINVAL;

	/* Read in BPB ... */
	memset(bpb, 0, sizeof(fat_bios_param_block_s));
	bpb->fat_sector_size	= b->sector_size;
	bpb->fat_sec_per_clus	= b->sec_per_clus;
	bpb->fat_reserved		= b->reserved;
	bpb->fat_fats			= b->fats;
	bpb->fat_dir_entries	= b->dir_entries;
	bpb->fat_sectors		= b->sectors;
	bpb->fat_fat_length		= b->fat_length;
	bpb->fat_total_sect		= b->total_sect;

	bpb->fat16_state		= b->fat16.state;
	bpb->fat16_vol_id		= *((uint32_t *)b->fat16.vol_id);

	bpb->fat32_length		= b->fat32.length;
	bpb->fat32_root_cluster	= b->fat32.root_cluster;
	bpb->fat32_info_sector	= b->fat32.info_sector;
	bpb->fat32_state		= b->fat32.state;
	bpb->fat32_vol_id		= *((uint32_t *)b->fat32.vol_id);

	/* Validate this looks like a FAT filesystem BPB */
	if (bpb->fat_reserved == 0)
		goto out;
	if (bpb->fat_fats == 0)
		goto out;

	/*
	 * Earlier we checked here that b->secs_track and b->head are nonzero,
	 * but it turns out valid FAT filesystems can have zero there.
	 */

	// if (!fat_valid_media(b->media))
	// 	goto out;

	// if (!is_power_of_2(bpb->fat_sector_size)
	//     || (bpb->fat_sector_size < 512)
	//     || (bpb->fat_sector_size > 4096))
	// 	goto out;

	// if (!is_power_of_2(bpb->fat_sec_per_clus))
	// 	goto out;

	if (bpb->fat_fat_length == 0 && bpb->fat32_length == 0)
		goto out;

	error = 0;

out:
	return error;
}

/*
 * Read the super block of an MS-DOS FS.
 */
int fat_fill_super(super_block_s *sb, void *data, int isvfat,
		void (*setup)(super_block_s *))
{
	inode_s *root_inode = NULL, *fat_inode = NULL;
	inode_s *fsinfo_inode = NULL;
	buffer_head_s *bh;
	fat_bios_param_block_s bpb;
	msdos_sb_info_s *sbi;
	uint16_t logical_sector_size;
	uint32_t total_sectors, total_clusters,
				fat_clusters, rootdir_sectors;
	int debug;
	long error;
	char buf[50];
	timespec64_s ts;

	/*
	 * GFP_KERNEL is ok here, because while we do hold the
	 * superblock lock, memory pressure can't call back into
	 * the filesystem, since we're only just about to mount
	 * it and have no inodes etc active!
	 */
	sbi = kzalloc(sizeof(msdos_sb_info_s), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;
	sb->s_fs_info = sbi;

	sb->s_flags |= SB_NODIRATIME;
	sb->s_magic = MSDOS_SUPER_MAGIC;
	sb->s_op = (super_ops_s *)&fat_sops;

	setup(sb); /* flavour-specific stuff that needs options */

	error = -EIO;
	// sb_min_blocksize(sb, 512);
	sb->s_blocksize = 512;
	bh = sb_bread(sb, 0);
	if (bh == NULL)
		goto out_fail;

	error = fat_read_bpb((fat_boot_sector_s *)bh->b_data, &bpb);
	// error = fat_read_bpb((fat_boot_sector_s *)data, &bpb);
	if (error == -EINVAL)
		goto out_invalid;
	else if (error)
		goto out_fail;

	logical_sector_size = bpb.fat_sector_size;
	sbi->sec_per_clus = bpb.fat_sec_per_clus;

	error = -EIO;
	if (logical_sector_size < sb->s_blocksize)
		goto out_fail;

	// if (logical_sector_size > sb->s_blocksize) {
	// 	struct buffer_head *bh_resize;

	// 	if (!sb_set_blocksize(sb, logical_sector_size))
	// 		goto out_fail;

	// 	/* Verify that the larger boot sector is fully readable */
	// 	bh_resize = sb_bread(sb, 0);
	// 	if (bh_resize == NULL)
	// 		goto out_fail;
	// 	brelse(bh_resize);
	// }

	sbi->cluster_size = sb->s_blocksize * sbi->sec_per_clus;
	sbi->cluster_bits = ffs(sbi->cluster_size) - 1;
	sbi->fats = bpb.fat_fats;
	sbi->fat_bits = 0;		/* Don't know yet */
	sbi->fat_start = bpb.fat_reserved;
	sbi->fat_length = bpb.fat_fat_length;
	sbi->root_cluster = 0;
	sbi->free_clusters = -1;	/* Don't know yet */
	sbi->free_clus_valid = 0;
	sbi->prev_free = FAT_START_ENT;
	sb->s_maxbytes = 0xffffffff;

	if (!sbi->fat_length && bpb.fat32_length) {
		fat_boot_fsinfo_s *fsinfo;
		buffer_head_s *fsinfo_bh;

		/* Must be FAT32 */
		sbi->fat_bits = 32;
		sbi->fat_length = bpb.fat32_length;
		sbi->root_cluster = bpb.fat32_root_cluster;

		// /* MC - if info_sector is 0, don't multiply by 0 */
		// sbi->fsinfo_sector = bpb.fat32_info_sector;
		// if (sbi->fsinfo_sector == 0)
		// 	sbi->fsinfo_sector = 1;

		// fsinfo_bh = sb_bread(sb, sbi->fsinfo_sector);
		// if (fsinfo_bh == NULL)
		// 	goto out_fail;

		// fsinfo = (fat_boot_fsinfo_s *)fsinfo_bh->b_data;
		// if (IS_FSINFO(fsinfo)) {
		// 	if (sbi->options.usefree)
		// 		sbi->free_clus_valid = 1;
		// 	sbi->free_clusters = fsinfo->free_clusters;
		// 	sbi->prev_free = fsinfo->next_cluster;
		// }

		// brelse(fsinfo_bh);
	}

// 	/* interpret volume ID as a little endian 32 bit integer */
// 	if (is_fat32(sbi))
// 		sbi->vol_id = bpb.fat32_vol_id;
// 	else /* fat 16 or 12 */
// 		sbi->vol_id = bpb.fat16_vol_id;

// 	sbi->dir_per_block = sb->s_blocksize / sizeof(struct msdos_dir_entry);
// 	sbi->dir_per_block_bits = ffs(sbi->dir_per_block) - 1;

// 	sbi->dir_start = sbi->fat_start + sbi->fats * sbi->fat_length;
// 	sbi->dir_entries = bpb.fat_dir_entries;
// 	if (sbi->dir_entries & (sbi->dir_per_block - 1))
// 		goto out_invalid;

// 	rootdir_sectors = sbi->dir_entries
// 		* sizeof(struct msdos_dir_entry) / sb->s_blocksize;
// 	sbi->data_start = sbi->dir_start + rootdir_sectors;
// 	total_sectors = bpb.fat_sectors;
// 	if (total_sectors == 0)
// 		total_sectors = bpb.fat_total_sect;

// 	total_clusters = (total_sectors - sbi->data_start) / sbi->sec_per_clus;

// 	if (!is_fat32(sbi))
// 		sbi->fat_bits = (total_clusters > MAX_FAT12) ? 16 : 12;

// 	/* some OSes set FAT_STATE_DIRTY and clean it on unmount. */
// 	if (is_fat32(sbi))
// 		sbi->dirty = bpb.fat32_state & FAT_STATE_DIRTY;
// 	else /* fat 16 or 12 */
// 		sbi->dirty = bpb.fat16_state & FAT_STATE_DIRTY;

// 	/* check that FAT table does not overflow */
// 	fat_clusters = calc_fat_clusters(sb);
// 	total_clusters = min(total_clusters, fat_clusters - FAT_START_ENT);
// 	if (total_clusters > max_fat(sb))
// 		goto out_invalid;

// 	sbi->max_cluster = total_clusters + FAT_START_ENT;
// 	/* check the free_clusters, it's not necessarily correct */
// 	if (sbi->free_clusters != -1 && sbi->free_clusters > total_clusters)
// 		sbi->free_clusters = -1;
// 	/* check the prev_free, it's not necessarily correct */
// 	sbi->prev_free %= sbi->max_cluster;
// 	if (sbi->prev_free < FAT_START_ENT)
// 		sbi->prev_free = FAT_START_ENT;

// 	/* set up enough so that it can read an inode */
// 	fat_hash_init(sb);
// 	dir_hash_init(sb);
// 	fat_ent_access_init(sb);

// 	/*
// 	 * The low byte of the first FAT entry must have the same value as
// 	 * the media field of the boot sector. But in real world, too many
// 	 * devices are writing wrong values. So, removed that validity check.
// 	 *
// 	 * The removed check compared the first FAT entry to a value dependent
// 	 * on the media field like this:
// 	 * == (0x0F00 | media), for FAT12
// 	 * == (0XFF00 | media), for FAT16
// 	 * == (0x0FFFFF | media), for FAT32
// 	 */

// 	error = -EINVAL;

// 	/* FIXME: utf8 is using iocharset for upper/lower conversion */
// 	if (sbi->options.isvfat)
// 		goto out_fail;

// 	error = -ENOMEM;
// 	fat_inode = new_inode(sb);
// 	if (!fat_inode)
// 		goto out_fail;
// 	sbi->fat_inode = fat_inode;

// 	fsinfo_inode = new_inode(sb);
// 	if (!fsinfo_inode)
// 		goto out_fail;
// 	fsinfo_inode->i_ino = MSDOS_FSINFO_INO;
// 	sbi->fsinfo_inode = fsinfo_inode;
// 	insert_inode_hash(fsinfo_inode);

// 	root_inode = new_inode(sb);
// 	if (!root_inode)
// 		goto out_fail;
// 	root_inode->i_ino = MSDOS_ROOT_INO;
// 	inode_set_iversion(root_inode, 1);
// 	error = fat_read_root(root_inode);
// 	if (error < 0) {
// 		iput(root_inode);
// 		goto out_fail;
// 	}
// 	error = -ENOMEM;
// 	insert_inode_hash(root_inode);
// 	fat_attach(root_inode, 0);
// 	sb->s_root = d_make_root(root_inode);
// 	if (!sb->s_root)
// 		goto out_fail;

// 	fat_set_state(sb, 1, 0);
// 	return 0;

out_invalid:
	error = -EINVAL;

out_fail:
	if (fsinfo_inode)
		iput(fsinfo_inode);
	if (fat_inode)
		iput(fat_inode);
	sb->s_fs_info = NULL;
	kfree(sbi);
	return error;
}