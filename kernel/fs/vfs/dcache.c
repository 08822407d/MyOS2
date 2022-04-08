#include <stddef.h>
#include <string.h>

#include <include/proto.h>
#include <include/fs/dcache.h>

// Linux function proto:
// struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name)
dentry_s * __d_lookup(dentry_s * parent, qstr_s * name)
{
	dentry_s *	dentry = NULL;

	dentry_s * dir_p;
	List_s * dir_lp;
	for (dir_lp = parent->childdir_lhdr.header.next;
			dir_lp != &parent->childdir_lhdr.header;
			dir_lp = dir_lp->next)
	{
		if ((dir_p = dir_lp->owner_p) != NULL &&
			!strncmp(name->name, dir_p->name, name->len))
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
dentry_s * __d_alloc(qstr_s * name)
{
	dentry_s * dentry = kmalloc(sizeof(dentry_s));
	if (dentry == NULL)
		return NULL;

	dentry->name = kmalloc(name->len + 1);
	if (dentry->name == NULL)
	{
		kfree(dentry);
		return NULL;
	}

	memset(dentry->name, 0, name->len + 1);
	memcpy(dentry->name, name->name, name->len);
	dentry->name_length = name->len;

	list_init(&dentry->dirent_list, dentry);
	list_hdr_init(&dentry->childdir_lhdr);

	return dentry;
}