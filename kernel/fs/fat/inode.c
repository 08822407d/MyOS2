#include <uapi/stat.h>
#include <uapi/msdos_fs.h>

#include <stddef.h>

#include <include/fs/vfs.h>
#include <include/fs/fat.h>


inode_s *fat_build_inode(super_block_s *sb,
				msdos_dir_entry_s *de, loff_t i_pos)
{
	inode_s *inode;
	int err;

// 	fat_lock_build_inode(MSDOS_SB(sb));
// 	inode = fat_iget(sb, i_pos);
// 	if (inode)
// 		goto out;
// 	inode = new_inode(sb);
// 	if (!inode) {
// 		inode = ERR_PTR(-ENOMEM);
// 		goto out;
// 	}
// 	inode->i_ino = iunique(sb, MSDOS_ROOT_INO);
// 	inode_set_iversion(inode, 1);
// 	err = fat_fill_inode(inode, de);
// 	if (err) {
// 		iput(inode);
// 		inode = ERR_PTR(err);
// 		goto out;
// 	}
// 	fat_attach(inode, i_pos);
// 	insert_inode_hash(inode);
// out:
// 	fat_unlock_build_inode(MSDOS_SB(sb));
	return inode;
}