---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-VMA-MERGE-22-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
evidence_class: 固定交付全文阅读、关键源码语义回核和远端增量比较；非运行或全量机械认证
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告CORE-USER-VMA-MERGE-22完成；28号同轮验收和后继派发持续约定
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: b32f3ebc9095
reviewed_base_short12: 16baace7bd3c
contract_short12: cffb519d089b
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-02
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: eccba115cf83
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R22-S01, R22-S02, R22-S03]
acceptance_scope: 两次合并入口、候选/目标选择、字段及必要列表/引用接口的条件性静态材料；原件须合读本回执
kernel_merge_or_resource_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_all_94_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-EXEC-STACK-23
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/42-user-exec-stack-contract.md
inputs_read:
  - PR17交付评论issuecomment-6567624838、PR16/17元数据、直接heads、开放PR第三项为空及本批增量compare。
  - 固定b32f3ebc9095六件全文；正文首次长响应后用90–150补末尾；evidence分1–320/321–640/641–950；facts分1–420/421–840/841–1260，截断后用1200–1510/1511–1790/1791–2070补齐；MANIFEST、deferred和DOT完整返回。
  - 固定time本轮重读mmap.c 195–417和650–795；util.c 1–110；double_list.h 160–325和620–745。
  - 同一固定对象的mmap.c前驱/迭代/分配包装、vm_map.h查询与vma_pages、vm_map_types.h、vm_map_macro.h、VM常量/PAGE常量、get_file/call_mmap/fput及options_flags.cmake沿用本对话此前实际读取；不申报本轮全部重读。
  - 41号任务、21/20/14回执及18/24/25/28/36既有规则沿用已加载固定版本；主线写区基线未发生未知漂移。
  - 为23范围预检：固定time mmap.c 976–1065、exec.c 90–230和440–630、arch/x86_64/mm/fault.c 1–250及250–370；12批deferred正文定点确认MF12-F01等旧根因。
  - 默认分支mprotect/expand_stack搜索只作导航，未以其替代固定time证据或宣告23已完成。
review_limits:
  - 全文阅读和关键语义核查，不是94段引文、59条边、36场景和四组算例的全量独立机械复验；未完成部分partial/pending_local。
  - 未执行命令、文本程序、解析器、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU或硬件测试。
  - 未认证NULL初始迭代、全部上游状态生产、回调、并发、资源净额、页表/TLB或具体故障的实际可达性。
  - 同厂模型分角色核查不构成异族或人类独立技术验收；RS-01、GAP-U04硬件/运行部分、GAP-U10及普通延期项保持。
open_questions:
  - 全量机械、本地与运行闸门仍未过；本回执已处理的报告局部问题不另派返工。
---

# 第22批限定接收；目标有值、动作已发生和函数返回须分别证明

## 1. 裁定与可复用成果

按PASS_PENDING_LOCAL接受，强制合读R22-S01/S02/S03。原六件冻结；以下替代口径由主线追加，不要求C03b另跑文档返工，也不表示原始每条标注无条件通过。

交付评论与实际执行头为b32f3ebc9095。compare 16baace7bd3c到该头为1个后继提交、恰user-vma-merge-22/六件A新增3463行：MANIFEST 58、正文126、facts 2038、DOT 125、deferred 195、evidence 921；无旧件净修改或删除。主线完整阅读六件，未冒称重新运行执行方94/94引文检查及12次双重回读。

核心结论可保留：初次prev由严格vm_end<addr产生，稳定且未改写时不满足前合并的addr==prev->vm_end；next由end的点包含查询产生，不能自动认定邻接；只后合并且prev非NULL时adjust_target缺少赋值；页偏移公式与摘链资源缺口分别追加14旧根因；第二次合并的临时新VMA和被合并/摘除的旧对象必须分开。前序初始NULL迭代缺口仍限制端到端可达性，不能靠有效有序链表消除。

## 2. R22-S01：完整分支前提与目标来源

### 条件矩阵不得漏掉另一侧匹配

M07的实际premise只写prev存在、next的flags不相容，却给出merge_prev=false、最终NULL。应补“前驱相邻/相容门也未通过”；否则前驱仍可能已经匹配，target为prev，不能从后继失败推出整次merge返回NULL。M07标题与premise的prev/next不一致，按具体两侧判据消费，不按标题推断。

M09中的“next或未初始化，取决于prev”不是完整目标分类。若merge_prev已经成立，target是prev，即使next并非严格邻接也不改变这个已赋值来源。统一分类：两侧均未匹配时在读取target前返回NULL；前侧匹配（含双侧）时target=prev；仅后侧匹配且prev==NULL时target=next；仅后侧匹配且prev!=NULL时，target在后续比较前未赋值，停止确定的后续写入/返回推演。

[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
		adjust_target = prev;
		merge_prev = true;
```
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
		if (!prev)
			adjust_target = next;
		merge_next = true;
```

已定义的两种目标赋值都来自非NULL的匹配对象。因此，在所读函数、无未定义行为/外部破坏的局部模型中，后面的“if(adjust_target==NULL) adjust_target=curr”不是第三条已证明可用的正常兜底路径。E38与正文对该句只能作为有条件文本依赖，不能用curr存在来修补未初始化读取。仅后匹配/非空prev的VM22-F01保留为条件问题，不预言某个具体错误地址或崩溃。

E31/X04等若以merge_prev=true说明动作，应理解为该分支结果，而非赋值发生前该布尔量已为true；实际顺序是写target再置布尔。M01/M02的“无变化”仅限所选对象副作用，不抹去pglen和局部变量计算。

### 第二次合并和直接调用模型分开

E13及M30/X14还须控制流实际越过共享映射登记、地址检查等先行门，不能只凭回调返回0、flags改变、prev非NULL断言第二次调用发生。

稳定首次生产路径的严格prev结论，同样不能在第二次调用时被悄悄撤掉：若prev仍未变且回调后起点仍为原addr，则前侧相邻门仍不成立；第二次调用又要求prev非NULL，故其“成功返回旧目标后的清理”只能作为以返回已成立为前提的下游描述，而非已经证明正常生产链能进入的成功路径。M30可保留此接口模型，但真实抵达需要额外生产/变化证据，不能借执行注释补全。

## 3. R22-S02：未返回不等于没有先行副作用

X07把字段写入的when与“normal return”并列，必须收窄到各写语句实际抵达。__simple_vma_adjust先写target字段，随后才进入后继遍历/摘链；后面的帮助函数不返回，不意味着前面字段被自动恢复。

[VERIFIED mykernel/mm/vm_map/mmap.c::__simple_vma_adjust]
```c
	vma->vm_start = start;
	vma->vm_pgoff += (start - vma->vm_pgoff) >> PAGE_SHIFT;
	vma->vm_end = end;
```

对计数也不能合并成一项原子效果：
[VERIFIED mykernel/mm/vm_map/util.c::__vma_unlink_from_list]
```c
	while (vma != NULL && !list_header_contains(&mm->mm_mt, &vma->list));
	mm->map_count--;
	list_header_delete_node(&mm->mm_mt, &vma->list);
```
[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
			}
```

map_count递减在嵌套删除前，mm_mt.count递减在list_del_init返回后。后者的__list_del_entry_valid还有检查循环；仅通过最外层membership，不等于所有指针操作和计数都完成。E48/X10已有正确的先后描述应保留，M23/X11/removed_successor等摘要不得将其简化成“只要成员存在两个计数必然同时减少”。

这里不把原文“不能保证全部完成”误读为它断言完全未写；补充的是分阶段状态账。只对前缀有效且抵达的源码动作作条件说明；出现未定义行为时不从源码顺序签发确定机器内存状态，也不声称已观察到损坏或泄漏。

二次合并清理中的fput、vm_area_free、vma=merge同样有先后门：后一个动作以此前调用正常完成为前提。fput是减少临时对象所持文件引用，只有计数等条件成立才进入文件释放；它不是无条件释放那个file对象。临时VMA的free调用不能给被摘旧后继补齐释放证据；kmem_cache_free的全分配器回收效果仍不在本批认证范围。

## 4. R22-S03：补真实调用锚点，保留临时/旧对象区别

E17/X15及M28/M29/M31关于simple_mmap_region→simple_vma_link，原A039止于匿名分支，A089只证明被调函数体；补实际调用点：
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	simple_vma_link(mm, vma, prev);
	/* Once vma denies write, undo our temporary denial count */
unmap_writable:
```

E22的find_vma_intersection→find_vma_prev_to_addr，原A055/A056/A057/A058组合缺少真正调用句；使用已回核固定vm_map.h中的：
[VERIFIED mykernel/mm/vm_map/vm_map.h::find_vma_intersection]
```c
			vma_s *vma = find_vma_prev_to_addr(mm, start);
```

A039标题中的“新VMA入链”不能由那一片段单独支持；补文后可保留该事实。引文文字存在、被调函数有定义，均不等于原调用锚点已充分。

E10、E56虽在meaning里说明新对象，端点N52/N50仍叫target.vm_file/target.vm_page_prot。消费图时将它们限定为新建临时VMA的字段，不能连成对旧adjust_target字段的写入。对应旧目标只按其真实保留/调整动作解释；同一展示节点不得证明两个对象相同。

E59消费的比较实参是调用者计算的pgoff+pglen，合读A021，而非只有原始requested_pgoff；且需前面的相容检查允许右侧比较求值。E40/E41的目标是局部vma_start/vma_end：读取prev->vm_start或next->vm_end赋给局部值，不是此时写回候选字段。字段写入发生在adjust中；DOT同ID亦采用这些限定。其余未逐项复核的边不因ID一致而自动获得语义认证。

## 5. 风险保留及同轮后继

3条本批条件候选和VL14-F01/F02/F03/F05四组历史追加保留身份与状态，不合计为七个新增已复现故障，也不声称已完成全仓根因去重。VM22-F03缺少字段比较只是待证相容性维度，不自动等于必须增加该比较。报告修正不是内核修复，Owner当前无需作修复决定。

第23批CORE-USER-EXEC-STACK-23转向exec用户栈VMA的建立、setup_arg_pages的地址/权限准备、expand_stack以及缺页入口如何消费这些对象。以12已有MF12-F01（点包含查询限制grow-down入口）等结论为边界，不重做12或22。主线仅为设计定点读了相关源码，尚未完成23的上游生产与调用时序取证。

仅在现有lead新增本回执、42号任务与检查点，再同步两PR入口。继续阶段2、18号延期、28号同轮派发、36号C03b分工及PR17 Draft。无内核/用户程序修改、补丁、构建运行、合并关闭、删分支、强推或阶段3授权。
