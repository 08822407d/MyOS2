---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-SMP-BOOT-PATH-100-REVIEW-001
record_type: independent_bounded_static_review_with_documentation_return
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式独立验收；本轮不设计后继批次）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
contract142_short12: 928c77c6bbb4
reviewed_base_short12: 3df2ee58f100
reviewed_head_short12: dd07c3b19fa1
reviewed_delivery_comment: 6971992040
executor: MYOS2-A-C06e / MYOS2-LEAD-002-CHAT-EXEC-14
status: RETURN
acceptance_verdict: RETURN
returned_items: [R100-01]
return_scope: DOCUMENTATION_ONLY_NO_WHOLE_BATCH_REDO
technical_core: RETAINED_WITH_R100_S01_S02_S03_PENDING_DOCUMENTATION_CLOSURE
mandatory_supplements: [R100-S01, R100-S02, R100-S03, R99-S01, R99-S02, R99-S03, R97-S01, R97-S02, R97-S03, R86-S01, R86-S02, R86-S03, R87-S01, R87-S02, R87-S03]
R99_01: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_RECEIPT
batch99_composite_verdict: PASS_PENDING_LOCAL
original_author_self_check: 101/101/0
original_raw_prefix_count: 101
literal_quotes_reconfirmed_by_mainline: 101
single_command_anchor_failure: SP100-E063
nonliteral_selector_corrections: [SP100-E032, SP100-E061, SP100-E062, SP100-E069, SP100-E070]
self_check: {verified_claims: 6, quotes_reconfirmed: 6, downgraded_to_inferred: 0, scope: 本回执六条固定源短锚，与作者原101条及锚资格失败分账}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
phase: 2
next_batch101: NOT_DESIGNED_NOT_PUBLISHED_NOT_STARTED_ON_HOLD
handoff: C06_TO_C07_PREPARED_ONLY
owner_decisions_requested_now: []
open_questions: []
---

# 第100批独立审核完成：文档退回；本轮转入主线交接

100裁定RETURN，返工项R100-01限于证据单元及定位文档。技术主干按本件三项补充保留，不要求重做三个专题，不改原六件或旧评论。99的14项纠正独立成立，R99-01由同轮单独DOC-CLOSURE回执关闭，99组合结论为PASS_PENDING_LOCAL；这不使100自动通过。

Owner本轮明确要求：验收100之后先做本主线新老对话交接，新对话接收成功后再由Owner要求设计下一批。因此本轮不设计、发布、派发或启动101；不借28号或旧142结尾越过这个暂停门。原C06e/14保持Owner申报的STOP_WRITING，不唤醒执行者处理本件。

## 1. 远端结构、原件身份与独立检查

作者正式评论[6971992040](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6971992040)创建及更新时间均为2026-11-18T07:24:41Z。dd07c3b19fa1唯一父为3df2ee58f100，compare ahead1/behind0、一提交，严格六件added4523行，旧文件零净修改、零删除。固定六件全文与活动执行分支全文、blob身份本轮逐件一致。

| 文件 | Git新增行数 | blob短12（连接器原样前缀） | 原始验证前缀数 |
|---|---:|---|---:|
| MANIFEST.md | 127 | 2e85dfc1f5ca | 0 |
| smp-boot-path.md | 74 | 5ba4d548cf57 | 0 |
| facts-and-boundaries.yaml | 2866 | e61f95542d90 | 0 |
| smp-boot-paths.dot | 188 | 67a0c3ea2fa0 | 0 |
| deferred-findings.yaml | 388 | dd0dfb9ac316 | 0 |
| evidence.md | 880 | 72ed64759bcb | 101 |

六件正文、D00、101个证据与对应索引、8组52行矩阵、18个有限模型、六候选九缺口和DOT均已审阅。纯JavaScript只做字符串、JSON解析和引用核对：101条引文逐字命中固定time源，均1–5行，辅助行段在统一CRLF表示后一致；原始前缀总101，完整40位十六进制0；facts/deferred可解析；所有所列证据ID有目标；DOT文本84个唯一节点、94条边、零悬空，并非运行Graphviz或完整DOT语义解析器。

MANIFEST十四字段、101/101/0、公约整句、time/master的配置与panic双金丝雀和connector通道成立。逐字命中与单定义/单命令资格分别检查，不能由前者替代后者。完整本地P9仍NOT_RUN，P9-2的下述已知失败足以按00工作令§5退回文档。PR16 Open/Ready，PR17 Open/Draft且均未合并；master与time直接头分别e8e60cd19540、a039d9803ade。

## 2. R100-01：单一声明式锚及真实定位符纠正

142§7只允许真实唯一CMake命令、链接赋值或输出节定位，禁止跨独立定义拼接。E063标为CMAKE_UNIQUE_COMMAND_BLOCK，却把KERNEL_C_SRCS和KERNEL_ASM_SRCS两条独立list命令合成一个锚。两行都是真实源码；错误在于把两条命令宣布成一个合格单元，以及MANIFEST/facts/正式评论的“101条全部满足定义/唯一命令规则”总括。

本主线Q1与Q2分别回源，不回填为作者已经分锚：

[VERIFIED mykernel/CMakeLists.txt::list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})]
```cmake
list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})
```

[VERIFIED mykernel/CMakeLists.txt::list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})]
```cmake
list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})
```

同时归一五个非原文定位别名：

| 原ID/选择符 | 固定源实际定位 | 处置 |
|---|---|---|
| E032 set(CMAKE_C_FLAGS_CONFIG) | options_flags.cmake的set(CMAKE_C_FLAGS ...)配置宏追加命令，含CONFIG_NR_CPUS=256；文件内另有初始flags set | 撤销不存在的变量名作为源码符号；明确真实命令及区别条件 |
| E061 file(GLOB_RECURSE_KC) | file(GLOB_RECURSE KERNEL_C_SRCS ...) | 按真实命令与输出变量定位 |
| E062 file(GLOB_RECURSE_KASM) | file(GLOB_RECURSE KERNEL_ASM_SRCS ...) | 同上，不造GLOB_RECURSE_KASM子命令 |
| E069/E070 .text_output | kernel.lds中的.text输出节 | .text_output只可说明是作者别名，不能当真实节名 |

这些短引文均真实，不把可读别名误归为伪造源码。未来追加纠正必须保留原101/101/0、原六件、原评论及E编号；撤回全部资格已通过的总括，逐项列处置/受影响消费者/真实定位，E063若仍需两条命令分别设新锚，并按新包全部原始文本重算自身数字。不能简单把101改为100，也不能由本回执六条主线补锚代替执行者的追加纠正和以后独立闭合。

主要消费者为B-L01/B-L04/B-L05/B-L06、B-P06及相关构建选择/链接解释；它们仍可按实际原文和本件限定消费。R100-01是报告问题，不是新增内核故障。未发纠正执行合同、未分配101范围；新主线以后在Owner解除暂停后再安排，当前不要求Owner审代码或处理数字。

## 3. R100-S01：证据支持链和读取范围

A-C05及正文的load_direct_gdt请求不能仅由E027的cpu==0分支和E028的wrmsrl行推出。实际同一函数内Q3补齐：

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
				load_direct_gdt(cpu);
```

Q3只证明调用文本；加载成功、真实GDT和GS值仍未证。A-T03的cursor推进也需要函数末尾，E007只有type/size赋值。Q4：

[VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
	return next_info_tag_addr(mb2_infotag_ptr);
```

调用正常返回、游标计算和目标对象可访问性须合读R97-S02/S03；不从return语句证明实际标签发布成功。

内核pre_init.c包含uefi/multiboot2.h，不能凭相似名称另造内核副本。固定target_kernel.cmake确有Q5：

[VERIFIED mykernel/scripts/target_kernel.cmake::target_include_directories(kernel)]
```cmake
target_include_directories(kernel PRIVATE
	${INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/../myloader
)
```

这提供myloader头文件的候选包含搜索入口，不认证实际预处理选择、宏组合或ABI。主线定点请求mykernel/include/uefi/multiboot2.h得到404，随后以上述已读真实包含目录解释来源；404不扩大为全仓无此头或缺少构建输入。loader的ASM_FILE及__PI_UEFI_H__条件继续分别保留。

作者21是“含新短锚源码数”，不自动等于全部必读依赖已完成。作者读取账没有逐件列出142要求的percpu_area.c和asm/apic.h本人读取/复用身份；本轮主线追加取得前者全文及后者所需接口上下文，明确不倒写成作者原先已经读过。percpu_area.c的pcpu_hot/current_task/idle_threads是不同源对象；asm/apic.h的apicid_to_cpunr是extern声明，不能新增为对象定义锚。后续纠正应准确补充读取账，不能借旧86/87作者的读取资格。现有PC86/PA87限定继续有效，不需重做旧整包。

## 4. R100-S02：计数、AP查表、地址和最早止证门

18个模型是条件分析，均不等于执行仓库程序或硬件观察。统一补充如下：

- M01–M04/M06/M07的0..N-1枚举只在int递增可表示、前序分配及每次循环体合法返回时成立。N=0只说明该循环无体内迭代，不证明此前simple_pcpu_setup_first_chunk已经成功。NR_CPUS=256是条件配置；fallback为1也需实际未定义CONFIG_NR_CPUS的预处理门。
- M05的超大nCores模型中，core_num的窄化先于后续CPU记录循环；真实背后对象/arena和前序调用门先行。所说“stop before final tag”必须消费为正常执行证明不能越过首个无效访问，不是代码主动检测容量并停止，也不保证先后每条语句均已执行。
- M08–M11中，链接符号的名字和0xE000赋值、C指针减法的对象域、目标映射、复制结果以及cr3_paddr与被复制低地址图像的同一性分别证明。kernel_cr3初始化0不证明使用时仍为0；本轮也未穷尽所有写者。memcpy之后写链接符号不自动证明已经复制出的另一物理位置同步更新。
- M11的0x100000123到0x123只在源值可表示、目的确为32位无符号并合法执行赋值时成立。C extern类型、ASM .long声明与实际链接存储身份不能互相替代。32位段的$cr3_paddr/间接读取和64位段的kernel_cr3操作数原样保留，不把地址、内存值和寄存器请求混称为同一值。
- M13/M14的首个候选索引为1；只有有效1..255均不匹配且此前步骤合法才讨论接下来索引256的首次无效读取。没有证明随后trap、死循环或上线。实际输入r8不因附近注释就成为已测APIC ID。
- M15/M17从l1落入AP_DEBUG还需要所述前驱、数组基址/索引有效。再次读取同一下标不保证两次取得相同值；若需要“等于刚匹配的r8”，必须额外假设该槽在两次读取间稳定。Q6展示真正的第二次读取与栈值形成：

[VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::AP_DEBUG]
```asm
	movq	(%rax, %rbx, 8),%rax
	addq	$THREAD_SIZE,	%rax
	movq	%rax,		%rsp
```

V+32768的64位模算例只说明显式数学前提下的数值；不证明V是任务指针或有32768字节有效栈。AP_DEBUG另有入口标签，不能把经l1到达的寄存器条件无条件推广给任何进入方式。M16的4096是声明算例输入，并非现场PAGE_SIZE或预处理测量。M18只说明所读局部jmp .文本，没有实际AP到达、在线握手或调度见证。

## 5. R100-S03：图、作者评论和问题身份

作者正式评论B中压缩箭头x86_64_start_kernel→myos_early_init_system→start_kernel应消费为同一上层调用者的源顺序：x86_64_start_kernel分别调用后二者，myos_early_init_system并不在本已读函数体内调用start_kernel。原DOT已经将两条DIRECT_CALL边分开，保留该区分。

DOT中的EFI数据节点、矩阵分类节点、UNPROVEN_JOIN、复制请求和硬件停止门不是实测调用链；84/94/0只证明文本引用结构。BSP所读几个完整函数未见调用独立AP copy/send函数，不外推全仓无调用者。CMake glob、链接脚本.text与输入.AP_boot.text的差异不能直接推出AP代码必被丢弃或链接必失败，真实选入对象、孤立节处理、符号和产物仍未证。

ASM数据标签的前置.balign、.org及末端标签是布局上下文；不能仅凭E098把临时栈大小、低地址放置或可写范围签成确定值。未取得版本化Intel/AMD手册，本件不解释MSR常量为已合规硬件发令，也未由BIOS日志文字证明启动来源。

六候选九缺口是收存栏目，不是十五个独立已复现故障。SP100-F01与PC86-U04、F05与PC86-U08、F06与PC86-U07/BT97-U09等有共同先门；保留原qualified_id及needs_evidence，不重复计根因。旧86文档闭合、96文档闭合、97/98限定接收均不重开。

## 6. 独立性、交接和未执行项

这是C06主线对另一执行对话成果的独立有界复核，同族模型而非异族复审或Owner人工逐字审查。作者自报读取日志、后台模型和STOP_WRITING不由主线后台认证；实际提交、文件、评论和refs则按本轮远端读取记录。主线新增支持和限制不冒充作者原有成果。

完整本地P9、CMake/预处理/汇编/编译/链接、仓库脚本/作者模型/安装/挂载/复制/清理/反汇编、Graphviz/QEMU/实机/真实并发均NOT_RUN。无源码修复、补丁、合并关闭、新分支PR、额外模型/子代理/API或Mnemosyne写入。

本轮只发布本回执、99单独闭合、检查点和143主线交接双件。先由新C07以次一档只读接收，旧C06依据真实接收回报确认写权，再由新C07回读确认；交接完成且Owner在新对话另行要求并选择Pro后，才设计下一批。当前无101合同、候选专题或执行启动词。
