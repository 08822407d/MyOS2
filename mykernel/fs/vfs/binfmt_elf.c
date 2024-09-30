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

#include <linux/kernel/kernel.h>
#include <linux/fs/fs.h>
#include <linux/kernel/log2.h>
#include <linux/kernel/mm.h>
#include <linux/lib/errno.h>
#include <linux/kernel/signal.h>
#include <linux/fs/binfmts.h>
#include <linux/lib/string.h>
#include <linux/fs/file.h>
#include <linux/init/init.h>
#include <linux/compiler/compiler.h>
#include <linux/kernel/elf.h>
#include <linux/kernel/sched.h>
#include <linux/sched/coredump.h>
#include <linux/kernel/sizes.h>
#include <linux/kernel/types.h>
#include <linux/kernel/uaccess.h>
#include <uapi/linux/mman.h>


#include <linux/kernel/ptrace.h>


#ifndef ELF_COMPAT
#  define ELF_COMPAT	0
#endif

#ifndef user_long_t
#  define user_long_t long
#endif
#ifndef user_siginfo_t
#  define user_siginfo_t siginfo_t
#endif

/* That's for binfmt_elf_fdpic to deal with */
#ifndef elf_check_fdpic
#  define elf_check_fdpic(ex)	false
#endif

static int load_elf_binary(linux_bprm_s *bprm);

// #ifdef CONFIG_USELIB
	static int load_elf_library(file_s *);
// #else
// #  define load_elf_library	NULL
// #endif

/*
 * If we don't support core dumping, then supply a NULL so we
 * don't even try.
 */
#ifdef CONFIG_ELF_CORE
	static int elf_core_dump(struct coredump_params *cprm);
#else
#  define elf_core_dump		NULL
#endif

// #if ELF_EXEC_PAGESIZE > PAGE_SIZE
// #	define ELF_MIN_ALIGN	ELF_EXEC_PAGESIZE
// #else
#  define ELF_MIN_ALIGN		PAGE_SIZE
// #endif

#ifndef ELF_CORE_EFLAGS
#  define ELF_CORE_EFLAGS	0
#endif

#define ELF_PAGESTART(_v)	((_v) & ~(unsigned long)(ELF_MIN_ALIGN - 1))
#define ELF_PAGEOFFSET(_v)	((_v) & (ELF_MIN_ALIGN - 1))
#define ELF_PAGEALIGN(_v)	(((_v) + ELF_MIN_ALIGN - 1) & ~(ELF_MIN_ALIGN - 1))

static linux_bfmt_s elf_format = {
	// .module			= THIS_MODULE,
	.load_binary	= load_elf_binary,
	.load_shlib		= load_elf_library,
	// .core_dump		= elf_core_dump,
	.min_coredump	= ELF_EXEC_PAGESIZE,
};

#define BAD_ADDR(x) (unlikely((unsigned long)(x) >= TASK_SIZE))

/*
 * We need to explicitly zero any trailing portion of the page that follows
 * p_filesz when it ends before the page ends (e.g. bss), otherwise this
 * memory will contain the junk from the file that should not be present.
 */
static int
padzero(ulong elf_bss) {
	ulong nbyte;
	nbyte = ELF_PAGEOFFSET(elf_bss);
	if (nbyte) {
		nbyte = ELF_MIN_ALIGN - nbyte;
		if (clear_user((void __user *) elf_bss, nbyte))
			return -EFAULT;
	}
	return 0;
}

#define STACK_ADD(sp, items)	\
			((elf_addr_t __user *)(sp) - (items))
#define STACK_ROUND(sp, items)	\
			(((unsigned long) (sp - items)) &~ 15UL)
#define STACK_ALLOC(sp, len) (sp -= len)

#ifndef ELF_BASE_PLATFORM
/*
 * AT_BASE_PLATFORM indicates the "real" hardware/microarchitecture.
 * If the arch defines ELF_BASE_PLATFORM (in asm/elf.h), the value
 * will be copied to the user stack in the same manner as AT_PLATFORM.
 */
#  define ELF_BASE_PLATFORM	NULL
#endif

static int
create_elf_tables(linux_bprm_s *bprm, const elfhdr_s *exec,
		ulong interp_load_addr, ulong e_entry, ulong phdr_addr) {

	mm_s *mm = current->mm;
	ulong p = bprm->p;
	int argc = bprm->argc;
	int envc = bprm->envc;
	elf_addr_t __user *sp = (void *)p;
	elf_addr_t __user *u_platform;
	elf_addr_t __user *u_base_platform;
	elf_addr_t __user *u_rand_bytes;
	const char *k_platform = ELF_PLATFORM;
	const char *k_base_platform = ELF_BASE_PLATFORM;
	unchar k_rand_bytes[16];
	int items;
	elf_addr_t *elf_info;
	elf_addr_t flags = 0;
	int ei_index;
	// const struct cred *cred = current_cred();
	vma_s *vma;

	/*
	 * In some cases (e.g. Hyper-Threading), we want to avoid L1
	 * evictions by the processes running on the same package. One
	 * thing we can do is to shuffle the initial stack for them.
	 */
	p = arch_align_stack(p);

	/*
	 * If this architecture has a platform capability string, copy it
	 * to userspace.  In some cases (Sparc), this info is impossible
	 * for userspace to get any other way, in others (i386) it is
	 * merely difficult.
	 */
	u_platform = NULL;
	if (k_platform) {
		size_t len = strlen(k_platform) + 1;

		u_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
		if (copy_to_user(u_platform, k_platform, len))
			return -EFAULT;
	}

	/*
	 * If this architecture has a "base" platform capability
	 * string, copy it to userspace.
	 */
	u_base_platform = NULL;
	if (k_base_platform) {
		size_t len = strlen(k_base_platform) + 1;

		u_base_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
		if (copy_to_user(u_base_platform, k_base_platform, len))
			return -EFAULT;
	}

	/*
	//  * Generate 16 random bytes for userspace PRNG seeding.
	//  */
	// get_random_bytes(k_rand_bytes, sizeof(k_rand_bytes));
	// u_rand_bytes = (elf_addr_t __user *)
	// 	       STACK_ALLOC(p, sizeof(k_rand_bytes));
	// if (copy_to_user(u_rand_bytes, k_rand_bytes, sizeof(k_rand_bytes)))
	// 	return -EFAULT;

	/* Create the ELF interpreter info */
	elf_info = (elf_addr_t *)mm->saved_auxv;
	/* update AT_VECTOR_SIZE_BASE if the number of NEW_AUX_ENT() changes */
#define NEW_AUX_ENT(id, val)	\
		do {					\
			*elf_info++ = id;	\
			*elf_info++ = val;	\
		} while (0)

#ifdef ARCH_DLINFO
	/* 
	 * ARCH_DLINFO must come first so PPC can do its special alignment of
	 * AUXV.
	 * update AT_VECTOR_SIZE_ARCH if the number of NEW_AUX_ENT() in
	 * ARCH_DLINFO changes
	 */
	ARCH_DLINFO;
#endif
	NEW_AUX_ENT(AT_HWCAP, ELF_HWCAP);
	NEW_AUX_ENT(AT_PAGESZ, ELF_EXEC_PAGESIZE);
	NEW_AUX_ENT(AT_CLKTCK, CLOCKS_PER_SEC);
	NEW_AUX_ENT(AT_PHDR, phdr_addr);
	NEW_AUX_ENT(AT_PHENT, sizeof(struct elf_phdr));
	NEW_AUX_ENT(AT_PHNUM, exec->e_phnum);
	NEW_AUX_ENT(AT_BASE, interp_load_addr);
	if (bprm->interp_flags & BINPRM_FLAGS_PRESERVE_ARGV0)
		flags |= AT_FLAGS_PRESERVE_ARGV0;
	NEW_AUX_ENT(AT_FLAGS, flags);
	NEW_AUX_ENT(AT_ENTRY, e_entry);
	// NEW_AUX_ENT(AT_UID, from_kuid_munged(cred->user_ns, cred->uid));
	// NEW_AUX_ENT(AT_EUID, from_kuid_munged(cred->user_ns, cred->euid));
	// NEW_AUX_ENT(AT_GID, from_kgid_munged(cred->user_ns, cred->gid));
	// NEW_AUX_ENT(AT_EGID, from_kgid_munged(cred->user_ns, cred->egid));
	NEW_AUX_ENT(AT_SECURE, bprm->secureexec);
	// NEW_AUX_ENT(AT_RANDOM, (elf_addr_t)(ulong)u_rand_bytes);
#ifdef ELF_HWCAP2
	NEW_AUX_ENT(AT_HWCAP2, ELF_HWCAP2);
#endif
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
	if (k_platform) {
		NEW_AUX_ENT(AT_PLATFORM,
			    (elf_addr_t)(ulong)u_platform);
	}
	if (k_base_platform) {
		NEW_AUX_ENT(AT_BASE_PLATFORM,
			    (elf_addr_t)(ulong)u_base_platform);
	}
	if (bprm->have_execfd) {
		NEW_AUX_ENT(AT_EXECFD, bprm->execfd);
	}
#ifdef CONFIG_RSEQ
	NEW_AUX_ENT(AT_RSEQ_FEATURE_SIZE, offsetof(struct rseq, end));
	NEW_AUX_ENT(AT_RSEQ_ALIGN, __alignof__(struct rseq));
#endif
#undef NEW_AUX_ENT
	/* AT_NULL is zero; clear the rest too */
	memset(elf_info, 0, (char *)mm->saved_auxv +
			sizeof(mm->saved_auxv) - (char *)elf_info);

	/* And advance past the AT_NULL entry.  */
	elf_info += 2;

	ei_index = elf_info - (elf_addr_t *)mm->saved_auxv;
	sp = STACK_ADD(p, ei_index);

	items = (argc + 1) + (envc + 1) + 1;
	bprm->p = STACK_ROUND(sp, items);

	/* Point sp at the lowest address on the stack */
	sp = (elf_addr_t __user *)bprm->p;

	/*
	 * Grow the stack manually; some architectures have a limit on how
	 * far ahead a user-space access may be in order to grow the stack.
	 */
	if (mmap_write_lock_killable(mm))
		return -EINTR;
	// vma = find_extend_vma_locked(mm, bprm->p);
	mmap_write_unlock(mm);
	if (!vma)
		return -EFAULT;

	/* Now, let's put argc (and argv, envp if appropriate) on the stack */
	if (put_user(argc, sp++))
		return -EFAULT;

	/* Populate list of argv pointers back to argv strings. */
	p = mm->arg_end = mm->arg_start;
	while (argc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
		p += len;
	}
	if (put_user(0, sp++))
		return -EFAULT;
	mm->arg_end = p;

	/* Populate list of envp pointers back to envp strings. */
	mm->env_end = mm->env_start = p;
	while (envc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
		p += len;
	}
	if (put_user(0, sp++))
		return -EFAULT;
	mm->env_end = p;

	/* Put the elf_info on the stack in the right place.  */
	if (copy_to_user(sp, mm->saved_auxv, ei_index * sizeof(elf_addr_t)))
		return -EFAULT;
	return 0;
}

/*
 * Map "eppnt->p_filesz" bytes from "filep" offset "eppnt->p_offset"
 * into memory at "addr". (Note that p_filesz is rounded up to the
 * next page, so any extra bytes from the file must be wiped.)
 */
static ulong
elf_map(file_s *filep, ulong addr, const elf_phdr_s *eppnt,
		int prot, int type, ulong total_size) {

	ulong map_addr;
	ulong size = eppnt->p_filesz + ELF_PAGEOFFSET(eppnt->p_vaddr);
	ulong off = eppnt->p_offset - ELF_PAGEOFFSET(eppnt->p_vaddr);
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
		if (!BAD_ADDR(map_addr))
			vm_munmap(map_addr+size, total_size-size);
	} else
		map_addr = vm_mmap(filep, addr, size, prot, type, off);

	if ((type & MAP_FIXED_NOREPLACE) &&
			PTR_ERR((void *)map_addr) == -EEXIST)
		pr_info("%d (%s): Uhuuh, elf segment at %px"
			" requested but the memory is mapped already\n",
			task_pid_nr(current), current->comm, (void *)addr);

	return(map_addr);
}

/*
 * Map "eppnt->p_filesz" bytes from "filep" offset "eppnt->p_offset"
 * into memory at "addr". Memory from "p_filesz" through "p_memsz"
 * rounded up to the next page is zeroed.
 */
static ulong
elf_load(file_s *filep, ulong addr, const elf_phdr_s *eppnt,
		int prot, int type, ulong total_size) {

	ulong zero_start, zero_end;
	ulong map_addr;

	if (eppnt->p_filesz) {
		map_addr = elf_map(filep, addr, eppnt, prot, type, total_size);
		if (BAD_ADDR(map_addr))
			return map_addr;
		if (eppnt->p_memsz > eppnt->p_filesz) {
			zero_start = map_addr + eppnt->p_filesz +
							ELF_PAGEOFFSET(eppnt->p_vaddr);
			zero_end = map_addr + eppnt->p_memsz +
							ELF_PAGEOFFSET(eppnt->p_vaddr);

			/*
			 * Zero the end of the last mapped page but ignore
			 * any errors if the segment isn't writable.
			 */
			if (padzero(zero_start) && (prot & PROT_WRITE))
				return -EFAULT;
		}
	} else {
		map_addr = zero_start = ELF_PAGESTART(addr);
		zero_end = zero_start + ELF_PAGEOFFSET(eppnt->p_vaddr) +
			eppnt->p_memsz;
	}
	if (eppnt->p_memsz > eppnt->p_filesz) {
		/*
		 * Map the last of the segment.
		 * If the header is requesting these pages to be
		 * executable, honour that (ppc32 needs this).
		 */
		int error;

		zero_start = ELF_PAGEALIGN(zero_start);
		zero_end = ELF_PAGEALIGN(zero_end);

		error = vm_brk_flags(zero_start, zero_end - zero_start,
						prot & PROT_EXEC ? VM_EXEC : 0);
		if (error)
			map_addr = error;
	}
	return map_addr;
}

static ulong
total_mapping_size(const elf_phdr_s *phdr, int nr) {
	elf_addr_t min_addr = -1;
	elf_addr_t max_addr = 0;
	bool pt_load = false;

	for (int i = 0; i < nr; i++) {
		if (phdr[i].p_type == PT_LOAD) {
			min_addr = min(min_addr, ELF_PAGESTART(phdr[i].p_vaddr));
			max_addr = max(max_addr, phdr[i].p_vaddr + phdr[i].p_memsz);
			pt_load = true;
		}
	}
	return pt_load ? (max_addr - min_addr) : 0;
}

static int
elf_read(file_s *file, void *buf, size_t len, loff_t pos) {
	ssize_t rv;
	rv = kernel_read(file, buf, len, &pos);
	if (unlikely(rv != len)) {
		return (rv < 0) ? rv : -EIO;
	}
	return 0;
}

static ulong
maximum_alignment(elf_phdr_s *cmds, int nr) {
	ulong alignment = 0;
	int i;

	for (i = 0; i < nr; i++) {
		if (cmds[i].p_type == PT_LOAD) {
			ulong p_align = cmds[i].p_align;

			/* skip non-power of two alignments as invalid */
			if (!is_power_of_2(p_align))
				continue;
			alignment = max(alignment, p_align);
		}
	}

	/* ensure we align to at least one page */
	return ELF_PAGEALIGN(alignment);
}

/**
 * load_elf_phdrs() - load ELF program headers
 * @elf_ex:   ELF header of the binary whose program headers should be loaded
 * @elf_file: the opened ELF binary file
 *
 * Loads ELF program headers from the binary file elf_file, which has the ELF
 * header pointed to by elf_ex, into a newly allocated array. The caller is
 * responsible for freeing the allocated data. Returns NULL upon failure.
 */
static elf_phdr_s
*load_elf_phdrs(const elfhdr_s *elf_ex, file_s *elf_file) {
	elf_phdr_s *elf_phdata = NULL;
	int retval = -1;
	uint size;

	/*
	 * If the size of this structure has changed, then punt, since
	 * we will be doing the wrong thing.
	 */
	if (elf_ex->e_phentsize != sizeof(elf_phdr_s))
		goto out;

	/* Sanity check the number of program headers... */
	/* ...and their total size. */
	size = sizeof(elf_phdr_s) * elf_ex->e_phnum;
	if (size == 0 || size > 65536 || size > ELF_MIN_ALIGN)
		goto out;

	elf_phdata = kmalloc(size, GFP_KERNEL);
	if (!elf_phdata)
		goto out;

	/* Read in the program headers */
	retval = elf_read(elf_file, elf_phdata, size, elf_ex->e_phoff);

out:
	if (retval) {
		kfree(elf_phdata);
		elf_phdata = NULL;
	}
	return elf_phdata;
}


/**
 * struct arch_elf_state - arch-specific ELF loading state
 *
 * This structure is used to preserve architecture specific data during
 * the loading of an ELF file, throughout the checking of architecture
 * specific ELF headers & through to the point where the ELF load is
 * known to be proceeding (ie. SET_PERSONALITY).
 *
 * This implementation is a dummy for architectures which require no
 * specific state.
 */
typedef struct arch_elf_state {
} arch_elf_state_s;
#define INIT_ARCH_ELF_STATE {}


static inline int
make_prot(u32 p_flags, arch_elf_state_s *arch_state,
		bool has_interp, bool is_interp) {

	int prot = 0;
	if (p_flags & PF_R)
		prot |= PROT_READ;
	if (p_flags & PF_W)
		prot |= PROT_WRITE;
	if (p_flags & PF_X)
		prot |= PROT_EXEC;

	return arch_elf_adjust_prot(prot,
			arch_state, has_interp, is_interp);
}

/* This is much more generalized than the library routine read function,
   so we keep this separate.  Technically the library read function
   is only provided so that we can read a.out libraries that have
   an ELF header */

static ulong
load_elf_interp(elfhdr_s *interp_elf_ex, file_s *interpreter, ulong no_base,
		elf_phdr_s *interp_elf_phdata, arch_elf_state_s *arch_state) {
	elf_phdr_s *eppnt;
	ulong load_addr = 0;
	int load_addr_set = 0;
	ulong error = ~0UL;
	ulong total_size;
	int i;

	/* First of all, some simple consistency checks */
	if (interp_elf_ex->e_type != ET_EXEC &&
			interp_elf_ex->e_type != ET_DYN)
		goto out;
	if (!elf_check_arch(interp_elf_ex) ||
			elf_check_fdpic(interp_elf_ex))
		goto out;
	if (!interpreter->f_op->mmap)
		goto out;

	total_size = total_mapping_size(interp_elf_phdata,
						interp_elf_ex->e_phnum);
	if (!total_size) {
		error = -EINVAL;
		goto out;
	}

	eppnt = interp_elf_phdata;
	for (i = 0; i < interp_elf_ex->e_phnum; i++, eppnt++) {
		if (eppnt->p_type == PT_LOAD) {
			int elf_type = MAP_PRIVATE;
			int elf_prot = make_prot(eppnt->p_flags,
								arch_state, true, true);
			ulong vaddr = 0;
			ulong k, map_addr;

			vaddr = eppnt->p_vaddr;
			if (interp_elf_ex->e_type == ET_EXEC || load_addr_set)
				elf_type |= MAP_FIXED;
			else if (no_base && interp_elf_ex->e_type == ET_DYN)
				load_addr = -vaddr;

			map_addr = elf_load(interpreter, load_addr + vaddr,
							eppnt, elf_prot, elf_type, total_size);
			total_size = 0;
			error = map_addr;
			if (BAD_ADDR(map_addr))
				goto out;

			if (!load_addr_set && interp_elf_ex->e_type == ET_DYN) {
				load_addr = map_addr - ELF_PAGESTART(vaddr);
				load_addr_set = 1;
			}

			/*
			 * Check to see if the section's size will overflow the
			 * allowed task size. Note that p_filesz must always be
			 * <= p_memsize so it's only necessary to check p_memsz.
			 */
			k = load_addr + eppnt->p_vaddr;
			if (BAD_ADDR(k) || eppnt->p_filesz > eppnt->p_memsz ||
					eppnt->p_memsz > TASK_SIZE || TASK_SIZE - eppnt->p_memsz < k) {
				error = -ENOMEM;
				goto out;
			}
		}
	}

	error = load_addr;
out:
	return error;
}


static int
load_elf_binary(linux_bprm_s *bprm) {
	file_s		*interpreter = NULL; /* to shut gcc up */
	ulong		load_addr,
				load_bias = 0,
				phdr_addr = 0;
	int			first_pt_load = 1;
	ulong		error;
	elf_phdr_s	*elf_ppnt,
				*elf_phdata,
				*interp_elf_phdata = NULL;
	elf_phdr_s	*elf_property_phdata = NULL;
	ulong		elf_brk;
	int			bss_prot = 0;
	int			retval, i;
	ulong		elf_entry;
	ulong		e_entry;
	ulong		interp_load_addr = 0;
	ulong		start_code,
				end_code,
				start_data,
				end_data;
	ulong		reloc_func_desc __maybe_unused = 0;
	int			executable_stack = EXSTACK_DEFAULT;
	elfhdr_s	*elf_ex = (elfhdr_s *)bprm->buf;
	elfhdr_s	*interp_elf_ex = NULL;
	mm_s		*mm;
	pt_regs_s	*regs;
	arch_elf_state_s	arch_state = INIT_ARCH_ELF_STATE;

	retval = -ENOEXEC;
	/* First of all, some simple consistency checks */
	if (memcmp(elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
		goto out;

	if (elf_ex->e_type != ET_EXEC && elf_ex->e_type != ET_DYN)
		goto out;
	if (!elf_check_arch(elf_ex))
		goto out;
	if (elf_check_fdpic(elf_ex))
		goto out;
	if (!bprm->file->f_op->mmap)
		goto out;

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
			goto out_free_file;
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

	elf_ppnt = elf_phdata;
	for (i = 0; i < elf_ex->e_phnum; i++, elf_ppnt++) {
		switch (elf_ppnt->p_type) {
		case PT_GNU_STACK:
			if (elf_ppnt->p_flags & PF_X)
				executable_stack = EXSTACK_ENABLE_X;
			else
				executable_stack = EXSTACK_DISABLE_X;
			break;

		case PT_LOPROC ... PT_HIPROC:
			/* x86-64 arch_elf_pt_proc() defination is empty */
			// retval = arch_elf_pt_proc(elf_ex, elf_ppnt,
			// 			bprm->file, false, &arch_state);
			// if (retval)
			// 	goto out_free_dentry;
			break;
		}
	}

	/* Some simple consistency checks for the interpreter */
	if (interpreter) {
		retval = -ELIBBAD;
		/* Not an ELF interpreter */
		if (memcmp(interp_elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
			goto out_free_dentry;
		/* Verify the interpreter has a valid arch */
		if (!elf_check_arch(interp_elf_ex) ||
			elf_check_fdpic(interp_elf_ex))
			goto out_free_dentry;

		/* Load the interpreter program headers */
		interp_elf_phdata = load_elf_phdrs(interp_elf_ex,
						   interpreter);
		if (!interp_elf_phdata)
			goto out_free_dentry;

		/* Pass PT_LOPROC..PT_HIPROC headers to arch code */
		elf_property_phdata = NULL;
		elf_ppnt = interp_elf_phdata;
		for (i = 0; i < interp_elf_ex->e_phnum; i++, elf_ppnt++)
			switch (elf_ppnt->p_type) {
			case PT_GNU_PROPERTY:
				elf_property_phdata = elf_ppnt;
				break;

			case PT_LOPROC ... PT_HIPROC:
				/* x86-64 arch_elf_pt_proc() defination is empty */
				// retval = arch_elf_pt_proc(interp_elf_ex, elf_ppnt,
				// 			interpreter, true, &arch_state);
				// if (retval)
				// 	goto out_free_dentry;
				break;
			}
	}

	/* x86-64 has no elf_arch_state */
	// retval = parse_elf_properties(interpreter ?: bprm->file,
	// 				elf_property_phdata, &arch_state);
	// if (retval)
	// 	goto out_free_dentry;

	// /*
	//  * Allow arch code to reject the ELF at this point, whilst it's
	//  * still possible to return an error to the code that invoked
	//  * the exec syscall.
	//  */
	/* x86-64 arch_check_elf() defination is empty */
	// retval = arch_check_elf(elf_ex, !!interpreter,
	// 			interp_elf_ex, &arch_state);
	// if (retval)
	// 	goto out_free_dentry;

	/* Flush all traces of the currently running executable */
	retval = begin_new_exec(bprm);
	if (retval)
		goto out_free_dentry;

	// /* Do this immediately, since STACK_TOP as used in setup_arg_pages
	//    may depend on the personality.  */
	// SET_PERSONALITY2(*elf_ex, &arch_state);
	// if (elf_read_implies_exec(*elf_ex, executable_stack))
	// 	current->personality |= READ_IMPLIES_EXEC;

	// const int snapshot_randomize_va_space = READ_ONCE(randomize_va_space);
	// if (!(current->personality & ADDR_NO_RANDOMIZE) && snapshot_randomize_va_space)
	// 	current->flags |= PF_RANDOMIZE;

	setup_new_exec(bprm);

	/* Do this so that we can load the interpreter, if need be.  We will
	   change some of these later */
	retval = setup_arg_pages(bprm,
				randomize_stack_top(STACK_TOP), executable_stack);
	if (retval < 0)
		goto out_free_dentry;
	
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
		ulong k, vaddr;
		ulong total_size = 0;
		ulong alignment;

		if (elf_ppnt->p_type != PT_LOAD)
			continue;

		elf_prot = make_prot(elf_ppnt->p_flags, &arch_state,
						!!interpreter, false);

		elf_flags = MAP_PRIVATE;

		vaddr = elf_ppnt->p_vaddr;
		/*
		 * The first time through the loop, first_pt_load is true:
		 * layout will be calculated. Once set, use MAP_FIXED since
		 * we know we've already safely mapped the entire region with
		 * MAP_FIXED_NOREPLACE in the once-per-binary logic following.
		 */
		if (!first_pt_load) {
			elf_flags |= MAP_FIXED;
		} else if (elf_ex->e_type == ET_EXEC) {
			/*
			 * This logic is run once for the first LOAD Program
			 * Header for ET_EXEC binaries. No special handling
			 * is needed.
			 */
			elf_flags |= MAP_FIXED_NOREPLACE;
		} else if (elf_ex->e_type == ET_DYN) {
			/*
			 * This logic is run once for the first LOAD Program
			 * Header for ET_DYN binaries to calculate the
			 * randomization (load_bias) for all the LOAD
			 * Program Headers.
			 */

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

			/* Calculate any requested alignment. */
			alignment = maximum_alignment(elf_phdata, elf_ex->e_phnum);

			/*
			 * There are effectively two types of ET_DYN
			 * binaries: programs (i.e. PIE: ET_DYN with PT_INTERP)
			 * and loaders (ET_DYN without PT_INTERP, since they
			 * _are_ the ELF interpreter). The loaders must
			 * be loaded away from programs since the program
			 * may otherwise collide with the loader (especially
			 * for ET_EXEC which does not have a randomized
			 * position). For example to handle invocations of
			 * "./ld.so someprog" to test out a new version of
			 * the loader, the subsequent program that the
			 * loader loads must avoid the loader itself, so
			 * they cannot share the same load range. Sufficient
			 * room for the brk must be allocated with the
			 * loader as well, since brk must be available with
			 * the loader.
			 *
			 * Therefore, programs are loaded offset from
			 * ELF_ET_DYN_BASE and loaders are loaded into the
			 * independently randomized mmap region (0 load_bias
			 * without MAP_FIXED nor MAP_FIXED_NOREPLACE).
			 */
			if (interpreter) {
				load_bias = ELF_ET_DYN_BASE;
				// if (current->flags & PF_RANDOMIZE)
				// 	load_bias += arch_mmap_rnd();
				alignment = maximum_alignment(elf_phdata, elf_ex->e_phnum);
				if (alignment)
					load_bias &= ~(alignment - 1);
				elf_flags |= MAP_FIXED_NOREPLACE;
			} else {
				/*
				 * For ET_DYN without PT_INTERP, we rely on
				 * the architectures's (potentially ASLR) mmap
				 * base address (via a load_bias of 0).
				 *
				 * When a large alignment is requested, we
				 * must do the allocation at address "0" right
				 * now to discover where things will load so
				 * that we can adjust the resulting alignment.
				 * In this case (load_bias != 0), we can use
				 * MAP_FIXED_NOREPLACE to make sure the mapping
				 * doesn't collide with anything.
				 */
				if (alignment > ELF_MIN_ALIGN) {
					load_bias = elf_load(bprm->file, 0, elf_ppnt,
									elf_prot, elf_flags, total_size);
					if (BAD_ADDR(load_bias)) {
						retval = IS_ERR_VALUE(load_bias) ?
									PTR_ERR((void*)load_bias) : -EINVAL;
						goto out_free_dentry;
					}
					vm_munmap(load_bias, total_size);
					/* Adjust alignment as requested. */
					if (alignment)
						load_bias &= ~(alignment - 1);
					elf_flags |= MAP_FIXED_NOREPLACE;
				} else
					load_bias = 0;
			}

			/*
			 * Since load_bias is used for all subsequent loading
			 * calculations, we must lower it by the first vaddr
			 * so that the remaining calculations based on the
			 * ELF vaddrs will be correctly offset. The result
			 * is then page aligned.
			 */
			load_bias = ELF_PAGESTART(load_bias - vaddr);
		}

		error = elf_load(bprm->file, load_bias + vaddr,
					elf_ppnt, elf_prot, elf_flags, total_size);
		if (BAD_ADDR(error)) {
			retval = IS_ERR_VALUE((void *)error) ?
				PTR_ERR((void*)error) : -EINVAL;
			goto out_free_dentry;
		}

		if (first_pt_load) {
			first_pt_load = 0;
			if (elf_ex->e_type == ET_DYN) {
				load_bias += error - ELF_PAGESTART(load_bias + vaddr);
				reloc_func_desc = load_bias;
			}
		}

		/*
		 * Figure out which segment in the file contains the Program
		 * Header table, and map to the associated memory address.
		 */
		if (elf_ppnt->p_offset <= elf_ex->e_phoff &&
		    elf_ex->e_phoff < elf_ppnt->p_offset + elf_ppnt->p_filesz) {
			phdr_addr = elf_ex->e_phoff - elf_ppnt->p_offset +
							elf_ppnt->p_vaddr;
		}

		k = elf_ppnt->p_vaddr;
		if ((elf_ppnt->p_flags & PF_X) && k < start_code)
			start_code = k;
		if (start_data < k)
			start_data = k;

		/*
		 * Check to see if the section's size will overflow the
		 * allowed task size. Note that p_filesz must always be
		 * <= p_memsz so it is only necessary to check p_memsz.
		 */
		if (BAD_ADDR(k) || elf_ppnt->p_filesz > elf_ppnt->p_memsz ||
		    elf_ppnt->p_memsz > TASK_SIZE ||
		    TASK_SIZE - elf_ppnt->p_memsz < k) {
			/* set_brk can never work. Avoid overflows. */
			retval = -EINVAL;
			goto out_free_dentry;
		}

		k = elf_ppnt->p_vaddr + elf_ppnt->p_filesz;

		if ((elf_ppnt->p_flags & PF_X) && end_code < k)
			end_code = k;
		if (end_data < k)
			end_data = k;
		k = elf_ppnt->p_vaddr + elf_ppnt->p_memsz;
		if (k > elf_brk)
			elf_brk = k;
	}

	e_entry = elf_ex->e_entry + load_bias;
	phdr_addr += load_bias;
	elf_brk += load_bias;
	start_code += load_bias;
	end_code += load_bias;
	start_data += load_bias;
	end_data += load_bias;

	current->mm->start_brk = current->mm->brk = ELF_PAGEALIGN(elf_brk);

	if (interpreter) {
		elf_entry = load_elf_interp(interp_elf_ex, interpreter,
						load_bias, interp_elf_phdata, &arch_state);
		if (!IS_ERR_VALUE(elf_entry)) {
			/*
			 * load_elf_interp() returns relocation
			 * adjustment
			 */
			interp_load_addr = elf_entry;
			elf_entry += interp_elf_ex->e_entry;
		}
		if (BAD_ADDR(elf_entry)) {
			retval = IS_ERR_VALUE(elf_entry) ?
					(int)elf_entry : -EINVAL;
			goto out_free_dentry;
		}
		reloc_func_desc = interp_load_addr;

		fput(interpreter);

		kfree(interp_elf_ex);
		kfree(interp_elf_phdata);
	} else {
		elf_entry = e_entry;
		if (BAD_ADDR(elf_entry)) {
			retval = -EINVAL;
			goto out_free_dentry;
		}
	}

	kfree(elf_phdata);

	set_binfmt(&elf_format);

// #ifdef ARCH_HAS_SETUP_ADDITIONAL_PAGES
	// retval = ARCH_SETUP_ADDITIONAL_PAGES(bprm, elf_ex, !!interpreter);
	// if (retval < 0)
	// 	goto out;
// #endif /* ARCH_HAS_SETUP_ADDITIONAL_PAGES */

	retval = create_elf_tables(bprm, elf_ex,
				interp_load_addr, e_entry, phdr_addr);
	if (retval < 0)
		goto out;

	mm = current->mm;
	mm->end_code = end_code;
	mm->start_code = start_code;
	mm->start_data = start_data;
	mm->end_data = end_data;
	mm->start_stack = bprm->p;

	// if ((current->flags & PF_RANDOMIZE) && (snapshot_randomize_va_space > 1)) {
	// 	/*
	// 	 * For architectures with ELF randomization, when executing
	// 	 * a loader directly (i.e. no interpreter listed in ELF
	// 	 * headers), move the brk area out of the mmap region
	// 	 * (since it grows up, and may collide early with the stack
	// 	 * growing down), and into the unused ELF_ET_DYN_BASE region.
	// 	 */
	// 	if (IS_ENABLED(CONFIG_ARCH_HAS_ELF_RANDOMIZE) &&
	// 	    elf_ex->e_type == ET_DYN && !interpreter) {
	// 		mm->brk = mm->start_brk = ELF_ET_DYN_BASE;
	// 	} else {
	// 		/* Otherwise leave a gap between .bss and brk. */
	// 		mm->brk = mm->start_brk = mm->brk + PAGE_SIZE;
	// 	}

	// 	mm->brk = mm->start_brk = arch_randomize_brk(mm);
	// }

	// if (current->personality & MMAP_PAGE_ZERO) {
	// 	/* Why this, you ask???  Well SVr4 maps page 0 as read-only,
	// 	   and some applications "depend" upon this behavior.
	// 	   Since we do not have the power to recompile these, we
	// 	   emulate the SVr4 behavior. Sigh. */
	// 	error = vm_mmap(NULL, 0, PAGE_SIZE, PROT_READ | PROT_EXEC,
	// 			MAP_FIXED | MAP_PRIVATE, 0);
	// }

	regs = current_pt_regs();
// #ifdef ELF_PLAT_INIT
	// /*
	//  * The ABI may specify that certain registers be set up in special
	//  * ways (on i386 %edx is the address of a DT_FINI function, for
	//  * example.  In addition, it may also specify (eg, PowerPC64 ELF)
	//  * that the e_entry field is the address of the function descriptor
	//  * for the startup routine, rather than the address of the startup
	//  * routine itself.  This macro performs whatever initialization to
	//  * the regs structure is required as well as any relocations to the
	//  * function descriptor entries when executing dynamically links apps.
	//  */
	// ELF_PLAT_INIT(regs, reloc_func_desc);
// #endif

	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;

	/* error cleanup */
out_free_dentry:
	kfree(interp_elf_ex);
	kfree(interp_elf_phdata);
out_free_file:
	if (interpreter)
		fput(interpreter);
out_free_ph:
	kfree(elf_phdata);
	goto out;
}

/* This is really simpleminded and specialized - we are loading an
   a.out library that is given an ELF header. */
static int
load_elf_library(file_s *file) {
	elf_phdr_s *elf_phdata;
	elf_phdr_s *eppnt;
	int retval, error, i, j;
	elfhdr_s elf_ex;

	error = -ENOEXEC;
	retval = elf_read(file, &elf_ex, sizeof(elf_ex), 0);
	if (retval < 0)
		goto out;

	if (memcmp(elf_ex.e_ident, ELFMAG, SELFMAG) != 0)
		goto out;

	/* First of all, some simple consistency checks */
	if (elf_ex.e_type != ET_EXEC || elf_ex.e_phnum > 2 ||
			!elf_check_arch(&elf_ex) || !file->f_op->mmap)
		goto out;
	if (elf_check_fdpic(&elf_ex))
		goto out;

	/* Now read in all of the header information */

	j = sizeof(struct elf_phdr) * elf_ex.e_phnum;
	/* j < ELF_MIN_ALIGN because elf_ex.e_phnum <= 2 */

	error = -ENOMEM;
	elf_phdata = kmalloc(j, GFP_KERNEL);
	if (!elf_phdata)
		goto out;

	eppnt = elf_phdata;
	error = -ENOEXEC;
	retval = elf_read(file, eppnt, j, elf_ex.e_phoff);
	if (retval < 0)
		goto out_free_ph;

	for (j = 0, i = 0; i<elf_ex.e_phnum; i++)
		if ((eppnt + i)->p_type == PT_LOAD)
			j++;
	if (j != 1)
		goto out_free_ph;

	while (eppnt->p_type != PT_LOAD)
		eppnt++;

	/* Now use mmap to map the library into memory. */
	error = elf_load(file, ELF_PAGESTART(eppnt->p_vaddr),
				eppnt, PROT_READ | PROT_WRITE | PROT_EXEC,
				MAP_FIXED_NOREPLACE | MAP_PRIVATE, 0);

	if (error != ELF_PAGESTART(eppnt->p_vaddr))
		goto out_free_ph;

	error = 0;

out_free_ph:
	kfree(elf_phdata);
out:
	return error;
}


// static int __init init_elf_binfmt(void)
int __init init_elf_binfmt(void)
{
	INIT_LIST_S(&elf_format.lh);
	register_binfmt(&elf_format);
	return 0;
}

static void __exit
exit_elf_binfmt(void) {
	/* Remove the COFF and ELF loaders. */
	unregister_binfmt(&elf_format);
}