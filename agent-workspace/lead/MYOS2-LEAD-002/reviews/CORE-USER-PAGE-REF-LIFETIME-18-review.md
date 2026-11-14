---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PAGE-REF-LIFETIME-18-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-01
read_channel: connector
authorization_ref: Owner报告18已完成并提交；28号验收与下一工作包同轮持续授权
reviewed_pr: 17
reviewed_head_short12: 15a162f13089
reviewed_base_short12: b7f89a7bc5c0
contract_short12: 3f1cefc573ac
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-02
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: c171b3b5e63d
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R18-S01, R18-S02, R18-S03]
mainline_additional_observation: R18-N01
acceptance_scope: 普通order-0页的选定初始化、出队、引用操作及两种释放接口的静态证据；须合读本回执
kernel_reference_or_allocator_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_84_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-ANON-ZERO-19
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/38-user-anon-zero-contract.md
inputs_read:
  - PR17交付评论issuecomment-6557543761、固定15a162f13089六件全文；evidence 1–797分三段，facts 1–1356分四段，未把截断当全文。
  - 固定time buddy_types.h 1–300；double_list.h 1–300和612至末尾；double_list_macro.h 1–210。
  - 固定time mm_init.c 1至末尾；buddy.h 240至末尾；page-flags.h 440–535；page-flags_const.h全文；buddy.c 230–330。
  - 固定time atomic_arch.h 90至末尾；memblock.c 1–200和600至末尾，1000之后查询为空并未据此声称文件为空。
  - buddy.c其余主体、buddy.h 1–240、mempolicy.c、highmem_macro.h、page-flags宏、options_flags.cmake等，沿用本对话先前实际读取的同一固定对象，不声称本轮重新读取全部原文件。
  - 05主线回执1–125含MR-02结论重新定向读取；37号、17回执与18/24/25/28/36规则沿用已加载版本。
  - 为19设计补读固定time fault.c 1–91和770–960，zero_pfn默认分支搜索仅作导航，不作为固定源码结论或19已完成证据。
  - PR16/17、直接heads、开放PR第三项为空、PR16完整变更路径和18增量compare。
review_limits:
  - 全文阅读和有界语义回核，不是执行方84/84逐字检查、JSON/YAML解析或全部50图边的独立机械复现。
  - 未运行命令、文本程序、仓库脚本、哈希、预处理、C/ASM、编译、内核/QEMU、硬件或并发实验。
  - 没有认证所有PFN的启动可达性、原子指令的实机执行、所有引用/页类型写者、全部free调用者或高阶合并正确性。
  - 同厂分角色审查不等于异族或人类技术验收；RS-01、GAP-U04硬件/运行部分、GAP-U10及普通延期项不关闭。
---

# 第18批限定接收：引用、成员身份、控制流完成分别记账

## 1. 交付与裁定

第18批以PASS_PENDING_LOCAL接收，无新的执行退回；本回执R18-S01/S02/S03直接给出必要补充和替代口径。原六件保留，不要求单独返工，不修改任何源码。

被审头15a162f13089与C03b交付评论一致。远端compare b7f89a7bc5c0→15a162f13089为1个后继提交、仅user-page-ref-lifetime-18/六件A新增，共2498行，无旧件净修改/删除。六件为MANIFEST 43、正文73、facts 1356、DOT 101、deferred 128、evidence 797行。PR17仍open/Draft。

执行方84个锚点、44节点/50边、13场景、16效果、4项新增条件问题与3项历史证据追加，以及双重远端回读，保留为其自查记录。主线读取六件并定点核对关键源码，不冒称重新执行全部检查。对已接受资料的消费必须合读本回执，不能把检查数或PASS_PENDING_LOCAL解释成物理页生命周期已经正确。

## 2. 可保留的核心结论

page描述符不同于物理数据内容；lru/buddy_list/pcp_list共用存储，compound_head与该union首部重叠；page_type/_mapcount共用另一union，_refcount独立。folio结构有page_s覆盖成员。本轮从真实类型体核对，不能仅凭单个字段声明推出所有布局或运行别名正确。

alloc_node_mem_map的实际新申请分支包含描述符memset；free_area_init在所读路径中先alloc_node_mem_map、free_area_init_core，再memmap_init。__init_single_page设置引用1，memblock_free_pages逐页设置0，__rmqueue_smallest对获得的order0页执行加1。只能在相应路径真实抵达、对象有效且没有别的写者时建立0→1或1→0；普遍复用关系仍为n→n+1，算术亦要求数值不溢出且无其他并发修改。

[VERIFIED mykernel/mm/page_alloc/buddy.c::__rmqueue_smallest]
```c
		del_page_from_free_list(page, area);
		expand(zone, page, order, current_order);
		for (int i = 0; i < (1 << order); i++)
			atomic_inc(&(page[i]._refcount));
```

get_page/put_page经page_folio转换对象，folio_get和folio_put对相应引用字段操作。folio_put归零进入当前空主体__folio_put；直接__free_pages进入另一条__free_pages_ok路径，不先检查或递减引用。这支持“计数归零不等于已入空闲链”，不支持全系统永不回收或已经发生双重释放。

所读分配出队先list_header_remove_head，再调用带contains门的list_header_delete_node；在一致链表、首删正常返回且没有并发插回时，第二次不实施删除。PageBuddy位测试与链表成员身份仍不同；以零初值推导的假值场景不能改写为全局恒假。

## 3. R18-S01：到达调用、完成入链和对象身份的必要前提

### 入链不只要求page/zone/order有效

正文L03“只要zone、page和order有效……会将块投入free_area”，以及M02/M10/M11/M12、E14/E15/E19、X05/X13，统一收窄为：还须控制流实际抵达并完成链表助手，链头/计数/相邻关系和插入节点满足接口条件，锁操作正常返回且无不受保护的修改。指针有效不等于链表结构一致。

[VERIFIED mykernel/lib/list/double_list.h::__list_add_valid]
```c
			while ((prev == NULL) || (next == NULL) ||
					(next->prev != prev) || (prev->next != next) ||
					(new == prev || new == next));
```

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_head]
```c
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```

校验有活动忙等，header count增加位于下层调用返回之后。失效状态不自动走到入链或解锁，也不能统一预测某一种硬件终态。contains同样沿anchor遍历，不能仅凭count数值证明遍历安全；M03的二次不删结论保留其一致性/无竞态前提。

合并循环确未显式摘除已识别buddy；PL18-F01的条件风险保留。但combined_pfn可能改变page变量的对象身份：若较低伙伴成为合并头，后续set_buddy_order/add_to_free_list消费的不是原传入页头。不能把两个头的_refcount或buddy_list当作同一变量始终指向同一对象。可能的旧节点复用、验证不返回、链表受破坏或重叠条目须按实际条件分开，不能把“识别buddy”直接等同“成功建立两个合法重叠空闲条目”。

PL18-F02/M10中正引用值本身不足以判定调用者错误。需要另证存在不允许直接归还的仍存活持有者，或违反明确接口契约；独占所有者主动结束分配并非仅凭n>0就已证明错误调用。本轮未穷尽这些调用者。

### 初始投放仍不是全启动通过

memblock_free_all中的先前保留区初始化、范围选择与实际执行前提仍须保留。新增有限观察R18-N01：本轮所读函数声明局部pages后未见赋初值即累加，故也不能依据其后有memblock_free_pages调用就保证该函数正常完成。

[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
	ulong pages;
```
[VERIFIED mykernel/mm/early/memblock.c::memblock_free_all]
```c
		pages += end_pfn - start_pfn;
```

R18-N01（全限定ID CORE-USER-PAGE-REF-LIFETIME-18-REVIEW-001::R18-N01）仅登记为deferred_owner_not_ready的局部未初始化读取观察；不是本轮新增修复任务，也未认定实际优化结果、启动失败或运行后果。18的条件化初值证据仍可使用，不能升级为所有页都成功入池。

## 4. R18-S02：调用条件、实参转换及类型证据修正

E33的expand调用条件应为“已获得候选且控制流抵达”；它在current_order等于请求order时也被调用，只是内部while(high>low)不执行。将调用发生与拆块副作用的条件分开，覆盖DOT同ID和正文摘要。

E37/E41不是page_folio内部调用folio_get/folio_put的串行调用链。get_page直接调用folio_get，put_page直接调用folio_put；page_folio/_compound_head是这些调用的实参求值路径。

[VERIFIED mykernel/mm/page_alloc/buddy.h::get_page]
```c
		get_page(page_s *page) {
			folio_get(page_folio(page));
		}
```

替代图读法：分别表示get_page→folio_get、get_page→page_folio/_compound_head及参数转换关系；put_page同理。旧call_path可作表达式求值顺序提示，但不得作为“转换宏调用了get/put”的call graph证据。E36只能表示包装对返回page描述符的转换依赖/结果，不能把返回folio当作预先存在的输入提供者。E47–E49只作结构包含/别名关系，不当运行调用或引用增加。

A04/A05的独立声明切片不足以单独证明union，E49也不能仅靠_refcount声明及page_folio证明folio布局；本轮已经回读buddy_types.h的struct page/struct folio完整相关结构，以上布局事实可按该源体合读，不要求重绘全图。

M11的“type bit set”不应解成PG_buddy置1；__SetPageBuddy实际清该位，BASE高位另有前提。page_type与_mapcount是同一存储，按位修改前者也改变后者的位表示；“未调用mapcount reset”不能解释成数值保持不变。

## 5. R18-S03：完整原子语句与风险/历史证据等级

A72只有asm开头、A73只有条件码/返回，单独合用仍漏掉decl。补以下真实语句，支撑本批减一/测零的接口解读；不认证机器实际执行或全局同步。

[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h::arch_atomic_dec_test_zero]
```c
			asm volatile(LOCK_PREFIX
							"decl	%0				\n\t"
						:	"+m"(v->counter) ,
							"=@cc" "e"(c)
```

计数示例是有效且稳定对象的条件算术，不是实际trace；数值范围、别名、并发、合法持有和接口正常抵达前提不可省。free路径有zone锁调用、分配路径未见对称取得与zone_init_internals中初始化注释，可保持needs_evidence；不能仅凭缺一条spin_lock_init认定零初始化锁非法或已死锁。

4项执行方新记录与3项追加保留原ID及限定，不全部改称已证实缺陷。MR-02是04审查回执中的文档裁定项，05回执明确为CLOSED_BY_EXPLICIT_NARROWING_NOT_GLOBAL_PROOF；原追加字段的简称仅指该文档关闭，不表示buddy技术问题被关闭。后继引用应指向CORE-MM-BASELINE-04-REVIEW-001的MR-02及CORE-USER-VFS-BASELINE-05-REVIEW-001 §2，而不是把MR-02当原源码问题ID。

## 6. 同轮下一工作包

第19批CORE-USER-ANON-ZERO-19检查尚未完整审查的匿名缺PTE生产侧：do_pte_missing的实际匿名判定、do_anonymous_page的读零页/写私有页分支、zero_pfn来源、PTE保护/安装及引用和返回边界。17负责已有PTE写保护，18负责普通页生命周期；19衔接这些已核端点，不重做17/18或扩成全MM/全启动/硬件隔离审计。

主线只新增本回执、38号任务及恢复检查点，再同步PR导航和评论；执行分支不改。保持阶段2、18号延期、28号同轮派发、36号C03b分工和PR17 Draft；不修内核、不生成补丁、不编译运行、不合并关闭/删除强推或进入阶段3。Owner无需现在作技术修复决定。
