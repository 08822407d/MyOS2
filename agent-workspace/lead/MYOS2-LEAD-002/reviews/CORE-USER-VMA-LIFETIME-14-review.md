---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-VMA-LIFETIME-14-REVIEW-001
record_type: bounded_static_review_with_additive_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-10-31
read_channel: connector
evidence_class: 固定交付全文阅读、关键控制流回源与远端差异核查；非运行认证
authorization_ref: Owner报告CORE-USER-VMA-LIFETIME-14完成；28号验收后同轮派发持续授权
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: dd8e81ba6fc7
reviewed_base_short12: 2d67bea2fe59
source_pin_short12: a039d9803ade
contract_short12: b8ae7762a96e
lead_before_this_turn_short12: d3107ee2279d
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_CORRECTIONS
acceptance_verdict: PASS_PENDING_LOCAL
acceptance_scope: 真实C入口、条件化VMA区间操作、选定引用及清理断点；原文须与本回执合读
returned_scope: []
consumption_corrections: [R14-S01, R14-S02]
prior13_lifecycle_followup: SELECTED_MUNMAP_EXIT_CONNECTIONS_TRACED_TO_EXPLICIT_BREAKPOINTS
kernel_cleanup_problem_closed: false
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
human_technical_review_attested: false
reviewer_execution: false
reviewer_reproduced_107_quote_checks: false
reviewer_reproduced_73_narrative_checks: false
reviewer_parsed_yaml_or_dot: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-MM-CONTEXT-15
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/33-user-mm-context-contract.md
inputs_read:
  - PR17交付评论issuecomment-6543731811及dd8e81ba6fc7六件全文；facts 1–1272行分段，截断后的边/effects用重叠范围补齐。
  - 固定time mmap.c 1–235、1060–1185、1312–1350；vm_map.h 89–105与248–360；vm_map_macro.h 1–65；vm_map_api.h 40–58；vm_map_types.h 1–115；util.c 1–180。
  - 固定time arch/x86_64/myos/syscall.c 39–138，完整brk主体及out标签；syscalls_64.h 8–18和unistd_64.h 10–20。
  - 固定time lib/list/double_list.h 120–305与620–710，含节点删除及其直接帮助函数；条件编译未预处理。
  - 固定time fork.c 145–485、490–760；exec.c 600–700；exit.c 85–170。copy_mm/dup_mm/mm_release等额外范围同时用作15设计预检，不冒称15已经执行。
  - 同一固定源码的mmap.c创建/错误清理、get_file/fput、options_flags.cmake及18/24/25/28/32号约束沿用本对话前次读取；不声称本轮重新加载整套指导。
  - 为15设计，默认分支copy_mm/switch_mm搜索只导航，mmu_arch.c 1–180回固定time；尚未闭合调度切换或全部mm引用路径。
  - 两PR状态、直接heads、开放PR第三项为空、PR16完整路径及本批compare。
review_limits:
  - 六件全文和关键语义核查，不是107条源文与73段正文机械检查的独立复现；形式解析及未遍历的全局调用仍partial/pending_local。
  - 未运行命令、JS/Python、解析器、哈希、旧脚本、预处理、C/ASM、构建、内核/QEMU或硬件实验。
  - 同厂模型分角色复核，不构成异族模型或人类独立技术验收；不证明完整MM并发、TLB或物理回收正确。
  - RS-01、GAP-U04硬件/运行部分、GAP-U10及既有普通延期项保持原边界。
---

# 第14批限定接收；明确纠正brk回滚误判

## 1. 裁定与真实覆盖

第14批按PASS_PENDING_LOCAL接收，必须合读本回执两组纠正。发现的brk字段回滚误判是真实报告错误，不因引文匹配而获得认可；§3给出完整替代读法，§4补足调用证据，不留下需要另跑一轮的小型返工。旧文件保留，不表示原错误曾经正确，也不改变内核。

compare 2d67bea2fe59→dd8e81ba6fc7为8个后继提交，仅user-vma-lifetime-14/六件A新增，共2375行，无旧件净修改/删除。六件全文已读：MANIFEST 82、正文701、facts 1272、DOT 112、延期132、evidence 76行；执行头与交付评论相符，PR17为open/Draft。执行方107/107、73/73及图集合计数保留为其文本检查记录，不能冒称主线已重新运行。

13留下的munmap/退出连接现在已在选定路径追到具体静态断点：不是“尚未读到”，也不是“清理已正常工作”。全树其他回收者、硬件PTE/TLB和实际泄漏未验证，不关闭FC13-F05等普通问题。

## 2. 可保留的主要结论

已核编号11、系统调用表和mmap.c的munmap C定义及vm_munmap宏别名；只确认C绑定，不重新认证08汇编/用户libc/运行链。sys_mmap确经vm_mmap_pgoff和do_mmap，不把中间层压成直调。

find_vma_intersection先找vm_end严格小于start的前驱，再只检查其后继；初始NULL遍历表达式和相邻边界必须保留对象有效性前提。报告的区间矩阵是条件化的分支推演，不是23个已执行测试。空区间、溢出、首VMA等情况不能套用标准Linux结果或预言固定错误码。

[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
			if (vma != NULL && (vma = vma_next_vma(vma)) != NULL &&
					__vma_intersect_with_range(vma, start, end))
				return vma;
```

__split_vma先分配/复制，再有条件get_file和vm_ops.open；第二次分割分配失败不会由simple_do_vma_munmap的错误标签撤销第一次分割。__simple_vma_adjust的vm_pgoff表达式与字段页单位确实需要限定；不据此断言某次文件访问已失败。

[VERIFIED mykernel/mm/vm_map/util.c::__vma_unlink_from_list]
```c
	mm->map_count--;
	list_header_delete_node(&mm->mm_mt, &vma->list);
```

已继续核list_header_delete_node→list_del_init及直接列表操作，所选实现是指针/列表计数更新，不释放VMA内存、file引用或页。源码的列表一致性检查本身可阻断，不能无条件称操作总会返回。simple_unmap_region主体清页表/TLB语句为注释，并非普通munmap摘链的活动下游；创建失败路径的fput/free不可移植成munmap已清理。

已核do_exit中的exit_mm注释、exec_mmap旧mm分支的mmput、mmput→__mmput及后者exit_mmap注释。exec_mmap另有old_mm为空时的mmdrop(active_mm)路径；不能把“__mmput无exit_mmap”扩大为exec所有分支都不释放任何对象。具体mm持有/切换/释放归15增量核查。

## 3. R14-S01：恢复brk字段，不等于回滚VMA事务

**错误位置：** vma-lifetime.md §2.4矩阵后的“失败路径没有恢复这次brk修改”；facts的VL14-M18.limits和VL14-X10.description；deferred的VL14-F05中把brk字段与VMA/引用变化并列为不回滚的premises/impact。原VL14-A057只覆盖提前赋值和goto out，未覆盖真实错误出口。

[VERIFIED mykernel/arch/x86_64/myos/syscall.c::sys_brk]
```c
		mm->brk = brk;
		if (simple_do_vma_munmap(mm, newbrk, oldbrk))
			goto out;
```

[VERIFIED mykernel/arch/x86_64/myos/syscall.c::sys_brk]
```c
out:
	mm->brk = origbrk;
	// mmap_write_unlock(mm);
	return origbrk;
```

**替代结论：** 在定义良好且unmap返回非零、控制流正常抵达out的条件下，sys_brk恢复mm->brk为入口保存的origbrk，并返回origbrk，而不是该helper的负errno。unmap内部此前可能发生的VMA边界、链表或file引用变化没有因此自动撤销。若helper不返回或发生无效对象访问，不可继续推定out执行；若helper因未选中目标而返回0，走的是另一条成功分支，也不认证映射已撤销。

```yaml
correction: R14-S01
replace_consumption:
  VL14-M18.limits: 非零unmap正常返回后out恢复mm->brk；此前VMA/引用副作用不保证回滚；不返回路径另列。
  VL14-X10.description: 先写请求brk并调用unmap；非零返回经out恢复origbrk，零返回走success；VMA事务状态分开。
  VL14-F05.premises: 第一次split成功且第二次失败，或覆盖旧区间后新映射失败；brk子情形只涉及内部VMA/引用副作用，不声称mm->brk字段未恢复。
  VL14-F05.impact: 错误返回不保证VMA区间和file引用完全回到入口状态；sys_brk正常out路径确实恢复brk字段。
retain:
  - 两次分割失败偏序与mmap覆盖失败的原有限结论。
  - VL14-F05编号、延期状态与未运行限制。
```

该替代同时约束正文及同义摘要。不能用“内部helper不管brk”给原“sys_brk不回滚”句辩护，也不能反向说完整地址空间事务已恢复。无需执行方重做整批或修改旧文件。

## 4. R14-S02：调用证据和图的精确消费

E31原A039在if(vma)处结束，没有包含vma_init调用。补以下真正调用点，并用于X01对调用关系的消费；A069/070证明被调函数体，不能单独替代调用证据。

[VERIFIED mykernel/mm/vm_map/mmap.c::vm_area_alloc]
```c
	if (vma)
		vma_init(vma, mm);
	return vma;
```

E39原A006只有out_fput清理，不支持sys_mmap→vm_mmap_pgoff；以下是实际调用证据。保留off未对齐、非匿名fget失败和匿名MAP_HUGETLB忙等等先行门，不将active解释为所有输入均调用。

[VERIFIED mykernel/mm/vm_map/mmap.c::sys_mmap]
```c
	retval = vm_mmap_pgoff(file, addr, len, prot, flags, pgoff);
out_fput:
```

E40也须连读A087的mmap_write_lock_killable早退；E33的simple_mmap_region→call_mmap是具名帮助函数调用，内部FOP分派才是间接调用。后继不得仅因节点标dynamic就丢掉帮助函数层。E49只能表示do_exit处有被注释的exit_mm语句和未闭合连接，不代表源码存在被注释的直接do_exit→mmput调用；图中虚线/点线不能充当已证明调用链。其余对象data边合读effects，不把副作用当资源来源流向。

这些修订不否定真实入口和对象链，也不认证全部58条边的形式/运行正确性。必须保留报告原件及本回执的明确覆盖关系。

## 5. 后继与权限

同轮发布CORE-USER-MM-CONTEXT-15：以任务mm/active_mm、mm_users及实际结构引用字段、fork共享/复制、exec替换和调度切换接口为增量范围。不是再次分析14的split/unlink，也不是补丁或阶段3架构实施。14的C清理断点作为输入，mm引用和实际CR3切换前提不能凭Linux习惯补全。

主线仅在现有lead新增本回执、33号任务和检查点，并同步PR导航/评论；不写执行结果。107/73检查未独立机械复跑，未构建或运行内核/QEMU。两PR及分支保留，PR17仍Draft，不合并/关闭/删除/强推。普通问题按18号继续入库，无需Owner现在作修复决定。
