// source: linux-6.4.9

/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2000, 2001, 2002 Andi Kleen, SuSE Labs
 *
 *  Pentium III FXSR, SSE support
 *	Gareth Hughes <gareth@valinux.com>, May 2000
 */

/*
 * Handle hardware traps and faults.
 */

// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/debug/kdebug.h>
#include <linux/kernel/export.h>
#include <linux/kernel/nmi.h>
#include <linux/smp/smp.h>

#include <asm/cpu.h>


// #include <asm/x86_init.h>


void __init trap_init(void)
{
	// /* Init cpu_entry_area before IST entries are set up */
	// setup_cpu_entry_areas();

	// /* Init GHCB memory pages when running as an SEV-ES guest */
	// sev_es_init_vc_handling();

	/* Initialize TSS before setting up traps so ISTs work */
	cpu_init_exception_handling();
	// /* Setup traps as cpu_init() might #GP */
	// idt_setup_traps();
	cpu_init();
}