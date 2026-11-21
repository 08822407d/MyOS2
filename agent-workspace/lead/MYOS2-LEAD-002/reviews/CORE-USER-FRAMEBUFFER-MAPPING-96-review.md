---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FRAMEBUFFER-MAPPING-96-REVIEW-001
record_type: independent_static_review_with_two_targeted_document_returns
evidence_class: 固定六件全文、源码回查及有限独立数学；非运行认证
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台未独立认证）
model_per_owner: Pro（既有正式验收与后继设计安排）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract135_short12: fda0f3d1cb60
reviewed_base_short12: be83c6095eb1
reviewed_head_short12: 6a1fa78d0356
reviewed_delivery_comment: 6941872312
executor: MYOS2-A-C06d / MYOS2-LEAD-002-CHAT-EXEC-13
status: REVIEW_COMPLETE_WITH_TARGETED_RETURN
acceptance_verdict: RETURN
returned_items: [R96-01, R96-02]
return_scope: DOCUMENTATION_AND_EVIDENCE_MAPPING_ONLY_NO_WHOLE_BATCH_REDO
mandatory_supplements: [R96-S01, R96-S02, R96-S03, R95-S01, R95-S02, R95-S03]
R95_01_02: CLOSED_BY_SEPARATE_95_CLOSURE_RECEIPT
batch95_composite: PASS_PENDING_LOCAL
original_author_self_check: 110/110/0
reviewer_readable_label_count: 110
self_check: {verified_claims: 7, quotes_reconfirmed: 7, downgraded_to_inferred: 0, scope: 本回执七段短锚；与原110条分账}
original_six_automatic_regex_JSON_DOT_scan: NOT_RUN
original110_automatic_definition_scan: NOT_RUN
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-BOOT-INFO-TRANSFER-97
next_contract: ../137-user-boot-info-transfer-contract.md
next_executor: MYOS2-A-C06e / MYOS2-LEAD-002-CHAT-EXEC-14
executor_rotation: OWNER_REQUESTED_NEW_CONVERSATION
phase: 2
inputs_read: [96六件与正式评论全文, 135及95审查与适用规则, 固定源码和历史边界，具体见本轮检查点]
owner_decisions_requested_now: []
open_questions: []
---

# 第96批定点补正；第95批纠正单独闭合

96裁定RETURN，仅R96-01引文定义边界和R96-02问题台账字段/引用映射。技术主体按R96-S01/S02/S03保留，不重做整包；97/D00追加纠正后由主线关闭。95的D00已达到独立关闭条件，见同轮95-DOC-CLOSURE；不因96新问题把95重新退回。

## 1. 实际交付与核验限界

初核lead=fda0f3d1cb60、exec=6a1fa78d0356、direct master=e8e60cd19540、time=a039d9803ade；PR16/17均Open/unmerged，17保持Draft。作者评论6941872312指向同一执行提交；STOP_WRITING来自Owner与交付声明，不是后台监控。

compare be83c6095eb1→6a1fa78d0356为ahead1/behind0、一个提交、严格六件added4666行：MANIFEST90、正文156、facts2893、DOT119、deferred464、evidence944，旧件零净改删。六件分段阅读全文，facts和引文长响应有重叠补齐。保留原110/110/0和全部旧文件/评论。

| 核验面 | 本轮实际判断 |
|---|---|
| P9-1/5/8/11 | 六件可读全文、110个原始标签枚举、规范三整数及等式、十四字段、公约和canary均核对；未见完整40hex或运行通过声明。没有运行原件自动扫描器。 |
| P9-2/13 | 全部110标签已读；D00、生产/消费、类型、请求区间、活动映射、返回、属性、刷新等强制链回固定源核对。E036明确越出所标结构定义；字面存在不等于定义边界通过。 |
| P9-3/4及语义索引 | 固定交付及实际源码路径可读，附带行号不是定义判据。FM96-F05含不存在的证据ID和错位字段；不能以文件可解析或总数正确代替引用正确。 |
| P9-6/9/10/12 | 外部EFI/硬件语义未补造；无代码/补丁/降级交付，相关项N/A。 |
| 本地及运行 | 全套P9、原件JSON/DOT程序解析、全110自动定义扫描、仓库实现与脚本、编译、硬件、真实并发均NOT_RUN。 |

8组55行矩阵、23有限模型、5候选/9缺口及图是不同栏目，不合计为独立测试数。27份主辅源码超过24软限的三份额外来源有直接类型/声明用途说明，不因软限机械退回。作者二读、临时检查和后台身份不由主线重新认证。

## 2. R96-01：FM96-E036越过结构定义进入独立typedef

原E036所标multiboot_tag_framebuffer_common，取了framebuffer_type、reserved、结构结束及紧随其后的独立typedef。固定源中是先定义struct，再另写typedef；不是一个typedef struct {...} alias的单一声明。P2/P9-2失败是最后一行进入另一独立定义，不是空白或花括号本身有问题。

[VERIFIED myloader/uefi/multiboot2.h::multiboot_tag_framebuffer_common]
```c
			multiboot_uint8_t framebuffer_type;
			multiboot_uint16_t reserved;
```

97/D00必须：缩为真实结构内短锚；若消费者确需别名定义，另取独立typedef锚。补旧E036→新锚→A-P06/M11/FM96-F04、producer索引及全部实际消费者映射；追加修正source_quote_index/source_quote_recheck、MANIFEST/evidence/正文的“110条全部单一定义内通过”总括。原110/110/0历史数量不改，不倒造原检查。

E030–E032各自的单一typedef别名定义不因没有函数花括号而失败；E038/E039的嵌套union仍在所标外层结构内；与E036的结构后独立声明不同。空白行不自动作退回理由，53旧误判继续撤回。

R96-01=OPEN_PENDING_EXECUTOR_APPEND_AND_C06_REVIEW；不给整包追加重做任务。

## 3. R96-02：FM96-F05的字段错位与不存在的证据ID

deferred-findings.yaml的FM96-F05实际为：distinct_root填了数字数组[35,41,44]；evidence_ids只有拼接串FM96-ECORE-USER-CONSOLE-REGISTRATION-OUTPUT-95::CN95-U05；prior_related_qualified_ids为空。该拼接串不是E001–E110中的任何条目，不能作为证据，也不是一个新的qualified_id。

固定源码支持所述输入域问题，但要用正确支持链：E035给bpp字段，E041给目标字段，E044给消费算式；其它必要别名/前提应按真实来源补齐。

[VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_framebuffer_info]
```c
	framebuffer.PixperScanline = common->framebuffer_pitch / (common->framebuffer_bpp / 8);
```

97/D00将distinct_root恢复为有意义的根因说明，evidence_ids只列实际证据ID，相关旧CN95-U05放入prior_related_qualified_ids并解释关系，保持FM96-F05原身份。对96五个候选、九个缺口及交叉索引作有界完整字段/引用核对，记录哪些修正、哪些保留；追加纠正原“全部证据引用存在”类总括。不能只删坏串而不补支持，也不把台账修正说成内核除零已修。

R96-02=OPEN_PENDING_EXECUTOR_APPEND_AND_C06_REVIEW。两项D00并入97六件，不改96原件、不加第七件，不要求Owner手工核数或选择修复。

## 4. R96-S01：算术模型必须注明阶段和输入来源

E030–E032取自__PI_UEFI_H__分支的UINT别名；核侧未选该分支时另用uint8_t/uint32_t/uint64_t。实际两支已回读，kernel types.h给对应u8/u32/u64别名。32位中间乘积/8位窄字段的数值例应保持其明确目标类型前提，不用UEFI一支的短锚单独认证kernel实际编译选择。

A-K04/M07的800×600、bpp32、832像素行距是隔离的已给定bpp模型：pitch3328、PixperScanline832、按宽度算1920000与按pitch算1996800可保留。不能直接当fill_framebuffer_info在所有这些上游条件下的完整输出：若另给FrameBufferSize=1996800且所有整数运算可表示，该loader公式得到bpp33，pitch3432，核侧PixperScanline858、FB_size1980000。主线独立整数算术只表明上游输入改变模型结果，不证明实际机器采用该模式，也不另报CN95同根因。

16384×8192×32在声明的u32乘积模型中先回绕0，数学字节预算536870912；目标uint64不倒推中间运算已扩宽。零分母、字段窄化、tag可读性和真实硬件分别判断。

正文损坏的公式排版统一读为B=P*floor(A/P)，S=P*ceil(L/P)，o=A-B；这些无限整数式仅在实际运算可表示时使用。正长度请求完整包含目标的条件为o+L<=S。非对齐不必然缺尾，既有额外映射也不是本请求的保证。

B-EXT06里的[B,0]应是实参二元组(start=B,size=0)，不是区间终点0。对本调用页对齐B、合法转换及无相关回绕，得到空请求[B,B)，不是反向范围；M17的start==end表述可保留。根函数初始值和循环门为：

[VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	paddr_last = paddr_end;
	vaddr = (ulong)__va(paddr_start);
	vaddr_end = (ulong)__va(paddr_end);
```

相等端点时正常跳过根遍历，返回该初值；后继flush可以发生，但不证明新增映射。M18无符号回绕本身不是CPU陷阱；只能停止把它解释为所期望物理区间，后续代码是否执行还须具体控制流。

## 5. R96-S02：固定参数分支、返回与保护属性

[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_ioremap]
```c
	return (void *)__kernel_physical_mapping_init(paddr_start,
			PFN_PHYS(PFN_UP(paddr_start + size)),
				1 << PG_LEVEL_4K, PAGE_KERNEL_NOENC, true);
```

C-IO03/05、C-PUB03及图PUD/PMD是通用分支图；对本次稳定的4K掩码，不含1G/2M位，已到达范围内的旧大页不能走“掩码允许则直接复用大页”的分支，而要按真实降级路径判断。已有非叶下钻、已有PTE跳过及范围外条目是另外情形。不能由此宣称整张页表已全变4K或所有旧保护均被请求属性替换。

NOENC与NOCACHE的源常量区别不只PWT/PCD。本树活动NOCACHE表达式还没有NOENC基础式中的NX位：

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::__PAGE_KERNEL_NOCACHE]
```c
	#define __PAGE_KERNEL_NOCACHE		(__PP|__RW|   0|___A|	0|___D|   0|___G| __NC)
```

这只是原始编码选择，默认/支持掩码可能再改变位，旧项也可能保留；不认证PAT/MTRR或UC/WC/WB。C-PUB04须合完整check_pgprot、massage_pgprot及present条件，不从调用名补算法。

paddr_last是底层物理端点记账，不是映射覆盖并集、字节成功数或虚拟起点；混合新旧PTE时按R83-S03消费。M21的(void*)8192是声明目标数值解释的例，不把整数转指针认证成真实可用地址。VBE忽略返回并沿framebuffer.FB_virbase取地址；HPET对照实际保存返回后供访问包装使用：

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_readl]
```c
inline uint hpet_readl(uint a) {
	return readl(hpet_virt_address + a);
}
```

这个直接消费者补证支持FM96-F03/U08，但未审HPET全生命周期或硬件。iounmap空体没有因此获得释放功能。

## 6. R96-S03：刷新、图和历史身份

[VERIFIED mykernel/arch/x86_64/mm/mmu/mmu_arch.c::flush_tlb_local]
```c
	write_cr3(__read_cr3());
```

本轮回读__read_cr3/write_cr3/load_cr3及__flush_tlb_all；当前实现后者调用local。C-PUB07“两个时间点和根身份不同”须收窄：两个调用时间点不同，根是否不同未证，也可能相同。不能由函数名推出全CPU或全局项失效，更不由刷新请求证明页表对象可写。

mode→mb_tag图边是前后服务状态的数据关联；两个函数各自LocateProtocol，不能认定它们的C局部Mode指针就是同一对象身份。register→console_unlock仅新节点路径；重复对象提前返回须保留。其它压缩SOURCE_CHAIN不改标直接CALL，旧E045到prot只作纠正文档关联，不是尺寸宏生产保护位。

原83台账已回查：TI83-F03为大页端点问题，TI83-F06为PTE先行指针/范围，TI83-F08为leaf/prot传递；一般返回覆盖关系另见TI83-U08和R83-S03，不能把所有返回问题归成F03的一种已证发生。其它旧ID以原条目及其正式回执为准，短说明不重新定义历史根因。

FM96-U09是95纠正待签的流程依赖，已由同轮95闭合件解除，不是一个已修内核缺口。其余FM96/CN95/BM80/PT81/DM82/TI83、PK94、SP93、KN91等保持真实前提与原身份。

## 7. 后继及未执行项

97先D00补本两项，再研究引导信息构造与长度记账、入口保存与定长复制、存储寿命与解析视图三专题。Owner指定换新对话：C06e/14先独立接收、九规则、110/124、单独指导和G00，随后连续执行；13停止后继业务写入，无需旧方确认。

本轮仅用独立Python计算4666行加总、padding例的整数关系、u32回绕和数学预算，没有翻译执行源码或作者模型。七段本回执短锚已对固定原文可读核对；全套P9、编译、运行和硬件仍NOT_RUN。角色独立不是人类逐字或异族认证，发布97不等于新会话已启动。
