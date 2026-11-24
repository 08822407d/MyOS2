---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-EXEC-ARGUMENT-LIMITS-143"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-22"
conversation_display_name: "MYOS2-A-C10a 代码分析执行"
produced_by: "GPT-6.2（系统模型族；界面/实际后台型号未独立核实）"
model_per_owner: "次一档GPT-6.2 Sol，Extra／高推理强度"
actual_effort: "UNATTESTED"
date: "2026-11-24"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
read_channel: "connector"
status: "AUTHOR_CONTENT_PREPARED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
full_local_P9: "NOT_RUN"
inputs_read: [{"path":"mykernel/scripts/options_flags.cmake","ref":"a039d9803ade","blob":"6019436417ad"},{"path":"mykernel/sched/forkexec/exec.c","ref":"a039d9803ade","blob":"0861ebae6fc2"},{"path":"mykernel/lib/string/strnlen_user.c","ref":"a039d9803ade","blob":"dae5c6c14aa4"},{"path":"mykernel/lib/string/string.c","ref":"a039d9803ade","blob":"cbb2752bc8b2"},{"path":"mykernel/include/uapi/linux/binfmts.h","ref":"a039d9803ade","blob":"a60f4e7c53b0"},{"path":"mykernel/include/uapi/linux/resource.h","ref":"a039d9803ade","blob":"6a683439847c"},{"path":"mykernel/include/uapi/linux/limits.h","ref":"a039d9803ade","blob":"bddfd09e4be2"},{"path":"mykernel/include/linux/fs/binfmts.h","ref":"a039d9803ade","blob":"002c2aec4aa8"},{"path":"mykernel/include/linux/kernel/minmax.h","ref":"a039d9803ade","blob":"d22095ba1acc"},{"path":"mykernel/include/linux/kernel/types.h","ref":"a039d9803ade","blob":"ef531baa8317"},{"path":"mykernel/mm/misc/mm_misc_const.h","ref":"a039d9803ade","blob":"ec983b79166f"},{"path":"mykernel/arch/x86_64/mm/page/page_const_arch.h","ref":"a039d9803ade","blob":"b10e280f26f0"},{"path":"mykernel/include/uapi/asm-generic/posix_types.h","ref":"a039d9803ade","blob":"0246766d9a75"},{"path":"mykernel/include/uapi/linux/const.h","ref":"a039d9803ade","blob":"d8bcaf7e3151"}]
open_questions: ["实际预处理、C对象/指针有效性、GUP返回/页、真实用户访问与运行仍未证"]
record_type: "phase2_source_evidence"
---

# 143新正式源码窄窗

以下仅SOURCE_FACT，单定义、原物理行，定义头、声明、宏、注释与条件门不当作运行。模型的组合依据在正文和facts。generic条件门只表明定义出现的条件，不认证实际预处理。旧141/142证据不重计。

### E143-001 · stk
mykernel/include/uapi/linux/resource.h:65–65；macro_definition；8*1024*1024
[VERIFIED mykernel/include/uapi/linux/resource.h::_STK_LIM]
```c
	#define _STK_LIM			(8*1024*1024)
```

### E143-002 · argmax
mykernel/include/uapi/linux/limits.h:8–8；macro_definition；131072
[VERIFIED mykernel/include/uapi/linux/limits.h::ARG_MAX]
```c
	#define ARG_MAX			131072	/* # bytes of args + environ for exec() */
```

### E143-003 · maxstrlen
mykernel/include/uapi/linux/binfmts.h:15–15；macro_definition；PAGE_SIZE*32
[VERIFIED mykernel/include/uapi/linux/binfmts.h::MAX_ARG_STRLEN]
```c
	#define MAX_ARG_STRLEN		(PAGE_SIZE * 32)
```

### E143-004 · maxstrings
mykernel/include/uapi/linux/binfmts.h:16–16；macro_definition；0x7fffffff
[VERIFIED mykernel/include/uapi/linux/binfmts.h::MAX_ARG_STRINGS]
```c
	#define MAX_ARG_STRINGS		0x7FFFFFFF
```

### E143-005 · pageshift
mykernel/arch/x86_64/mm/page/page_const_arch.h:9–9；macro_definition；12
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	#define PAGE_SHIFT				12
```

### E143-006 · pagesize
mykernel/arch/x86_64/mm/page/page_const_arch.h:10–10；macro_definition；_AC(1,UL) << PAGE_SHIFT
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

### E143-007 · ac
mykernel/include/uapi/linux/const.h:20–20；macro_definition；C branch token paste X##Y
[VERIFIED mykernel/include/uapi/linux/const.h::_AC]
```c
	#	define _AC(X,Y)		(X##Y)
```

### E143-008 · ulong
mykernel/include/linux/kernel/types.h:105–105；type_definition；unsigned long alias
[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```c
		typedef unsigned long		ulong;
```

### E143-009 · size_t
mykernel/include/linux/kernel/types.h:71–71；type_definition；__kernel_size_t alias
[VERIFIED mykernel/include/linux/kernel/types.h::size_t]
```c
			typedef __kernel_size_t		size_t;
```

### E143-010 · kernel_size_t
mykernel/include/uapi/asm-generic/posix_types.h:73–73；type_definition；generic selected alias __kernel_ulong_t
[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_size_t]
```c
			typedef __kernel_ulong_t	__kernel_size_t;
```

### E143-011 · kernel_ulong
mykernel/include/uapi/asm-generic/posix_types.h:17–17；type_definition；unsigned long
[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_ulong_t]
```c
		typedef unsigned long	__kernel_ulong_t;
```

### E143-012 · bprm_p_argmin
mykernel/include/linux/fs/binfmts.h:21–22；struct_member；ulong p and argmin
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		ulong			p; /* current top of mem */
		ulong			argmin; /* rlimit marker for copy_strings() */
```

### E143-013 · bprm_count
mykernel/include/linux/fs/binfmts.h:49–49；struct_member；argc/envc int
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		int				argc, envc;
```

### E143-014 · bprm_exec
mykernel/include/linux/fs/binfmts.h:57–57；struct_member；ulong loader, exec
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		ulong			loader, exec;
```

### E143-015 · cmp
mykernel/include/linux/kernel/minmax.h:28–28；macro_definition；conditional expression with two operand types
[VERIFIED mykernel/include/linux/kernel/minmax.h::__cmp]
```c
	#define __cmp(x, y, op)	((x) op (y) ? (x) : (y))
```

### E143-016 · max
mykernel/include/linux/kernel/minmax.h:59–59；macro_definition；active __cmp not commented careful_cmp
[VERIFIED mykernel/include/linux/kernel/minmax.h::max]
```c
	#define max(x, y)	__cmp(x, y, >)
```

### E143-017 · max_t
mykernel/include/linux/kernel/minmax.h:123–123；macro_definition；active casts before __cmp
[VERIFIED mykernel/include/linux/kernel/minmax.h::max_t]
```c
	#define max_t(type, x, y)	__cmp((type)(x), (type)(y), >)
```

### E143-018 · valid_len
mykernel/sched/forkexec/exec.c:147–147；active_function_fragment；only len <= MAX_ARG_STRLEN
[VERIFIED mykernel/sched/forkexec/exec.c::valid_arg_len]
```c
	return len <= MAX_ARG_STRLEN;
```

### E143-019 · set_limit
mykernel/sched/forkexec/exec.c:246–250；active_function_fragment；strict p<limit reject, else assign argmin and return0
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_set_stack_limit]
```c
	if (bprm->p < limit)
		return -E2BIG;
	bprm->argmin = bprm->p - limit;
	return 0;
}
```

### E143-020 · hit_limit
mykernel/sched/forkexec/exec.c:253–253；active_function_fragment；same expression helper definition, not a call at copy sites
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_hit_stack_limit]
```c
	return bprm->p < bprm->argmin;
```

### E143-021 · limit_decl
mykernel/sched/forkexec/exec.c:258–258；local_declaration；limit and ptr_size ulong
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	ulong limit, ptr_size;
```

### E143-022 · limit0
mykernel/sched/forkexec/exec.c:268–268；active_function_fragment；3/4 _STK_LIM
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	limit = _STK_LIM / 4 * 3;
```

### E143-023 · rlim_comment
mykernel/sched/forkexec/exec.c:269–269；comment_only；rlim_cur min is comment only
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	// limit = min(limit, bprm->rlim_stack.rlim_cur / 4);
```

### E143-024 · argmax_floor
mykernel/sched/forkexec/exec.c:274–274；active_function_fragment；max_t floor
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	limit = max_t(ulong, limit, ARG_MAX);
```

### E143-025 · negative
mykernel/sched/forkexec/exec.c:276–277；active_function_fragment；negative count early E2BIG
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	if (bprm->argc < 0 || bprm->envc < 0)
		return -E2BIG;
```

### E143-026 · ptr_gate
mykernel/sched/forkexec/exec.c:291–294；active_function_fragment；int sum prior to sizeof multiply; <= rejection; decrement only after gate
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	ptr_size = (max(bprm->argc, 1) + bprm->envc) * sizeof(void *);
	if (limit <= ptr_size)
		return -E2BIG;
	limit -= ptr_size;
```

### E143-027 · limit_call
mykernel/sched/forkexec/exec.c:296–296；active_function_fragment；return helper result
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	return bprm_set_stack_limit(bprm, limit);
```

### E143-028 · generic_gate
mykernel/lib/string/string.c:488–488；conditional_definition_gate；generic strnlen condition, not proof of actual preprocessing
[VERIFIED mykernel/lib/string/string.c::__HAVE_ARCH_STRNLEN]
```c
#ifndef __HAVE_ARCH_STRNLEN
```

### E143-029 · generic_signature
mykernel/lib/string/string.c:494–496；definition_head；size_t signature and local sc declaration
[VERIFIED mykernel/lib/string/string.c::strnlen]
```c
	size_t strnlen(const char *s, size_t count)
	{
		const char *sc;
```

### E143-030 · generic_loop
mykernel/lib/string/string.c:497–499；active_function_fragment；count-- short circuit before *sc, return sc-s
[VERIFIED mykernel/lib/string/string.c::strnlen]
```c
		for (sc = s; count-- && *sc != '\0'; ++sc)
			/* nothing */;
		return sc - s;
```

### E143-031 · user_signature
mykernel/lib/string/strnlen_user.c:92–94；definition_head；long signature; initial commented variable
[VERIFIED mykernel/lib/string/strnlen_user.c::strnlen_user]
```c
long strnlen_user(const char __user *str, long count)
{
	// unsigned long max_addr, src_addr;
```

### E143-032 · user_read_comment
mykernel/lib/string/strnlen_user.c:112–116；comment_only；user access and do_strnlen_user are commented
[VERIFIED mykernel/lib/string/strnlen_user.c::strnlen_user]
```c
	// 	if (user_read_access_begin(str, max)) {
	// 		retval = do_strnlen_user(str, count, max);
	// 		user_read_access_end();
	// 		return retval;
	// 	}
```

### E143-033 · user_return_comment
mykernel/lib/string/strnlen_user.c:118–118；comment_only；commented return 0 not active
[VERIFIED mykernel/lib/string/strnlen_user.c::strnlen_user]
```c
	// return 0;
```

### E143-034 · user_len
mykernel/lib/string/strnlen_user.c:120–120；active_function_fragment；only active return strnlen+1
[VERIFIED mykernel/lib/string/strnlen_user.c::strnlen_user]
```c
	return strnlen(str, count) + 1;
```

### E143-035 · user_loop
mykernel/sched/forkexec/exec.c:311–315；active_function_fragment；reverse postdecrement argc and locals
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	while (argc-- > 0) {
		const char __user *str;
		int len;
		ulong pos;

```

### E143-036 · user_ptr
mykernel/sched/forkexec/exec.c:316–319；active_function_fragment；EFAULT default and get_user_arg_ptr error gate
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		ret = -EFAULT;
		str = get_user_arg_ptr(argv, argc);
		if (IS_ERR(str))
			goto out;
```

### E143-037 · user_measure
mykernel/sched/forkexec/exec.c:321–323；active_function_fragment；strnlen_user and !len gate
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		len = strnlen_user(str, MAX_ARG_STRLEN);
		if (!len)
			goto out;
```

### E143-038 · user_valid
mykernel/sched/forkexec/exec.c:325–327；active_function_fragment；E2BIG before valid_len rejection
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		ret = -E2BIG;
		if (!valid_arg_len(bprm, len))
			goto out;
```

### E143-039 · user_predebit
mykernel/sched/forkexec/exec.c:330–334；active_function_fragment；pos old p; str advance; whole-length debit before p<argmin
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		pos = bprm->p;
		str += len;
		bprm->p -= len;
		if (bprm->p < bprm->argmin)
			goto out;
```

### E143-040 · user_chunk
mykernel/sched/forkexec/exec.c:345–349；active_function_fragment；offset and chunk calculation
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
			offset = pos % PAGE_SIZE;
			if (offset == 0)
				offset = PAGE_SIZE;

			bytes_to_copy = offset;
```

### E143-041 · user_chunk2
mykernel/sched/forkexec/exec.c:350–351；active_function_fragment；chunk bounded by len
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
			if (bytes_to_copy > len)
				bytes_to_copy = len;
```

### E143-042 · user_progress
mykernel/sched/forkexec/exec.c:353–356；active_function_fragment；pos/str/len advanced before get page
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
			offset -= bytes_to_copy;
			pos -= bytes_to_copy;
			str -= bytes_to_copy;
			len -= bytes_to_copy;
```

### E143-043 · user_page
mykernel/sched/forkexec/exec.c:361–365；active_function_fragment；get_arg_page and null failure
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				page = get_arg_page(bprm, pos, 1);
				if (!page) {
					ret = -E2BIG;
					goto out;
				}
```

### E143-044 · user_copy
mykernel/sched/forkexec/exec.c:367–368；active_function_fragment；copy to page then put
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				memcpy_to_page(page, offset_in_page(pos), str, bytes_to_copy);
				put_arg_page(page);
```

### E143-045 · user_zero
mykernel/sched/forkexec/exec.c:386–387；active_function_fragment；ret=0 after full loop, out label
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	ret = 0;
out:
```

### E143-046 · user_ret
mykernel/sched/forkexec/exec.c:393–393；active_function_fragment；actual return ret; no local p rollback at out
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	return ret;
```

### E143-047 · kernel_measure
mykernel/sched/forkexec/exec.c:401–402；active_function_fragment；strnlen+1 and old pos
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	int len = strnlen(arg, MAX_ARG_STRLEN) + 1 /* terminating NUL */;
	ulong pos = bprm->p;
```

### E143-048 · kernel_valid
mykernel/sched/forkexec/exec.c:404–407；active_function_fragment；len==0 EFAULT and valid length E2BIG before debit
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	if (len == 0)
		return -EFAULT;
	if (!valid_arg_len(bprm, len))
		return -E2BIG;
```

### E143-049 · kernel_predebit
mykernel/sched/forkexec/exec.c:410–413；active_function_fragment；arg advance and p debit before strict space gate
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	arg += len;
	bprm->p -= len;
	if (bprm->p < bprm->argmin)
		return -E2BIG;
```

### E143-050 · kernel_chunk
mykernel/sched/forkexec/exec.c:415–418；active_function_fragment；chunk computation
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	while (len > 0) {
		uint bytes_to_copy = min_t(uint, len,
				min_not_zero(offset_in_page(pos), PAGE_SIZE));
		page_s *page;
```

### E143-051 · kernel_progress
mykernel/sched/forkexec/exec.c:420–422；active_function_fragment；pos/arg/len advance before page
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		pos -= bytes_to_copy;
		arg -= bytes_to_copy;
		len -= bytes_to_copy;
```

### E143-052 · kernel_page
mykernel/sched/forkexec/exec.c:424–426；active_function_fragment；page null returns E2BIG
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		page = get_arg_page(bprm, pos, 1);
		if (!page)
			return -E2BIG;
```

### E143-053 · kernel_copy
mykernel/sched/forkexec/exec.c:428–429；active_function_fragment；copy and put
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		memcpy_to_page(page, offset_in_page(pos), arg, bytes_to_copy);
		put_arg_page(page);
```

### E143-054 · kernel_ret
mykernel/sched/forkexec/exec.c:432–432；active_function_fragment；return0 after loop
[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	return 0;
```

### E143-055 · kernel_array
mykernel/sched/forkexec/exec.c:440–443；active_function_fragment；reverse loop, negative return propagates
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
	while (argc-- > 0) {
		int ret = copy_string_kernel(argv[argc], bprm);
		if (ret < 0)
			return ret;
```

### E143-056 · kernel_array_ret
mykernel/sched/forkexec/exec.c:448–448；active_function_fragment；return0 after loop including argc=0
[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
	return 0;
```

### E143-057 · get_page_ret_decl
mykernel/sched/forkexec/exec.c:69–72；local_declaration；page declaration, mm, int ret
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	page_s *page;
	vma_s *vma = bprm->vma;
	mm_s *mm = bprm->mm;
	int ret;
```

### E143-058 · get_page_gup
mykernel/sched/forkexec/exec.c:79–82；active_function_fragment；GUP return stored in int, unlock and <=0 check
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	ret = myos_get_user_pages(mm, pos, 1, &page);
	mmap_read_unlock(mm);
	if (ret <= 0)
		return NULL;
```

### E143-059 · get_page_return
mykernel/sched/forkexec/exec.c:87–87；active_function_fragment；return page not proof of page validity
[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	return page;
```

### E143-060 · count_user
mykernel/sched/forkexec/exec.c:203–207；active_function_fragment；non-null outer gate and pointer read
[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
	if (argv != NULL) {
		for (;;) {
			const char __user *p =
					get_user_arg_ptr(argv, i);

```

### E143-061 · count_user_error
mykernel/sched/forkexec/exec.c:208–212；active_function_fragment；NULL terminates, ERR gives EFAULT
[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			if (!p)
				break;

			if (IS_ERR(p))
				return -EFAULT;
```

### E143-062 · count_user_limit
mykernel/sched/forkexec/exec.c:214–216；active_function_fragment；max check and count increment
[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			if (i >= max)
				return -E2BIG;
			++i;
```

### E143-063 · count_user_ret
mykernel/sched/forkexec/exec.c:223–223；active_function_fragment；return i
[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
	return i;
```

### E143-064 · count_kernel_null
mykernel/sched/forkexec/exec.c:230–231；active_function_fragment；null vector returns0
[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	if (!argv)
		return 0;
```

### E143-065 · count_kernel_loop
mykernel/sched/forkexec/exec.c:233–235；active_function_fragment；test slot before max bound
[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	for (i = 0; argv[i]; ++i) {
		if (i >= MAX_ARG_STRINGS)
			return -E2BIG;
```

### E143-066 · count_kernel_ret
mykernel/sched/forkexec/exec.c:240–240；active_function_fragment；return i
[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	return i;
```

### E143-067 · user_alloc
mykernel/sched/forkexec/exec.c:1316–1320；active_function_fragment；alloc error gate; not production proof
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	bprm = alloc_bprm(fd, filename, flags);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}
```

### E143-068 · user_count_warn
mykernel/sched/forkexec/exec.c:1322–1325；active_function_fragment；argv count, zero warning
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = count(argv, MAX_ARG_STRINGS);
	if (retval == 0)
		pr_warn_once("process '%s' launched '%s' with NULL argv:"
				" empty string added\n", current->comm, bprm->filename);
```

### E143-069 · user_argc
mykernel/sched/forkexec/exec.c:1326–1328；active_function_fragment；negative gate precedes argc assignment
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;
```

### E143-070 · user_envc
mykernel/sched/forkexec/exec.c:1330–1333；active_function_fragment；env count and assignment after gate
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```

### E143-071 · user_limits
mykernel/sched/forkexec/exec.c:1335–1337；active_function_fragment；limit call and failure goto
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;
```

### E143-072 · user_filename
mykernel/sched/forkexec/exec.c:1339–1342；active_function_fragment；filename copy normal condition then exec=p
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

### E143-073 · user_envcopy
mykernel/sched/forkexec/exec.c:1344–1346；active_function_fragment；env reverse helper and negative goto
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;
```

### E143-074 · user_argcopy
mykernel/sched/forkexec/exec.c:1348–1350；active_function_fragment；argv helper and failure goto
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

### E143-075 · user_synth
mykernel/sched/forkexec/exec.c:1358–1362；active_function_fragment；zero argc branch, empty literal, negative gate and argc=1
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (bprm->argc == 0) {
		retval = copy_string_kernel("", bprm);
		if (retval < 0)
			goto out_free;
		bprm->argc = 1;
```

### E143-076 · user_exec
mykernel/sched/forkexec/exec.c:1365–1367；active_function_fragment；bprm_execve then cleanup label free_bprm
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = bprm_execve(bprm, fd, filename, flags);
out_free:
	free_bprm(bprm);
```

### E143-077 · user_cleanup
mykernel/sched/forkexec/exec.c:1369–1372；active_function_fragment；putname and return retval conditional cleanup
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
out_ret:
	putname(filename);
	return retval;
}
```

### E143-078 · kernel_alloc
mykernel/sched/forkexec/exec.c:1393–1397；active_function_fragment；alloc gate
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm = alloc_bprm(fd, filename, 0);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}
```

### E143-079 · kernel_zero_reject
mykernel/sched/forkexec/exec.c:1399–1403；active_function_fragment；zero result -> -EINVAL then goto before argc assignment
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(argv);
	if (retval == 0)
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
```

### E143-080 · kernel_argc
mykernel/sched/forkexec/exec.c:1404–1404；active_function_fragment；assignment only if gate passed
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm->argc = retval;
```

### E143-081 · kernel_envc
mykernel/sched/forkexec/exec.c:1406–1409；active_function_fragment；env count and gate assignment
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(envp);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```

### E143-082 · kernel_limits
mykernel/sched/forkexec/exec.c:1411–1413；active_function_fragment；limits call and negative gate
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;
```

### E143-083 · kernel_filename
mykernel/sched/forkexec/exec.c:1415–1418；active_function_fragment；filename copy before exec assignment
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

### E143-084 · kernel_envcopy
mykernel/sched/forkexec/exec.c:1420–1422；active_function_fragment；env kernel array helper and negative gate
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;
```

### E143-085 · kernel_argcopy
mykernel/sched/forkexec/exec.c:1424–1426；active_function_fragment；argv kernel array helper and negative gate
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

### E143-086 · kernel_exec
mykernel/sched/forkexec/exec.c:1428–1430；active_function_fragment；exec call then free_bprm label
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);
```

### E143-087 · kernel_cleanup
mykernel/sched/forkexec/exec.c:1432–1435；active_function_fragment；putname and return retval conditional cleanup
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
out_ret:
	putname(filename);
	return retval;
}
```

### E143-088 · free_bprm_tail
mykernel/sched/forkexec/exec.c:1091–1095；active_function_fragment；kfree bprm; no later field read justified
[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm->fdpath);
	kfree(bprm);
}
```

### E143-089 · build_arch
mykernel/scripts/options_flags.cmake:7–9；build_declaration；default x86_64
[VERIFIED mykernel/scripts/options_flags.cmake::ARCH]
```c
if (NOT DEFINED ${ARCH})
	set(ARCH x86_64)
endif()
```

### E143-090 · build_c
mykernel/scripts/options_flags.cmake:38–42；build_declaration；C flags -m64 etc
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
# kernel common C make flags
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
```

### E143-091 · build_config
mykernel/scripts/options_flags.cmake:58–62；build_declaration；explicit CONFIG_64BIT not all actual preprocessing
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

