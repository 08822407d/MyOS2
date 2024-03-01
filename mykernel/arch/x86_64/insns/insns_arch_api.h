#ifndef _ASM_X86_INSNS_API_H_
#define _ASM_X86_INSNS_API_H_

    #include "processor_const.h"
    #include "processor_types.h"

    #include "desc_arch.h"
    #include "io_arch.h"
    #include "special_insns_arch.h"

	/*
	 * capabilities of CPUs
	 */
	extern cpuinfo_x86_s boot_cpu_data;

	extern void early_cpu_init(void);
	extern void identify_boot_cpu(void);
	extern void identify_secondary_cpu(cpuinfo_x86_s *);

#endif /* _ASM_X86_INSNS_API_H_ */
