---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXCEPTION-ENTRY-65
record_type: bounded_exception_entry_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (self-reported)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
source_pin_short12: a039d9803ade
execution_base_short12: 16817a5be4d1
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第65批：异常入口、寄存器帧、参数分发与有条件返回

本报告以固定 time@a039d9803ade 的源码为唯一源码基线。汇编、C结构、已存在的生成头和宏来源是四类可直接核对的文本；编译后的ELF、IDTR、实际CPU压栈、GS状态、CR2、调度和用户指令恢复均未测。凡需Intel/AMD手册才能严格认证的硬件细节，本次未获得独立原始规范核证，一律只作明确前提的模型，不能记为VERIFIED硬件行为。

## D00 — 第64批R64-01追加纠正，不回写旧物

固定 common.c 的 cpu_init 于L597开始、L639结束；arch_cpu_finalize_init 于L643开始。原64 evidence.md 的C23引文恰为L689–693，其中 *c、initialized以及alternative_instructions调用都只是注释，真正位于后一个函数。新的D01为正确定义入口，D02原样带两处物理空行，D03证明前一个函数已闭合。另一实际所读候选位于bugs.c::check_bugs，alternative_instructions也是注释（D04）。

原64的MANIFEST、正文C、facts.source_ledger/source_context/alternative_activation_matrix、UR64-F06、DOT cpu节点和PR17旧评论#6738378936凡称“两处为check_bugs与cpu_init”都应改按“check_bugs与arch_cpu_finalize_init”消费。facts.prior_return_disposition逐项列明替代字段，所有历史文件与评论原封不动。本结论不否定其它未读代码可能存在patcher，也未对cpu_init以外的启动顺序作运行证明。64旧“66/66”是作者申报的标签总量，C23已被主线认定不合P2定义归属，其余65条并非本批自动复核；不能据旧全通过口径推导P9已过。

D00状态为documentation_disposition_submitted_pending_C05，R64-01仍待独立主线决议。63另有独立闭合回执，R63-01空行指控已由主线撤回，63按PASS_PENDING_LOCAL_WITH_R63C-S01消费；58 B25不重新打开。

## G01/A — IDT向量源和异常stub

A01显示静态 idt_table 和 idt_descr，size=IDT_TABLE_SIZE-1且address指向idt_table。A02–A04显示set_intr_gate对每个vector先init_idt_data，再idt_init_desc和write_idt_entry。三者以及load_idt的真实选定直接定义，在processor/desc_arch.h：init_idt_data把vector、addr、__KERNEL_CS、GATE_INTERRUPT及present放入id_t_data；idt_init_desc分拆偏移低中高字段并复制门属性；write_idt_entry调用memcpy到idt[entry]；load_idt包含lidt汇编文本（A15–A20）。这里区分了源级构造、表内存写入和CPU加载请求；没有因为源有lidt就声称硬件IDTR已装载。实现头的编译配置门与链接选择未运行。

早期idt_setup_early_handler遍历NUM_EXCEPTION_VECTORS=32，按idt_handler_array + IDT_ALIGN*i选入口地址并请求装载。后续idt_setup_apic_and_irq_gates重新为0–31写异常门，为FIRST_EXTERNAL_VECTOR=0x20起的中断向量选择irq_entries_start + IDT_ALIGN*(i-0x20)，再请求装载；注释的CEA映射和表只读设置并非活动步骤（A05–A14）。IDT_ALIGN的本源表达式为8*(2+HAS_KERNEL_IBT)，HAS_KERNEL_IBT=0，即16的数值槽距；这是汇编位置生成模型，不是实际指令字节长度已验证。

GP向量13、PF向量14由trapnr和myos_irq_vectors两处直接常量交叉约束（A23–A25）。EXCEPTION_ERRCODE_MASK=0x20027d00，其13、14位在本轮纯整数位运算中均为1。汇编宏区间idt_handler_array在每个vector依据该mask决定是否以pushq $0补无硬件错误码的槽，再pushq $vector、跳sa_entintr_retp；因GP/PF两个配置位为1，二者在此源条件下不补0，而假设CPU已经提供一槽错误码（A26–A30）。.fill是填至下一个IDT_ALIGN槽的汇编填充指令，不是可执行路径的第二个异常处理函数。该mask与所有CPU异常的真实错误码规范是否完全一致，以及最终ELF/CPU是否按此配置响应，仍未认证。

## G02/B — 栈槽位、reg_t、生成偏移与实际C参数

PUSH_REGS按rdi、rsi、rdx、rcx、rax、r8、r9、r10、r11、rbx、rbp、r12、r13、r14、r15依次压栈，故最后栈顶是r15；POP_REGS按反向顺序恢复（B01–B05）。假设64位pushq每次一个8字节槽，GP/PF的硬件错误码与RIP/CS/RFLAGS/RSP/SS返回尾帧均按预期存在，且同一栈未改变，软件vector位于offset120、错误码位于128、后续ip136/cs144/flags152/sp160/ss168。pt_regs_s字段的声明次序与reg_t typedef __UINT64_TYPE__支持此源级模型；已被汇编真正include的generated/asm-offsets.h写有对应数字，后附kbuild/asm-offsets_64.c对应生成表达（B06–B14）。这是22个槽×8=176字节的**文本一致性核对**，不是生成器重新运行，也不是C编译器offsetof或真实ABI实测。

|从PUSH_REGS结束后的rsp算起|保存字段|偏移bytes|来源层|
|---|---|---:|---|
|起点|r15|0|软件PUSH|
|最后的通用寄存器|di|112|软件PUSH|
|异常编号|irq_nr|120|每向量stub的软件push|
|异常错误码|orig_ax|128|#PF/#GP硬件错误码假设；其它按mask条件补0|
|返回指令|ip|136|假设的CPU尾帧|
|代码段|cs|144|假设的CPU尾帧|
|标志位|flags|152|假设的CPU尾帧|
|栈指针|sp|160|假设的CPU尾帧|
|栈段|ss|168|假设的CPU尾帧|

完整22槽逐项offset及来源标签在facts.entry_frame_layout_matrix。原汇编入口先cli、PUSH_REGS，再以CS(%rsp)&3判断是否swapgs，然后cld、mov %rsp,%rdi、sti、callq excep_hwint_context（B15–B16）。rdi成为指向r15的C寄存器帧指针，绝非vector立即值。C函数取regs->irq_nr为vec；vec<0x20转exception_handler，否则转硬件IRQ处理（B17–B19）。switch中GEN_PROT_VEC=13调用excep_gen_prot(regs)，PAGE_FAULT_VEC=14调用exc_page_fault(regs, regs->orig_ax)（B20）。GP处理函数诊断同样从regs->orig_ax读取error_code，因此这两个分支读取的是同一个语义槽，不允许人为造出两个互不相干的错误参数。

真正的PF地址读取在fault.c::exc_page_fault内的read_cr2之后才传递到handle_page_fault（B21–B23）。其fault_in_kernel_space(address)把地址划为高地址和其它地址，两者与前面“vector为14而非13”、用户CS、X86_PF_USER错误位是独立判别，不能混淆。

**有界风险：CR2的保存时机。** 源级确定的顺序是entry_64.S L315 sti，随后call excep_hwint_context、exception_handler的PF分支，最后fault.c L418 read_cr2。假设在这段窗口内真实发生并处理了另一次会写CR2的异常、且外层CR2未被其他机制提前保全，则外层读取可能不是原地址。但仅据“sti在前”不能推出CPU真的会递送嵌套#PF、IF必开、CR2已经覆盖或发生系统卡死。中断屏蔽与栈转换的具体CPU规范、本次外部手册与真实运行均未验证；EE65-F01为条件候选，不是已复现缺陷。

## G03/C — 条件swapgs、可能schedule、栈恢复和iretq

公共入口在C调用前已经检查一次CS；C调用返回后**重新**以当前帧的CS(%rsp)&3判断是否执行第二次swapgs（C01）。若C callee改写CS、切换到另一帧或根本不返回，不能无条件把两次条件swapgs认定为必然配对，也不据此断言GS已经泄露。随后源码为cli、POP_REGS、addq $0x10,%rsp、sti、iretq（C02）：addq明确跳过vector/error两个8字节槽，要求此前的帧不被破坏。执行到iretq只是源码层面的目标；CPU真正接受帧以及错误指令可重执行另外需要硬件与MM条件。

excep_hwint_context在exception_handler正常返回后还有if(!in_atomic()) schedule()，实际in_atomic宏只测试preempt_count()!=0（C03–C04）。因此不是“每次异常必调度”，而是**可能**调用opaque调度器；不能保证该调用正常返回时仍为同一任务与同一寄存器帧，亦不能以此推论完整任务切换错误。

excep_gen_prot的活动体读取orig_ax、执行诊断并进入while(1)（C05–C06）。当前限定分支如果诊断全部有效并到达该处，则本地不正常返回到公共iretq；但不是实机已出现死锁。PF高地址do_kern_addr_fault的所读活动体也到while(1)（C07），低地址do_user_addr_fault则在有效mm/VMA前提下调用myos_handle_mm_fault一次；非零结果在while(fault!=ENOERR)里不重新求值，正常不能越过，零值可能越过却不证明PTE已恢复（C08–C09）。必须继续沿用MF12-F01/F02/F03/F04与R12-S01/S02的原条件限定。user_mode仅从保存CS低2位判断，不替代地址范围或缺页错误代码判断（C10）。

返回前sti与iretq文本相邻，可能存在需要CPU中断屏蔽/中断影子规则确认的状态边界；未核官方硬件规范，不能认定真正发生了中断插入。恢复至少分四层：1.C处理函数正常返回；2.汇编到达iretq；3.CPU接受保存帧并恢复；4.原错误指令重执行成功。第1、第2层仅能在上述校验条件下构造源码路径；第3、第4层全部NOT_RUN。

## 裁定与方法边界

A、B、C是completed_in_scope，即所限定源码和条件反例均有证据，但并非代码正确性认证；D00只供C05独立验收。6项EE65-Fxx是条件候选、8项EE65-Uxx是缺口，没有一项被声称为真实运行漏洞。18份固定time完整Blob的读取用于定位真实直接定义、生成偏移、类型和历史纠正，不做整个中断、NMI/IST、syscall、调度、signals、全部页表或全部外部设备的审计。纯数值模型22例仅是当前对话内的简单布尔和整数计算，不运行仓库代码。

本地完整P9、预处理、编译、链接、旧夹具、Graphviz、QEMU、实机、CPU异常、ELF、IDTR、GS或PTE确认全部NOT_RUN。六件提交后只申报READY_FOR_MAINLINE_REVIEW / mainline_acceptance=PENDING；第63批已闭合不触碰；R64-01不自关；第65批不自签PASS、不开始第66批。发布PR17本人正式评论后STOP_WRITING。
