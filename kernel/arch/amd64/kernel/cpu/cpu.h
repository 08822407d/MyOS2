/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ARCH_X86_CPU_H
#define ARCH_X86_CPU_H

	/* attempt to consolidate cpu attributes */
	typedef struct cpu_dev
	{
		const char	*c_vendor;
		/* some have two possibilities for cpuid string */
		const char	*c_ident[2];

		void	(*c_early_init)(cpuinfo_x86_s *);
		void	(*c_bsp_init)(cpuinfo_x86_s *);
		void	(*c_init)(cpuinfo_x86_s *);
		void	(*c_identify)(cpuinfo_x86_s *);
		void	(*c_detect_tlb)(cpuinfo_x86_s *);
		int		c_x86_vendor;
	} cpu_dev_s;

	// struct _tlb_table
	// {
	// 	unsigned char descriptor;
	// 	char tlb_type;
	// 	unsigned int entries;
	// 	/* unsigned int ways; */
	// 	char info[128];
	// };

	// #define cpu_dev_register(cpu_devX)				\
	// 			static const cpu_dev_s *const		\
	// 			__cpu_dev_##cpu_devX __used			\
	// 			__section(".x86_cpu_dev.init") =	\
	// 			&cpu_devX;

	// extern const cpu_dev_s
	// 	*const __x86_cpu_dev_start[],
	// 	*const __x86_cpu_dev_end[];

	// #ifdef CONFIG_CPU_SUP_INTEL
	// 	enum tsx_ctrl_states
	// 	{
	// 		TSX_CTRL_ENABLE,
	// 		TSX_CTRL_DISABLE,
	// 		TSX_CTRL_RTM_ALWAYS_ABORT,
	// 		TSX_CTRL_NOT_SUPPORTED,
	// 	};

	// 	extern __ro_after_init enum tsx_ctrl_states tsx_ctrl_state;

	// 	extern void __init tsx_init(void);
	// void tsx_ap_init(void);
	// #else
	// 	static inline void tsx_init(void)
	// 	{
	// 	}
	// 	static inline void tsx_ap_init(void) {}
	// #endif /* CONFIG_CPU_SUP_INTEL */

	// extern void get_cpu_cap(cpuinfo_x86_s *c);
	// extern void get_cpu_address_sizes(cpuinfo_x86_s *c);
	extern void cpu_detect_cache_sizes(cpuinfo_x86_s *c);
	// extern void init_scattered_cpuid_features(cpuinfo_x86_s *c);
	// extern void init_intel_cacheinfo(cpuinfo_x86_s *c);
	// extern void init_amd_cacheinfo(cpuinfo_x86_s *c);
	// extern void init_hygon_cacheinfo(cpuinfo_x86_s *c);

	extern void detect_num_cpu_cores(cpuinfo_x86_s *c);
	// extern int detect_extended_topology_early(cpuinfo_x86_s *c);
	// extern int detect_extended_topology(cpuinfo_x86_s *c);
	// extern int detect_ht_early(cpuinfo_x86_s *c);
	// extern void detect_ht(cpuinfo_x86_s *c);
	// extern void check_null_seg_clears_base(cpuinfo_x86_s *c);

	// unsigned int aperfmperf_get_khz(int cpu);

	// extern void x86_spec_ctrl_setup_ap(void);
	// extern void update_srbds_msr(void);

	// extern u64 x86_read_arch_cap_msr(void);

#endif /* ARCH_X86_CPU_H */
