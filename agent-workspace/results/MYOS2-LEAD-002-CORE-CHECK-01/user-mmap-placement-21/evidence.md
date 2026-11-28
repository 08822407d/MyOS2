---
task_id: CORE-USER-MMAP-PLACEMENT-21
track_id: MYOS2-LEAD-002
record_type: fixed_time_source_evidence
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected, backend not attestable)
source_branch: time
source_pin_short12: a039d9803ade
task_contract_short12: 84ca207f5cc2
result_previous_head_short12: 0e139c6ca86d
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
self_check:
  verified_claims: 91
  quotes_reconfirmed: 91
  downgraded_to_inferred: 0
---

# AP21 固定源码证据账

每项AP21-A###有固定time正文的path::symbol与1–5行逐字C/宏引文；引用说明仅限被引文本及实际控制流前提。注释不是活动调用、函数名称不是完整契约、C源码中的不返回或指针问题不能直接提升为实机观察。

### AP21-A001 — 拒绝零长度
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (len == 0)
		return -EINVAL;
```

### AP21-A002 — FIXED_NOREPLACE标记及非fixed提示对齐
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (flags & MAP_FIXED_NOREPLACE)
		flags |= MAP_FIXED;

	if (!(flags & MAP_FIXED))
		addr = round_hint_to_min(addr);
```

### AP21-A003 — PAGE_ALIGN与舍入为0的出口
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	len = PAGE_ALIGN(len);
	if (!len)
		return -ENOMEM;
```

### AP21-A004 — pgoff与len页数和的回绕检查
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	/* offset overflow? */
	if ((pgoff + (len >> PAGE_SHIFT)) < pgoff)
		return -EOVERFLOW;
```

### AP21-A005 — map_count限制
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	/* Too many mappings? */
	if (mm->map_count > sysctl_max_map_count)
		return -ENOMEM;
```

### AP21-A006 — 调用实际地址选择器并消费编码错误
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	addr = __get_unmapped_area(file, addr, len, pgoff, flags, vm_flags);
	if (IS_ERR_VALUE(addr))
		return addr;
```

### AP21-A007 — NOREPLACE唯一的选中后交叉查询
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (flags & MAP_FIXED_NOREPLACE)
		if (find_vma_intersection(mm, addr, addr + len))
			return -EEXIST;
```

### AP21-A008 — 向已有映射处理转交候选地址
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	addr = simple_mmap_region(file, addr, len, vm_flags, pgoff);

	return addr;
```

### AP21-A009 — 候选地址进入旧VMA区间处理
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	/* Clear old maps, set up prev */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;

```

### AP21-A010 — 非FIXED提示按页掩码与mmap_min_addr处理
[VERIFIED mykernel/mm/vm_map/mmap.c::round_hint_to_min]
```c
round_hint_to_min(ulong hint) {
	hint &= PAGE_MASK;
	if (((void *)hint != NULL) &&
	    (hint < mmap_min_addr))
		return PAGE_ALIGN(mmap_min_addr);
```

### AP21-A011 — 最小映射地址当前对象初值
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap_min_addr]
```c
static bool ignore_rlimit_data = false;
ulong mmap_min_addr = PAGE_SIZE;

```

### AP21-A012 — 默认候选输入与current mm
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_get_unmapped_area]
```c
		const ulong len, const ulong pgoff, const ulong flags)
{
	vma_s *vma;
	unmapped_vma_info_s info;
	ulong addr = addr0;
```

### AP21-A013 — info字段分层赋值: len/align/limits
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_get_unmapped_area]
```c
	info.length = len;
	info.align_mask = 0,
	info.align_offset = pgoff << PAGE_SHIFT,
	info.low_limit = PAGE_SIZE,
	info.high_limit = mm->mmap_base;
```

### AP21-A014 — 非零提示仅检查起点是否包含于VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_get_unmapped_area]
```c
	if (addr != 0) {
		addr &= PAGE_MASK;
		if (!simple_find_vma(mm, addr))
			return addr;
	}
```

### AP21-A015 — 超出DEFAULT窗口的上限调节
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_get_unmapped_area]
```c
	 * If hint address is above DEFAULT_MAP_WINDOW, look for unmapped area
	 * in the full address space.
	 */
	if (addr > DEFAULT_MAP_WINDOW)
		info.high_limit += TASK_SIZE_MAX - DEFAULT_MAP_WINDOW;
```

### AP21-A016 — 文件对齐控制代码为注释
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_get_unmapped_area]
```c
	// if (filp) {
	// 	info.align_mask = get_align_mask();
	// 	info.align_offset += get_align_bits();
	// }

```

### AP21-A017 — 实际只调用topdown并直接返回
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_get_unmapped_area]
```c
	addr = simple_find_vm_unmapped_area_topdown(mm, &info);
	// if ((addr & ~PAGE_MASK) != 0)
		return addr;
}
```

### AP21-A018 — 选址包装的入参及回调指针
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
__get_unmapped_area(file_s *file, ulong addr, ulong len,
		ulong pgoff, ulong flags, vm_flags_t vm_flags)
{
	ulong (*get_area)(file_s *, ulong, ulong, ulong, ulong);

```

### AP21-A019 — 长度超过TASK_SIZE的前置门
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	if (len > TASK_SIZE)
		return -ENOMEM;
```

### AP21-A020 — fixed请求在后置校验之前返回
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	/* No address checking. See comment at mmap_address_hint_valid() */
	if (flags & MAP_FIXED)
		return addr;
```

### AP21-A021 — mm->回调缺省和文件回调覆盖
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	get_area = current->mm->get_unmapped_area;
	if (file) {
		if (file->f_op->get_unmapped_area)
			get_area = file->f_op->get_unmapped_area;
```

### AP21-A022 — 匿名共享说明与路径
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	} else if (flags & MAP_SHARED) {
		/*
		 * mmap_region() will call shmem_zero_setup() to create a file,
		 * so use shmem's get_unmapped_area in case it can be huge.
		 * do_mmap() will clear pgoff, so match alignment.
```

### AP21-A023 — 匿名共享只将pgoff置0; 替代回调注释
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
		 */
		pgoff = 0;
		// get_area = shmem_get_unmapped_area;
	}
```

### AP21-A024 — 动态回调及错误编码检测
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	addr = get_area(file, addr, len, pgoff, flags);
	if (IS_ERR_VALUE(addr))
		return addr;
```

### AP21-A025 — 回调返回后上界和对齐校验
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	if (addr > TASK_SIZE - len)
		return -ENOMEM;
	if (offset_in_page(addr))
		return -EINVAL;
```

### AP21-A026 — 成功仅返回起始地址
[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	return addr;
}
```

### AP21-A027 — topdown局部gap哨兵及tmp初态
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
simple_find_vm_unmapped_area_topdown(mm_s *mm, unmapped_vma_info_s *info)
{
	ulong gap = ~0, gap_end = ~0;
	ulong tmp_gap, tmp_gap_end;
	vma_s *tmp = NULL, *next;
```

### AP21-A028 — topdown遍历与上边界跳过/后继
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
	for_each_vma_topdown(mm, tmp) {
		if (tmp->vm_end >= info->high_limit)
			continue;
		next = vma_next_vma(tmp);
```

### AP21-A029 — topdown候选低界设置
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
		// get gap start
		if (tmp->vm_end <= info->low_limit)
			tmp_gap = info->low_limit;
		else
			tmp_gap = tmp->vm_end;
```

### AP21-A030 — topdown候选高界设置
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
		if (next != NULL && next->vm_start <= info->high_limit)
			tmp_gap_end = next->vm_start;
		else
			tmp_gap_end = info->high_limit;
```

### AP21-A031 — topdown跨度检验及记录命中
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
		// calculate gap size
		if (tmp_gap_end - tmp_gap >= info->length) {
			gap = tmp_gap;
			gap_end = tmp_gap_end;
			break;
```

### AP21-A032 — topdown跳出条件
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
		}
		if (tmp->vm_start <= info->low_limit)
			break;
	}
```

### AP21-A033 — 遍历结束后对NULL tmp调用vma_next_vma
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
	if (tmp == NULL) {
		next = vma_next_vma(tmp);
		tmp_gap = info->low_limit;
		tmp_gap_end = next->vm_start;

```

### AP21-A034 — topdown候选最后区间判断
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
		if (tmp_gap_end - tmp_gap >= info->length) {
			gap = tmp_gap;
			gap_end = tmp_gap_end;
		}
	}
```

### AP21-A035 — 赋值if(gap=~0)覆盖已选候选
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
	if (gap = ~0) {
		gap = info->high_limit - info->length;
		gap &= PAGE_MASK;
	}
```

### AP21-A036 — topdown最终返回gap
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
	return gap;
```

### AP21-A037 — bottomup局部值和tmp初态
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
{
	ulong gap = 0, gap_end = 0;
	ulong tmp_gap, tmp_gap_end;
	vma_s *tmp = NULL, *prev;

```

### AP21-A038 — bottomup迭代和prev准备
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
	for_each_vma(mm, tmp) {
		if (tmp->vm_start <= info->low_limit)
			continue;
		prev = vma_prev_vma(tmp);

```

### AP21-A039 — bottomup候选低界
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
		// get gap start
		if (prev != NULL && prev->vm_end >= info->low_limit)
			tmp_gap = prev->vm_end;
		else
			tmp_gap = info->low_limit;
```

### AP21-A040 — bottomup候选高界
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
		// get gap end
		if (tmp->vm_start >= info->high_limit)
			tmp_gap_end = info->high_limit;
		else
			tmp_gap_end = tmp->vm_start;
```

### AP21-A041 — bottomup命中并记录
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
		if (tmp_gap_end - tmp_gap >= info->length) {
			gap = tmp_gap;
			gap_end = tmp_gap_end;
			break;
		}
```

### AP21-A042 — bottomup遍历停止门
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
		if (tmp->vm_end >= info->high_limit)
			break;
	}
```

### AP21-A043 — bottomup耗尽后对NULL调用vma_next_vma
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
	if (tmp == NULL) {
		prev = vma_next_vma(tmp);
		tmp_gap = info->low_limit;
		tmp_gap_end = prev->vm_start;

```

### AP21-A044 — bottomup最后区间检验
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
		if (tmp_gap_end - tmp_gap >= info->length) {
			gap = tmp_gap;
			gap_end = tmp_gap_end;
		}
	}
```

### AP21-A045 — 赋值if(gap=0)与返回
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_bottomup]
```c
	if (gap = 0)
		gap = (info->low_limit & PAGE_MASK);

	return gap;
```


### AP21-A046 — anchor节点归NULL或非头返回VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::__none_header_listnode_to_vma]
```c
__none_header_listnode_to_vma(List_s *lp, List_hdr_s *lhdr) {
	if (lp == &lhdr->anchor)
		return NULL;
	else
		return LIST_TO_VMA(lp);
```

### AP21-A047 — vma_prev_vma空参数活动while
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_prev_vma]
```c
vma_s *
vma_prev_vma(vma_s *vma)
{
	// BUG_ON(vma == NULL);
	while (vma == NULL);
```

### AP21-A048 — vma_prev从当前节点prev求对象
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_prev_vma]
```c
	List_hdr_s *lhdr = &vma->vm_mm->mm_mt;
	List_s *lp = vma->list.prev;
	return __none_header_listnode_to_vma(lp, lhdr);
}
```

### AP21-A049 — vma_next_vma空参数活动while
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_next_vma]
```c
vma_s *
vma_next_vma(vma_s *vma)
{
	// BUG_ON(vma == NULL);
	while (vma == NULL);
```

### AP21-A050 — vma_next从当前节点next求对象
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_next_vma]
```c
	List_hdr_s *lhdr = &vma->vm_mm->mm_mt;
	List_s *lp = vma->list.next;
	return __none_header_listnode_to_vma(lp, lhdr);
}
```

### AP21-A051 — mm_next空指针成员表达式与列表门
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_next_vma]
```c
{
	// BUG_ON((vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list)) ||
	// 		list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
```

### AP21-A052 — mm_next从anchor开始并容许vma非空后继
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_next_vma]
```c
	List_hdr_s *lhdr = &mm->mm_mt;
	List_s *lp = lhdr->anchor.next;
	if (vma != NULL)
		lp = vma->list.next;
	return __none_header_listnode_to_vma(lp, lhdr);
```

### AP21-A053 — mm_prev空指针成员表达式与列表门
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_prev_vma]
```c
{
	// BUG_ON((vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list)) ||
	// 		list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
```

### AP21-A054 — mm_prev从anchor.prev或vma.prev返回
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_prev_vma]
```c
	List_hdr_s *lhdr = &mm->mm_mt;
	List_s *lp = lhdr->anchor.prev;
	if (vma != NULL)
		lp = vma->list.prev;
	return __none_header_listnode_to_vma(lp, lhdr);
```

### AP21-A055 — simple_find_vma仅比较addr在VMA内
[VERIFIED mykernel/mm/vm_map/vm_map.h::simple_find_vma]
```c
		*simple_find_vma(mm_s *mm, ulong addr) {
			vma_s *vma = NULL;
			for_each_vma(mm, vma) {
				if (addr >= vma->vm_start && addr < vma->vm_end)
					return vma;
```

### AP21-A056 — simple_find_vma未命中返回NULL
[VERIFIED mykernel/mm/vm_map/vm_map.h::simple_find_vma]
```c
			}
			return NULL;
		}
```

### AP21-A057 — 前驱条件严格vm_end<addr
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_prev_to_addr]
```c
			vma_s *vma = NULL;
			for_each_vma_topdown(mm, vma) {
				if (vma->vm_end < addr)
					return vma;
			}
```

### AP21-A058 — 后继按vm_start>addr
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_next_to_addr]
```c
			vma_s *vma = NULL;
			for_each_vma(mm, vma) {
				if (vma->vm_start > addr)
					return vma;
			}
```

### AP21-A059 — 半开区间交集的真实谓词
[VERIFIED mykernel/mm/vm_map/vm_map.h::__vma_intersect_with_range]
```c
		PREFIX_STATIC_INLINE
		bool
		__vma_intersect_with_range(vma_s *vma, ulong start, ulong end) {
			return !((vma->vm_end <= start) || (vma->vm_start >= end));
		}
```

### AP21-A060 — 交集入口仅拒start>end
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
		vma_s
		*find_vma_intersection(mm_s *mm, ulong start, ulong end) {
			// BUG_ON(start> end);
			while (start > end);

```

### AP21-A061 — 交集助手首先找严格前驱再取下一项
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
			vma_s *vma = find_vma_prev_to_addr(mm, start);

			if (vma != NULL && (vma = vma_next_vma(vma)) != NULL &&
					__vma_intersect_with_range(vma, start, end))
```

### AP21-A062 — 交集NULL出口
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
				return vma;
			else
				return NULL;
		}
```

### AP21-A063 — 正向迭代宏调用mm_next_vma
[VERIFIED mykernel/mm/vm_map/vm_map_macro.h::for_each_vma]
```c
	#define for_each_vma(mm, __vma)								\
			while (												\
				((__vma) = mm_next_vma((mm), (__vma))) != NULL	\
			)
```

### AP21-A064 — 逆向迭代宏调用mm_prev_vma
[VERIFIED mykernel/mm/vm_map/vm_map_macro.h::for_each_vma_topdown]
```c
	#define for_each_vma_topdown(mm, __vma)						\
			while (												\
				((__vma) = mm_prev_vma((mm), (__vma))) != NULL	\
			)
```

### AP21-A065 — list_is_head_anchor仅指针相等
[VERIFIED mykernel/lib/list/double_list.h::list_is_head_anchor]
```c
		PREFIX_STATIC_INLINE
		bool
		list_is_head_anchor(const List_s *list, const List_s *head) {
			return list == head;
		}
```

### AP21-A066 — 包含查询遍历anchor链
[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```c
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
```

### AP21-A067 — info结构flags length low_limit
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::vm_unmapped_area_info]
```c
	struct vm_unmapped_area_info {
	#define VM_UNMAPPED_AREA_TOPDOWN 1
		ulong	flags;
		ulong	length;
		ulong	low_limit;
```

### AP21-A068 — info结构high_limit与align字段
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::vm_unmapped_area_info]
```c
		ulong	high_limit;
		ulong	align_mask;
		ulong	align_offset;
		ulong	start_gap;
	};
```

### AP21-A069 — info.start_gap预留字段
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::vm_unmapped_area_info]
```c
	};

#endif /* _LINUX_VM_MAP_TYPES_H_ */
```

### AP21-A070 — mm列表/回调槽/mmap_base存储
[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_struct]
```c
			List_hdr_s		mm_mt;
			// u64 vmacache_seqnum; /* per-thread vmacache */
			ulong (*get_unmapped_area)(file_s *filp, ulong addr,
					ulong len, ulong pgoff, ulong flags);
			ulong			mmap_base;			/* base of mmap area */
```

### AP21-A071 — mm.task_size为独立字段
[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_struct]
```c
			// ulong mmap_compat_legacy_base;
	// #endif
			ulong			task_size;			/* size of task vm space */
			ulong			highest_vm_end;		/* highest vma end address */
			pgd_t			*pgd;
```

### AP21-A072 — arch赋mm选址回调与mmap_base
[VERIFIED mykernel/arch/x86_64/mm/vm_map/mmap_arch.c::arch_pick_mmap_layout]
```c
void
arch_pick_mmap_layout(mm_s * mm, rlimit_s *rlim_stack)
{
	mm->get_unmapped_area = simple_get_unmapped_area;
	mm->mmap_base = TASK_UNMAPPED_BASE;
```

### AP21-A073 — 替代回调绑定为注释
[VERIFIED mykernel/arch/x86_64/mm/vm_map/mmap_arch.c::arch_pick_mmap_layout]
```c
	// if (mmap_is_legacy())
	// 	mm->get_unmapped_area = arch_get_unmapped_area;
	// else
	// 	mm->get_unmapped_area = arch_get_unmapped_area_topdown;
```

### AP21-A074 — setup_new_exec活动调用arch_pick_mmap_layout
[VERIFIED mykernel/sched/forkexec/exec.c::setup_new_exec]
```c
{
	/* Setup things that can depend upon the personality */
	task_s *me = current;

	arch_pick_mmap_layout(me->mm, &bprm->rlim_stack);
```

### AP21-A075 — 设置mm.task_size在布局设置之后
[VERIFIED mykernel/sched/forkexec/exec.c::setup_new_exec]
```c
	 * depend on TIF_32BIT which is only updated in flush_thread() on
	 * some architectures like powerpc
	 */
	me->mm->task_size = TASK_SIZE;
	// up_write(&me->signal->exec_update_lock);
```

### AP21-A076 — x86_64固定页位移12与4096
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT/SIZE/MASK]
```c
	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```

### AP21-A077 — 用户上界与DEFAULT_MAP_WINDOW表达式
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::TASK_SIZE_MAX]
```c
	#define __VIRTUAL_MASK_SHIFT	47
	#define task_size_max()			((_AC(1, UL) << __VIRTUAL_MASK_SHIFT) - PAGE_SIZE)

	#define TASK_SIZE_MAX			task_size_max()
	#define DEFAULT_MAP_WINDOW		((1UL << 47) - PAGE_SIZE)
```

### AP21-A078 — TASK_SIZE与TASK_SIZE_MAX同源定义
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::TASK_SIZE]
```c

	// #define TASK_SIZE_LOW			(test_thread_flag(TIF_ADDR32) ? IA32_PAGE_OFFSET : DEFAULT_MAP_WINDOW)
	#define TASK_SIZE_LOW			DEFAULT_MAP_WINDOW
	// #define TASK_SIZE				(test_thread_flag(TIF_ADDR32) ? IA32_PAGE_OFFSET : TASK_SIZE_MAX)
	#define TASK_SIZE				TASK_SIZE_MAX
```

### AP21-A079 — mmap_base为TASK_SIZE_LOW/3取页对齐
[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::TASK_UNMAPPED_BASE]
```c
	 * This decides where the kernel will search for a free chunk of vm
	 * space during mmap's.
	 */
	#define __TASK_UNMAPPED_BASE(task_size)	(PAGE_ALIGN(task_size / 3))
	#define TASK_UNMAPPED_BASE				__TASK_UNMAPPED_BASE(TASK_SIZE_LOW)
```


### AP21-A080 — PAGE_ALIGN委托ALIGN而非只掩码
[VERIFIED mykernel/mm/page_alloc/buddy_const.h::PAGE_ALIGN]
```c
	/* to align the pointer to the (next) page boundary */
	#define PAGE_ALIGN_DOWN(addr)	ALIGN_DOWN(addr, PAGE_SIZE)
	#define PAGE_ALIGN(addr)		ALIGN(addr, PAGE_SIZE)
```

### AP21-A081 — ALIGN宏与power-of-two前提
[VERIFIED mykernel/include/linux/kernel/align.h::ALIGN]
```c
	/* @a is a power of 2 value */
	#define ALIGN(x, a)				__ALIGN_KERNEL((x), (a))
	#define ALIGN_DOWN(x, a)		__ALIGN_KERNEL((x) - ((a) - 1), (a))
	#define __ALIGN_MASK(x, mask)	__ALIGN_KERNEL_MASK((x), (mask))
```

### AP21-A082 — file_ops中回调确有五参数槽
[VERIFIED mykernel/include/linux/fs/fs.h::file_ops_s.get_unmapped_area]
```c
	// 	int			(*lock) (file_s *, int, file_s_lock *);
	// 	ssize_t		(*sendpage) (file_s *, page_s *, int, size_t, loff_t *, int);
		ulong		(*get_unmapped_area)(file_s *, unsigned long, unsigned long, unsigned long, unsigned long);
	// 	int			(*check_flags)(int);
	// 	int			(*flock) (file_s *, int, file_s_lock *);
```

### AP21-A083 — offset_in_page以PAGE_MASK检查对齐
[VERIFIED mykernel/mm/highmem/highmem_macro.h::offset_in_page]
```c
	#define offset_in_page(p)	((unsigned long)(p) & ~PAGE_MASK)

	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)
```

### AP21-A084 — 错误指针/地址按高端编码区间分类
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#ifndef __ASSEMBLY__
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

### AP21-A085 — 已有映射清理的选定下游接口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap]
```c
int
simple_do_vma_munmap(mm_s *mm, ulong start, ulong end)
{
	vma_s *vma, *prev, *last;
	int error = -ENOERR;
```

### AP21-A086 — 解除映射也消费同一交集查询
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap]
```c
	/* Find the first overlapping VMA where start < vma->vm_end */
	vma = find_vma_intersection(mm, start, end);
	if (vma == NULL)
		goto end;
```

### AP21-A087 — 新建VMA前可能合并旧VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma = simple_vma_merge(mm, prev, addr,
			addr + len, vm_flags, file, pgoff);
	if (vma != NULL)
		goto out;
```

### AP21-A088 — 另有栈向下保护间距计算接口
[VERIFIED mykernel/mm/vm_map/vm_map.h::vm_start_gap]
```c
		vm_start_gap(vma_s *vma) {
			ulong vm_start = vma->vm_start;
			if (vma->vm_flags & VM_GROWSDOWN) {
				vm_start -= stack_guard_gap;
				if (vm_start > vma->vm_start)
```

### AP21-A089 — vm_start_gap绕回保护分支
[VERIFIED mykernel/mm/vm_map/vm_map.h::vm_start_gap]
```c
					vm_start = 0;
			}
			return vm_start;
```

### AP21-A090 — 另有栈向上保护间距计算接口
[VERIFIED mykernel/mm/vm_map/vm_map.h::vm_end_gap]
```c
		vm_end_gap(vma_s *vma) {
			ulong vm_end = vma->vm_end;
			if (vma->vm_flags & VM_GROWSUP) {
				vm_end += stack_guard_gap;
				if (vm_end < vma->vm_end)
```

### AP21-A091 — vm_end_gap越界回退分支
[VERIFIED mykernel/mm/vm_map/vm_map.h::vm_end_gap]
```c
				if (vm_end < vma->vm_end)
					vm_end = -PAGE_SIZE;
			}
			return vm_end;
```




### AP21-A092 — simple_munmap_vma_range实际设置前驱并调用do_vma_munmap
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_munmap_vma_range]
```c
static inline int
simple_munmap_vma_range(mm_s *mm, ulong start, ulong len, vma_s **pprev) {
	*pprev = find_vma_prev_to_addr(mm, start);
	return simple_do_vma_munmap(mm, start, start + len);
```

## 证据边界及已读前序

- 已完整读取40号固定任务与20主线回执R20-S01/S02/S03/S04；14回执关于解除旧映射和非事务回滚、12关于simple_find_vma的包含谓词均作为限定消费，不重做19/20。
- 主要源码来自time固定对象mmap.c、vm_map.h、vm_map_macro.h、vm_map_types.h、mm_misc_types.h、arch/x86_64/mm/vm_map/mmap_arch.c、sched/forkexec/exec.c、list helpers与x86-64 PAGE/TASK常量。
- 选择的执行链为do_mmap→__get_unmapped_area→mm/file get_area callback；arch_pick_mmap_layout在setup_new_exec中有直接调用。bottomup仅是读到的实现，尚无本批实际执行者绑定。
- 数字算例基于64位ulong、PAGE_SHIFT=12、TASK_SIZE==TASK_SIZE_MAX==DEFAULT_MAP_WINDOW、TASK_UNMAPPED_BASE=PAGE_ALIGN(TASK_SIZE_LOW/3)，不是真实内核运行。
- mm_next_vma/mm_prev_vma在vma NULL时仍构造&vma->list并传给list_is_head_anchor；C定义良好前提未闭合。迭代耗尽后直接调用vma_next_vma(NULL)存在活动while。没有实机执行或确定CPU停机判断。
- 部分源码包含赋值表达式if(gap = ~0)、if(gap = 0)，不能按比较运算解释。没有硬件/VM实验、仓库脚本运行、预处理、编译、C/ASM执行、QEMU或完整VM管理修复。

