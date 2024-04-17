// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/filesystems.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  table of configured filesystems
 */

#include <linux/kernel/syscalls.h>
#include <linux/fs/fs.h>
// #include <linux/proc_fs.h>
// #include <linux/seq_file.h>
// #include <linux/kmod.h>
#include <linux/init/init.h>
// #include <linux/module.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/uaccess.h>
// #include <linux/fs_parser.h>


/*
 * Handling of filesystem drivers list.
 * Rules:
 *	Inclusion to/removals from/scanning of list are protected by spinlock.
 *	During the unload module must call unregister_filesystem().
 *	We can access the fields of list element if:
 *		1) spinlock is held or
 *		2) we hold the reference to the module.
 *	The latter can be guaranteed by call of try_module_get(); if it
 *	returned 0 we must skip the element, otherwise we got the reference.
 *	Once the reference is obtained we can drop the spinlock.
 */

static fs_type_s tmp = {.name = "filesystems", .next = NULL, .fs_flags = 0};
fs_type_s *file_systems = &tmp;
// static DEFINE_RWLOCK(file_systems_lock);

void put_filesystem(fs_type_s *fs)
{

}

static fs_type_s **find_filesystem(const char *name, unsigned len)
{
	fs_type_s **p;
	for (p = &file_systems; *p; p = &(*p)->next)
		if (strncmp((*p)->name, name, len) == 0 &&
		    !(*p)->name[len])
			break;
	return p;
}

/**
 *	register_filesystem - register a new filesystem
 *	@fs: the file system structure
 *
 *	Adds the file system passed to the list of file systems the kernel
 *	is aware of for mount and other syscalls. Returns 0 on success,
 *	or a negative errno code on an error.
 *
 *	The &struct file_system_type that is passed is linked into the kernel 
 *	structures and must not be freed until the file system has been
 *	unregistered.
 */
 
int register_filesystem(fs_type_s *fs)
{
	int res = 0;
	fs_type_s **p;

	List_s *si_hdr = &fs->fs_supers.anchor;
	if (fs->fs_supers.count == 0 &&
		(si_hdr->next != si_hdr ||
		si_hdr->prev != si_hdr))
		INIT_LIST_HEADER_S(&fs->fs_supers);

	if (fs->next)
		return -EBUSY;
	p = find_filesystem(fs->name, strlen(fs->name));
	if (*p)
		res = -EBUSY;
	else
		*p = fs;
	return res;
}

/**
 *	unregister_filesystem - unregister a file system
 *	@fs: filesystem to unregister
 *
 *	Remove a file system that was previously successfully registered
 *	with the kernel. An error is returned if the file system is not found.
 *	Zero is returned on a success.
 *	
 *	Once this function has returned the &struct file_system_type structure
 *	may be freed or reused.
 */
 
int unregister_filesystem(fs_type_s *fs)
{
	fs_type_s **tmp;

	tmp = &file_systems;
	while (*tmp) {
		if (fs == *tmp) {
			*tmp = fs->next;
			fs->next = NULL;
			return 0;
		}
		tmp = &(*tmp)->next;
	}

	return -EINVAL;
}

static fs_type_s *__get_fs_type(const char *name, int len)
{
	struct file_system_type *fs;

	// read_lock(&file_systems_lock);
	fs = *(find_filesystem(name, len));
	// if (fs && !try_module_get(fs->owner))
	// 	fs = NULL;
	// read_unlock(&file_systems_lock);
	return fs;
}

fs_type_s *get_fs_type(const char *name)
{
	struct file_system_type *fs;
	// const char *dot = strchr(name, '.');
	// int len = dot ? dot - name : strlen(name);
	int len = strlen(name);

	fs = __get_fs_type(name, len);
	// if (!fs && (request_module("fs-%.*s", len, name) == 0)) {
	// 	fs = __get_fs_type(name, len);
	// 	if (!fs)
	// 		pr_warn_once("request_module fs-%.*s succeeded, but still no fs?\n",
	// 			     len, name);
	// }

	// if (dot && fs && !(fs->fs_flags & FS_HAS_SUBTYPE)) {
	// 	put_filesystem(fs);
	// 	fs = NULL;
	// }
	return fs;
}