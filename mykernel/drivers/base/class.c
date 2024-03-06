// SPDX-License-Identifier: GPL-2.0
/*
 * class.c - basic device class management
 *
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 * Copyright (c) 2003-2004 Greg Kroah-Hartman
 * Copyright (c) 2003-2004 IBM Corp.
 */

#include <linux/kernel/kdev_t.h>
#include <linux/kernel/err.h>
#include <linux/device/class.h>
#include <linux/device/device.h>
#include <linux/init/init.h>
#include <linux/lib/string.h>
#include <linux/block/genhd.h>
#include <linux/kernel/mm.h>

/**
 * __class_create - create a struct class structure
 * @owner: pointer to the module that is to "own" this struct class
 * @name: pointer to a string for the name of this class.
 * @key: the lock_class_key for this class; used by mutex lock debugging
 *
 * This is used to create a struct class pointer that can then be used
 * in calls to device_create().
 *
 * Returns &struct class pointer on success, or ERR_PTR() on error.
 *
 * Note, the pointer created here is to be destroyed when finished by
 * making a call to class_destroy().
 */
// struct class *__class_create(struct module *owner, const char *name,
// 				 struct lock_class_key *key)
class_s *class_create(const char *name)
{
	class_s *cls;
	int retval;

	cls = kzalloc(sizeof(class_s), GFP_KERNEL);
	if (!cls) {
		retval = -ENOMEM;
		goto error;
	}

	cls->name = name;
	// cls->owner = owner;
	// cls->class_release = class_create_release;

	// retval = __class_register(cls, key);
	// if (retval)
	// 	goto error;

	return cls;

error:
	kfree(cls);
	return ERR_PTR(retval);
}