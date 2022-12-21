/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CPU_H
#define _ASM_X86_CPU_H

	#include <linux/device/device.h>
	// #include <linux/cpu.h>
	// #include <linux/topology.h>
	// #include <linux/nodemask.h>
	// #include <linux/percpu.h>

	// extern void prefill_possible_map(void);

	// struct x86_cpu
	// {
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

	// int mwait_usable(const cpuinfo_x86_s *);

	unsigned int x86_family(unsigned int sig);
	unsigned int x86_model(unsigned int sig);
	unsigned int x86_stepping(unsigned int sig);
	// #ifdef CONFIG_CPU_SUP_INTEL
	// 	extern void __init sld_setup(cpuinfo_x86_s *c);
	// 	extern void switch_to_sld(unsigned long tifn);
	// 	extern bool handle_user_split_lock(pt_regs_s *regs, long error_code);
	// 	extern bool handle_guest_split_lock(unsigned long ip);
	// 	extern void handle_bus_lock(pt_regs_s *regs);
	// 	u8 get_this_hybrid_cpu_type(void);
	// #else
	// 	static inline void __init sld_setup(cpuinfo_x86_s *c)
	// 	{
	// 	}
	// 	static inline void switch_to_sld(unsigned long tifn) {}
	// 	static inline bool handle_user_split_lock(pt_regs_s *regs, long error_code)
	// 	{
	// 		return false;
	// 	}

	// 	static inline bool handle_guest_split_lock(unsigned long ip)
	// 	{
	// 		return false;
	// 	}

	// 	static inline void handle_bus_lock(pt_regs_s *regs) {}

	// 	static inline u8 get_this_hybrid_cpu_type(void)
	// 	{
	// 		return 0;
	// 	}
	// #endif
	// #ifdef CONFIG_IA32_FEAT_CTL
	// 	void init_ia32_feat_ctl(cpuinfo_x86_s *c);
	// #else
	// static inline void init_ia32_feat_ctl(cpuinfo_x86_s *c)
	// {
	// }
	// #endif

#endif /* _ASM_X86_CPU_H */
