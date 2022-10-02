/* SPDX-License-Identifier: GPL-2.0 */
/*
 * kobj_map.h
 */

#ifndef _KOBJ_MAP_H_

	#define _KOBJ_MAP_H_

	// #include <linux/mutex.h>

	#include <linux/kernel/types.h>
	#include <linux/kernel/kobject.h>

	typedef kobj_s *kobj_probe_t(dev_t, int *, void *);
	struct kobj_map;
	typedef struct kobj_map kobj_map_s;

	int kobj_map(kobj_map_s *domain, dev_t devt, unsigned long count,
					kobj_probe_t *probe, void *data);
	// void kobj_unmap(kobj_map_s *domain, dev_t, unsigned long);
	kobj_s *kobj_lookup(kobj_map_s *domain, dev_t devt, int *data);
	kobj_map_s *kobj_map_init(kobj_probe_t *base_probe);

#endif /* _KOBJ_MAP_H_ */