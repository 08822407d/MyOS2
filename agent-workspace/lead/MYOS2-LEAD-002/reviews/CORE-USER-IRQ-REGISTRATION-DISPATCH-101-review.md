---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-IRQ-REGISTRATION-DISPATCH-101-REVIEW-001
record_type: independent_static_review_with_targeted_document_return
evidence_class: 固定六件全文、105条字面回查、定义资格与有界语义核查；非运行认证
conversation_display_name: MYOS2-A-C07 内核分析主线续建
produced_by: GPT-6 Astra Pro（当前会话模型标识；后台不可独立认证）
model_per_owner: Pro（沿用Owner本主线选择）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
contract144_short12: 0cd745b92280
reviewed_base_short12: dd07c3b19fa1
reviewed_head_short12: 14e8f1580f5a
reviewed_delivery_comment: 6976123017
executor: MYOS2-A-C06e / MYOS2-LEAD-002-CHAT-EXEC-14
status: REVIEW_COMPLETE_WITH_TARGETED_RETURN
acceptance_verdict: RETURN
returned_items: [R101-01]
return_scope: DOCUMENTATION_QUALIFICATION_AND_MANDATORY_MACRO_CHAIN_ONLY
mandatory_supplements: [R101-S01, R101-S02, R101-S03, R65-S01, R65-S02, R65C-S01]
R100_01: CLOSED_BY_SEPARATE_100_CLOSURE_RECEIPT
batch100_composite: PASS_PENDING_LOCAL
original_author_self_check: 105/105/0
reviewer_literal_quote_matches: 105
reviewer_definition_qualification: FAIL_SEE_R101_01
self_check: {verified_claims: 5, quotes_reconfirmed: 5, downgraded_to_inferred: 0, scope: 本回执Q1–Q5；与作者105条分账}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-HPET-ENABLE-CLOCKSOURCE-102
next_contract: ../146-user-hpet-enable-clocksource-contract.md
next_executor: MYOS2-A-C07a / MYOS2-LEAD-002-CHAT-EXEC-15
executor_rotation: OWNER_REQUESTED_NEW_CONVERSATION
phase: 2
inputs_read: [144全文、101六件及正式评论全文、22份锚点源文件的对应定义范围、D00原100证据与消费者、65和96后置回执、旧HPET身份原始条目及限制]
owner_decisions_requested_now: []
open_questions: []
---

# 第101批完成独立核收；R101-01定点文档补正

执行成果已完整写入GitHub，执行者停在READY_FOR_MAINLINE_REVIEW是正确的作者终态。本C07现在裁定RETURN，仅退回证据资格和强制宏链补正；技术主干按R101-S01/S02/S03保留。第102批D00追加纠正，不重做101整包，也不要求上下文已耗尽的旧C06e继续。

第100批R100-01由同轮独立闭合回执关闭，组合PASS_PENDING_LOCAL；101新文档项不会反向否定已经核实的100纠正。

## 1. 固定交付与可读核验

[VRF] 固定14e8f1580f5a为单父提交，父dd07c3b19fa1；compare为ahead1/behind0、一个提交、严格六件added共4754行，旧文件零净修改/删除。以下目录为agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-irq-registration-dispatch-101/：

| 文件 | 行数 | Git blob短SHA |
|---|---:|---|
| MANIFEST.md | 156 | a943e439b0a1 |
| irq-registration-dispatch.md | 82 | 08bb166afb14 |
| facts-and-boundaries.yaml | 2949 | 289025caefd9 |
| irq-registration-dispatch-paths.dot | 233 | 772073bfc5d2 |
| deferred-findings.yaml | 411 | c79e844bc1f9 |
| evidence.md | 923 | db62e13ebfac |

短SHA均自连接器真实输出复制。六件固定全文与活动全文/blob一致；本轮直接exec头仍为14e8f1580f5a。此前存储核查与本次语义核收在同一C07内复用同对象，不冒充另一个模型复审。

全六件原始文本计数为105，均在evidence；40位hex命中0。facts/deferred为JSON兼容YAML，纯JSON解析成功；七组矩阵55行、21模型、8候选、9缺口均已阅读全文。DOT全文233行，纯文本登记99个唯一节点、124条边、0未定义端点；没有运行Graphviz。所有消费者出现的105个证据ID均存在。计数、可解析和编号存在不代替语义支持。

重新取回固定time的22份锚点源文件，对105条短引文逐条比较：105/105字面命中，均为连续1–5行，辅助行号和索引匹配；下面P2资格问题仍成立。不是“105条全部合格”。

[VRF] 正式作者评论6976123017全文已读，created/updated相同；marker为MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-IRQ-REGISTRATION-DISPATCH-101 / FORMAL_DELIVERY。最新评论查询since=2026-11-18T09:15:32Z、per_page=100、page=1，实际返回6974193742/6976123017两条，作者marker一次；不宣称遍历全部历史。作者评论中的旧完整SHA不被混入严格六件的P9-1计数；新文件和后续评论只写真实短SHA，旧评论冻结。MANIFEST的发布前PENDING_G04由实际后置评论消费，不因该冻结占位另造缺件。

## 2. R101-01：定义体边界与LVT向量链

判据来自协议v2 P2/P9-2和144§7，未新增证据规则。

### A. 一条纯参数头与十九条跨函数头短锚

IR101-E017的175–179行全部是register_irq参数列表，函数体起于181行。它确实逐字存在于实际定义的声明部分，不能说它是虚构函数或独立原型；但引文没有任何函数体内容，不能满足P2“定义体内部”的门。

同次统一核查列出以下另外19条混入函数头的短锚。它们多数含有效体内语句，保留相应源码事实；D00只须换成同一函数体内部连续1–5行并保持命题支持，不把这19条算成19项内核故障。

| 旧证据 | 实际符号 |
|---|---|
| E017、E022、E026 | register_irq、unregister_irq、hwint_irq_handler |
| E032、E034、E036、E037 | IOAPIC_enable、IOAPIC_disable、IOAPIC_install、IOAPIC_uninstall |
| E038、E040、E041、E045、E048 | IOAPIC_level_ack、IOAPIC_edge_ack、ioapic_rte_read、ioapic_rte_write、IOAPIC_init |
| E044 | apic_msr_write |
| E051、E061 | myos_init_bsp_intr、myos_init_percpu_intr |
| E065、E067 | HPET_handler、myos_HPET_init |
| E076、E078、E081 | LVT_timer_handler、LVT_timer_init、LVT_timer_ack |

表中均为IR101-E前缀，共20个处置ID。声明参数若确需解释，可作为非证据上下文单独展示，不能再计为函数体锚。不是因空白、体内注释、合法嵌套结构或闭括号本身返工。

### B. 强制独立宏缺最后一层

E084给LAPIC_LVT_TIMER_IRQ→VECTOR_IPI(2)，E085给VECTOR_IPI→FIRST_SYSTEM_VECTOR，E062给FIRST_SYSTEM_VECTOR→LOCAL_TIMER_VECTOR；作者105条中缺LOCAL_TIMER_VECTOR本身的独立定义锚。B-V03、C-S07、M19及正文/图却使用0xec/0xee数值结论，144已明确要求实际向量链中的独立宏。

本回执Q2独立补证源码值确为0xec，说明数值方向可保留；主线补证不等于作者原件在交付时已经满足强制锚。102必须自己取得新宏锚并对齐完整链和所有真实消费者，不用主线Q2替代本人工作。

### C. D00闭合条件

在102六件中给出原101 source_ref、supersedes路径、上述20个旧ID逐条处置/新锚/消费者，以及缺失宏的新ID；撤回原“105条全部定义内合格/强制齐全”的总括。保持旧105/105/0与六件/旧评论原样，不机械改成85或126；102按自己的最终原始文本独立计数。

同时在消费者中落实下述S01/S02/S03，涉及原行的限定只做追加映射。D00终态CORRECTION_SUBMITTED_PENDING_C07_REVIEW，由下一轮C07独立关闭R101-01；执行者不得自签。

## 3. 保留的技术主干与强制消费限定

A的描述符字段发布、回调调用顺序与结果未消费；B的向量到索引窄化、RTE读写顺序和局部entry所有权；C的HPET/LVT注册与handler/ack接合，均可作为有条件源码分析消费。不能整体升级成实际IRQ安装、送达、SMP安全或正确返回认证。

### R101-S01：索引与回调后的当前对象

A-L02的irq>=256需要细分：256形成one-past地址与随后成员访问，和大于256时指针运算本身的合法性，不是同一步。只标最早失去正常C语义的步骤，不把源码止证画成CPU必然陷阱。

Q3/Q4表明，外层controller非空检查发生在install/disable之前，随后又读取当前p->controller的enable/uninstall。即使第一个回调正常返回，也须另保证当前controller仍有效、非空且其字段可读；回调可改变描述符的条件模型必须消费这一门。不要从“正常返回”单独推出必然继续enable。对M05/M06及对应回调表作追加限定；handler后重读controller的M07/M08同理。没有真实重入/并发观测，不宣布实际竞态。

注册return 1与控制器成功分开；handler为空与是否仍可ack分开；静态存储对象存在与真实启动清零/可达分开。

### R101-S02：IOAPIC类型、请求次序和合法读取

index+1先按整数提升求值，再经uint8_t目标存储发生窄化；M17的256→0是存储转换，不能说加法表达式本身在8位中溢出。RTE读高低、写低高是多个源码访问请求；mb不是互斥锁，udelay合法返回及有效映射分别是前提。

Q5补全IOAPIC_map三个指针字段，virt_EOI_addr为uint32_t指针；原E012未覆盖这一字段，E049写地址/强转不能单独证明结构声明。102在相关消费处保留此支持或自己取得对应字段锚，不另重做IOAPIC整包。

HPET局部位域对象未赋字段、packed布局/对齐/别名与unsigned long读取合法性，仍是推导完整RTE值之前的门；LVT对象重解释同样有条件。普通指针访问、屏障和源码常量不等于已证明MMIO观测或设备接受。两个半字写请求不认证原子路由更新。

### R101-S03：历史身份与返回边界

本轮定点重读L/07-core-audit-map.yaml的CA-05和固定0d62c4d19711的R/core/MANIFEST.md、R/core/recheck-01/consumer-fix-02/results.yaml相关条目，合读CORE-CHECK-01-recheck-02-review及CORE-SCHED-INTEGRATION-03-review§2/§3/§5。旧HPET/jiffies根身份为MYOS2-LEAD-002::CA-05，原局部案例MYOS2-LEAD-002-CORE-CHECK-01::V14；不是CORE-SCHED-INTEGRATION-03::IR-01。后者是旧NR6/idle账目的文档项，不得改名合并。

101 IR101-U05未编造根ID，原来留缺可理解；现由主线补齐身份入口，后继保留“链接别名成立且handler正常走完”的前提及真实ELF/IRQ/频率未证。F08关于hpet_enable返回未消费接到102，重遇旧根因不计新发现。

LVT有符号计数须先保证递增/显示的可表示域，显示调用不是定时频率证明。in_atomic为假只支持源码可请求schedule，不证明发生上下文切换或成功iret。R65-S01/S02、R65C-S01以及R100-S02/S03继续约束入口、布局、调用和硬件边界。

## 4. P9可读子集

| 项 | 本轮结论 |
|---|---|
| 1 | 六件40hex命中0；PASS |
| 2 | 105条字面及行范围核对成功；函数体资格FAIL，见R101-01 |
| 3 | 22个锚点源路径及D00 supersedes实际取得；输入读账与实际文件对应；不认证作者内部读取过程 |
| 4 | 105条辅助行号有效；PASS |
| 5 | 原始105与105/105/0等式相符；不代表定义资格通过 |
| 6 | 没有据未取版本手册签硬件结论；外部硬件认证NOT_DONE |
| 7 | time/master的options_flags与panic双金丝雀各自回源且不同；PASS |
| 8 | 公约整句、read_channel与必备MANIFEST字段核对；PASS |
| 9/10 | 无代码改动件/补丁，N/A |
| 11 | 未将静态/JSON/图文本核对写成编译、运行或硬件测试通过；PASS |
| 12 | GitHub正式交付，非聊天降级件；N/A |
| 13 | LOCAL_TIMER_VECTOR独立宏及相关P2资格未齐；FAIL |

完整本地P9仍NOT_RUN；纯JavaScript字符串比较、JSON解析和图引用检查不是运行仓库实现、脚本或本地验证器。未运行预处理、构建、链接、Graphviz、QEMU/实机或真实并发。同族局限：本裁定由GPT对话对GPT对话成果作出，不构成异族独立复核或后台型号认证。

## 5. 主线五条限定短锚

Q1在全文读完options_flags后记录配置命令，作为本回执所有有限否定的配置前置；否定只限定所读快照与所读函数体，不扩大成全树不存在。

### Q1 配置前置

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

此为第二条配置追加set命令内部片段；首条通用编译flags命令另在39–51行。配置声明不是实际预处理或编译结果。 辅助行号58–61，固定time短SHA a039d9803ade；行号不替代符号定位。

### Q2 LVT向量末端宏

[VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::LOCAL_TIMER_VECTOR]
```c
	#define LOCAL_TIMER_VECTOR				0xec
```

只支持该宏字面值；完整向量链的选入与实际IDT/硬件行为另有门。 辅助行号109–109，固定time短SHA a039d9803ade；行号不替代符号定位。

### Q3 install之后再读controller

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
		if (p->controller->install != NULL)
			p->controller->install(irq, arg);
		if (p->controller->enable != NULL)
			p->controller->enable(irq);
```

正常返回后仍读取当前p->controller；完整外层条件在同函数188–189行，不能把旧非空检查当作回调后的稳定性证明。 辅助行号190–193，固定time短SHA a039d9803ade；行号不替代符号定位。

### Q4 disable之后再读controller

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::unregister_irq]
```c
		if (p->controller->disable != NULL)
			p->controller->disable(irq);
		if (p->controller->uninstall != NULL)
			p->controller->uninstall(irq);
```

同样需要回调后当前controller有效且非空；此短段只证源码次序和字段再读。 辅助行号204–207，固定time短SHA a039d9803ade；行号不替代符号定位。

### Q5 IOAPIC指针字段

[VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::IOAPIC_map]
```c
		uint8_t *	virt_idx_addr;
		uint32_t *	virt_data_addr;
		uint32_t *	virt_EOI_addr;
```

三个字段均为实际结构体内部声明，支持访问宽度的源类型区分；不证明指向有效MMIO。 辅助行号24–26，固定time短SHA a039d9803ade；行号不替代符号定位。


## 6. 后继与写权

Owner明确下一批在新开对话执行。新C07a/CHAT-EXEC-15按145三件独立接收、规则与适用指导加载、G00后执行146；旧C06e/14保持STOP_WRITING且退出后续执行写者，无需它再确认。C07继续独占lead/PR16及主线导航，15仅102六件及本人PR17正式评论。第102批未因本回执自动启动，第103批未授权。
