// SPDX-License-Identifier: GPL-2.0
/*
 *  Copyright (C) 1994  Linus Torvalds
 *
 *  Cyrix stuff, June 1998 by:
 *	- Rafael R. Reilova (moved everything from head.S),
 *        <rreilova@ececs.uc.edu>
 *	- Channing Corn (tests & fixes),
 *	- Andrew D. Balsa (code cleanup).
 */
#include <linux/init/init.h>
#include <linux/kernel/cpu.h>

#include <asm/bugs.h>
#include <asm/insns.h>
#include <asm/alternative.h>

#include "cpu.h"


void __init check_bugs(void)
{
	identify_boot_cpu();

	// /*
	//  * identify_boot_cpu() initialized SMT support information, let the
	//  * core code know.
	//  */
	// cpu_smt_check_topology();

	// 	pr_info("CPU: ");
	// 	print_cpu_info(&boot_cpu_data);

	// /*
	//  * Read the SPEC_CTRL MSR to account for reserved bits which may
	//  * have unknown values. AMD64_LS_CFG MSR is cached in the early AMD
	//  * init code as it is not enumerated and depends on the family.
	//  */
	// if (boot_cpu_has(X86_FEATURE_MSR_SPEC_CTRL))
	// 	rdmsrl(MSR_IA32_SPEC_CTRL, &x86_spec_ctrl_base);

	// /* Allow STIBP in MSR_SPEC_CTRL if supported */
	// if (boot_cpu_has(X86_FEATURE_STIBP))
	// 	x86_spec_ctrl_mask |= SPEC_CTRL_STIBP;

	// /* Select the proper CPU mitigations before patching alternatives: */
	// spectre_v1_select_mitigation();
	// spectre_v2_select_mitigation();
	// ssb_select_mitigation();
	// l1tf_select_mitigation();
	// mds_select_mitigation();
	// taa_select_mitigation();
	// srbds_select_mitigation();
	// l1d_flush_select_mitigation();

	// /*
	//  * As MDS and TAA mitigations are inter-related, print MDS
	//  * mitigation until after TAA mitigation selection is done.
	//  */
	// mds_print_mitigation();
	// arch_smt_update();
	// alternative_instructions();
	// /*
	//  * Make sure the first 2MB area is not mapped by huge pages
	//  * There are typically fixed size MTRRs in there and overlapping
	//  * MTRRs into large pages causes slow downs.
	//  *
	//  * Right now we don't do that with gbpages because there seems
	//  * very little benefit for that case.
	//  */
	// if (!direct_gbpages)
	// 	set_memory_4k((unsigned long)__va(0), 1);
}