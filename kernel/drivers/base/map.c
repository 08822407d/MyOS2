// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/drivers/base/map.c
 *
 * (C) Copyright Al Viro 2002,2003
 *
 * NOTE: data structure needs to be changed.  It works, but for large dev_t
 * it will be too slow.  It is isolated, though, so these changes will be
 * local to that file.
 */

// #include <linux/module.h>
// #include <linux/slab.h>
// #include <linux/mutex.h>
#include <linux/kernel/kdev_t.h>
// #include <linux/kobject.h>
#include <linux/kernel/kobj_map.h>


#include <linux/lib/errno.h>

#include <obsolete/proto.h>

struct probe;
typedef struct probe probe_s;

typedef struct probe
{
	probe_s	*next;
	dev_t	dev;
	unsigned long	range;
	// struct module *owner;
	// kobj_probe_t *get;
	// int (*lock)(dev_t, void *);
	void	*data;
} probe_s;

typedef struct kobj_map
{
	probe_s	*probes[255];
	// struct mutex *lock;
} kobj_map_s;

int kobj_map(kobj_map_s *domain, dev_t dev, unsigned long range, void *data)
{
	unsigned int n = MAJOR(dev + range - 1) - MAJOR(dev) + 1;
	unsigned int index = MAJOR(dev);
	unsigned int i;
	struct probe *p;

	if (n > 255)
		n = 255;

	p = kmalloc(n * sizeof(probe_s));
	if (p == NULL)
		return -ENOMEM;

	for (i = 0; i < n; i++, p++)
	{
		p->dev = dev;
		p->range = range;
		p->data = data;
	}
	// mutex_lock(domain->lock);
	for (i = 0, p -= n; i < n; i++, p++, index++)
	{
		probe_s **s = &domain->probes[index % 255];
		while (*s && (*s)->range < range)
			s = &(*s)->next;
		p->next = *s;
		*s = p;
	}
	// mutex_unlock(domain->lock);
	return 0;
}

// void kobj_unmap(struct kobj_map *domain, dev_t dev, unsigned long range)
// {
// 	unsigned int n = MAJOR(dev + range - 1) - MAJOR(dev) + 1;
// 	unsigned int index = MAJOR(dev);
// 	unsigned int i;
// 	struct probe *found = NULL;

// 	if (n > 255)
// 		n = 255;

// 	mutex_lock(domain->lock);
// 	for (i = 0; i < n; i++, index++)
// 	{
// 		struct probe **s;
// 		for (s = &domain->probes[index % 255]; *s; s = &(*s)->next)
// 		{
// 			struct probe *p = *s;
// 			if (p->dev == dev && p->range == range)
// 			{
// 				*s = p->next;
// 				if (!found)
// 					found = p;
// 				break;
// 			}
// 		}
// 	}
// 	mutex_unlock(domain->lock);
// 	kfree(found);
// }

kobj_s *kobj_lookup(kobj_map_s *domain, dev_t dev, int *index)
{
	kobj_s *kobj;
	probe_s *p;
	unsigned long best = ~0UL;

retry:
	// mutex_lock(domain->lock);
	for (p = domain->probes[MAJOR(dev) % 255]; p; p = p->next)
	{
		struct kobject *(*probe)(dev_t, int *, void *);
		struct module *owner;
		void *data;

		if (p->dev > dev || p->dev + p->range - 1 < dev)
			continue;
		if (p->range - 1 >= best)
			break;
		data = p->data;
		best = p->range - 1;
		*index = dev - p->dev;
		// if (p->lock && p->lock(dev, data) < 0)
		// {
		// 	module_put(owner);
		// 	continue;
		// }
		// mutex_unlock(domain->lock);
		kobj = probe(dev, index, data);
		/* Currently ->owner protects _only_ ->probe() itself. */
		if (kobj)
			return kobj;
		goto retry;
	}
	// mutex_unlock(domain->lock);
	return NULL;
}

struct kobj_map *kobj_map_init()
{
	kobj_map_s *p = kmalloc(sizeof(kobj_map_s));
	probe_s *base = kzalloc(sizeof(*base));
	int i;

	if ((p == NULL) || (base == NULL))
	{
		kfree(p);
		kfree(base);
		return NULL;
	}

	base->dev = 1;
	base->range = ~0;
	for (i = 0; i < 255; i++)
		p->probes[i] = base;
	// p->lock = lock;
	return p;
}