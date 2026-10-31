---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VMA-LIFETIME-14
record_type: bounded_vma_lifetime_readable_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
results_base: "2d67bea2fe59"
task_contract: "32-user-vma-lifetime-contract.md @ b8ae7762a96e"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions:
  - "VMA iterator startup and boundary checks have language/semantic preconditions; all normal paths are conditional"
  - "No binary/ISA, TLB synchronization, page freeing or concurrent process execution observed"
---

# L00–L03：VMA 区间成员、资源引用与页表不是同一个生命周期

## 0. 核心结论

固定 `time @ a039d9803ade` 的 `munmap` **确有活动系统调用定义与编号11的表项**，并不是空壳；但它到达的`simple_do_vma_munmap`主体只负责查找、必要时分割、最后调用`__vma_unlink_from_list`。摘链时减少`mm.map_count`，不调用`vm_area_free/fput(vm_file)/vm_ops.close`，更未在这条munmap链调用清PTE/TLB及归还page的函数。真实页表是否仍有效、物理页/文件引用是否泄漏只能按相关前提限定，不能从返回0直接证明硬件解除或资源回收。

此外，`find_vma_intersection`依赖严格的前驱`vm_end < start`，会遗漏**没有严格前驱的首VMA、某些首部或相邻边界情况**；迭代器从NULL初始化时存在C成员取址前提。因此下文“正常删除”“分割成功”的说明必须以有效对象、可执行的列表迭代、正确选择交叠VMA为先决条件，**不构成实际运行保证**。

## 1. L01：真实入口与新映射的引用初态

`__NR_munmap=11`在`unistd_64.h`，`syscalls_64.h`登记`sys_munmap`；`mmap.c::MYOS_SYSCALL_DEFINE2(munmap)`活动地`return __vm_munmap(addr,len)`。API另以`#define vm_munmap __vm_munmap`提供内部别名，不能当作第二个入口。此次不重做08的完整汇编分派或用户态调用实验。

**[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::syscall_numbers · VL14-A001，15–17行]**

```c

	#define __NR_mprotect		10
	#define __NR_munmap			11
```

**[VERIFIED mykernel/arch/x86_64/include/asm/syscalls_64.h::syscall_table · VL14-A002，13–15行]**

```c
__SYSCALL(__NR_mprotect, sys_mprotect)
__SYSCALL(__NR_munmap, sys_munmap)
__SYSCALL(__NR_brk, sys_brk)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::sys_munmap · VL14-A003，1344–1346行]**

```c
	// addr = untagged_addr(addr);
	return __vm_munmap(addr, len);
}
```

**[VERIFIED mykernel/mm/vm_map/vm_map_api.h::vm_munmap alias · VL14-A107，49–51行]**

```c
	// extern int vm_munmap(ulong, size_t);
	extern int __vm_munmap(ulong start, size_t len);
	#define vm_munmap __vm_munmap
```


`sys_mmap`则先核文件offset页对齐，非匿名映射有条件`fget(fd)`，再经`vm_mmap_pgoff→do_mmap→simple_mmap_region`，入口的临时file在out_fput中归还。结构图保留了`do_mmap`中间层，未画虚构直连。新VMA经`vm_area_alloc→vma_init`建立，字段为`vm_start=addr,vm_end=addr+len,vm_pgoff=pgoff`；文件映射`vm_file=get_file(file)`才产生其独立引用增量，`call_mmap`通过file.f_op.mmap动态分派，然后`simple_vma_link→__vma_link_to_list`正式入链。file引用、PTE/page引用、VMA对象内存各不相同。

**[VERIFIED mykernel/mm/vm_map/mmap.c::sys_mmap · VL14-A004，1316–1318行]**

```c
{
	if (off & ~PAGE_MASK)
		return -EINVAL;
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::sys_mmap · VL14-A005，1326–1328行]**

```c
		// audit_mmap_fd(fd, flags);
		file = fget(fd);
		if (!file)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::sys_mmap · VL14-A006，1336–1338行]**

```c
out_fput:
	if (file != NULL)
		fput(file);
```

**[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap_pgoff · VL14-A088，95–97行]**

```c
		return -EINTR;
	ret = do_mmap(file, addr, len, prot, flag, 0, pgoff);
	mmap_write_unlock(mm);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap · VL14-A105，642–644行]**

```c

	addr = simple_mmap_region(file, addr, len, vm_flags, pgoff);

```

**[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_alloc · VL14-A039，32–34行]**

```c
{
	vma_s *vma = kmem_cache_alloc(vm_area_cachep, GFP_KERNEL);
	if (vma)
```

**[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_init · VL14-A069，96–98行]**

```c
			static const vm_ops_s dummy_vm_ops = {};
			memset(vma, 0, sizeof(*vma));
			INIT_LIST_S(&vma->list);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A044，691–693行]**

```c
	if (file) {
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A045，692–694行]**

```c
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
		if (error != -ENOERR)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A046，745–747行]**

```c

	simple_vma_link(mm, vma, prev);
	/* Once vma denies write, undo our temporary denial count */
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_link · VL14-A106，179–181行]**

```c

	__vma_link_to_list(mm, vma, prev);
	vma_link_file(vma);
```


`simple_mmap_region`在新VMA分配前先尝试用`simple_munmap_vma_range`解除旧重叠区间，再尝试merge；若后续分配/回调失败，创建失败分支可调用`vm_ops.close/fput/vm_area_free`，合并临时VMA时也可能fput/free。**这些是mmap自己的失败/合并处理，不是munmap摘链后默认执行的清理。** 覆盖旧区间的摘链发生在新映射创建之前，失败并没有源码级事务回滚。

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A086，662–664行]**

```c
	/* Clear old maps, set up prev */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A047，668–670行]**

```c
	 */
	vma = simple_vma_merge(mm, prev, addr,
			addr + len, vm_flags, file, pgoff);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A048，727–729行]**

```c
				 */
				fput(vma->vm_file);
				vm_area_free(vma);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A049，778–780行]**

```c
close_and_free_vma:
	if (file && vma->vm_ops && vma->vm_ops->close)
		vma->vm_ops->close(vma);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A050，782–784行]**

```c
unmap_and_free_vma:
	fput(vma->vm_file);
	vma->vm_file = NULL;
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A052，790–792行]**

```c
free_vma:
	vm_area_free(vma);
unacct_error:
```


## 2. L02：区间选择、分割/摘链与失败副作用

### 2.1 首VMA及交集边界不能按标准Linux推断

`find_vma_prev_to_addr`按top-down扫描寻找`vm_end < addr`，不是`<=`；`find_vma_intersection`只在该前驱非NULL时检查后继，且区间谓词是`!((vma_end<=start)||(vma_start>=end))`。因此起点落在整个列表第一个VMA内、起点正好位于某前驱的vm_end且缺更早严格前驱，均可在源文条件上得到NULL而不处理真实交叠段。多个区间跨越时，成功搜索之后还要求列表实际可迭代。不能将一次返回0解释成目标页表一定被解除。

**[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_prev_to_addr · VL14-A061，302–304行]**

```c
			for_each_vma_topdown(mm, vma) {
				if (vma->vm_end < addr)
					return vma;
```

**[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection · VL14-A063，339–341行]**

```c
			// BUG_ON(start> end);
			while (start > end);

```

**[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection · VL14-A064，341–343行]**

```c

			vma_s *vma = find_vma_prev_to_addr(mm, start);

```

**[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection · VL14-A065，343–345行]**

```c

			if (vma != NULL && (vma = vma_next_vma(vma)) != NULL &&
					__vma_intersect_with_range(vma, start, end))
```

**[VERIFIED mykernel/mm/vm_map/vm_map.h::__vma_intersect_with_range · VL14-A067，324–326行]**

```c
		__vma_intersect_with_range(vma_s *vma, ulong start, ulong end) {
			return !((vma->vm_end <= start) || (vma->vm_start >= end));
		}
```


另`for_each_vma_topdown`用初始NULL作为传给`mm_prev_vma`的实参。实际`mm_prev_vma/mm_next_vma`有一个检查NULL的while之后，又把`&vma->list`传给`list_is_head_anchor`，没有先保护NULL；`vma_prev_vma/vma_next_vma`本身对NULL实参还有活动的`while(vma==NULL)`。**这是静态C对象有效性前提，而不是已观察到的崩溃或确定运行错误码。** 下表的正常路径建立在满足有效列表/迭代器语义的条件推演上。

**[VERIFIED mykernel/mm/vm_map/vm_map_macro.h::for_each_vma_topdown · VL14-A072，34–36行]**

```c
			while (												\
				((__vma) = mm_prev_vma((mm), (__vma))) != NULL	\
			)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::mm_prev_vma · VL14-A079，122–124行]**

```c
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));

```

**[VERIFIED mykernel/mm/vm_map/mmap.c::mm_next_vma · VL14-A078，107–109行]**

```c
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));

```

**[VERIFIED mykernel/mm/vm_map/mmap.c::vma_prev_vma · VL14-A073，73–75行]**

```c
	// BUG_ON(vma == NULL);
	while (vma == NULL);
	
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::vma_next_vma · VL14-A075，84–86行]**

```c
	// BUG_ON(vma == NULL);
	while (vma == NULL);

```


### 2.2 两次分割真实改写了什么

`simple_do_vma_munmap(mm,start,end)`首先`find_vma_intersection`；若无交叠直接以初始`error=-ENOERR`返回。若命中且`start>vma->vm_start`，先调用`__split_vma(...,start,1)`，取`prev=vma_prev_vma(vma)`；随后`last=simple_find_vma(mm,end)`，仅在非NULL且`end>last->vm_start`时进行第二次`__split_vma(...,end,1)`。最后循环取`vma_next_vma(prev)`并删除`tmp->vm_end<=end`的节点。**end恰等于下一段vm_start时不会分割那个下一段**，但能否按预期选中目标段仍由前面的交集/迭代器条件决定。

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A009，1130–1132行]**

```c
	/* Find the first overlapping VMA where start < vma->vm_end */
	vma = find_vma_intersection(mm, start, end);
	if (vma == NULL)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A011，1139–1141行]**

```c
	if (start > vma->vm_start) {
		error = __split_vma(mm, vma, start, 1);
		if (error != ENOERR)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A013，1143–1145行]**

```c
	}
	prev = vma_prev_vma(vma);

```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A014，1146–1148行]**

```c
	/* Does it split the last one? */
	last = simple_find_vma(mm, end);
	if (last != NULL && end > last->vm_start) {
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A015，1148–1150行]**

```c
	if (last != NULL && end > last->vm_start) {
		error = __split_vma(mm, last, end, 1);
		if (error != ENOERR)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A017，1155–1157行]**

```c
	vma_s *tmp;
	while ((tmp = vma_next_vma(prev)) != NULL && tmp->vm_end <= end) {
		BUG_ON(tmp == last);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A018，1157–1159行]**

```c
		BUG_ON(tmp == last);
		__vma_unlink_from_list(mm, tmp);
	}
```


`__split_vma`使用`vm_area_creat_dup`进行浅拷贝`*new=*orig`，重新初始化new的list；若new.vm_file非NULL，额外调用`get_file`，并有条件调用`new.vm_ops->open(new)`。在`new_below`情况下，新副本变成原段的下半部，原VMA起点改为split地址；另一分支则保留原下半段并设置新上半段的pgoff。可见**拷贝字段不等于所有页表、页或引用被自动复制**，真实file引用增加须看`get_file`调用。

**[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_creat_dup · VL14-A021，45–47行]**

```c
		// *new = data_race(*orig);
		*new = *orig;
		// INIT_LIST_HEAD(&new->anon_vma_chain);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::__split_vma · VL14-A024，1093–1095行]**

```c
__split_vma(mm_s *mm, vma_s *vma, ulong addr, int new_below) {
	vma_s *new = vm_area_creat_dup(vma);
	if (!new)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::__split_vma · VL14-A026，1098–1100行]**

```c
	if (new->vm_file)
		get_file(new->vm_file);

```

**[VERIFIED mykernel/mm/vm_map/mmap.c::__split_vma · VL14-A027，1101–1103行]**

```c
	if (new->vm_ops && new->vm_ops->open)
		new->vm_ops->open(new);

```

**[VERIFIED mykernel/mm/vm_map/mmap.c::__split_vma · VL14-A028，1104–1106行]**

```c
	if (new_below != 0) {
		new->vm_end = addr;
		__simple_vma_adjust(vma, addr, vma->vm_end);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::__split_vma · VL14-A031，1109–1111行]**

```c
		new->vm_start = addr;
		new->vm_pgoff += ((addr - vma->vm_start) >> PAGE_SHIFT);
		__simple_vma_adjust(vma, vma->vm_start, addr);
```


`__simple_vma_adjust`计算的文件页偏移还存在独立单位前提：

**[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust · VL14-A035，205–207行]**

```c
	// adjust vma's bounds
	vma->vm_start = start;
	vma->vm_pgoff += (start - vma->vm_pgoff) >> PAGE_SHIFT;
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust · VL14-A036，206–208行]**

```c
	vma->vm_start = start;
	vma->vm_pgoff += (start - vma->vm_pgoff) >> PAGE_SHIFT;
	vma->vm_end = end;
```


`start`是字节虚拟地址，而`vm_pgoff`在`vm_map_types.h`按页为单位；表达式`(start-vma->vm_pgoff)>>PAGE_SHIFT`并不是通常的`(new_start-old_start)>>PAGE_SHIFT`，即使vm_start未发生改变也可能改写pgoff。不能据原函数名宣称分割前后文件页偏移正确。该helper在扩张区间时还可能直接unlink被完全覆盖的后继VMA，源码此处未见fput/free/close。

**[VERIFIED mykernel/mm/vm_map/vm_map_types.h::vm_area_struct · VL14-A091，66–68行]**

```c
		ulong			vm_pgoff;			/* Offset (within vm_file) in PAGE_SIZE units */
		file_s			*vm_file;			/* File we map to (can be NULL). */
		void			*vm_private_data;	/* was vm_pte (shared mem) */
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust · VL14-A037，211–213行]**

```c
	while ((next = vma_next_vma(vma))!= NULL && end >= next->vm_end) {
		__vma_unlink_from_list(mm, next);
	}
```


### 2.3 失败的偏序

如果第一次`vm_area_creat_dup`就失败，`__split_vma`在本次get_file/open/链修改前返回`-ENOMEM`；但第二次分割分配失败时，**第一次分割可能已经增file引用、调用vm_ops.open、改变vm_start/vm_end/vm_pgoff并向列表加入VMA**。调用者`end_split_failed`只跳到统一返回，不撤销第一次副作用。执行者不能把“报错”解释成“与调用前状态完全一样”。

**[VERIFIED mykernel/mm/vm_map/mmap.c::__split_vma · VL14-A025，1095–1097行]**

```c
	if (!new)
		return -ENOMEM;

```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A012，1141–1143行]**

```c
		if (error != ENOERR)
			goto start_split_failed;
	}
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A016，1150–1152行]**

```c
		if (error != ENOERR)
			goto end_split_failed;
	}
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap · VL14-A020，1164–1166行]**

```c
start_split_failed:
	// validate_mm(mm);
	goto end;
```


### 2.4 代表区间行为矩阵（详见facts的VL14-M01～M23）

| 输入/状态 | 在交集与指针前提成立时的源码变化 | 未证事项 |
|---|---|---|
| 无交叠 | error仍0并返回 | 不意味着物理映射已不存在 |
| 首个VMA、无严格前驱 | 交集助手可能返回NULL，跳过操作 | 不能保证首VMA可正常munmap |
| 前驱end恰等于start | 严格小于判据可能选错后继或不命中 | 不保证相邻边界正确 |
| 整段删除 | 符合循环条件的VMA被unlink、map_count减1 | 不等于VMA对象已free |
| 删前缀/后缀 | 有条件split成上下段，再摘相应部分 | 原/新段vm_pgoff计算受上述表达式限制 |
| 中间挖洞 | start与end处两次split、unlink中段 | 只在两次分配及列表操作均完成时成立 |
| 跨多个VMA | 循环unlink满足结束边界的节点 | 未核每个VMA的fput/close/page release |
| end等于另一VMA起点 | 第二次split严格`end>last.start`，不拆右侧VMA | 前述查找成功仍需单独证明 |
| 零长度/未对齐 | `__vm_munmap`未做本地零长度/对齐校验 | 不签发无操作或固定EINVAL |
| start+len溢出 | 无本地上界检查，若start>end可碰到交集函数忙等 | 具体C/机器行为不推断 |
| 首次split失败 | 返回-ENOMEM，此次get_file/list尚未发生 | 上游先前副作用另算 |
| 第二次split失败 | 第一段可能已改变并增ref | 无回滚/事务保证 |

`__vm_munmap`只是将`start,start+len`传入，不对齐、不做0长度及溢出门；而`sys_brk`缩小时**先设置mm->brk=brk**，再调用同一个`simple_do_vma_munmap`，失败路径没有恢复这次brk修改。`simple_mmap_region`和`do_brk_flags`使用`simple_munmap_vma_range`连接同一解除逻辑；不得把它们与用户munmap的入参条件合并。

**[VERIFIED mykernel/mm/vm_map/mmap.c::__vm_munmap · VL14-A007，1177–1179行]**

```c

	ret = simple_do_vma_munmap(mm, start, start + len);
	// if (ret || !unlock)
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_munmap_vma_range · VL14-A053，147–149行]**

```c
simple_munmap_vma_range(mm_s *mm, ulong start, ulong len, vma_s **pprev) {
	*pprev = find_vma_prev_to_addr(mm, start);
	return simple_do_vma_munmap(mm, start, start + len);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_munmap_vma_range · VL14-A054，148–150行]**

```c
	*pprev = find_vma_prev_to_addr(mm, start);
	return simple_do_vma_munmap(mm, start, start + len);
}
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags · VL14-A058，1211–1213行]**

```c
	/* Clear old maps, set up prev, and uf */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;
```

**[VERIFIED mykernel/arch/x86_64/myos/syscall.c::sys_brk · VL14-A057，82–84行]**

```c
		mm->brk = brk;
		if (simple_do_vma_munmap(mm, newbrk, oldbrk))
			goto out;
```


## 3. L03：VMA、文件、页表与退出清理必须分开

| 对象或结果层 | 实际可定位行为 | 不允许的等价推断 |
|---|---|---|
| mm.mm_mt和map_count | `__vma_unlink_from_list`调整list与map_count，实际底层`list_header_delete_node`按节点移除 | 从链表消失≠vm_area_free |
| VMA结构体 | `vm_area_free`确实用kmem_cache_free释放VMA，然而在mmap失败/合并等其它路径；munmap的unlink循环没有调用 | 删除节点≠VMA内存释放 |
| vm_file引用 | 创建文件VMA、split新片段可能get_file；mmap失败清理可能fput | munmap unlink并不在所读路径fput |
| vm_ops.open/close | split有条件open；mmap失败清理有条件close | 不能将回调存在当成unlink自动close |
| PTE、页表、TLB | `simple_unmap_region`有函数定义，但内部unmap_vmas/free_pgtables/tlb_finish_mmu均注释 | 内部unmap接口存在≠munmap执行或硬件撤销 |
| 页缓存和私有COW页 | R13-S03单独记录fault处理的源缓存页`put_page`及私有folio错误释放 | 它们不是本批unlink对缓存槽的清除 |
| mm对象/退出 | `mmput`在mm_users归零时进`__mmput`；实际exit_mmap注释，do_exit的exit_mm也注释 | 任务退出/exec释放mm用户计数≠全部VMA释放 |

**[VERIFIED mykernel/mm/vm_map/util.c::__vma_unlink_from_list · VL14-A084，33–35行]**

```c
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	mm->map_count--;
	list_header_delete_node(&mm->mm_mt, &vma->list);
```

**[VERIFIED mykernel/mm/vm_map/util.c::__vma_unlink_from_list · VL14-A085，34–36行]**

```c
	mm->map_count--;
	list_header_delete_node(&mm->mm_mt, &vma->list);
}
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_free · VL14-A040，57–59行]**

```c
	// free_anon_vma_name(vma);
	kmem_cache_free(vm_area_cachep, vma);
}
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region · VL14-A050，782–784行]**

```c
unmap_and_free_vma:
	fput(vma->vm_file);
	vma->vm_file = NULL;
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_unmap_region · VL14-A055，1079–1081行]**

```c
	// update_hiwater_rss(mm);
	// unmap_vmas(&tlb, mas, vma, start, end, tree_end, mm_wr_locked);
	// mas_set(mas, mt_start);
```

**[VERIFIED mykernel/mm/vm_map/mmap.c::simple_unmap_region · VL14-A056，1082–1084行]**

```c
	// free_pgtables(&tlb, mas, vma, prev ? prev->vm_end : FIRST_USER_ADDRESS,
	// 			 next ? next->vm_start : USER_PGTABLES_CEILING,
	// 			 mm_wr_locked);
```


**需要特别区分调用者：** `simple_unmap_region`是`simple_mmap_region`失败清理路径调用的**静态函数**；本批`simple_do_vma_munmap`成功摘链没有活动到它的调用边，且前者内部清页表操作仍全为注释。不能凭“unmap”字样合并两条路径。

文件引用原语`get_file`只增加`file.f_count`；`fput`减少file引用，最终释放file对象，二者不是page_s引用。split新VMA的file引用不因其后unlink自动归还；但实际泄漏率、page生命周期和页表状态均未运行。页面引用/缓存槽归还按第13批限定，保留源缓存页与private cow页不同对象的区分。

**[VERIFIED mykernel/include/linux/fs/fs.h::get_file · VL14-A093，905–907行]**

```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
```

**[VERIFIED mykernel/fs/vfs/file_table.c::fput · VL14-A094，109–111行]**

```c
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;
```

**[VERIFIED mykernel/fs/vfs/file_table.c::fput · VL14-A095，126–128行]**

```c

		kmem_cache_free(filp_cachep, file);
	}
```


`do_exit`中的`exit_mm()`调用被注释。`exec_mmap`在特定old_mm分支活动调用`mmput(old_mm)`，后者计数归零才调用`__mmput`；但`__mmput`的`exit_mmap(mm)`同样是注释，所以即使实际进入mmput也不能从它获得VMA/PTE完全清理结论。fork的`dup_mmap`也会浅拷贝VMA并对file有条件get_file，此路径的`vm_ops.open`却为注释，不能和split的真实open路径混为一谈。

**[VERIFIED mykernel/sched/forkexec/exit.c::do_exit · VL14-A104，119–121行]**

```c

	// exit_mm();

```

**[VERIFIED mykernel/sched/forkexec/exec.c::exec_mmap · VL14-A103，676–678行]**

```c
		// mm_update_next_owner(old_mm);
		mmput(old_mm);
		return 0;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::mmput · VL14-A101，459–461行]**

```c

	if (atomic_dec_and_test(&mm->mm_users))
		__mmput(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::__mmput · VL14-A102，439–441行]**

```c
	// khugepaged_exit(mm); /* must run before exit_mmap */
	// exit_mmap(mm);
	// mm_put_huge_zero_page(mm);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · VL14-A097，187–189行]**

```c
		}
		if ((tmp = vm_area_creat_dup(mpnt)) == NULL)
			goto fail_nomem;
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · VL14-A098，198–200行]**

```c

			get_file(file);
			i_mmap_lock_write(mapping);
```

**[VERIFIED mykernel/sched/forkexec/fork.c::dup_mmap · VL14-A100，220–222行]**

```c
		// if (tmp->vm_ops && tmp->vm_ops->open)
		// 	tmp->vm_ops->open(tmp);

```


## 4. 结果、反证和停止范围

本批把三种结果严格分开：（1）在有效遍历前提下函数返回0/非0及VMA成员表变化；（2）PTE和多CPU TLB是否真的清除；（3）VMA对象、file引用、页面/缓存槽是否真的释放。**第1类的成功不能替代第2或第3类的证明**。分割错误也不保证已发生的第一分割副作用被撤销。

主动反证：摘链不是free；浅复制不自动获得全部引用；0返回不说明PTE/TLB已刷新；第二分割失败并非无副作用；首个VMA未必有严格前驱；函数名存在不等于被do_exit/mmput活动调用。对于非法/空指针、溢出、不同源码配置等，只给C语义风险和未证范围，不预测运行中的固定崩溃或_errno。

普通问题与`CORE-USER-FILE-CACHE-13::FC13-F05`、已有VMA/MM退出条目之间只在`deferred-findings.yaml`登记增量证据，不修内核、不重做12/13、不变更旧状态。编译预处理/C/ASM/内核/QEMU/硬件、完整页表清理和多核并发试验均**NOT_RUN**。仅申请`READY_FOR_MAINLINE_REVIEW`，主线`mainline_acceptance=PENDING`，阶段2上限`PASS_PENDING_LOCAL`。
