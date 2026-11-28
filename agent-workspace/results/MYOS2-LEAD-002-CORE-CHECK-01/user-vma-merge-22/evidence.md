---
task_id: CORE-USER-VMA-MERGE-22
track_id: MYOS2-LEAD-002
record_type: frozen_source_anchor_ledger
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected; backend not independently attested)
date: 2026-11-02
read_channel: connector
source_branch: time
source_pin_short12: a039d9803ade
task_contract_short12: cffb519d089b
result_base_short12: 16baace7bd3c
review21: CORE-USER-MMAP-PLACEMENT-21-REVIEW-001
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
self_check:
  quote_segments: 94
  quotes_reconfirmed: 94
  downgraded_to_inferred: 0
---

# VM22 固定源码逐字证据

每条VM22-A###来自固定time源码真实path::symbol的1–5行原始文本。源码引文只能证明局部代码和实际门，注释不等于活动指令，例子和推论仍按语言、对象、输入及调用前提。

### VM22-A001 — 调用方先写前驱再解除旧映射
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_munmap_vma_range]
```c
static inline int
simple_munmap_vma_range(mm_s *mm, ulong start, ulong len, vma_s **pprev) {
	*pprev = find_vma_prev_to_addr(mm, start);
	return simple_do_vma_munmap(mm, start, start + len);
}
```

### VM22-A002 — 调整函数局部变量
[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust]
```c
__simple_vma_adjust(vma_s *vma, ulong start, ulong end)
{
	mm_s *mm = vma->vm_mm;
	// file_s *file = vma->vm_file;
	vma_s *next = NULL;
```

### VM22-A003 — 先赋start再计算pgoff和end
[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust]
```c
	// adjust vma's bounds
	vma->vm_start = start;
	vma->vm_pgoff += (start - vma->vm_pgoff) >> PAGE_SHIFT;
	vma->vm_end = end;
```

### VM22-A004 — 完全覆盖后继摘链
[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust]
```c
	// due with totally overlapping situation
	while ((next = vma_next_vma(vma))!= NULL && end >= next->vm_end) {
		__vma_unlink_from_list(mm, next);
	}
```

### VM22-A005 — 部分覆盖后继的pgoff和start更新
[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust]
```c
	// due with partially overlapping situation
	if (next != NULL && end > next->vm_start) {
		next->vm_pgoff += (end - next->vm_start) >> PAGE_SHIFT;
		next->vm_start = end;
	}
```

### VM22-A006 — 相容函数入口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_is_mergeable_vma]
```c
static inline bool
simple_is_mergeable_vma(vma_s *vma, file_s *file, ulong vm_flags) {
```

### VM22-A007 — VM标志和文件指针对比
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_is_mergeable_vma]
```c
	if ((vma->vm_flags ^ vm_flags) & ~VM_SOFTDIRTY)
		return false;
	if (vma->vm_file != file)
		return false;
```

### VM22-A008 — close操作阻止合并
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_is_mergeable_vma]
```c
	if (vma->vm_ops != NULL && vma->vm_ops->close != NULL)
		return false;
	return true;
```

### VM22-A009 — 后继merge helper入口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_can_vma_merge_before]
```c
static bool
simple_can_vma_merge_before(vma_s *vma, ulong vm_flags,
		file_s *file, pgoff_t vm_pgoff) {
```

### VM22-A010 — 后继仅检查相容及vm_pgoff相等
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_can_vma_merge_before]
```c
	if (simple_is_mergeable_vma(vma, file, vm_flags) &&
			(vma->vm_pgoff == vm_pgoff))
		return true;
	return false;
```

### VM22-A011 — 前驱merge helper入口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_can_vma_merge_after]
```c
static bool
simple_can_vma_merge_after(vma_s *vma, ulong vm_flags,
		file_s *file, pgoff_t vm_pgoff) {
```

### VM22-A012 — 前驱页号连续性
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_can_vma_merge_after]
```c
	if (simple_is_mergeable_vma(vma, file, vm_flags) &&
			(vma->vm_pgoff + vma_pages(vma) == vm_pgoff))
		return true;
	else
		return false;
```

### VM22-A013 — 调用入口和pglen
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
simple_vma_merge(mm_s *mm, vma_s *prev, ulong addr, ulong end,
		ulong vm_flags, file_s *file, pgoff_t pgoff)
{
	pgoff_t pglen = (end - addr) >> PAGE_SHIFT;
	vma_s *curr, *next, *adjust_target;
```

### VM22-A014 — 指针未初始化及合并标志初值
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	vma_s *curr, *next, *adjust_target;
	int err;
	pgoff_t vma_pgoff;
	bool merge_prev = false;
	bool merge_next = false;
```

### VM22-A015 — 边界初值和bool
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	pgoff_t vma_pgoff;
	bool merge_prev = false;
	bool merge_next = false;
	ulong vma_start = addr;
	ulong vma_end = end;
```

### VM22-A016 — VM_SPECIAL提前返回
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	 * We later require that vma->vm_flags == vm_flags,
	 * so this tests vma->vm_flags & VM_SPECIAL, too.
	 */
	if (vm_flags & VM_SPECIAL)
		return NULL;
```

### VM22-A017 — curr来源和后继查询前门
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	/* Does the input range span an existing VMA? (cases 5 - 8) */
	curr = find_vma_intersection(mm, prev ? prev->vm_end : 0, end);
	if (curr == NULL ||					/* cases 1 - 4 */
			end == curr->vm_end)		/* cases 6 - 8, adjacent VMA */
		next = vma_lookup(mm, end);
```

### VM22-A018 — next来源与NULL分支
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	if (curr == NULL ||					/* cases 1 - 4 */
			end == curr->vm_end)		/* cases 6 - 8, adjacent VMA */
		next = vma_lookup(mm, end);
	else
		next = NULL;					/* case 5 */
```

### VM22-A019 — prev相邻与相容门
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	/* Can we merge the predecessor? */
	if (prev != NULL && addr == prev->vm_end
			&& simple_can_vma_merge_after(prev, vm_flags, file, pgoff)) {
		vma_start = prev->vm_start;
		vma_pgoff = prev->vm_pgoff;
```

### VM22-A020 — prev成功赋adjust_target
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
		vma_pgoff = prev->vm_pgoff;
		adjust_target = prev;
		merge_prev = true;
	}
```

### VM22-A021 — next兼容与偏移门
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	/* Can we merge the successor? */
	if (next != NULL && simple_can_vma_merge_before(next,
			vm_flags, file, pgoff+pglen)) {
		vma_end = next->vm_end;
```

### VM22-A022 — next成功仅在!prev写目标
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
		vma_end = next->vm_end;
		if (!prev)
			adjust_target = next;
		merge_next = true;
	}
```

### VM22-A023 — 两侧都未匹配返回NULL
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	if (!merge_prev && !merge_next)
		return NULL;	/* Not mergeable. */
```

### VM22-A024 — 未初始化target的NULL判断
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	if (adjust_target == NULL)
		adjust_target = curr;

	while (prev == NULL && curr == NULL && next == NULL);
	__simple_vma_adjust(adjust_target, vma_start, vma_end);
```

### VM22-A025 — 调整和返回目标
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	while (prev == NULL && curr == NULL && next == NULL);
	__simple_vma_adjust(adjust_target, vma_start, vma_end);
	return adjust_target;
```

### VM22-A026 — 首次merge前的局部变量
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		ulong len, vm_flags_t vm_flags, ulong pgoff)
{
	mm_s *mm = current->mm;
	vma_s *vma, *prev, *merge;
	bool writable_file_mapping = false;
```

### VM22-A027 — 旧区间先行处理
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	/* Clear old maps, set up prev */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;
```

### VM22-A028 — 第一次merge传参和跳过新建
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma = simple_vma_merge(mm, prev, addr,
			addr + len, vm_flags, file, pgoff);
	if (vma != NULL)
		goto out;
```

### VM22-A029 — 创建新VMA及错误出口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma = vm_area_alloc(mm);
	if (vma == NULL) {
		error = -ENOMEM;
		goto unacct_error;
	}
```

### VM22-A030 — 新VMA字段赋值
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_flags = vm_flags;
	vma->vm_page_prot = vm_get_page_prot(vm_flags);
	vma->vm_pgoff = pgoff;
```

### VM22-A031 — 新file引用及回调
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	if (file) {
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
		if (error != -ENOERR)
			goto unmap_and_free_vma;
```

### VM22-A032 — 共享可写映射登记门
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		if (vma_is_shared_maywrite(vma)) {
			error = mapping_map_writable(file->f_mapping);
			if (error)
				goto close_and_free_vma;
```

### VM22-A033 — 回调后地址不变门
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		error = -EINVAL;
		if (WARN_ON((addr != vma->vm_start)))
			goto close_and_free_vma;
```

### VM22-A034 — 二次合并仅在flags变化且prev非空
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		if (unlikely(vm_flags != vma->vm_flags && prev != NULL)) {
			merge = simple_vma_merge(mm, prev, vma->vm_start, vma->vm_end,
					vma->vm_flags, vma->vm_file, vma->vm_pgoff);
```

### VM22-A035 — 二次merge成功分支说明
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
			if (merge) {
				/*
				 * ->mmap() can change vma->vm_file and fput
				 * the original file. So fput the vma->vm_file
				 * here or we would add an extra fput for file
```

### VM22-A036 — fput和vm_area_free临时新VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
				fput(vma->vm_file);
				vm_area_free(vma);
				vma = merge;
				/* Update vm_flags to pick up the change. */
				vm_flags = vma->vm_flags;
```

### VM22-A037 — 目标替换为旧merge后跳过link
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
				vma = merge;
				/* Update vm_flags to pick up the change. */
				vm_flags = vma->vm_flags;
				goto unmap_writable;
			}
```

### VM22-A038 — 二次不成功与shmem分支
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		vm_flags = vma->vm_flags;
	} else if (vm_flags & VM_SHARED) {
		error = shmem_zero_setup(vma);
		if (error)
```

### VM22-A039 — 匿名设置与新VMA入链
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
			goto free_vma;
	} else {
		vma_set_anonymous(vma);
	}

```

### VM22-A040 — 解除写映射与file局部赋值
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
unmap_writable:
	if (file && (vm_flags & VM_SHARED))
		mapping_unmap_writable(file->f_mapping);
	file = vma->vm_file;
out:
```

### VM22-A041 — SOFTDIRTY按宏OR与返回addr
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma->vm_flags |= VM_SOFTDIRTY;
	// vma_set_page_prot(vma);

	return addr;
```

### VM22-A042 — 错误close回调条件
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
close_and_free_vma:
	if (file && vma->vm_ops && vma->vm_ops->close)
		vma->vm_ops->close(vma);

```

### VM22-A043 — 错误fput和清vm_file
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
unmap_and_free_vma:
	fput(vma->vm_file);
	vma->vm_file = NULL;
```

### VM22-A044 — 错误映射范围撤回及writable
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	/* Undo any partial mapping done by a device driver. */
	simple_unmap_region(mm, vma, prev, vma->vm_start, vma->vm_end);
	if (vm_flags & VM_SHARED)
		mapping_unmap_writable(file->f_mapping);
```

### VM22-A045 — free和错误返回
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
free_vma:
	vm_area_free(vma);
unacct_error:
	return error;
```

### VM22-A046 — 头锚转NULL或返回VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::__none_header_listnode_to_vma]
```c
__none_header_listnode_to_vma(List_s *lp, List_hdr_s *lhdr) {
	if (lp == &lhdr->anchor)
		return NULL;
	else
		return LIST_TO_VMA(lp);
```

### VM22-A047 — 前驱助手的NULL检查循环
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_prev_vma]
```c
vma_s *
vma_prev_vma(vma_s *vma)
{
	// BUG_ON(vma == NULL);
	while (vma == NULL);
```

### VM22-A048 — 从list.prev换回VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_prev_vma]
```c
	List_hdr_s *lhdr = &vma->vm_mm->mm_mt;
	List_s *lp = vma->list.prev;
	return __none_header_listnode_to_vma(lp, lhdr);
}
```

### VM22-A049 — 后继助手的NULL检查循环
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_next_vma]
```c
vma_s *
vma_next_vma(vma_s *vma)
{
	// BUG_ON(vma == NULL);
	while (vma == NULL);
```

### VM22-A050 — 从list.next换回VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_next_vma]
```c
	List_hdr_s *lhdr = &vma->vm_mm->mm_mt;
	List_s *lp = vma->list.next;
	return __none_header_listnode_to_vma(lp, lhdr);
}
```

### VM22-A051 — mm正向NULL成员求址前提
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_next_vma]
```c
{
	// BUG_ON((vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list)) ||
	// 		list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
```

### VM22-A052 — 正向从头或当前节点下一项
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_next_vma]
```c
	List_hdr_s *lhdr = &mm->mm_mt;
	List_s *lp = lhdr->anchor.next;
	if (vma != NULL)
		lp = vma->list.next;
	return __none_header_listnode_to_vma(lp, lhdr);
```

### VM22-A053 — mm逆向NULL成员求址前提
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_prev_vma]
```c
{
	// BUG_ON((vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list)) ||
	// 		list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
```

### VM22-A054 — 逆向从尾或当前节点上一项
[VERIFIED mykernel/mm/vm_map/mmap.c::mm_prev_vma]
```c
	List_hdr_s *lhdr = &mm->mm_mt;
	List_s *lp = lhdr->anchor.prev;
	if (vma != NULL)
		lp = vma->list.prev;
	return __none_header_listnode_to_vma(lp, lhdr);
```


### VM22-A055 — 严格前驱以vm_end小于addr
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_prev_to_addr]
```c
			for_each_vma_topdown(mm, vma) {
				if (vma->vm_end < addr)
					return vma;
			}
			return NULL;
```

### VM22-A056 — 交集助手拒start大于end但不拒相等
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
		vma_s
		*find_vma_intersection(mm_s *mm, ulong start, ulong end) {
			// BUG_ON(start> end);
			while (start > end);
```

### VM22-A057 — 交集助手只检查严格前驱的下一项
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
			if (vma != NULL && (vma = vma_next_vma(vma)) != NULL &&
					__vma_intersect_with_range(vma, start, end))
				return vma;
			else
				return NULL;
```

### VM22-A058 — 真正的半开区间交集谓词
[VERIFIED mykernel/mm/vm_map/vm_map.h::__vma_intersect_with_range]
```c
		PREFIX_STATIC_INLINE
		bool
		__vma_intersect_with_range(vma_s *vma, ulong start, ulong end) {
			return !((vma->vm_end <= start) || (vma->vm_start >= end));
		}
```

### VM22-A059 — vma_lookup只采用点包含查询
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_lookup]
```c
			vma_s *vma = simple_find_vma(mm, addr);
			if (vma != NULL && addr < vma->vm_start)
				vma = NULL;
			return vma;
		}
```

### VM22-A060 — simple_find_vma需起点落在VMA内部
[VERIFIED mykernel/mm/vm_map/vm_map.h::simple_find_vma]
```c
			for_each_vma(mm, vma) {
				if (addr >= vma->vm_start && addr < vma->vm_end)
					return vma;
			}
			return NULL;
```

### VM22-A061 — 页数等于字节差右移
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_pages]
```c
		PREFIX_STATIC_INLINE
		ulong
		vma_pages(vma_s *vma) {
			return (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
		}
```

### VM22-A062 — VMA以字节存起止
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::struct vm_area_struct]
```c
			struct {
				/* VMA covers [vm_start; vm_end) addresses within mm */
				ulong	vm_start;
				ulong	vm_end;
			};
```

### VM22-A063 — VMA与mm和pgprot字段
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::struct vm_area_struct]
```c

		mm_s			*vm_mm;		/* The address space we belong to. */
		pgprot_t		vm_page_prot;
```

### VM22-A064 — vm_flags独立可变字段
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::struct vm_area_struct]
```c
		// 	const vm_flags_t		vm_flags;
		// 	vm_flags_t __private	__vm_flags;
		// };
		vm_flags_t		vm_flags;

```

### VM22-A065 — VMA链表节点
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::struct vm_area_struct]
```c
		List_s			list;
```

### VM22-A066 — VMA的pgoff、file与private_data字段
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::struct vm_area_struct]
```c
		/* Information about our backing store: */
		ulong			vm_pgoff;			/* Offset (within vm_file) in PAGE_SIZE units */
		file_s			*vm_file;			/* File we map to (can be NULL). */
		void			*vm_private_data;	/* was vm_pte (shared mem) */
```

### VM22-A067 — 入链时初始化并可加入头部
[VERIFIED mykernel/mm/vm_map/util.c::__vma_link_to_list]
```c
void __vma_link_to_list(mm_s *mm, vma_s *vma, vma_s *prev)
{
	INIT_LIST_S(&vma->list);
	if (prev == NULL) {
		list_header_add_to_head(&mm->mm_mt, &vma->list);
```

### VM22-A068 — prev分支和mm计数
[VERIFIED mykernel/mm/vm_map/util.c::__vma_link_to_list]
```c

		list_add_to_next(&vma->list, &prev->list);
		mm->mm_mt.count++;
	}
	mm->map_count++;
```

### VM22-A069 — 摘链减map_count并调用list_header_delete_node
[VERIFIED mykernel/mm/vm_map/util.c::__vma_unlink_from_list]
```c
{
	// BUG_ON(!list_header_contains(&mm->mm_mt, &vma->list));
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	mm->map_count--;
	list_header_delete_node(&mm->mm_mt, &vma->list);
```

### VM22-A070 — contains遍历不等于内存归还
[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```c
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
			}
			return false;
```

### VM22-A071 — list_header_delete_node的membership门与链表摘除
[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
		*list_header_delete_node(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
```

### VM22-A072 — list_del_init重设链接不释放对象
[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
		void
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
		}
```

### VM22-A073 — 双链直接更新邻居指针
[VERIFIED mykernel/lib/list/double_list.h::__list_del]
```c
		void
		__list_del(List_s * prev, List_s * next) {
			next->prev = prev;
			WRITE_ONCE(prev->next, next);
		}
```

### VM22-A074 — 头插计数递增
[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
		void
		list_header_add_to_head(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
```

### VM22-A075 — 头部身份只是指针相等
[VERIFIED mykernel/lib/list/double_list.h::list_is_head_anchor]
```c
		PREFIX_STATIC_INLINE
		bool
		list_is_head_anchor(const List_s *list, const List_s *head) {
			return list == head;
		}
```

### VM22-A076 — 列表迭代定义
[VERIFIED mykernel/lib/list/double_list_macro.h::list_for_each]
```c
	#define list_for_each(pos, head)			\
				for (pos = (head)->next;		\
					!list_is_head(pos, (head));	\
					pos = pos->next)
```

### VM22-A077 — vma遍历绑定mm_next_vma
[VERIFIED mykernel/mm/vm_map/vm_map_macro.h::for_each_vma]
```c
	#define for_each_vma(mm, __vma)								\
			while (												\
				((__vma) = mm_next_vma((mm), (__vma))) != NULL	\
			)
```

### VM22-A078 — vma逆向遍历绑定mm_prev_vma
[VERIFIED mykernel/mm/vm_map/vm_map_macro.h::for_each_vma_topdown]
```c
	#define for_each_vma_topdown(mm, __vma)						\
			while (												\
				((__vma) = mm_prev_vma((mm), (__vma))) != NULL	\
			)
```

### VM22-A079 — SOFTDIRTY宏有条件可能为0
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_SOFTDIRTY]
```c
	#	ifdef CONFIG_MEM_SOFT_DIRTY
	#		define VM_SOFTDIRTY		0x08000000	/* Not soft dirty clean area */
	#	else
	#		define VM_SOFTDIRTY		0
	#	endif
```

### VM22-A080 — VM_SPECIAL是若干特征位的掩码
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_SPECIAL]
```c
	/*
	 * Special vmas that are non-mergable, non-mlock()able.
	 */
	#define VM_SPECIAL			(VM_IO | VM_DONTEXPAND | VM_PFNMAP | VM_MIXEDMAP)
```

### VM22-A081 — 页位移12与PAGE_SIZE 4096
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```c
	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```

### VM22-A082 — file引用递增并返回相同对象
[VERIFIED mykernel/include/linux/fs/fs.h::get_file]
```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
	}
```

### VM22-A083 — mmap helper的间接FOP回调
[VERIFIED mykernel/include/linux/fs/fs.h::call_mmap]
```c
	static inline int call_mmap(file_s *file, vma_s *vma) {
		return file->f_op->mmap(file, vma);
	}
```

### VM22-A084 — fput递减引用至零时进入释放
[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;
```

### VM22-A085 — fput零计数的实际cache_free
[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// if (llist_add(&file->f_u.fu_llist, &delayed_fput_list))
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
```

### VM22-A086 — VMA分配时调用vma_init
[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_alloc]
```c
{
	vma_s *vma = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);
	if (vma)
		vma_init(vma, mm);
	return vma;
```

### VM22-A087 — VMA对象free包装
[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_free]
```c
vm_area_free(vma_s *vma)
{
	// free_anon_vma_name(vma);
	kmem_cache_free(vm_area_cachep, vma);
}
```

### VM22-A088 — 文件VMA连接额外映射登记
[VERIFIED mykernel/mm/vm_map/mmap.c::vma_link_file]
```c
		addr_spc_s *mapping = file->f_mapping;
		i_mmap_lock_write(mapping);

		if (vma->vm_flags & VM_SHARED)
			mapping_allow_writable(mapping);
```

### VM22-A089 — 新VMA链入与file链接接口
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_link]
```c
	__vma_link_to_list(mm, vma, prev);
	vma_link_file(vma);

	// validate_mm(mm);
	return 0;
```

### VM22-A090 — 合并之前保存严格前驱
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_munmap_vma_range]
```c
static inline int
simple_munmap_vma_range(mm_s *mm, ulong start, ulong len, vma_s **pprev) {
	*pprev = find_vma_prev_to_addr(mm, start);
	return simple_do_vma_munmap(mm, start, start + len);
}
```

### VM22-A091 — 前驱页连续性比较
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_can_vma_merge_after]
```c
	if (simple_is_mergeable_vma(vma, file, vm_flags) &&
			(vma->vm_pgoff + vma_pages(vma) == vm_pgoff))
		return true;
```

### VM22-A092 — 后继页连续性缺少地址邻接要求
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_can_vma_merge_before]
```c
	if (simple_is_mergeable_vma(vma, file, vm_flags) &&
			(vma->vm_pgoff == vm_pgoff))
		return true;
```


### VM22-A093 — vm_ops是独立回调指针字段
[VERIFIED mykernel/mm/vm_map/vm_map_types.h::struct vm_area_struct]
```c
		anon_vma_s		*anon_vma;			/* Serialized by page_table_lock */

		/* Function pointers to deal with this struct. */
		const vm_ops_s	*vm_ops;

```

### VM22-A094 — delete_node只在membership成立时减count
[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
			}
```


## 取证范围和限定

- 已完整读取41号任务和21主线回执R21-S01/S02/S03，定点读取20回执R20-S02/S04、14回执R14-S01/S02及14延期VL14-F01/F02/F03/F05。
- 源码限定time @ a039d9803ade；已读mmap.c合并/调整/前后调用、vm_map.h和vm_map_types.h、vm_map_macro.h、util.c、double_list.h及相关宏、page_const_arch.h、vm_map_const.h、fs.h与file_table.c。
- 迭代从NULL开始时的vma对象成员求址、查找严格前驱、未初始化adjust_target及失效对象的编译/执行后果均不能定量认证。字段修改、链表摘除、引用释放、页表映射是不同事件。
- 没有运行仓库脚本、预处理、编译或执行C/ASM、QEMU、内核、硬件，不做完整内存/文件引用或锁审计；runtime_evidence=NOT_RUN。

