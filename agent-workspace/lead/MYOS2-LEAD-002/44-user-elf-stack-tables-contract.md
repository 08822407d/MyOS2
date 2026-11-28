---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-STACK-TABLES-25
record_type: bounded_elf_initial_stack_tables_static_analysis_contract
evidence_class: 后续静态执行授权及验收条件；不是已完成的代码分析
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告24完成并要求按工作量考虑执行对话轮换；28号同轮验收和后继派发持续约定
status: READY_AFTER_C03C_HANDOFF_AND_PREFLIGHT
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
receiving_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
handoff_entry_ref: agent-workspace/lead/MYOS2-LEAD-002/45-c03b-to-c03c-executor-handoff.md
instruction_base_short12: 5836fac0267d
required_review_record: CORE-USER-ARG-PAGES-24-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ARG-PAGES-24-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: c0f4d6a603f8
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-elf-stack-tables-25/
allowed_files: [MANIFEST.md, elf-stack-tables.md, facts-and-boundaries.yaml, elf-stack-tables-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: c0f4d6a603f8及此前全部文件
writer_assignment: 仅C03c本批六件及自己的PR17进度或交付评论；正式交付后停写
activation_gate: Owner在新C03c发送45号固定交接入口；接收报告、项目规则、适用Mnemosyne指导及远端预检完成后同轮执行
old_executor_authority: C03b在24交付后停写，不获得25授权；C03a和CC02不恢复
external_execution_started: false
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
inputs_read:
  - 24固定六件及本轮24主线回执，43号与18/24/25/28/36通用边界。
  - 固定time binfmt_elf.c1–310及1040–1175，create_elf_tables及其调用/返回消费仅作范围预检。
  - 23栈VMA/地址准备与24参数页交接作为既有上游条件，不宣称完整执行路径已通过。
open_questions:
  - 栈表布局、辅助向量容量、用户写入接口及局部变量可达性由本批取证，主线预检不替代交付。
---

# 第25批：argc、argv、envp及ELF辅助向量怎样进入初始用户栈

## 1. 目标、接收与连续执行

24已按PASS_PENDING_LOCAL限定接收，必须合读R24-S01/S02/S03；不另交24勘误，不重做参数页取得。执行对话本批切换到新C03c、身份CHAT-EXEC-03，接收档位次一档、Owner选择GPT-6＋Extra。先按45号完成自己的首次接收和规则加载，不假装继承C03b上下文、沙箱或工具预检。接收通过后本44号即为已绑定任务，无需再等Owner第二次派工。

本批从create_elf_tables及所选load_elf_binary调用点展开，解释字符串已经准备这一上游条件之后，初始栈上的整数、指针数组、终止项和辅助向量如何布局、写入、更新边界以及交给最终入口。区分内核saved_auxv缓冲区、用户栈指针表、已有字符串页面、bprm.p数值与用户寄存器接口。不是完整ELF加载、外部libc兼容性认证、修复任务或真实用户态启动实验。

T00–T04连续推进；缺少证据标partial，不把文档齐全当作完成。只在权限、输入身份或不明并行写入等真实阻断时停下相应动作；普通问题登记，不逐步骤索取确认。发布文件不自动建立或唤醒对话。

## 2. T00：冻结输入和历史限定

完整读取本任务及5836fac0267d中的24主线回执。首次接收规则按45号；同一固定输入不重复整套仪式。源码time固定a039d9803ade，结果基线c0f4d6a603f8，指令版本分别登记。先读mykernel/scripts/options_flags.cmake；默认分支搜索只用于定位，结论回固定源码。

按主题定点合读23主线R23-S01/S02/S03以及24 ES23-F02/F03历史追加：参数字节与返回契约未闭合，不能在下游直接假设所有参数页面已正确安装。23只证明所选setup_arg_pages的字段/接口，不认证最终RSP；12/21的点包含和NULL迭代限制继续有效。上游未定义行为不能被“假定跨过”成一个真实端到端证明，下游可单独给有效输入的局部模型。

主要导航mykernel/fs/vfs/binfmt_elf.c、sched/forkexec/exec.c、include/linux/fs/binfmts.h、ELF类型/常量及mm.saved_auxv定义，相关架构栈对齐/ELF宏、直接put_user/copy_to_user/strnlen_user接口。只补证本任务需要的定义、绑定和返回约定；不全量加载08–24大型事实库。

## 3. T01：实际调用者、地址阶段与栈表布局

从load_elf_binary中的真实create_elf_tables调用核bprm、elf_ex、interp_load_addr、e_entry和phdr_addr的最小必要来源，区分e_entry与最终elf_entry。记录它相对setup_arg_pages、程序段映射、mm替换及最终START_THREAD的先后；只标明未展开的中间步骤，不重新审计全部ELF段装载和解释器流程。

完整读取create_elf_tables，核所有局部变量声明、初值和首次使用。特别核vma的声明、被注释的查找/扩展语句与if(!vma)之间是否存在活动赋值。若缺失，明确此处对后续确定性分析的限制；不可把未初始化值默认解释为NULL、某个已有VMA或必然通过。此前已经发生的有效前缀写入与函数成功返回分别记录，不预测未定义行为后的确定机器状态。

核bprm->p进入时、arch_align_stack后的局部p、平台字符串预留、sp、ei_index、items以及STACK_ADD/STACK_ROUND/STACK_ALLOC。每一步说明单位是字节、elf_addr_t元素还是辅助向量的成对条目，给真实类型宽度和对齐依据。原字符串区和新指针表不得重名混用；argc/envc的符号、加法和乘法条件须保留。

在正常有效输入的局部模型中给一幅带地址/大小的布局说明：argc槽、argv指针及NULL、envp指针及NULL、auxv含AT_NULL、对齐空隙及另放的平台字符串。无需真实地址实测；算例必须清楚标为纸面，不能推成整个程序能够启动。

## 4. T02：saved_auxv容量、值来源与用户副本

从mm真实结构和AT_VECTOR_SIZE等定义核saved_auxv容量、elf_addr_t宽度、NEW_AUX_ENT的两个写入及elf_info推进。逐类说明实际启用条目、条件条目、注释掉的条目和每个值的来源，至少覆盖PHDR/PHENT/PHNUM、BASE/ENTRY、PAGESZ、FLAGS、EXECFN、平台字符串指针与末尾AT_NULL。不要根据Linux通用清单补写不存在的随机数、身份或能力条目。

核memset清尾的起点和剩余长度计算、elf_info再前进两个元素、ei_index及最后copy_to_user的字节数。容量足够须由本固定宏组合与条目数量给条件证据，不能只凭数组名；若条件编译信息不够，分支列unknown，不编译或预处理补出假结果。

内核saved_auxv已写、其尾部清零、用户空间副本复制成功以及用户程序正确解释是不同断言。指针型auxv值必须与实际字符串/地址生产者匹配；保留失败前已经写入内核缓冲区的状态，不凭最后返回错误假设全部回滚。

## 5. T03：用户写入、字符串消费、返回与最后交接

按源顺序追put_user(argc,sp++)、argv/envp循环内put_user与strnlen_user、NULL终止项、mm.arg_start/arg_end/env_start/env_end和末尾copy_to_user。真实宏实参是否求值一次、错误返回如何消费、指针增长发生时机，须按直接定义检查，不能凭函数名或Linux惯例推断。

对put_user/copy_to_user/strnlen_user只核当前配置的直接包装、必要选择和返回约定，足以区分写入语句、错误检测与字节计数；不扩大整个uaccess、异常表、汇编或硬件安全审计。遇必须跨大量底层才能证明的性质，明确边界与待核接口，而不是假称其可靠。

覆盖平台复制失败、局部vma尚无有效来源、argc/envc写入中途失败、strnlen_user返回0或超过MAX_ARG_STRLEN、auxv复制失败以及正常return0的条件模型。后置错误不自动撤回先行saved_auxv、bprm.p、用户槽或mm边界写入；未定义/无效对象后的状态不得继续作为已证真实执行。

核调用者对create_elf_tables返回值的真实判定及之后mm->start_stack、START_THREAD所用bprm->p。只交接到实际宏/函数入口，不重审系统调用/中断返回、外部libc启动或CPU实际RSP。23/24遗留条件和本批vma来源缺口不得因最终有START_THREAD字样而消失。

## 6. T04：六件交付、证据和自查

严格仅allowed_files，不新增第七件、脚本或通用检查框架。MANIFEST含本对话接收和规则加载状态以及T00–T04完成/partial；elf-stack-tables.md给顺序与布局；facts给对象快照、返回/写入矩阵及条件数量；DOT区分调用、宏使用/绑定、数据消费和写入；deferred按历史全限定ID去重追加；evidence含实际读源、1–5行逐字引文、检查方式与未做项。

新ID建议ET25-*。关键调用须引用调用者真实语句，缺少return/未消费返回值不得画成有效数据流，字段和宏不是可凭空调用的函数。数量与ID一致仅证明机械一致性，不替代语义正确性；不设报告长度、问题数或引文数指标。主线R24-S01/S02/S03同类错误纳入自查。

允许已有临时沙箱文本工具作字符串、JSON/YAML、图ID和有限算术检查；不安装、不运行仓库脚本/C/ASM，不使用额外付费模型/API。自己的检查与历史作者检查分列，未做写NOT_RUN/unchecked。仅申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING。

## 7. 单写者、提交和中断恢复

C03c是25唯一执行者，C03b在24交付后停写，无25写权。继续原PR17及claude/dazzling-cori-q0dnyt，首次写前期望c0f4d6a603f8。现场核真实master/time/执行头、开放PR和路径交集、目标目录；已有同名结果先核归属，不能盲覆盖或重复起批。lead合法前进不是执行冲突；time前进记录但不静默换源。

所有旧件冻结；正式交付前仅可按当前blob身份修订自己本批新件。普通快进，接口有expected-head则使用但不称全局锁；不新建竞争分支/PR、不改PR标题正文/Draft或他人评论。调用超时先读远端判断是否已成功，再补剩余项。

最终固定提交全文回读六件，再与活动分支核正文/blob/头；compare c0f4d6a603f8到交付头须仅本目录六件A新增，无历史净修改/删除或不明提交。真实后置检查和短提交写自己的PR17交付评论，不为自引用最终提交反复改文件。

正式交付后停写，由C03验收并同轮安排下一步；不自行选下一批。中断材料仅保存在六件内或自己的PR17评论，不承诺后台执行。Owner只转发45号入口和通知完成，不搬运长报告。

## 8. 权限不扩展

仅阶段2静态分析。禁止修改内核、用户程序、旧成果/脚本或生成修复补丁；禁止预处理、编译、运行C/ASM、内核/QEMU/硬件测试，安装/挂盘/读取凭据、操作Owner本机、读取仓库外libc/镜像；无master/time/lead写入、PR合并关闭、删改名/强推分支或阶段3权限。C03独占lead、规划、PR导航和最终验收，导览/教学只读。普通问题按18号延期，当前不要求Owner决定修复。
