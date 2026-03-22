/*
 * Common hypervisor code
 *
 * Copyright (C) 2008, VMware, Inc.
 * Author : Alok N Kataria <akataria@vmware.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <linux/compiler/compiler.h>
#include <linux/init/init.h>
#include <linux/kernel/export.h>
#include <asm/processor.h>
#include <asm/hypervisor.h>


static const __initconst hypervisor_x86_s * const
hypervisors[] = {
#ifdef CONFIG_XEN_PV
	&x86_hyper_xen_pv,
#endif
#ifdef CONFIG_XEN_PVHVM
	&x86_hyper_xen_hvm,
#endif
	&x86_hyper_vmware,
	// &x86_hyper_ms_hyperv,
#ifdef CONFIG_KVM_GUEST
	&x86_hyper_kvm,
#endif
#ifdef CONFIG_JAILHOUSE_GUEST
	&x86_hyper_jailhouse,
#endif
#ifdef CONFIG_ACRN_GUEST
	&x86_hyper_acrn,
#endif
};

enum x86_hypervisor_type x86_hyper_type;
EXPORT_SYMBOL(x86_hyper_type);

bool __initdata nopv;



PREFIX_STATIC_INLINE const hypervisor_x86_s * __init
detect_hypervisor_vendor(void) {
	const hypervisor_x86_s *h = NULL, * const *p;
	uint32_t pri, max_pri = 0;

	for (p = hypervisors; p < hypervisors + ARRAY_SIZE(hypervisors); p++) {
		if (unlikely(nopv) && !(*p)->ignore_nopv)
			continue;

		pri = (*p)->detect();
		if (pri > max_pri) {
			max_pri = pri;
			h = *p;
		}
	}

	if (h)
		pr_info("Hypervisor detected: %s\n", h->name);

	return h;
}



static void __init
copy_array(const void *src, void *target, uint size) {
	uint i, n = size / sizeof(void *);
	const void * const *from = (const void * const *)src;
	const void **to = (const void **)target;

	for (i = 0; i < n; i++)
		if (from[i])
			to[i] = from[i];
}

void __init simple_init_hypervisor_platform(void)
{
	const hypervisor_x86_s *h;

	h = detect_hypervisor_vendor();

	if (!h)
		return;

	copy_array(&h->init, &x86_init.hyper, sizeof(h->init));
	copy_array(&h->runtime, &x86_platform.hyper, sizeof(h->runtime));

	x86_hyper_type = h->type;
	x86_init.hyper.init_platform();
}