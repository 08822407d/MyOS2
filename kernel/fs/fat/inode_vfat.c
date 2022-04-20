#include <sys/err.h>

#include <uapi/stat.h>
#include <uapi/msdos_fs.h>

#include <string.h>
#include <stddef.h>
#include <errno.h>

#include <include/fs/vfs.h>
#include <include/fs/fat.h>

void fat_attach(inode_s *inode, loff_t i_pos)
{
	// struct msdos_sb_info *sbi = MSDOS_SB(inode->i_sb);

	// if (inode->i_ino != MSDOS_ROOT_INO) {
	// 	struct hlist_head *head =   sbi->inode_hashtable
	// 				  + fat_hash(i_pos);

	// 	spin_lock(&sbi->inode_hash_lock);
	// 	MSDOS_I(inode)->i_pos = i_pos;
	// 	hlist_add_head(&MSDOS_I(inode)->i_fat_hash, head);
	// 	spin_unlock(&sbi->inode_hash_lock);
	// }

	// /* If NFS support is enabled, cache the mapping of start cluster
	//  * to directory inode. This is used during reconnection of
	//  * dentries to the filesystem root.
	//  */
	// if (S_ISDIR(inode->i_mode) && sbi->options.nfs) {
	// 	struct hlist_head *d_head = sbi->dir_hashtable;
	// 	d_head += fat_dir_hash(MSDOS_I(inode)->i_logstart);

	// 	spin_lock(&sbi->dir_hash_lock);
	// 	hlist_add_head(&MSDOS_I(inode)->i_dir_hash, d_head);
	// 	spin_unlock(&sbi->dir_hash_lock);
	// }
}

void fat_detach(inode_s *inode)
{
	// struct msdos_sb_info *sbi = MSDOS_SB(inode->i_sb);
	// spin_lock(&sbi->inode_hash_lock);
	// MSDOS_I(inode)->i_pos = 0;
	// hlist_del_init(&MSDOS_I(inode)->i_fat_hash);
	// spin_unlock(&sbi->inode_hash_lock);

	// if (S_ISDIR(inode->i_mode) && sbi->options.nfs) {
	// 	spin_lock(&sbi->dir_hash_lock);
	// 	hlist_del_init(&MSDOS_I(inode)->i_dir_hash);
	// 	spin_unlock(&sbi->dir_hash_lock);
	// }
}

inode_s *fat_iget(super_block_s *sb, loff_t i_pos)
{
	// msdos_sb_info_s *sbi = MSDOS_SB(sb);
	// msdos_inode_info_s *i;
	// inode_s *inode = NULL;

	// hlist_for_each_entry(i, head, i_fat_hash) {
	// 	if (i->i_pos != i_pos)
	// 		continue;
	// 	inode = igrab(&i->vfs_inode);
	// 	if (inode)
	// 		break;
	// }
	// spin_unlock(&sbi->inode_hash_lock);
	// return inode;
}

static int is_exec(unsigned char *extension)
{
	unsigned char exe_extensions[] = "EXECOMBAT", *walk;

	for (walk = exe_extensions; *walk; walk += 3)
		if (!strncmp(extension, walk, 3))
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
int fat_fill_inode(inode_s *inode, msdos_dir_entry_s *de)
{
	// msdos_sb_info_s *sbi = MSDOS_SB(inode->i_sb);
	int error;

	inode->i_mode = 0;
	// MSDOS_I(inode)->i_pos = 0;
	// inode->i_uid = sbi->options.fs_uid;
	// inode->i_gid = sbi->options.fs_gid;
	// inode_inc_iversion(inode);

	if ((de->attr & ATTR_DIR) && !IS_FREE(de->name)) {
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
				msdos_dir_entry_s *de, loff_t i_pos)
{
	inode_s *inode;
	int err;

	// inode = fat_iget(sb, i_pos);
	// if (inode)
	// 	goto out;
	inode = new_inode(sb);
	if (!inode) {
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