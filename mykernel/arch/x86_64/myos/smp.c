#include <linux/lib/string.h>
#include <asm/fsgsbase.h>
#include <asm/setup.h>
#include <asm/tlbflush.h>
#include <asm/percpu.h>
#include <asm/processor.h>
#include <asm/insns.h>
#include <asm/msr.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <linux/kernel/sched.h>

#include <obsolete/arch_proto.h>
#include <obsolete/mutex.h>

extern phys_addr_t 		kernel_cr3;
extern u32	cr3_paddr;
extern struct cputopo	smp_topos[CONFIG_NR_CPUS];

void myos_early_init_smp(void)
{
	// copy ap_boot entry code to its address
	extern char _APboot_phys_start;
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apboot_len = &_APboot_etext - &_APboot_text;
	memcpy((void *)phys_to_virt((phys_addr_t)&_APboot_phys_start),
			(void *)&_APboot_text, apboot_len);
	cr3_paddr = kernel_cr3;
}

void myos_startup_smp()
{
	wrmsrl(0x830,0xc4500);	//INIT IPI
	wrmsrl(0x830,0xc4620);	//Start-up IPI
}