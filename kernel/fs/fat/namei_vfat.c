#include <sys/err.h>

#include <errno.h>

#include <uapi/msdos_fs.h>

#include <include/fs/vfs.h>
#include <include/fs/fat.h>


static int vfat_find(inode_s *dir, const qstr_s *qname,
				fat_slot_info_s *sinfo)
{
	// unsigned int len = vfat_striptail_len(qname);
	// if (len == 0)
	// 	return -ENOENT;
	// return fat_search_long(dir, qname->name, len, sinfo);
}

static dentry_s *vfat_lookup(inode_s *dir, dentry_s *dentry,
				unsigned int flags)
{
	super_block_s	*sb = dir->i_sb;
	fat_slot_info_s	sinfo;
	inode_s			*inode;
	dentry_s		*alias;
	int err;

	err = vfat_find(dir, &dentry->d_name, &sinfo);
	if (err) {
		if (err == -ENOENT) {
			inode = NULL;
			goto out;
		}
		goto error;
	}

	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
// 	brelse(sinfo.bh);
// 	if (IS_ERR(inode)) {
// 		err = PTR_ERR(inode);
// 		goto error;
// 	}

// 	alias = d_find_alias(inode);
// 	/*
// 	 * Checking "alias->d_parent == dentry->d_parent" to make sure
// 	 * FS is not corrupted (especially double linked dir).
// 	 */
// 	if (alias && alias->d_parent == dentry->d_parent) {
// 		/*
// 		 * This inode has non anonymous-DCACHE_DISCONNECTED
// 		 * dentry. This means, the user did ->lookup() by an
// 		 * another name (longname vs 8.3 alias of it) in past.
// 		 *
// 		 * Switch to new one for reason of locality if possible.
// 		 */
// 		if (!S_ISDIR(inode->i_mode))
// 			d_move(alias, dentry);
// 		iput(inode);
// 		mutex_unlock(&MSDOS_SB(sb)->s_lock);
// 		return alias;
// 	} else
// 		dput(alias);

out:
	// if (!inode)
	// 	vfat_d_version_set(dentry, inode_query_iversion(dir));
	// return d_splice_alias(inode, dentry);
error:
	return ERR_PTR(err);
}