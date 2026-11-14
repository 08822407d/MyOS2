---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-UACCESS-RECOVERY-64-REVIEW-001
record_type: bounded_static_review_with_definition_attribution_return
evidence_class: 固定六件阅读、直接回源与增量核对；不是内核运行裁定
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（Owner此前报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-09
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract94_short12: aeefb9025a55
reviewed_base_short12: 51477ed51ee2
reviewed_head_short12: 16817a5be4d1
reviewed_delivery_comment: 6738378936
acceptance_verdict: RETURN_DOCUMENTATION
returned_scope: [R64-01]
subjects: {D00: ACCEPTED_WITH_MAINLINE_RETRACTION, A: CORE_RETAINED_WITH_LIMITS, B: CORE_RETAINED_WITH_LIMITS, C: CORE_RETAINED_PENDING_ATTRIBUTION_CORRECTION}
mandatory_supplements: [R63C-S01, R64-S01, R64-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original66_full_automatic_byte_definition_scan: NOT_RUN
original_facts_deferred_JSON_parse: NOT_RUN
reviewer_finite_model_cases: 36
self_check: {verified_claims: 2, quotes_reconfirmed: 2, downgraded_to_inferred: 0}
next_followup: CORE-USER-EXCEPTION-ENTRY-65
next_executor: MYOS2-A-C05b / MYOS2-LEAD-002-CHAT-EXEC-08
inputs_read: [94号, 64完整六件, 评论6738378936, 63回执及D00原件, 本文所列固定源码, 00及63号规则, 87交接模板]
open_questions: []
---

# 第64批审查：63异议成立；64仅退定义归属文档项

## 1. 交付、边界和分别裁定

compare 51477ed51ee2→16817a5be4d1为ahead1/behind0、单个提交，只新增授权六件3074行：MANIFEST78、正文84、facts1817、evidence692、DOT135、deferred268；旧件零净改删。PR16/直接lead为aeefb9025a55，PR17/执行头16817a5be4d1；两PR Open/unmerged，17 Draft。direct time a039d9803ade、master e8e60cd19540；缓存base_sha不代替直接ref。
六件及6738378936全文已读。作者声明STOP_WRITING；Owner本轮明确下一批换新对话，不再派给C05a。隐含后台型号、会话物理状态和全部历史工具调用不予认证。
D00对63三条空行指控的异议成立，R63-01由另附63-DOC-CLOSURE回执闭合，63组合材料PASS_PENDING_LOCAL。64本身C23存在独立的P2定义归属错误，故64为RETURN_DOCUMENTATION；不把63闭合与64新退回捆成一个状态，不要求整批重做。

## 2. R64-01：C23的真正定义及传播修正

固定common.c对象cae051b123c8，完整读cpu_init和其后arch_cpu_finalize_init的边界。cpu_init在load_direct_gdt(cpu)后结束；C23所引的*c、initialized和alternative_instructions注释在后一个函数里，不在cpu_init内。字面存在并不满足标签定义归属。

真实定义入口：
[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::arch_cpu_finalize_init]
```c
void __init arch_cpu_finalize_init(void)
{
	// struct cpuinfo_x86 *c = this_cpu_ptr(&cpu_info);
```

真实所属函数中的相关连续片段：
[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::arch_cpu_finalize_init]
```c
	// *c = boot_cpu_data;
	// c->initialized = true;

	// alternative_instructions();

```

唯一退回项R64-01要求新65的D00追加：C23标签归属改为arch_cpu_finalize_init；提供该定义入口及注释调用的独立固定源锚；列出MANIFEST、正文C节、facts.source_ledger/source_context/alternative_activation_matrix、UR64-F06、DOT cpu节点及正式评论6738378936中相同误归属的替代读法。旧件/旧评论一字不改。
“两处所读启动函数的alternative调用均为注释”的技术观察可保留，但两处应为check_bugs和arch_cpu_finalize_init，不能继续称cpu_init。这不证明其它路径永久无patcher，亦不证明两函数实际启动可达或硬件状态。
原66/66及P9相关全通过申报不能继续作为该定义闸门合格的证据；66是作者标签数。本轮明确至少C23不合格，其余65条并不自动成为主线全量通过。新D00只计实际新锚，不复跑整个64，不自关R64-01。

## 3. 技术主干及R64-S01

A：两个_ASM_EXTABLE_TYPE分支都描述对齐后的三个long字段；from与to位移分别以各自字段地址为基点。copy_user_generic中的标签与_UA注记只给源级生产关系。EX_TYPE_UACCESS数值、最终ELF和搜索/修正消费者未建立，不据符号存在推恢复成功。
B：fault_in_kernel_space采用TASK_SIZE_MAX阈值；它与x86 access_ok数值域、user_mode(regs)、错误位是不同输入。所读高地址分支是while(1)，低地址分支的myos_handle_mm_fault正常非零返回后比较同一fault而未重新调用；零返回不证明映射恢复。沿用MF12-F01/F02/F03/F04及R12限定，不重新登记旧根因。
C：OLDINSTR、ALTINSTR_ENTRY、替换节和stac/clac是候选及元数据源；14字节名义表布局不是最终二进制证明。未找到rep_movs_alternative定义和活动patcher的结论只限查询/读取范围。C原型uint不自动约束内联asm的寄存器call。

强制消费限定：
1. options_flags.cmake明确有set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections")。facts/UR64-F02所谓GC flags unknown收窄为最终链接命令、版本、覆盖和实际保留效果未知，不是源码选项未见；脚本未显式列节仍不能推出一定丢弃。
2. 高地址行的user_flag_if_user_route仅是假设进入低地址分支时的条件值；真实选择do_kern_addr_fault的模型不能同时认定执行了do_user_addr_fault中的user_mode/置位。
3. do_kern_addr_fault本地活动体没有regs成员读取；有效入口是上游前提，不能把无效regs画成该函数内一个已定位的访问。myos_bad_area则确有诊断分配/访问/打印前缀，必须先满足它们才能推后续循环。
4. B-CTRL-6的invalid CR2 read只作为未认证环境假设，不是已读read_cr2接口会返回某种“错误地址”的证据。真实异常类别、CR2、入口与恢复帧仍未认证。
5. padding=max(new-old,0)是显式数学投影；没有运行汇编器，不把C/Python布尔值规则直接代入源字符串中的汇编比较表达式。候选的机器字节长度和最终填充仍未知。
6. 所读函数“未见活动查表”并不否定可能由MM正常处理后返回；也不能越过坏对象/不返回callee推后继。七条UR64是条件细化，不是七项已复现新漏洞；八项缺口及UC63-U03/U04均保留。

## 4. R64-S02：图、方法和实际覆盖

DOT全文阅读，作者60节点63边的结构数本轮未独立机械重算、未运行Graphviz或给图重建哈希。cpu节点按R64-01归一为arch_cpu_finalize_init。test到两分支是handle_page_fault内部控制，不是fault_in_kernel_space直接call它们；bad→diagnostic是聚合节点，不是名为diagnostic的真实函数。ANALYSIS_STOP是证据边界，不自动等同程序停止。
36个主线自建有限Python投影实际满足断言：相对字段8、地址/权限组合18、fault局部控制6、抽象padding4；不是作者55例重放。另做63三个原引文的连续性/行数核查；readdir.c全文完整Git对象身份吻合。没有据作者P6数冒称全量66引文字节/定义通过。
固定新读：readdir.c全文和原63三锚；asm.h全文；fault.c1–240及241至末尾；common.c520–650及630至末尾覆盖两个完整定义；bugs.c全文；options_flags全文。uaccess_64.h、alternative.h相关宏、smap.h、page_const_arch.h、target_kernel.cmake、kernel.lds和12回执/原条目复用本有效C05已读同pin原文。
64facts按1–300/301–620/621–940/941–1260/1261–1580/1581至EOF1817完整读；evidence1–340/341至EOF692；其余四件及评论全文。原facts/deferred全文JSON机械解析、原66全量字节/定义扫描、完整P9、Graphviz、仓库C/ASM/脚本/夹具、预处理/编译/链接、QEMU/实机均NOT_RUN。

## 5. 后继与轮换

Owner确认C05a上下文过长，65必须面向全新C05b/CHAT-EXEC-08；旧07维持STOP_WRITING，不要求旧对话再制作材料。C05仅追加本轮主线记录与96接收包，不写执行成果。
65合并D00定义归属纠正及A异常向量入口/IDT绑定、B保存帧与C参数来源、C正常返回与调度/寄存器状态边界。预读固定idt.c、entry_64.S、myos/interrupt.c和ptrace.h已提供具体范围；这不是预签65或扩大为全异常/调度器审计。
