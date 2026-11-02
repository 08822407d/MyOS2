---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ELF-STACK-TABLES-25-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
evidence_class: 固定六件全文阅读、关键源码语义复核与GitHub增量核对；非运行或全量机械认证
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告25于897c3a935aa8交付并要求按44号独立验收；28号同轮安排后继持续约定
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 897c3a935aa8
reviewed_base_short12: c0f4d6a603f8
contract_short12: f8c0b05a1864
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-03
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 799a39b36e47
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R25-S01, R25-S02, R25-S03]
acceptance_scope: create_elf_tables所选布局、auxv容量及用户写入/返回接口的条件性静态资料；须合读本回执
kernel_stack_or_uaccess_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_all_62_quote_checks: false
reviewer_parsed_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-ELF-SEGMENT-LOAD-26
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/46-user-elf-segment-load-contract.md
inputs_read:
  - 最新交付评论issuecomment-6572042091；PR16/17元数据、直接heads、开放PR第三项为空、PR16完整变更路径及c0f4d6a603f8到897c3a935aa8的compare。
  - 44号固定任务全文分1–80/81–130读取；45号身份/写权、24回执及28号持续约定沿用本对话此前完整读取的固定材料，不冒称本轮重新加载全部指导。
  - 897c3a935aa8六件全文：MANIFEST、deferred和DOT完整返回；正文长响应后用63–85补尾；facts1–220/221–440；evidence1–220/221–440/441–650。
  - 本轮先读固定time options_flags.cmake全文；binfmt_elf.c1–315、315–460、615–685、790–865、960–1145，其中elf_map/elf_load部分亦用于26范围预检。
  - 本轮固定time linux/kernel/uaccess.h1–225，asm/uaccess.h1–220，asm/uaccess_64.h1–210至末尾，strnlen_user.c全文及mmap_lock.h120–168。
  - 本轮固定time mm_misc_const.h全文、mm_misc_types.h1–140、asm/elf.h1–255、linux/kernel/elf.h全文、uapi/linux/elf.h1–85、asm-generic/param.h全文。
  - 本轮固定time exec.c1310–1378及process64_arch.c240–315；arch_align_stack和setup_arg_pages已读的同一固定源码沿用，不算本轮全量重读。
  - 05与07主线回执定向读取用于后继范围核对；不重审旧批或全仓去重。
review_limits:
  - 未机械复跑62段引文、47条图边、全部结构/算术检查或双分支12次回读；执行者自查保持其原归属。
  - 未执行命令、文本程序、解析器、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU或硬件。
  - 未认证全部构建注入宏、C指针对象语义、异常表装配/运行恢复、用户参数字节、CPU寄存器或外部libc兼容性。
  - 同厂分角色复核不是异族或人类技术验收；RS-01、GAP-U04硬件/运行部分、GAP-U10及普通延期项保持。
open_questions:
  - 未初始化vma及23/24上游返回/页有效性缺口仍阻断端到端成功结论。
  - C03c接收报告和指导加载完成情况有MANIFEST自报，主线未读取其私人聊天或沙箱进行独立环境认证。
---

# 第25批限定接收；布局、写入接口与真实可达性分开

## 1. 交付身份与裁定

以PASS_PENDING_LOCAL接收，无新的执行退回。原件须合读R25-S01/S02/S03；这些主线补充直接替代相应不完整读法，不修改PR17原文件，不要求另跑小型文档返工。该接收不代表原47条关系全部语义通过，更不代表内核已经运行正确。

交付评论及实际执行头一致为897c3a935aa8；compare从c0f4d6a603f8到该头恰1个后继提交，仅user-elf-stack-tables-25/六件A新增1483行：MANIFEST98、正文80、facts423、DOT93、deferred159、evidence630；没有历史净修改/删除。主线完整阅读六件并回源核关键函数、宏、类型、返回和调用顺序。62段引文及其他机械检查是执行方记录，未改写成主线重新运行。

C03c/CHAT-EXEC-03完成首次正式分析交付，已在评论声明STOP_WRITING；其接收/指导记录只能按MANIFEST所报范围消费，不把它视为主线独立访问了新聊天。继续由C03验收和派工，无须新开执行对话。

可保留主体：create_elf_tables中的vma未初始化，原查找赋值被注释；字符串字节、用户指针表、内核saved_auxv与CPU栈状态是不同层。所选宏组合支持12个非终止auxv条目（have_execfd时13个），加终止对后208/224字节，低于46个8字节元素的368字节数组。put_user为直接存储后返回0；strnlen_user的活动体为strnlen+1；AT_ENTRY与最终解释器入口可不同。上述均有固定源码依据，但运行和全配置边界不关闭。

## 2. R25-S01：真正返回来源、未定义门和接口保证

**M02及正文的-EINTR分支仅保留为调用者文本分支。** 本轮再次核到mmap_write_lock_killable活动返回0，原down_write_killable和return ret是注释；mmap_write_unlock也没有活动up_write。固定实现正常完成时不能产生这里假定的非零锁错误，也不能依据函数名宣称已取得排他锁。

[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_lock_killable]
```c
		// return ret;
		return 0;
```

vma读前无活动赋值的ET25-F01保留。正文“规避此未定义门”只能表示单独分析后续代码片段，不是一个已经存在、能把未定义值变成有效VMA的生产路径。任何下游return0、argc/argv写入或START_THREAD结论均不能回接成完整exec成功证明。门之前的写入只登记源顺序及有效前缀条件，不推定含未定义行为的实际编译执行必然保留全部先行写入。

**copy_to_user的0是接口成功信号，不是本轮对底层异常恢复的认证。** 已回读copy_to_user→_inline_copy_to_user→raw_copy_to_user→copy_user_generic。包装在access_ok通过时使用raw返回值，否则保留原n；raw转发到含ALTERNATIVE、stac/clac和_ASM_EXTABLE_UA的底层。未验证这些机制的实际选择、异常分发和恢复，不能仅凭包装return n就证明所有故障都会转换成可靠剩余字节数。access_ok的数值检查也不是页已映射、权限正确或源/目标可访问的证明。

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::raw_copy_to_user]
```c
	raw_copy_to_user(void __user *dst, const void *src, ulong size) {
		return copy_user_generic((__force void *)dst, src, size);
	}
```

对正常可访问对象、底层复制契约成立的局部模型，可继续按剩余字节消费；不把假想非零结果误认成已实测部分复制。put_user使用sp++时，所选标量elf_addr_t的typeof不再求值一次，赋值表达式正常完成的局部模型中自增一次；不推广到所有宏实参类型或无效地址。strnlen_user范围论断仍以实际strnlen返回满足其长度契约、对象有效且转换无溢出为前提；本轮未将底层字符串/异常机制全部展开。

## 3. R25-S02：两段对齐间隙、容量与地址值的消费边界

正文布局表和facts.stack_layout漏列了**第一次arch_align_stack的间隙[p1,p0)**。它不同于最终STACK_ROUND在auxv尾与平台字符串之间留下的间隙。保留原公式，但低到高应是：argc/argv/envp/auxv → 最终表对齐间隙 → base/platform字节 → 首次对齐间隙 → 从p0开始的既有字符串区（后者仍需上游生产条件）。

原算例中最终表对齐间隙为9字节；首次对齐另有0x7fffffffeff0至0x7fffffffeff8的8字节。原padding_bytes=9只指前一段，不能读成全部空隙；两段合计17字节。源码本函数未专门清零这些间隙，不等于已经证明里面含有旧秘密或形成泄露。地址均是纸面整数模型，不认证相关指针转换/指针算术、对象覆盖和实际页内容具有完整C/硬件保证。

items实际是int表达式，argc/envc非负还不够；各加法必须可表示，相关地址减法/乘法不回绕，并与所选8字节单位一致。有效模型下原表底、auxv位置和208/224字节计算可保留。argc==0不能无条件当作本选定成功exec生产状态：do_execveat_common有空字符串补入并设argc=1的分支，只有另证生产者才讨论真实零argc到达表构造。

auxv容量结论只对已列宏与宽度假设成立，谓词N<=22+AT_VECTOR_SIZE_ARCH包含为AT_NULL预留一对。CONFIG_RSEQ未在已读显式选项中开启、arch头中的相关定义是注释，不等于穷尽外部构建注入。ET25-F04保留为**配置变更时的容量不变量/预防性记录**，不是当前已证越界，不与另外三项一起包装成四个实测漏洞。

AT_PHDR的数值来源还须保留未命中与范围前提：load_elf_binary初始化phdr_addr=0，所选PT_LOAD条件只测试e_phoff起点落入文件段区间；命中才赋值，随后统一加load_bias。不命中时不能称其必指向实际程序头表，也不能从起点命中推导整个表都在映射内。这里不重审ELF完整输入验证；26将定向续查相关段接口。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	ulong		load_addr,
				load_bias = 0,
				phdr_addr = 0;
```

AT_ENTRY取主程序e_entry与START_THREAD取最终elf_entry的区分保留。regs->sp赋值还要求此前segment帮助调用实际返回、regs有效且控制流抵达，不能因展开了start_thread就宣布CPU已采用该栈。

## 4. R25-S03：补调用/赋值证据，修正DOT关系

A02标题包含envc而片段未展示保存envc；A36是vma门及注释，并不含argc写入。A25未覆盖PHNUM，A38未覆盖argv终止/arg_end更新。补固定源的实际语句，避免用标题代替证据：

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(argc, sp++))
		return -EFAULT;
```
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_PHNUM, exec->e_phnum);
```
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(0, sp++))
		return -EFAULT;
	mm->arg_end = p;
```

A44的默认ARCH容量需合读mm_misc_const.h的ifndef/define 0；A62的NULL兜底需合读#ifndef ELF_BASE_PLATFORM。两者是条件定义，不是所有构建的无条件值。setup_new_exec的调用也由本轮load_elf_binary真实语句补核，不能仅凭A05/A06两端标签自动推出中间动作。

DOT原件保留；以下替代读法约束同义摘要，不把它当已经无误的直接调用图：

| 原关系/标签 | 必须采用的读法 |
|---|---|
| exec→argbytes标call | copy_strings是被调函数；argbytes是其条件写入目标/结果，不能被函数调用。补实际函数节点或标明经copy_strings的折叠效果关系。 |
| localp→align标call | 实际调用者是create_elf_tables；localp是实参，返回值被赋给局部p。 |
| platform→platptr | u_platform在STACK_ALLOC时先计算，随后copy_to_user读取该指针作为目的地址；不是copy返回了平台地址。 |
| saved→auxnull标write | saved_auxv是被清尾的内核缓冲，真实动作是create_elf_tables调用memset；传入起点的数据依赖与memset写入方向分别表示。 |
| tables→_inline_copy_to_user标直接call | 源码先调用copy_to_user包装，再由其调用_inline_copy_to_user；这是折叠链，不能省略包装却仍称直接调用。 |
| sp→bprmp、bprmp→mmsp标write | 前者/后者是取值来源，实际字段写入者分别为create_elf_tables和load_elf_binary。 |
| copy→useraux标uncopied byte count | 剩余长度是数值结果，用户auxv是目标内存；分开结果消费与条件字节写入，不能共用一个对象身份。 |
| len节点为strnlen_user(str,count)+1 | +1在strnlen_user内部；调用者不再加一次。改读为strnlen_user→strnlen(str,count)+1，避免长度双加。 |
| put→argv同时标argc | argc单槽与argv指针槽是两个布局类别，不能把argc当argv指针。 |

其余sequence边仅为源顺序说明，不证明跨未定义门的真实可达性。34节点/47边端点一致不能代替以上语义检查；未逐条独立机械核查的部分仍限界使用。

## 5. 后继选择及停止线

本轮不启动修复，ET25-F01–F04及旧23/24依赖保持原编号和延期状态，未完成全仓去重。C03c当前累计1个正式分析工作包、6件1483行；继续该对话，不据行数推测剩余token，也不重复接收。

下一工作包选择CORE-USER-ELF-SEGMENT-LOAD-26：定向核主程序PT_LOAD到elf_load/elf_map的文件映射、页尾清零、匿名扩展及错误消费。相比扩大为全系统uaccess/异常审计，该方向能在已冻结的mmap/VMA接口基础上补齐25尚未证明的装载前提；只读预检不等于26已经完成。05已有exec基线及13/14/20/21接口结论须按主题合读，不重做整批。

C03仅新增lead回执/任务/检查点并同步PR导航，不写执行分支。保持阶段2、原PR16/17与PR17 Draft；无内核/用户程序修改、补丁、构建运行、合并关闭、删改名/强推分支或阶段3授权。
