// SPDX-License-Identifier: GPL-2.0-only
/*
 * linux/fs/binfmt_elf.c
 *
 * These are the functions used to load ELF format executables as used
 * on SVr4 machines.  Information on the format may be found in the book
 * "UNIX SYSTEM V RELEASE 4 Programmers Guide: Ansi C and Programming Support
 * Tools".
 *
 * Copyright 1993, 1994: Eric Youngdale (ericy@cais.com).
 */

// #include <linux/module.h>
#include <linux/kernel/kernel.h>
#include <linux/fs/fs.h>
#include <linux/kernel/log2.h>
#include <linux/mm/mm.h>
// #include <linux/mman.h>
#include <linux/lib/errno.h>
// #include <linux/signal.h>
#include <linux/fs/binfmts.h>
#include <linux/lib/string.h>
#include <linux/fs/file.h>
#include <linux/kernel/slab.h>
// #include <linux/personality.h>
// #include <linux/elfcore.h>
#include <linux/init/init.h>
// #include <linux/highuid.h>
#include <linux/kernel/compiler.h>
// #include <linux/highmem.h>
// #include <linux/hugetlb.h>
// #include <linux/pagemap.h>
// #include <linux/vmalloc.h>
// #include <linux/security.h>
// #include <linux/random.h>
#include <linux/kernel/elf.h>
// #include <linux/elf-randomize.h>
// #include <linux/utsname.h>
// #include <linux/coredump.h>
#include <linux/kernel/sched.h>
// #include <linux/sched/coredump.h>
// #include <linux/sched/task_stack.h>
// #include <linux/sched/cputime.h>
#include <linux/kernel/sizes.h>
#include <linux/kernel/types.h>
// #include <linux/cred.h>
// #include <linux/dax.h>
// #include <linux/uaccess.h>
// #include <asm/param.h>
#include <asm/page.h>


static linux_bfmt_s elf_format = {
	// .module			= THIS_MODULE,
	// .load_binary	= load_elf_binary,
	// .load_shlib		= load_elf_library,
	// .core_dump		= elf_core_dump,
	// .min_coredump	= ELF_EXEC_PAGESIZE,
};

// static int __init init_elf_binfmt(void) {
int __init init_elf_binfmt(void) {
	register_binfmt(&elf_format);
	return 0;
}

static void __exit exit_elf_binfmt(void) {
	/* Remove the COFF and ELF loaders. */
	unregister_binfmt(&elf_format);
}