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
#include <linux/kernel/signal.h>
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


static int load_elf_binary(struct linux_binprm *bprm);

/*
 * If we don't support core dumping, then supply a NULL so we
 * don't even try.
 */
#ifdef CONFIG_ELF_CORE
	static int elf_core_dump(struct coredump_params *cprm);
#else
#	define elf_core_dump	NULL
#endif

#if ELF_EXEC_PAGESIZE > PAGE_SIZE
#	define ELF_MIN_ALIGN	ELF_EXEC_PAGESIZE
#else
#	define ELF_MIN_ALIGN	PAGE_SIZE
#endif

#ifndef ELF_CORE_EFLAGS
#	define ELF_CORE_EFLAGS	0
#endif

#define ELF_PAGESTART(_v)	((_v) & ~(unsigned long)(ELF_MIN_ALIGN-1))
#define ELF_PAGEOFFSET(_v)	((_v) & (ELF_MIN_ALIGN-1))
#define ELF_PAGEALIGN(_v)	(((_v) + ELF_MIN_ALIGN - 1) & ~(ELF_MIN_ALIGN - 1))

static linux_bfmt_s elf_format = {
	// .module			= THIS_MODULE,
	.load_binary	= load_elf_binary,
	// .load_shlib		= load_elf_library,
	// .core_dump		= elf_core_dump,
	// .min_coredump	= ELF_EXEC_PAGESIZE,
};

#define BAD_ADDR(x) ((unsigned long)(x) >= TASK_SIZE)

static int
set_brk(unsigned long start, unsigned long end, int prot)
{
	start = ELF_PAGEALIGN(start);
	end = ELF_PAGEALIGN(end);
	// if (end > start) {
	// 	/*
	// 	 * Map the last of the bss segment.
	// 	 * If the header is requesting these pages to be
	// 	 * executable, honour that (ppc32 needs this).
	// 	 */
	// 	int error = vm_brk_flags(start, end - start,
	// 			prot & PROT_EXEC ? VM_EXEC : 0);
	// 	if (error)
	// 		return error;
	// }
	current->mm->start_brk = current->mm->brk = end;
	return 0;
}


static unsigned long
elf_map(file_s *filep, unsigned long addr, const elf_phdr_t *eppnt,
		int prot, int type, unsigned long total_size)
{
	unsigned long map_addr;
	unsigned long size = eppnt->p_filesz + ELF_PAGEOFFSET(eppnt->p_vaddr);
	unsigned long off = eppnt->p_offset - ELF_PAGEOFFSET(eppnt->p_vaddr);
	addr = ELF_PAGESTART(addr);
	size = ELF_PAGEALIGN(size);

	/* mmap() will return -EINVAL if given a zero size, but a
	 * segment with zero filesize is perfectly valid */
	if (!size)
		return addr;

	/*
	 * total_size is the size of the ELF (interpreter) image.
	 * The _first_ mmap needs to know the full size, otherwise
	 * randomization might put this image into an overlapping
	 * position with the ELF binary image. (since size < total_size)
	 * So we first map the 'big' image - and unmap the remainder at
	 * the end. (which unmap is needed for ELF images with holes.)
	 */
	if (total_size) {
		total_size = ELF_PAGEALIGN(total_size);
		map_addr = vm_mmap(filep, addr, total_size, prot, type, off);
	// 	if (!BAD_ADDR(map_addr))
	// 		vm_munmap(map_addr+size, total_size-size);
	} else
		map_addr = vm_mmap(filep, addr, size, prot, type, off);

	// if ((type & MAP_FIXED_NOREPLACE) &&
	//     PTR_ERR((void *)map_addr) == -EEXIST)
	// 	pr_info("%d (%s): Uhuuh, elf segment at %px requested but the memory is mapped already\n",
	// 		task_pid_nr(current), current->comm, (void *)addr);

	return(map_addr);
}

static unsigned long
total_mapping_size(const elf_phdr_t *cmds, int nr)
{
	int i, first_idx = -1, last_idx = -1;

	for (i = 0; i < nr; i++) {
		if (cmds[i].p_type == PT_LOAD) {
			last_idx = i;
			if (first_idx == -1)
				first_idx = i;
		}
	}
	if (first_idx == -1)
		return 0;

	return cmds[last_idx].p_vaddr + cmds[last_idx].p_memsz -
				ELF_PAGESTART(cmds[first_idx].p_vaddr);
}

static int
elf_read(file_s *file, void *buf, size_t len, loff_t pos)
{
	ssize_t rv;

	rv = kernel_read(file, buf, len, &pos);
	if (rv != len) {
		return (rv < 0) ? rv : -EIO;
	}
	return 0;
}


/**
 * load_elf_phdrs() - load ELF program headers
 * @elf_ex:   ELF header of the binary whose program headers should be loaded
 * @elf_file: the opened ELF binary file
 *
 * Loads ELF program headers from the binary file elf_file, which has the ELF
 * header pointed to by elf_ex, into a newly allocated array. The caller is
 * responsible for freeing the allocated data. Returns an ERR_PTR upon failure.
 */
static elf_phdr_t
*load_elf_phdrs(const elfhdr_t *elf_ex, file_s *elf_file)
{
	elf_phdr_t		*elf_phdata = NULL;
	int				retval,
					err = -1;
	unsigned int	size;

	/*
	 * If the size of this structure has changed, then punt, since
	 * we will be doing the wrong thing.
	 */
	if (elf_ex->e_phentsize != sizeof(elf_phdr_t))
		goto out;

	/* Sanity check the number of program headers... */
	/* ...and their total size. */
	size = sizeof(elf_phdr_t) * elf_ex->e_phnum;
	if (size == 0 || size > 65536 || size > ELF_MIN_ALIGN)
		goto out;

	elf_phdata = kmalloc(size, GFP_KERNEL);
	if (!elf_phdata)
		goto out;

	/* Read in the program headers */
	retval = elf_read(elf_file, elf_phdata, size, elf_ex->e_phoff);
	if (retval < 0) {
		err = retval;
		goto out;
	}

	/* Success! */
	err = 0;
out:
	if (err) {
		kfree(elf_phdata);
		elf_phdata = NULL;
	}
	return elf_phdata;
}


static int load_elf_binary(linux_bprm_s *bprm)
{
	file_s			*interpreter = NULL; /* to shut gcc up */
	unsigned long	load_addr,
					load_bias = 0,
					phdr_addr = 0;
	int				load_addr_set = 0;
	unsigned long	error;
	elf_phdr_t		*elf_ppnt,
					*elf_phdata,
					*interp_elf_phdata = NULL;
	elf_phdr_t		*elf_property_phdata = NULL;
	unsigned long	elf_bss,
					elf_brk;
	int				bss_prot = 0;
	int				retval, i;
	unsigned long	elf_entry;
	unsigned long	e_entry;
	unsigned long	interp_load_addr = 0;
	unsigned long	start_code,
					end_code,
					start_data,
					end_data;
	unsigned long	reloc_func_desc __maybe_unused = 0;
	int				executable_stack = EXSTACK_DEFAULT;
	elfhdr_t		*elf_ex = (elfhdr_t *)bprm->buf;
	elfhdr_t		*interp_elf_ex = NULL;
	// struct arch_elf_state arch_state = INIT_ARCH_ELF_STATE;
	mm_s			*mm;
	pt_regs_s		*regs;

	retval = -ENOEXEC;
	/* First of all, some simple consistency checks */
	if (memcmp(elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
		goto out;

	if (elf_ex->e_type != ET_EXEC && elf_ex->e_type != ET_DYN)
		goto out;
	if (!elf_check_arch(elf_ex))
		goto out;
	// if (elf_check_fdpic(elf_ex))
	// 	goto out;
	// if (!bprm->file->f_op->mmap)
	// 	goto out;

	elf_phdata = load_elf_phdrs(elf_ex, bprm->file);
	if (!elf_phdata)
		goto out;

	elf_ppnt = elf_phdata;
	for (i = 0; i < elf_ex->e_phnum; i++, elf_ppnt++) {
		char *elf_interpreter;

		if (elf_ppnt->p_type == PT_GNU_PROPERTY) {
			elf_property_phdata = elf_ppnt;
			continue;
		}

		if (elf_ppnt->p_type != PT_INTERP)
			continue;

		/*
		 * This is the program interpreter used for shared libraries -
		 * for now assume that this is an a.out format binary.
		 */
		retval = -ENOEXEC;
		if (elf_ppnt->p_filesz > PATH_MAX || elf_ppnt->p_filesz < 2)
			goto out_free_ph;

		retval = -ENOMEM;
		elf_interpreter = kmalloc(elf_ppnt->p_filesz, GFP_KERNEL);
		if (!elf_interpreter)
			goto out_free_ph;

		retval = elf_read(bprm->file, elf_interpreter, elf_ppnt->p_filesz,
					elf_ppnt->p_offset);
		if (retval < 0)
			goto out_free_interp;
		/* make sure path is NULL terminated */
		retval = -ENOEXEC;
		if (elf_interpreter[elf_ppnt->p_filesz - 1] != '\0')
			goto out_free_interp;

		interpreter = open_exec(elf_interpreter);
		kfree(elf_interpreter);
		retval = PTR_ERR(interpreter);
		if (IS_ERR(interpreter))
			goto out_free_ph;

		/*
		 * If the binary is not readable then enforce mm->dumpable = 0
		 * regardless of the interpreter's permissions.
		 */
		// would_dump(bprm, interpreter);

		interp_elf_ex = kmalloc(sizeof(*interp_elf_ex), GFP_KERNEL);
		if (!interp_elf_ex) {
			retval = -ENOMEM;
			goto out_free_ph;
		}

		/* Get the exec headers */
		retval = elf_read(interpreter, interp_elf_ex,
				  sizeof(*interp_elf_ex), 0);
		if (retval < 0)
			goto out_free_dentry;

		break;

out_free_interp:
		kfree(elf_interpreter);
		goto out_free_ph;
	}

	// elf_ppnt = elf_phdata;
	// for (i = 0; i < elf_ex->e_phnum; i++, elf_ppnt++)
	// 	switch (elf_ppnt->p_type) {
	// 	case PT_GNU_STACK:
	// 		if (elf_ppnt->p_flags & PF_X)
	// 			executable_stack = EXSTACK_ENABLE_X;
	// 		else
	// 			executable_stack = EXSTACK_DISABLE_X;
	// 		break;

	// 	case PT_LOPROC ... PT_HIPROC:
	// 		retval = arch_elf_pt_proc(elf_ex, elf_ppnt,
	// 				bprm->file, false, NULL);
	// 		if (retval)
	// 			goto out_free_dentry;
	// 		break;
	// 	}

	/* Some simple consistency checks for the interpreter */
	if (interpreter) {
		// retval = -ELIBBAD;
		// /* Not an ELF interpreter */
		// if (memcmp(interp_elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
		// 	goto out_free_dentry;
		// /* Verify the interpreter has a valid arch */
		// if (!elf_check_arch(interp_elf_ex) ||
		//     elf_check_fdpic(interp_elf_ex))
		// 	goto out_free_dentry;

		// /* Load the interpreter program headers */
		// interp_elf_phdata = load_elf_phdrs(interp_elf_ex,
		// 				   interpreter);
		// if (!interp_elf_phdata)
		// 	goto out_free_dentry;

		// /* Pass PT_LOPROC..PT_HIPROC headers to arch code */
		// elf_property_phdata = NULL;
		// elf_ppnt = interp_elf_phdata;
		// for (i = 0; i < interp_elf_ex->e_phnum; i++, elf_ppnt++)
		// 	switch (elf_ppnt->p_type) {
		// 	case PT_GNU_PROPERTY:
		// 		elf_property_phdata = elf_ppnt;
		// 		break;

		// 	case PT_LOPROC ... PT_HIPROC:
		// 		retval = arch_elf_pt_proc(interp_elf_ex,
		// 					  elf_ppnt, interpreter,
		// 					  true, &arch_state);
		// 		if (retval)
		// 			goto out_free_dentry;
		// 		break;
		// 	}
	}

	// retval = parse_elf_properties(interpreter ?: bprm->file,
	// 		elf_property_phdata);
	// if (retval)
	// 	goto out_free_dentry;

	// /*
	//  * Allow arch code to reject the ELF at this point, whilst it's
	//  * still possible to return an error to the code that invoked
	//  * the exec syscall.
	//  */
	// retval = arch_check_elf(elf_ex,
	// 			!!interpreter, interp_elf_ex,
	// 			&arch_state);
	// if (retval)
	// 	goto out_free_dentry;

	/* Flush all traces of the currently running executable */
	// retval = begin_new_exec(bprm);
	// if (retval)
	// 	goto out_free_dentry;

	// /* Do this immediately, since STACK_TOP as used in setup_arg_pages
	//    may depend on the personality.  */
	// SET_PERSONALITY2(*elf_ex, &arch_state);
	// if (elf_read_implies_exec(*elf_ex, executable_stack))
	// 	current->personality |= READ_IMPLIES_EXEC;

	// if (!(current->personality & ADDR_NO_RANDOMIZE) && randomize_va_space)
	// 	current->flags |= PF_RANDOMIZE;

	// setup_new_exec(bprm);

	// /* Do this so that we can load the interpreter, if need be.  We will
	//    change some of these later */
	// retval = setup_arg_pages(bprm, randomize_stack_top(STACK_TOP),
	// 			 executable_stack);
	// if (retval < 0)
	// 	goto out_free_dentry;
	
	elf_bss = 0;
	elf_brk = 0;

	start_code = ~0UL;
	end_code = 0;
	start_data = 0;
	end_data = 0;

	/* Now we do a little grungy work by mmapping the ELF image into
	   the correct location in memory. */
	for(i = 0, elf_ppnt = elf_phdata;
	    i < elf_ex->e_phnum; i++, elf_ppnt++) {
		int elf_prot, elf_flags;
		unsigned long k, vaddr;
		unsigned long total_size = 0;
		unsigned long alignment;

		if (elf_ppnt->p_type != PT_LOAD)
			continue;

		if (elf_brk > elf_bss) {
			unsigned long nbyte;

			/* There was a PT_LOAD segment with p_memsz > p_filesz
			   before this one. Map anonymous pages, if needed,
			   and clear the area.  */
			retval = set_brk(elf_bss + load_bias,
					 elf_brk + load_bias, bss_prot);
			if (retval)
				goto out_free_dentry;
			// nbyte = ELF_PAGEOFFSET(elf_bss);
			// if (nbyte) {
			// 	nbyte = ELF_MIN_ALIGN - nbyte;
			// 	if (nbyte > elf_brk - elf_bss)
			// 		nbyte = elf_brk - elf_bss;
			// 	if (clear_user((void __user *)elf_bss +
			// 				load_bias, nbyte)) {
			// 		/*
			// 		 * This bss-zeroing can fail if the ELF
			// 		 * file specifies odd protections. So
			// 		 * we don't check the return value
			// 		 */
			// 	}
			// }
		}

		// elf_prot = make_prot(elf_ppnt->p_flags, &arch_state,
		// 		     !!interpreter, false);

		// elf_flags = MAP_PRIVATE;

		vaddr = elf_ppnt->p_vaddr;
		// /*
		//  * The first time through the loop, load_addr_set is false:
		//  * layout will be calculated. Once set, use MAP_FIXED since
		//  * we know we've already safely mapped the entire region with
		//  * MAP_FIXED_NOREPLACE in the once-per-binary logic following.
		//  */
		// if (load_addr_set) {
		// 	elf_flags |= MAP_FIXED;
		// } else if (elf_ex->e_type == ET_EXEC) {
		// 	/*
		// 	 * This logic is run once for the first LOAD Program
		// 	 * Header for ET_EXEC binaries. No special handling
		// 	 * is needed.
		// 	 */
		// 	elf_flags |= MAP_FIXED_NOREPLACE;
		// } else if (elf_ex->e_type == ET_DYN) {
		// 	/*
		// 	 * This logic is run once for the first LOAD Program
		// 	 * Header for ET_DYN binaries to calculate the
		// 	 * randomization (load_bias) for all the LOAD
		// 	 * Program Headers.
		// 	 *
		// 	 * There are effectively two types of ET_DYN
		// 	 * binaries: programs (i.e. PIE: ET_DYN with INTERP)
		// 	 * and loaders (ET_DYN without INTERP, since they
		// 	 * _are_ the ELF interpreter). The loaders must
		// 	 * be loaded away from programs since the program
		// 	 * may otherwise collide with the loader (especially
		// 	 * for ET_EXEC which does not have a randomized
		// 	 * position). For example to handle invocations of
		// 	 * "./ld.so someprog" to test out a new version of
		// 	 * the loader, the subsequent program that the
		// 	 * loader loads must avoid the loader itself, so
		// 	 * they cannot share the same load range. Sufficient
		// 	 * room for the brk must be allocated with the
		// 	 * loader as well, since brk must be available with
		// 	 * the loader.
		// 	 *
		// 	 * Therefore, programs are loaded offset from
		// 	 * ELF_ET_DYN_BASE and loaders are loaded into the
		// 	 * independently randomized mmap region (0 load_bias
		// 	 * without MAP_FIXED nor MAP_FIXED_NOREPLACE).
		// 	 */
		// 	if (interpreter) {
		// 		load_bias = ELF_ET_DYN_BASE;
		// 		if (current->flags & PF_RANDOMIZE)
		// 			load_bias += arch_mmap_rnd();
		// 		alignment = maximum_alignment(elf_phdata, elf_ex->e_phnum);
		// 		if (alignment)
		// 			load_bias &= ~(alignment - 1);
		// 		elf_flags |= MAP_FIXED_NOREPLACE;
		// 	} else
		// 		load_bias = 0;

		// 	/*
		// 	 * Since load_bias is used for all subsequent loading
		// 	 * calculations, we must lower it by the first vaddr
		// 	 * so that the remaining calculations based on the
		// 	 * ELF vaddrs will be correctly offset. The result
		// 	 * is then page aligned.
		// 	 */
		// 	load_bias = ELF_PAGESTART(load_bias - vaddr);

			/*
			 * Calculate the entire size of the ELF mapping
			 * (total_size), used for the initial mapping,
			 * due to load_addr_set which is set to true later
			 * once the initial mapping is performed.
			 *
			 * Note that this is only sensible when the LOAD
			 * segments are contiguous (or overlapping). If
			 * used for LOADs that are far apart, this would
			 * cause the holes between LOADs to be mapped,
			 * running the risk of having the mapping fail,
			 * as it would be larger than the ELF file itself.
			 *
			 * As a result, only ET_DYN does this, since
			 * some ET_EXEC (e.g. ia64) may have large virtual
			 * memory holes between LOADs.
			 *
			 */
			total_size = total_mapping_size(elf_phdata,
							elf_ex->e_phnum);
			if (!total_size) {
				retval = -EINVAL;
				goto out_free_dentry;
			}
		// }

		error = elf_map(bprm->file, load_bias + vaddr,
				elf_ppnt, elf_prot, elf_flags, total_size);
		if (BAD_ADDR(error)) {
			retval = IS_ERR((void *)error) ?
				PTR_ERR((void*)error) : -EINVAL;
			goto out_free_dentry;
		}

	// 	if (!load_addr_set) {
	// 		load_addr_set = 1;
	// 		load_addr = (elf_ppnt->p_vaddr - elf_ppnt->p_offset);
	// 		if (elf_ex->e_type == ET_DYN) {
	// 			load_bias += error -
	// 			             ELF_PAGESTART(load_bias + vaddr);
	// 			load_addr += load_bias;
	// 			reloc_func_desc = load_bias;
	// 		}
	// 	}

	// 	/*
	// 	 * Figure out which segment in the file contains the Program
	// 	 * Header table, and map to the associated memory address.
	// 	 */
	// 	if (elf_ppnt->p_offset <= elf_ex->e_phoff &&
	// 	    elf_ex->e_phoff < elf_ppnt->p_offset + elf_ppnt->p_filesz) {
	// 		phdr_addr = elf_ex->e_phoff - elf_ppnt->p_offset +
	// 			    elf_ppnt->p_vaddr;
	// 	}

	// 	k = elf_ppnt->p_vaddr;
	// 	if ((elf_ppnt->p_flags & PF_X) && k < start_code)
	// 		start_code = k;
	// 	if (start_data < k)
	// 		start_data = k;

	// 	/*
	// 	 * Check to see if the section's size will overflow the
	// 	 * allowed task size. Note that p_filesz must always be
	// 	 * <= p_memsz so it is only necessary to check p_memsz.
	// 	 */
	// 	if (BAD_ADDR(k) || elf_ppnt->p_filesz > elf_ppnt->p_memsz ||
	// 	    elf_ppnt->p_memsz > TASK_SIZE ||
	// 	    TASK_SIZE - elf_ppnt->p_memsz < k) {
	// 		/* set_brk can never work. Avoid overflows. */
	// 		retval = -EINVAL;
	// 		goto out_free_dentry;
	// 	}

	// 	k = elf_ppnt->p_vaddr + elf_ppnt->p_filesz;

	// 	if (k > elf_bss)
	// 		elf_bss = k;
	// 	if ((elf_ppnt->p_flags & PF_X) && end_code < k)
	// 		end_code = k;
	// 	if (end_data < k)
	// 		end_data = k;
	// 	k = elf_ppnt->p_vaddr + elf_ppnt->p_memsz;
	// 	if (k > elf_brk) {
	// 		bss_prot = elf_prot;
	// 		elf_brk = k;
	// 	}
	}

	// e_entry = elf_ex->e_entry + load_bias;
	// phdr_addr += load_bias;
	// elf_bss += load_bias;
	// elf_brk += load_bias;
	// start_code += load_bias;
	// end_code += load_bias;
	// start_data += load_bias;
	// end_data += load_bias;

	// /* Calling set_brk effectively mmaps the pages that we need
	//  * for the bss and break sections.  We must do this before
	//  * mapping in the interpreter, to make sure it doesn't wind
	//  * up getting placed where the bss needs to go.
	//  */
	// retval = set_brk(elf_bss, elf_brk, bss_prot);
	// if (retval)
	// 	goto out_free_dentry;
	// if (likely(elf_bss != elf_brk) && unlikely(padzero(elf_bss))) {
	// 	retval = -EFAULT; /* Nobody gets to see this, but.. */
	// 	goto out_free_dentry;
	// }

	// if (interpreter) {
	// 	elf_entry = load_elf_interp(interp_elf_ex,
	// 				    interpreter,
	// 				    load_bias, interp_elf_phdata,
	// 				    &arch_state);
	// 	if (!IS_ERR((void *)elf_entry)) {
	// 		/*
	// 		 * load_elf_interp() returns relocation
	// 		 * adjustment
	// 		 */
	// 		interp_load_addr = elf_entry;
	// 		elf_entry += interp_elf_ex->e_entry;
	// 	}
	// 	if (BAD_ADDR(elf_entry)) {
	// 		retval = IS_ERR((void *)elf_entry) ?
	// 				(int)elf_entry : -EINVAL;
	// 		goto out_free_dentry;
	// 	}
	// 	reloc_func_desc = interp_load_addr;

	// 	allow_write_access(interpreter);
	// 	fput(interpreter);

	// 	kfree(interp_elf_ex);
	// 	kfree(interp_elf_phdata);
	// } else {
	// 	elf_entry = e_entry;
	// 	if (BAD_ADDR(elf_entry)) {
	// 		retval = -EINVAL;
	// 		goto out_free_dentry;
	// 	}
	// }

	// kfree(elf_phdata);

	// set_binfmt(&elf_format);

// #ifdef ARCH_HAS_SETUP_ADDITIONAL_PAGES
// 	retval = ARCH_SETUP_ADDITIONAL_PAGES(bprm, elf_ex, !!interpreter);
// 	if (retval < 0)
// 		goto out;
// #endif /* ARCH_HAS_SETUP_ADDITIONAL_PAGES */

// 	retval = create_elf_tables(bprm, elf_ex, interp_load_addr,
// 				   e_entry, phdr_addr);
// 	if (retval < 0)
// 		goto out;

// 	mm = current->mm;
// 	mm->end_code = end_code;
// 	mm->start_code = start_code;
// 	mm->start_data = start_data;
// 	mm->end_data = end_data;
// 	mm->start_stack = bprm->p;

// 	if ((current->flags & PF_RANDOMIZE) && (randomize_va_space > 1)) {
// 		/*
// 		 * For architectures with ELF randomization, when executing
// 		 * a loader directly (i.e. no interpreter listed in ELF
// 		 * headers), move the brk area out of the mmap region
// 		 * (since it grows up, and may collide early with the stack
// 		 * growing down), and into the unused ELF_ET_DYN_BASE region.
// 		 */
// 		if (IS_ENABLED(CONFIG_ARCH_HAS_ELF_RANDOMIZE) &&
// 		    elf_ex->e_type == ET_DYN && !interpreter) {
// 			mm->brk = mm->start_brk = ELF_ET_DYN_BASE;
// 		}

// 		mm->brk = mm->start_brk = arch_randomize_brk(mm);
// #ifdef compat_brk_randomized
// 		current->brk_randomized = 1;
// #endif
// 	}

// 	if (current->personality & MMAP_PAGE_ZERO) {
// 		/* Why this, you ask???  Well SVr4 maps page 0 as read-only,
// 		   and some applications "depend" upon this behavior.
// 		   Since we do not have the power to recompile these, we
// 		   emulate the SVr4 behavior. Sigh. */
// 		error = vm_mmap(NULL, 0, PAGE_SIZE, PROT_READ | PROT_EXEC,
// 				MAP_FIXED | MAP_PRIVATE, 0);
// 	}

// 	regs = current_pt_regs();
// #ifdef ELF_PLAT_INIT
// 	/*
// 	 * The ABI may specify that certain registers be set up in special
// 	 * ways (on i386 %edx is the address of a DT_FINI function, for
// 	 * example.  In addition, it may also specify (eg, PowerPC64 ELF)
// 	 * that the e_entry field is the address of the function descriptor
// 	 * for the startup routine, rather than the address of the startup
// 	 * routine itself.  This macro performs whatever initialization to
// 	 * the regs structure is required as well as any relocations to the
// 	 * function descriptor entries when executing dynamically links apps.
// 	 */
// 	ELF_PLAT_INIT(regs, reloc_func_desc);
// #endif

// 	finalize_exec(bprm);
// 	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;

	/* error cleanup */
out_free_dentry:
	kfree(interp_elf_ex);
	kfree(interp_elf_phdata);
	// allow_write_access(interpreter);
	if (interpreter)
		fput(interpreter);
out_free_ph:
	kfree(elf_phdata);
	goto out;
}


// static int __init init_elf_binfmt(void)
int __init init_elf_binfmt(void)
{
	list_init(&elf_format.lh, &elf_format);
	register_binfmt(&elf_format);
	return 0;
}

static void __exit exit_elf_binfmt(void)
{
	/* Remove the COFF and ELF loaders. */
	unregister_binfmt(&elf_format);
}