/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BINFMTS_H
#define _LINUX_BINFMTS_H

	#include <linux/kernel/sched.h>
	// #include <linux/unistd.h>
	// #include <asm/exec.h>
	#include <uapi/linux/binfmts.h>

	struct filename;

	#define CORENAME_MAX_SIZE 128

	/*
	 * This structure is used to hold the arguments that are used when loading binaries.
	 */
	typedef struct linux_binprm {
		vma_s			*vma;
		ulong			vma_pages;
		mm_s			*mm;
		ulong			p; /* current top of mem */
		ulong			argmin; /* rlimit marker for copy_strings() */
		uint
			/* Should an execfd be passed to userspace? */
			have_execfd:1,

			/* Use the creds of a script (see binfmt_misc) */
			execfd_creds:1,
			/*
			 * Set by bprm_creds_for_exec hook to indicate a
			 * privilege-gaining exec has happened. Used to set
			 * AT_SECURE auxv for glibc.
			 */
			secureexec:1,
			/*
			 * Set when errors can no longer be returned to the
			 * original userspace.
			 */
			point_of_no_return:1;
	// #ifdef __alpha__
	// 	unsigned int taso:1;
	// #endif
		file_s			*executable; /* Executable to pass to the interpreter */
		file_s			*interpreter;
		file_s			*file;
	// 	struct cred *cred;	/* new credentials */
	// 	int unsafe;		/* how unsafe this exec is (mask of LSM_UNSAFE_*) */
	// 	unsigned int per_clear;	/* bits to clear in current->personality */
		int				argc, envc;
		const char		*filename;	/* Name of binary as seen by procps */
		const char		*interp;	/* Name of the binary really executed. Most
								   of the time same as filename, but could be
								   different for binfmt_{misc,script} */
		const char		*fdpath;	/* generated filename for execveat */
		uint			interp_flags;
		int				execfd;		/* File descriptor of the executable */
		ulong			loader, exec;

		rlimit_s		rlim_stack; /* Saved RLIMIT_STACK used during exec. */

		char		buf[BINPRM_BUF_SIZE];
	} linux_bprm_s;

	// #define BINPRM_FLAGS_ENFORCE_NONDUMP_BIT 0
	// #define BINPRM_FLAGS_ENFORCE_NONDUMP (1 << BINPRM_FLAGS_ENFORCE_NONDUMP_BIT)

	// /* filename of the binary will be inaccessible after exec */
	// #define BINPRM_FLAGS_PATH_INACCESSIBLE_BIT 2
	// #define BINPRM_FLAGS_PATH_INACCESSIBLE (1 << BINPRM_FLAGS_PATH_INACCESSIBLE_BIT)

	// /* preserve argv0 for the interpreter  */
	// #define BINPRM_FLAGS_PRESERVE_ARGV0_BIT 3
	// #define BINPRM_FLAGS_PRESERVE_ARGV0 (1 << BINPRM_FLAGS_PRESERVE_ARGV0_BIT)

	// /* Function parameter for binfmt->coredump */
	// struct coredump_params {
	// 	const kernel_siginfo_t *siginfo;
	// 	pt_regs_s *regs;
	// 	file_s *file;
	// 	unsigned long limit;
	// 	unsigned long mm_flags;
	// 	loff_t written;
	// 	loff_t pos;
	// 	loff_t to_skip;
	// 	int vma_count;
	// 	size_t vma_data_size;
	// 	struct core_vma_metadata *vma_meta;
	// };

	/*
	* This structure defines the functions that are used to load the binary formats that
	* linux accepts.
	*/
	typedef struct linux_binfmt {
		List_s		lh;
		// module_s	*module;
		int			(*load_binary)(linux_bprm_s *);
		int			(*load_shlib)(file_s *);
		// int			(*core_dump)(struct coredump_params *cprm);
		ulong		min_coredump;	/* minimal dump size */
	} linux_bfmt_s;

	// extern void __register_binfmt(linux_bfmt_s *fmt, int insert);

	// /* Registration of default binfmt handlers */
	// static inline void register_binfmt(linux_bfmt_s *fmt) {
	// 	__register_binfmt(fmt, 0);
	// }
	void register_binfmt(linux_bfmt_s *fmt);

	// /* Same as above, but adds a new binfmt at the top of the list */
	// static inline void insert_binfmt(struct linux_binfmt *fmt)
	// {
	// 	__register_binfmt(fmt, 1);
	// }

	extern void unregister_binfmt(linux_bfmt_s *);

	// extern int __must_check remove_arg_zero(linux_bprm_s *);
	extern int begin_new_exec(linux_bprm_s * bprm);
	extern void setup_new_exec(linux_bprm_s * bprm);
	extern void finalize_exec(linux_bprm_s *bprm);
	// extern void would_dump(linux_bprm_s *, file_s *);

	// extern int suid_dumpable;

	/* Stack area protections */
	#define EXSTACK_DEFAULT		0	/* Whatever the arch defaults to */
	#define EXSTACK_DISABLE_X	1	/* Disable executable stacks */
	#define EXSTACK_ENABLE_X	2	/* Enable executable stacks */

	extern int setup_arg_pages(linux_bprm_s * bprm,
			ulong stack_top, int executable_stack);
	// extern int transfer_args_to_stack(linux_bprm_s *bprm,
	// 				unsigned long *sp_location);
	// extern int bprm_change_interp(const char *interp, linux_bprm_s *bprm);
	// int copy_string_kernel(const char *arg, linux_bprm_s *bprm);
	extern void set_binfmt(linux_bfmt_s *new);
	// extern ssize_t read_code(file_s *, unsigned long, loff_t, size_t);

	int kernel_execve(const char *filename,
			const char *const *argv, const char *const *envp);

#endif /* _LINUX_BINFMTS_H */
