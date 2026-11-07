---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-MMAP-PLACEMENT-21-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
evidence_class: 固定交付全文阅读、关键源码语义核对及远端增量比较；非运行或全量机械认证
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告CORE-USER-MMAP-PLACEMENT-21完成；28号验收及下一工作包同轮持续约定
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 16baace7bd3c
reviewed_base_short12: 0e139c6ca86d
contract_short12: 84ca207f5cc2
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-02
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 80a9c00e8704
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R21-S01, R21-S02, R21-S03]
acceptance_scope: 所选地址回调、提示/固定地址分支、候选覆盖、迭代与交集查询和VMA入口的条件性静态材料；必须合读本回执
kernel_placement_or_lifetime_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_all_92_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-VMA-MERGE-22
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/41-user-vma-merge-contract.md
inputs_read:
  - PR17交付评论issuecomment-6566959979、PR16/17元数据、直接heads、开放PR第三项为空、PR16完整变更路径及本批compare。
  - 固定16baace7bd3c六件全文；evidence分1–300/301–600/601至末尾，facts分1–400/401–820/821–1230/1231–1620/1621至末尾；其余四件完整返回。
  - 固定time mmap.c 1–215、200–417、418–650、797–1008、1060–1185；650–795创建主体沿用本对话前次同一固定对象实际读取。
  - 固定time vm_map.h 230–405；double_list.h 1–260与400–480；vm_map_types.h全文；vm_map_macro.h沿用本对话前次固定全文。
  - 固定time mmap_arch.c全文、exec.c 900–1110、page_const_arch.h全文、processor_const_arch.h 1–170与171–269至末尾；270–425空返回仅说明越过文件末尾。
  - 固定time align.h及uapi/linux/const.h全文；同源options_flags.cmake、PAGE_ALIGN绑定及offset_in_page等沿用此前实际读取，不申报本轮全部重新加载。
  - 80a9c00e8704的14主线回执正文和dd8e81ba6fc7的14 deferred-findings正文，用于限定VL14-F01/F02/F03/F05及历史归属；输出截断发生于后续元数据，不把未显示元数据当已核。
  - 40号任务、20回执及18/24/25/28/36分工沿用已实际加载的固定版本。
review_limits:
  - 六件全文和关键路径回源，不是92段引文、63条边、30场景与五组算例的独立机械全量认证。
  - 未执行命令、文本程序、解析器、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU或硬件测试。
  - 未证明初始NULL遍历表达式的目标编译语义、完整调用时序、实际冲突/覆盖、全部配置、文件回调、锁、PTE/TLB或资源回收。
  - 同厂模型分角色核查不构成异族模型或人类技术验收；RS-01、GAP-U04硬件/运行部分、GAP-U10及普通延期项不关闭。
open_questions:
  - 全量机械及本地闸门仍partial/pending_local；本回执明确处置的报告问题不另发小型返工。
---

# 第21批限定接收；地址值不等于已验证的空闲区间

## 1. 裁定与交付身份

按PASS_PENDING_LOCAL接收，原六件必须与R21-S01/S02/S03合读。主线直接追加有证据的替代口径，不改旧结果、不要求C03b重做21，也不把源码风险当作已修复。

交付评论和实际执行头均为16baace7bd3c。compare 0e139c6ca86d→16baace7bd3c为1个后继提交，仅user-mmap-placement-21/六件A新增3230行，无历史文件净改动或删除。MANIFEST 49、正文83、facts 1865、DOT 126、deferred 213、evidence 894行均已阅读。执行方的12次双重回读和92条引文机械检查是其自报；主线独立核对本次差异与下述关键语义，不冒称重跑全部检查。

## 2. 可以保留的主要结论

固定arch_pick_mmap_layout给mm设置simple_get_unmapped_area与TASK_UNMAPPED_BASE，setup_new_exec有实际调用并随后设置独立task_size字段。只证明一个生产路径，不证明每个mm的初始化和使用时序均已闭合。

默认提示快速路径只查询起点是否被simple_find_vma包含；返回候选不证明整段无交集。非固定回调之后的__get_unmapped_area主要检查编码错误、TASK_SIZE上界和页内偏移，没有在该函数补完整VMA交集检查。MAP_FIXED则早于这些后置门返回，由外层继续消费。

[VERIFIED mykernel/mm/vm_map/mmap.c::simple_find_vm_unmapped_area_topdown]
```c
	if (gap = ~0) {
		gap = info->high_limit - info->length;
		gap &= PAGE_MASK;
	}
```

这是赋值，不是比较；若在相应语言和对象前提下抵达，原候选被覆盖。底部搜索的if(gap = 0)同样是赋值，抵达后返回0，但所选默认链没有调用bottomup。不能把两个局部事实扩大成所有mmap必然返回错误地址；初始迭代和耗尽分支均可能先阻断证明。

选定PAGE/TASK宏、TASK_UNMAPPED_BASE和真实ALIGN底层表达式支持报告的条件性常量推演。没有运行这些表达式或核实二进制布局；对齐、上界相减和实际可达仍分层。选址后的simple_mmap_region先调用旧VMA处理，而simple_do_vma_munmap又消费同一个find_vma_intersection，所以后者不是独立正确性兜底；不把“调用清理”写成“已经消除重叠”。

## 3. R21-S01：检查顺序、值的阶段与早退

### 提示值

目标为C13、E02/E27以及正文A01/A02的对应概括。先保存输入含义，再说明实际检查值：

[VERIFIED mykernel/mm/vm_map/mmap.c::round_hint_to_min]
```c
	hint &= PAGE_MASK;
	if (((void *)hint != NULL) &&
	    (hint < mmap_min_addr))
		return PAGE_ALIGN(mmap_min_addr);
	return hint;
```

替代C13：只有掩码后的hint非零且小于当前mmap_min_addr才提高到对齐后的最小值。原始0<addr<PAGE_SIZE的输入会先变成0，不能概括为“任何非零低提示都提升到最小地址”。mmap_min_addr的读取参与比较，其发生也不要求最终比较为真，E02不能把读取门写成已知低于最小值。

simple_get_unmapped_area的if(addr!=0)在其本地掩码之前。因此E27应以进入该if的值非零且控制流抵达调用为门，不是笼统“掩码后非零”；正常do_mmap调用已经经过round_hint_to_min，但独立函数语义仍须保留两次处理的顺序。

C15、E23/E29、X09的高窗口调整还要求此前没有由提示快速路径return。addr超过DEFAULT_MAP_WINDOW本身不是执行该加法的充分条件；读取常量以求比较与条件为真后修改info也是两个动作。所选宏下加数为0的结论保留。

### 错误值与固定标志

[VERIFIED mykernel/mm/vm_map/mmap.c::__get_unmapped_area]
```c
	addr = get_area(file, addr, len, pgoff, flags);
	if (IS_ERR_VALUE(addr))
		return addr;
```

C28的-ENOMEM须补“回调正常返回且!IS_ERR_VALUE(addr)”及后续上界门确实抵达。高端数值若先被归为编码错误就原样返回，不再执行该上界检查。C05/C09/C30的外层消费也不能略过do_mmap在NOREPLACE查询之前的IS_ERR_VALUE门。

C12/F01中“非fixed提示路径，另有NOREPLACE门”的措辞不能读成两者同时属于本入口的实际路径：do_mmap先因MAP_FIXED_NOREPLACE置MAP_FIXED，有该标志的请求不会再进入选定非固定提示回调。可以比较两种不同请求，不能将它们拼接成一条运行路径。F01关于起点查询的局部结论不因此撤回。

## 4. R21-S02：初始NULL与相邻区间是独立前提

[VERIFIED mykernel/mm/vm_map/mmap.c::mm_prev_vma]
```c
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	while (list_is_head_anchor(&vma->list, &mm->mm_mt.anchor));
```

mm_next_vma有同形代码；所读list_is_head_anchor只比较实参指针，它不替调用方补上NULL对象检查。地址表达式由调用方求值，不能写成该被调函数内部解引用NULL。

所有以vma/tmp=NULL开始的所选迭代，第一次就会面对这一表达式，非空且有序的链表也不消除它；它不只是“空表”和“遍历耗尽”才出现的问题。因此C06/C07/C12/C17等纸面推演不能仅以“列表有效”当作完整可达证明。保留它们作为条件模型：下层谓词/赋值的局部含义成立，但端到端执行仍须独立建立初始迭代的语言与目标实现前提。不能假定绕过该缺口后再把后续结果标为已发生。

find_vma_intersection的首VMA漏检分析可保留，同时严格前驱还可能使其他贴邻情形漏检。条件例：有序非空VMA为A=[0x1000,0x2000)、B=[0x3000,0x4000)、C=[0x4000,0x6000)，请求[0x4000,0x5000)。假定查询帮助函数正常完成，严格vm_end<start选择A而跳过end==start的B；其后仅测试B，因B恰邻接而返回NULL，没有继续检查真正相交的C。这是对已读分支的纸面补充，不是绕过初始NULL问题的实机反例，也不另外计新根因。

[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
			if (vma != NULL && (vma = vma_next_vma(vma)) != NULL &&
					__vma_intersect_with_range(vma, start, end))
				return vma;
```

“半开区间交集谓词正确”的适用前提还包括start<end以及VMA非空有效。helper只对start>end循环，并没有保证空区间被拒绝。正常do_mmap的非零、页对齐长度与未回绕前提可排除相应空区间，但不能推广到该helper所有调用者。R14关于空区间和溢出的既有限定继续有效。

## 5. R21-S03：证据数量、图关系与历史根因

- evidence.md头部仍是verified_claims=91、quotes_reconfirmed=91，但正文已追加AP21-A092；MANIFEST、facts和交付评论记92。按当前文件登记92个编号引文片段，头部91为过时计数；不将此纠正冒称主线92/92逐字机械复验。P9的计数差异由此追加处置，运行与全量机械闸门仍未过。
- A069只引用结构闭合和头文件尾，不能独立支持start_gap字段。该事实使用A068及固定vm_map_types.h的真实字段，A069不增加独立支持强度。对象账info同样合读，不要求重写旧件。
- E30/E33/E40/E55的终点是所用迭代宏而不是最终被调函数。按明确的宏展开关系消费，不把它们画成省略了未列中间函数的call_path；宏中的mm_next_vma/mm_prev_vma实际调用由E41/E34及A063/A064支持。宏不是额外运行函数。DOT同ID使用同一限定；未全面独立复核的其余边仍不得凭ID相同自动通过。
- AP21-F03中的初始NULL成员表达式、AP21-F04的严格前驱交集条件已见14的CORE-USER-VMA-LIFETIME-14::VL14-F01。本批在地址搜索/NOREPLACE消费侧扩展证据，保留AP21原ID便于追踪，但相关子结论按该旧项的evidence_append理解，不统计为新增独立根因。F03关于搜索耗尽后vma_next_vma(NULL)的具体路径单独保留，不能因共享旧根因而删掉新路径事实。

七条本批条件记录加一项历史追加不等于八个新缺陷，更不是八个已复现问题。VL14-F01/F02/F03/F05的编号、延期状态及R14-S01的brk字段恢复限定均不改变；不要求Owner现在决定修复。

## 6. 下一工作包与停止边界

同轮发布CORE-USER-VMA-MERGE-22：只补simple_mmap_region的合并调用、候选对象选择、__simple_vma_adjust的边界/页偏移与必要链表及引用副作用。20的权限链、21的选址与查询缺口、14的split/unlink清理已建立，不重新全面取证；已知根因只追加merge使用路径证据。

本轮固定源码的合并主体仅作为任务设计预检，不宣告22已完成。后继应特别区分输入prev与实际前驱、局部变量是否已赋值、合并目标是否真正覆盖请求，以及返回旧对象与创建新对象的区别。遇未初始化或无效对象前提必须停止确定结果推演，不能按注释中的Linux案例补全实现。

主线只在既有lead分支新增回执、41号任务及恢复检查点并同步PR入口；执行分支和旧成果冻结。保持阶段2、18号延期、28号同轮派发、36号C03b分工、PR17 Draft；无内核修改、补丁、构建运行、合并关闭、删分支、强推或阶段3授权。