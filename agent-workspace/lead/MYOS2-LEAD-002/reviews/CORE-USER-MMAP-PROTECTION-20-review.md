---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-MMAP-PROTECTION-20-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
evidence_class: 固定交付全文阅读、关键源码回核与远端增量比较；非运行或全量机械认证
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-01
read_channel: connector
authorization_ref: Owner报告第20批完成；28号同轮验收与下一工作包持续约定
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 0e139c6ca86d
reviewed_base_short12: 6166b3e2f5b9
contract_short12: 038217219faa
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-02
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 7f29149ac743
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R20-S01, R20-S02, R20-S03, R20-S04]
acceptance_scope: mmap入口及参数、所选权限转换和文件检查、新建或合并VMA与代表回调、保护模板的有限静态材料；须合读本回执
kernel_permission_or_resource_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_155_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-MMAP-PLACEMENT-21
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/40-user-mmap-placement-contract.md
inputs_read:
  - PR17交付评论issuecomment-6559534657、两PR元数据、直接heads、开放PR第三项为空、PR16完整变更路径及20增量compare。
  - 固定0e139c6ca86d六件全文；正文分1–55/56至末尾，evidence分1–330/300–560/561–820/821–1080/1081–1340/1341至末尾，facts分1–410/411–820/821–1230/1231–1610/1611–2085及2000至末尾补齐截断。
  - 固定time mmap.c 468–647、650–795、1310–1345；vm_map.h 400–445及159–172；vm_map_macro.h全文；vm_map_const.h 70至末尾。
  - 固定time file.c 200–235；file_table.c 25–132；fs.h 895–912、1950–1960、3420–3433；mmap_lock.h 110–143。
  - 固定time syscalls_64.h 1–20、syscall_64.c全文、syscall_wrapper.h 1–105/103–112/235至末尾、syscalls.h 1380–1395。
  - 固定time options_flags.cmake及pgprot.c全文；pgtable_const_arch.h 60–95；FAT32_file_ops 350–368；filemap.c 395–425；exec.c 58–66；mman-common.h 1–70。
  - 同源util.c、PTE模板、VMA合并和地址选择等部分依据沿用本对话前次实际返回的固定对象；不申报本轮全部重新读取。39号、19回执及18/24/25/28/36规则沿用已加载版本。
  - 工作令本轮定向重读§5和§12等段；长响应截断已补关键段，不将其称为整套规则重新加载。
  - 为21设计：simple_get_unmapped_area默认分支搜索仅导航，实际回固定time读mmap_arch.c全文；既有同源选址正文只作范围预检，不认定21已完成。
review_limits:
  - 全文阅读与有界语义核对，不是155条引文逐字检查、60条图边或32场景的全部独立机械认证。
  - 未执行命令、文本程序、解析器、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU或硬件实验。
  - 未穷尽所有调用者、文件回调、文件表状态、配置与保护掩码写者，不认证实际W^X、共享语义、同步、引用平衡或TLB。
  - 同厂分角色审查不等于异族模型或人类技术验收；RS-01、GAP-U04硬件/运行部分、GAP-U10及普通延期项不关闭。
open_questions:
  - 全量机械及本地闸门仍partial/pending_local；本回执已明确处置下列报告问题，无另发执行返工项。
---

# 第20批限定接收：转换顺序、对象身份和源码归属不能省略

## 1. 裁定及实际覆盖

按PASS_PENDING_LOCAL接受第20批可复用静态材料，强制合读R20-S01至S04。原六件保留，主线直接追加修正；不是原始每条表述无条件通过，也不要求C03b单独重跑一轮文档返工。任何内核问题均未因报告接收而关闭。

交付评论与实际执行头均为0e139c6ca86d。远端compare 6166b3e2f5b9→0e139c6ca86d为1个后继提交、恰user-mmap-protection-20/六件A新增3964行，无旧件净修改或删除。MANIFEST 51、正文93、facts 2077、DOT 120、deferred 214、evidence 1409行均已分段阅读。

执行方报告155个锚点、53节点/60边、32场景、21 effects、6个条件新记录与2个历史追加及12次远端回读。主线没有重新运行这些检查，也不把计数等同独立性质证明。P9可读语义闸门按本回执的纠正后口径消费；未机械完成部分仍partial/pending_local，最高不超过PASS_PENDING_LOCAL。

## 2. 可以保留的主链

所读syscalls_64.h、x64_sys_call及包装宏提供mmap到六参数主体的静态绑定；这不是预处理或链接产物认证。off按字节校验后转pgoff；该syscall直接走vm_mmap_pgoff，不能套用另一vm_mmap包装的全部检查。

在有效文件对象与正常返回的前提下，fget临时持有、新建VMA的get_file和最后fput分开记账。引用算术还要求对象身份一致、无其他增减及数值有效；合并、回调替换或错误清理不能套相同固定轨迹。

do_mmap实际包含PROT_READ条件增PROT_EXEC、VM位组合、文件模式/noexec/append及回调存在检查。所选mmap_write_lock_killable返回0，mmap_write_unlock活动体没有up_write；只能判本包装未取得所命名的锁，不能证明全系统没有其他同步。

新VMA保护先于文件回调赋值；FAT32_file_ops.mmap绑定generic_file_mmap，后者所读主体仅设置vm_ops并返回0。默认保护模板、实际PTE和CPU行为分别保留，不由成功返回地址推导硬件映射通过。14及19主线限定继续适用。

## 3. R20-S01：flags过滤发生在VM属性生成之后

目标：正文P02、facts的permission_layers L1/L2、N21相关E17/E22、X04/X05及M19/M20。原分层图可作概念分层，但不能当成所有归一化完成后才生成VM属性的时间顺序。

[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	vm_flags |= calc_vm_prot_bits(prot, pkey) | calc_vm_flag_bits(flags) |
				mm->def_flags | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC;
```

上式位于选址和文件类型分支之前；后面的文件MAP_SHARED分支才执行：
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			flags &= LEGACY_MAP_MASK;
			fallthrough;
		case MAP_SHARED_VALIDATE:
			if (flags & ~flags_mask)
				return -EOPNOTSUPP;
```

替代口径：分别记录传入flags、FIXED_NOREPLACE处理后的flags、生成vm_flags时的快照和文件共享分支过滤后的flags。calc_vm_flag_bits消费的是过滤前快照；后续修改flags本身不回溯撤销已生成的VM属性，所读do_mmap没有为该过滤重新调用转换。M20的vm_flags字段不能写成flags的赋值表达式。

[VERIFIED mykernel/mm/vm_map/vm_map.h::calc_vm_flag_bits]
```c
			return _calc_vm_trans(flags, MAP_GROWSDOWN, VM_GROWSDOWN) |
					_calc_vm_trans(flags, MAP_LOCKED, VM_LOCKED) |
					_calc_vm_trans(flags, MAP_SYNC, VM_SYNC) |
					arch_calc_vm_flag_bits(flags);
```

因此，即使后续从原始flags清除MAP_SYNC，也不能据此说已生成的VM_SYNC同时清除。这里只是所选表达式的条件推导，不认证同步I/O语义或实际故障。M19能力掩码应明确为LEGACY_MAP_MASK，再在fop_flags含FOP_MMAP_SYNC时OR入MAP_SYNC；FOP_MMAP_SYNC不是直接加入MAP标志集合的那个值。

正文“只有显式EXEC或READ增EXEC才触发noexec拒绝”也须收窄：拒绝门检查的是组合后的vm_flags & VM_EXEC，还可能受到传入vm_flags或mm->def_flags影响；本syscall传入vm_flags为0不等于def_flags已被全局证明为0。

## 4. R20-S02：宏有条件执行，不等于一定改变位或返回同一对象

A123–A126标题及E38、permission_layers L4和template mask模型中的“PKEY可增位”，必须合读固定常量：
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::_PAGE_PKEY_BIT0]
```c
	#define _PAGE_PKEY_BIT0				(_AT(pteval_t, 0))
	#define _PAGE_PKEY_BIT1				(_AT(pteval_t, 0))
	#define _PAGE_PKEY_BIT2				(_AT(pteval_t, 0))
	#define _PAGE_PKEY_BIT3				(_AT(pteval_t, 0))
```

在这些定义下OR这些值不会新增任何位。不能仅因pgprot.c有条件编译块，就认定本树实现了有效PKEY附加权限；本轮也不保证非默认配置可编译。VM_SOFTDIRTY同样可能为0，E54及正文“设置/增位”按“执行OR宏，数值效果取决于配置和旧值”理解，不保证产生新置位。

M11的“VM_MAYSHARE+MAY*”不得包含已被无FMODE_WRITE分支清掉的VM_MAYWRITE。正常模板表要求实际低位VM标志、支持掩码等前提；其new/merge列不能把新建时计算的模板推广到任意既有VMA。合并保留的保护须另核旧对象状态。

回调后若再次合并成功，函数会释放新VMA并执行vma=merge，随后使用另一个对象的字段。F04/M24和正文应理解为“本函数没有活动的自动保护刷新；最终保护取决于回调是否改保护、是否改用合并对象及其他写者”，不是所有返回路径都保留最初新对象的保护。已核generic_file_mmap未改访问位，不能充当实际失配生产者。

E53对应局部vm_flags赋值即使数值没有变化也可以发生；条件应是控制流抵达相应赋值，而不是必须观察到flags变化。VM_MAY*的上限用途是语义说明，本批未核mprotect，不认证该上限已在所有消费者处强制执行。

## 5. R20-S03：无文件分支不能漏掉DROPPABLE；fd范围按转换后实参

正文P02“其它无文件类型均走default错误”过宽。所读do_mmap明确有MAP_DROPPABLE分支：先检查VM_DROPPABLE能力、LOCKED/HUGETLB及GROW条件，再设置相关VM位并落入PRIVATE的pgoff处理。它不能被统称为default；M21关于无文件MAP_SHARED_VALIDATE的限定仍成立。

[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		case MAP_DROPPABLE:
			if (VM_DROPPABLE == VM_NONE)
				return -ENOTSUPP;
```
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			vm_flags |= VM_WIPEONFORK | VM_DONTDUMP;
			fallthrough;
		case MAP_PRIVATE:
```

本补充仅保留真实分支及其条件，不认证droppable回收、fork语义或任一实际调用成功；矩阵32行不是全部输入空间穷举。

入口fd为unsigned long，而fget及__simple_fget形参是uint。M26及正文范围比较必须针对进入fget后的转换值；不能仅用原始宽fd大小判定一定超出fd_count。本轮不追加类型宽度实机认证，也不把条件转换或空槽风险称为实际可利用故障。读取槽后未检查NULL的局部事实及needs_evidence状态保留。

## 6. R20-S04：函数归属、宏体证据与回调边

A145/A146的引文确实来自file_table.c，但其标签alloc_empty_file错误；这些赋值位于__alloc_file。以下覆盖原标签及相关E57/E60、N53和正文的直接作用者表述：

[VERIFIED mykernel/fs/vfs/file_table.c::__alloc_file]
```c
	f->f_flags = flags;
	f->f_mode = OPEN_FMODE(flags);
```
[VERIFIED mykernel/fs/vfs/file_table.c::alloc_empty_file]
```c
	f = __alloc_file(flags);
```

正确链为alloc_empty_file调用__alloc_file，由后者设置字段并求值OPEN_FMODE。原图可按明确的间接效果消费，不将外层函数画成直接执行上述赋值或直接调用宏。字面引文相同但函数归属错误，不应计为原标签通过；此处已由主线给出替代，不要求重绘全部旧图。

A134实际引用vma_is_anonymous，标签不能是vma_set_anonymous；前者读取vm_ops判NULL，后者才写NULL。A129的标签PAGE_NONE也不能覆盖其实际引用的PAGE_COPY/PAGE_READONLY/PAGE_READONLY_EXEC定义，各宏分开归属。

E02包装转发补真实__SYS_STUBx宏体，避免仅靠它在另一个宏中的出现证明内部调用：
[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
					return __se_##name(regs);					\
```

E45的file->f_op->mmap与FAT32_file_ops.mmap绑定是间接目标解析证据，不是两个真实中间函数。按“call_mmap经槽间接调用generic_file_mmap，条件为实际文件采用此表”理解；不把表项作为一次额外函数调用，也不推广至全部文件类型。其它未逐条独立复核的图边不因ID集合相同而获得语义认证。

## 7. 风险状态与同轮下一步

6项新记录继续为各自的条件问题/策略候选，2项旧追加保留原ID和状态。F01/F05/F06可能涉及设计约定，不能仅凭与惯常实现不同判成已证实安全缺陷；F02/F03/F04仍须并发、槽生产者或真实变更回调等证据。报告纠正不等于代码已修，不要求Owner现在作修复决定。

下一批CORE-USER-MMAP-PLACEMENT-21只补本批显式留出的地址选择接口：回调绑定、提示地址与整段区间、空洞遍历、固定地址及冲突校验、结果如何送入已有映射处理。已读mmap_arch.c提供一个实际绑定端点；选址控制流尚待执行方系统取证，不把主线预检写成21成果。

主线只新增本回执、40号任务和恢复检查点并同步两PR入口。保持阶段2、18号延期、28号同轮派发、36号C03b分工和PR17 Draft；无内核/用户程序修改、补丁、构建运行、合并关闭、删除强推或阶段3授权。
