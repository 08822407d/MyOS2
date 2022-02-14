#include <stddef.h>
#include <string.h>

#include <include/proto.h>
#include <include/fs/dcache.h>

// Linux function proto:
// struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name)
dirent_s * __d_lookup(nameidata_s * nd)
{
	dirent_s *	dentry = NULL,
			*	parent = nd->path.dentry;

	dirent_s * dir_p;
	List_s * dir_lp;
	for (dir_lp = parent->childdir_lhdr.header.next;
			dir_lp != &parent->childdir_lhdr.header;
			dir_lp = dir_lp->next)
	{
		if ((dir_p = dir_lp->owner_p) != NULL &&
			!strncmp(nd->last_name, dir_p->name, nd->last_len))
		{
			dentry = dir_p;
			break;
		}
	}
	return dentry;
}

/**
 * __d_alloc	-	allocate a dcache entry
 * @sb: filesystem it will belong to
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
// Linux function proto:
// static struct dentry *__d_alloc(struct super_block *sb, const struct qstr *name)
dirent_s * __d_alloc(const char * name, size_t name_len)
{
	dirent_s * dentry = kmalloc(sizeof(dirent_s));

	dentry->name = kmalloc(name_len + 1);
	memset(dentry->name, 0, name_len + 1);
	memcpy(dentry->name, name, name_len);
	dentry->name_length = name_len;

	list_init(&dentry->dirent_list, dentry);
	list_hdr_init(&dentry->childdir_lhdr);

	return dentry;
}