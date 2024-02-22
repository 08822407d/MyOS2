// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CPU_H
#define _ASM_X86_CPU_H

	#include <linux/device/device.h>
	#include <linux/kernel/cpu.h>
	// #include <linux/topology.h>
	// #include <linux/nodemask.h>
	#include <linux/smp/percpu.h>

	// extern void prefill_possible_map(void);

	// struct x86_cpu {
	// 	struct cpu cpu;
	// };

	// #ifdef CONFIG_HOTPLUG_CPU
	// 	extern int arch_register_cpu(int num);
	// 	extern void arch_unregister_cpu(int);
	// 	extern void start_cpu0(void);
	// #	ifdef CONFIG_DEBUG_HOTPLUG_CPU0
	// 		extern int _debug_hotplug_cpu(int cpu, int action);
	// #	endif
	// #endif

	// extern void ap_init_aperfmperf(void);

	// int mwait_usable(const cpuinfo_x86_s *);

	unsigned int x86_family(unsigned int sig);
	unsigned int x86_model(unsigned int sig);
	unsigned int x86_stepping(unsigned int sig);
	// #ifdef CONFIG_CPU_SUP_INTEL
	// extern void __init sld_setup(struct cpuinfo_x86 *c);
	// extern bool handle_user_split_lock(pt_regs_s *regs, long error_code);
	// extern bool handle_guest_split_lock(unsigned long ip);
	// extern void handle_bus_lock(pt_regs_s *regs);
	// u8 get_this_hybrid_cpu_type(void);
	// #else
	// static inline void __init sld_setup(struct cpuinfo_x86 *c) {}
	// static inline bool handle_user_split_lock(pt_regs_s *regs, long error_code)
	// {
	// 	return false;
	// }

	// static inline bool handle_guest_split_lock(unsigned long ip)
	// {
	// 	return false;
	// }

	// static inline void handle_bus_lock(pt_regs_s *regs) {}

	// static inline u8 get_this_hybrid_cpu_type(void)
	// {
	// 	return 0;
	// }
	// #endif
	// #ifdef CONFIG_IA32_FEAT_CTL
	// void init_ia32_feat_ctl(struct cpuinfo_x86 *c);
	// #else
	// static inline void init_ia32_feat_ctl(struct cpuinfo_x86 *c) {}
	// #endif

	// extern __noendbr void cet_disable(void);

	// struct ucode_cpu_info;

	// int intel_cpu_collect_info(struct ucode_cpu_info *uci);

	// static inline bool intel_cpu_signatures_match(unsigned int s1, unsigned int p1,
	// 						unsigned int s2, unsigned int p2)
	// {
	// 	if (s1 != s2)
	// 		return false;

	// 	/* Processor flags are either both 0 ... */
	// 	if (!p1 && !p2)
	// 		return true;

	// 	/* ... or they intersect. */
	// 	return p1 & p2;
	// }

	// extern u64 x86_read_arch_cap_msr(void);
	// int intel_find_matching_signature(void *mc, unsigned int csig, int cpf);
	// int intel_microcode_sanity_check(void *mc, bool print_err, int hdr_type);

	// extern struct cpumask cpus_stop_mask;

#endif /* _ASM_X86_CPU_H */
