#include <linux/kernel/types.h>
#include <linux/init/init.h>
#include <asm/processor.h>
#include <asm/insns.h>

/*
 * Initialize the registers found in all CPUs, CR0 and CR4:
 */
static void fpu__init_cpu_generic(void)
{
	ulong cr0;
	ulong cr4_mask = 0;

// 	if (boot_cpu_has(X86_FEATURE_FXSR))
// 		cr4_mask |= X86_CR4_OSFXSR;
// 	if (boot_cpu_has(X86_FEATURE_XMM))
// 		cr4_mask |= X86_CR4_OSXMMEXCPT;
// 	if (cr4_mask)
// 		cr4_set_bits(cr4_mask);

	cr0 = read_cr0();
	cr0 &= ~(X86_CR0_TS|X86_CR0_EM); /* clear TS and EM */
	// if (!boot_cpu_has(X86_FEATURE_FPU))
	if (!boot_cpu_data.x86_capa_bits.X87FPU_On_Chip)
		cr0 |= X86_CR0_EM;
	write_cr0(cr0);

// 	/* Flush out any pending x87 state: */
// #ifdef CONFIG_MATH_EMULATION
// 	if (!boot_cpu_has(X86_FEATURE_FPU))
// 		fpstate_init_soft(&current->thread.fpu.fpstate->regs.soft);
// 	else
// #endif
// 		asm volatile ("fninit");
}

/*
 * Enable all supported FPU features. Called when a CPU is brought online:
 */
void fpu__init_cpu(void)
{
	fpu__init_cpu_generic();
	// fpu__init_cpu_xstate();
}




/*
 * Called on the boot CPU once per system bootup, to set up the initial
 * FPU state that is later cloned into all processes:
 */
void __init fpu__init_system(void)
{
	// fpstate_reset(&current->thread.fpu);
	// fpu__init_system_early_generic();

	/*
	 * The FPU has to be operational for some of the
	 * later FPU init activities:
	 */
	fpu__init_cpu();

	// fpu__init_system_generic();
	// fpu__init_system_xstate_size_legacy();
	// fpu__init_system_xstate(fpu_kernel_cfg.max_size);
	// fpu__init_task_struct_size();
}