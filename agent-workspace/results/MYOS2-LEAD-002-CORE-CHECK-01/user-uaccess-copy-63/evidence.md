---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-UACCESS-COPY-63
record_type: bounded_direct_fixed_time_source_anchors
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---
# 第63批源码证据（A/B/C）

所有VERIFIED标签均来自固定time真实函数/宏/结构体定义中的连续1–5物理行。源码中的空行、制表符和原引文保留。source_context另列头文件#include、条件编译保护与调用者传值，绝不将通用fallback与x86定义视为运行时连续两种检查。不得从未预处理头文件推出已编译ABI或真实硬件行为。

## A 固定源直接定义锚点

### A01 · valid_user_address

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::valid_user_address]
```c
	#define valid_user_address(x) ((__force long)(x) >= 0)
```

### A02 · __access_ok

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
	static inline bool
    __access_ok(const void __user *ptr, ulong size) {
		if (__builtin_constant_p(size <= PAGE_SIZE) && size <= PAGE_SIZE) {
			return valid_user_address(ptr);
		} else {
```

### A03 · __access_ok

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
		} else {
			ulong sum = size + (__force ulong)ptr;
			return valid_user_address(sum) && sum >= (__force ulong)ptr;
		}
	}
```

### A04 · __access_ok

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
			ulong sum = size + (__force ulong)ptr;
			return valid_user_address(sum) && sum >= (__force ulong)ptr;
		}
```

### A05 · __access_ok

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
	#define __access_ok __access_ok
```

### A06 · __access_ok

[VERIFIED mykernel/include/uapi/asm-generic/access_ok.h::__access_ok]
```c
	static inline int
	__access_ok(const void *ptr, ulong size) {
		ulong limit = TASK_SIZE_MAX;
		ulong addr = (ulong)ptr;

```

### A07 · __access_ok

[VERIFIED mykernel/include/uapi/asm-generic/access_ok.h::__access_ok]
```c
		ulong limit = TASK_SIZE_MAX;
		ulong addr = (ulong)ptr;

		return (size <= limit) && (addr <= (limit - size));
	}
```

### A08 · access_ok

[VERIFIED mykernel/include/uapi/asm-generic/access_ok.h::access_ok]
```c
	#  define access_ok(addr, size) likely(__access_ok(addr, size))
```

### A09 · PAGE_SHIFT

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```

### A10 · PAGE_SIZE

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

### A11 · __VIRTUAL_MASK_SHIFT

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::__VIRTUAL_MASK_SHIFT]
```c
	#define __VIRTUAL_MASK_SHIFT	47
```

### A12 · task_size_max

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::task_size_max]
```c
	#define task_size_max()			((_AC(1, UL) << __VIRTUAL_MASK_SHIFT) - PAGE_SIZE)
```

### A13 · TASK_SIZE_MAX

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::TASK_SIZE_MAX]
```c
	#define TASK_SIZE_MAX			task_size_max()
```

### A14 · ulong

[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```c
		typedef unsigned long		ulong;
```

### A15 · __kernel_size_t

[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_size_t]
```c
			typedef __kernel_ulong_t	__kernel_size_t;
```

### A16 · __access_ok

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
		if (__builtin_constant_p(size <= PAGE_SIZE) && size <= PAGE_SIZE) {
			return valid_user_address(ptr);
		} else {
			ulong sum = size + (__force ulong)ptr;
			return valid_user_address(sum) && sum >= (__force ulong)ptr;
```


## B 固定源直接定义锚点

### B01 · _inline_copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
	static inline __must_check ulong
	_inline_copy_from_user(void *to, const void __user *from, ulong n) {
		ulong res = n;
		// might_fault();
		// if (!should_fail_usercopy() && likely(access_ok(from, n))) {
```

### B02 · _inline_copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
		// 	barrier_nospec();
		// 	instrument_copy_from_user_before(to, from, n);
			res = raw_copy_from_user(to, from, n);
		// 	instrument_copy_from_user_after(to, from, n, res);
		// }
```

### B03 · _inline_copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
		// }
		if (unlikely(res))
			memset(to + (n - res), 0, res);
		return res;
	}
```

### B04 · _inline_copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
	static inline __must_check ulong
	_inline_copy_to_user(void __user *to, const void *from, ulong n) {
		// might_fault();
		// if (should_fail_usercopy())
		// 	return n;
```

### B05 · _inline_copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```

### B06 · copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_from_user]
```c
	static __always_inline ulong __must_check
	copy_from_user(void *to, const void __user *from, ulong n) {
		// if (!check_copy_size(to, n, false))
		// 	return n;
	// #ifdef INLINE_COPY_FROM_USER
```

### B07 · copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_from_user]
```c
	// #ifdef INLINE_COPY_FROM_USER
		return _inline_copy_from_user(to, from, n);
	// #else
	// 	return _copy_from_user(to, from, n);
	// #endif
```

### B08 · copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```c
	static __always_inline ulong __must_check
	copy_to_user(void __user *to, const void *from, ulong n) {
		// if (!check_copy_size(from, n, true))
		// 	return n;
	// #ifdef INLINE_COPY_TO_USER
```

### B09 · copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```c
	// #ifdef INLINE_COPY_TO_USER
		return _inline_copy_to_user(to, from, n);
	// #else
	// 	return _copy_to_user(to, from, n);
	// #endif
```

### B10 · raw_copy_from_user

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::raw_copy_from_user]
```c
	static __always_inline __must_check ulong
	raw_copy_from_user(void *dst, const void __user *src, ulong size) {
		return copy_user_generic(dst, (__force void *)src, size);
	}
```

### B11 · raw_copy_to_user

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::raw_copy_to_user]
```c
	static __always_inline __must_check ulong
	raw_copy_to_user(void __user *dst, const void *src, ulong size) {
		return copy_user_generic((__force void *)dst, src, size);
	}
```

### B12 · copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
	static __always_inline __must_check ulong
	copy_user_generic(void *to, const void *from, ulong len) {
		stac();
		/*
		 * If CPU has FSRM feature, use 'rep movs'.
```

### B13 · copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
		asm volatile(	"1:\n\t"
						ALTERNATIVE("rep movsb",
							"call rep_movs_alternative",
							ALT_NOT(X86_FEATURE_FSRM)
						)
```

### B14 · copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
						"2:\n"
						_ASM_EXTABLE_UA(1b, 2b)
					:	"+c" (len), "+D" (to),
						"+S" (from), ASM_CALL_CONSTRAINT
					:
```

### B15 · copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
					:	"memory", "rax"
					);
		clac();
		return len;
	}
```

### B17 · ALT_NOT

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALT_NOT]
```c
	#define ALT_NOT(feature)			((ALT_FLAG_NOT << ALT_FLAGS_SHIFT) | (feature))
```

### B18 · ALTERNATIVE

[VERIFIED mykernel/arch/x86_64/include/asm/alternative.h::ALTERNATIVE]
```c
	#define ALTERNATIVE(oldinstr, newinstr, ft_flags)			\
		OLDINSTR(oldinstr)						\
		ALTINSTR_ENTRY(ft_flags)					\
		ALTINSTR_REPLACEMENT(newinstr)
```

### B19 · _ASM_EXTABLE_UA

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::_ASM_EXTABLE_UA]
```c
	#  define _ASM_EXTABLE_UA(from, to)	\
				_ASM_EXTABLE_TYPE(from, to, EX_TYPE_UACCESS)
```

### B20 · stac

[VERIFIED mykernel/arch/x86_64/include/asm/smap.h::stac]
```c
	static __always_inline void
	stac(void) {
		/* Note: a barrier is implicit in alternative() */
		alternative("", __ASM_STAC, X86_FEATURE_SMAP);
	}
```

### B21 · clac

[VERIFIED mykernel/arch/x86_64/include/asm/smap.h::clac]
```c
	static __always_inline void
	clac(void) {
		/* Note: a barrier is implicit in alternative() */
		alternative("", __ASM_CLAC, X86_FEATURE_SMAP);
	}
```

### B22 · __kernel_ssize_t

[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_ssize_t]
```c
			typedef __kernel_long_t		__kernel_ssize_t;
```

### B23 · _inline_copy_from_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
		if (unlikely(res))
			memset(to + (n - res), 0, res);
		return res;
```

### B24 · _inline_copy_to_user

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```

### B25 · copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
						_ASM_EXTABLE_UA(1b, 2b)
					:	"+c" (len), "+D" (to),
						"+S" (from), ASM_CALL_CONSTRAINT
					:
					:	"memory", "rax"
```

### B26 · copy_user_generic

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::copy_user_generic]
```c
	copy_user_generic(void *to, const void *from, ulong len) {
		stac();
		/*
```


## C 固定源直接定义锚点

### C01 · get_user

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::get_user]
```c
	#define get_user(x, ptr) ({					\
				(x) = *(__typeof(&(x)))(ptr);	\
				-ENOERR;						\
			})
```

### C02 · put_user

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::put_user]
```c
	#define put_user(x, ptr) ({					\
				*(ptr) = (__typeof(*(ptr)))(x);	\
				-ENOERR;						\
			})
```

### C03 · ENOERR

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

### C04 · MYOS_SYSCALL_DEFINE0(rt_sigreturn)

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::MYOS_SYSCALL_DEFINE0(rt_sigreturn)]
```c
MYOS_SYSCALL_DEFINE0(rt_sigreturn)
{
	pt_regs_s *regs = current_pt_regs();
	rt_sigframe_s __user *frame;
	sigset_t set;
```

### C05 · MYOS_SYSCALL_DEFINE0(rt_sigreturn)

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::MYOS_SYSCALL_DEFINE0(rt_sigreturn)]
```c
	ulong uc_flags;

	// while (1);

	frame = (rt_sigframe_s __user *)(regs->sp - sizeof(long));
```

### C06 · MYOS_SYSCALL_DEFINE0(rt_sigreturn)

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::MYOS_SYSCALL_DEFINE0(rt_sigreturn)]
```c
	frame = (rt_sigframe_s __user *)(regs->sp - sizeof(long));
	if (!access_ok(frame, sizeof(*frame)))
		goto badframe;
	if (get_user(*(__u64 *)&set, (__u64 __user *)&frame->uc.uc_sigmask))
		goto badframe;
```

### C07 · MYOS_SYSCALL_DEFINE0(rt_sigreturn)

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::MYOS_SYSCALL_DEFINE0(rt_sigreturn)]
```c
	if (get_user(*(__u64 *)&set, (__u64 __user *)&frame->uc.uc_sigmask))
		goto badframe;
	if (get_user(uc_flags, &frame->uc.uc_flags))
		goto badframe;

```

### C08 · MYOS_SYSCALL_DEFINE0(rt_sigreturn)

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::MYOS_SYSCALL_DEFINE0(rt_sigreturn)]
```c
	set_current_blocked(&set);

	if (!restore_sigcontext(regs, &frame->uc.uc_mcontext, uc_flags))
		goto badframe;

```

### C09 · MYOS_SYSCALL_DEFINE0(rt_sigreturn)

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::MYOS_SYSCALL_DEFINE0(rt_sigreturn)]
```c
	return regs->ax;

badframe:
	// signal_fault(regs, frame, "rt_sigreturn");
	return 0;
```

### C10 · rt_sigframe

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_types_arch.h::rt_sigframe]
```c
	struct rt_sigframe {
		char __user		*pretcode;
		ucontext_s		uc;
		siginfo_t		info;
		/* fp state follows here */
```

### C11 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
static int filldir64(dir_ctxt_s *ctx, const char *name,
		int namelen, loff_t offset, u64 ino, unsigned int d_type)
{
	ulong cplen;
	linux_dirent64_s *dirent, *prev;
```

### C12 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	getdents_cbk64_s *buf =
			container_of(ctx, getdents_cbk64_s, ctx);
	int reclen = ALIGN(offsetof(linux_dirent64_s, d_name) + namelen + 1,
			sizeof(u64));
	int prev_reclen = 0;
```

### C13 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->error = verify_dirent_name(name, namelen);
	if (unlikely(buf->error))
		return buf->error;
	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
```

### C14 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
		return -EINVAL;
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;

```

### C15 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev->d_off = offset;
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
	dirent->d_type = d_type;
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
```

### C16 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	cplen = copy_to_user(dirent->d_name, (char *)name, namelen);
	dirent->d_name[namelen] = '\0';

	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
```

### C17 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	buf->prev_reclen = reclen;
	buf->current_dir = (void *)dirent + reclen;
	buf->count -= reclen;
	return 0;

```

### C18 · filldir64

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
// efault_end:
// 	user_write_access_end();
efault:
	buf->error = -EFAULT;
	return -EFAULT;
```

### C19 · MYOS_SYSCALL_DEFINE3(getdents64)

[VERIFIED mykernel/fs/vfs/readdir.c::MYOS_SYSCALL_DEFINE3(getdents64)]
```c

	// 	lastdirent = (void *) buf.current_dir - buf.prev_reclen;
	// 	if (put_user(d_off, &lastdirent->d_off))
	// 		error = -EFAULT;
	// 	else
```

### C20 · linux_dirent64_s

[VERIFIED mykernel/include/linux/kernel/dirent.h::linux_dirent64_s]
```c
	typedef struct linux_dirent64 {
		u64		d_ino;
		s64		d_off;
		unsigned short	d_reclen;
		unsigned char	d_type;
```

### C21 · linux_dirent64_s

[VERIFIED mykernel/include/linux/kernel/dirent.h::linux_dirent64_s]
```c
		unsigned short	d_reclen;
		unsigned char	d_type;
		char	d_name[];
	} linux_dirent64_s;
```

### C22 · put_user

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::put_user]
```c
				*(ptr) = (__typeof(*(ptr)))(x);	\
				-ENOERR;						\
			})
```

### C23 · get_user

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::get_user]
```c
				(x) = *(__typeof(&(x)))(ptr);	\
				-ENOERR;						\
			})
```

