---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-HEADER-READ-27
record_type: bounded_elf_header_read_and_dispatch_contract
evidence_class: 后续静态执行授权与验收条件；不是已完成的源码分析
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告26完成；28号同轮验收及后继派发持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: 895acd739f07
required_review_record: CORE-USER-ELF-SEGMENT-LOAD-26-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-SEGMENT-LOAD-26-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: 487936efec09
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-elf-header-read-27/
allowed_files: [MANIFEST.md, elf-header-read.md, facts-and-boundaries.yaml, elf-header-read-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 487936efec09及此前全部文件
writer_assignment: C03c仅本批六件及本人PR17进度/交付评论；正式交付后停写
activation_gate: Owner向现有C03c转发本任务，输入绑定和现场预检通过
old_executors_reactivated: false
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
  - 26固定六件、46任务与26主线回执；18延期、28同轮派发、45角色分离保持。
  - 固定time binfmt_elf.c的elf_read/load_elf_phdrs及主程序入口，为26回核和27范围预检；不当成27执行成果。
  - 固定time exec.c1000–1245中的prepare_binprm/search_binary_handler，read_write.c1–180中的kernel_read/__kernel_read/rw_verify_area，为本任务范围预检。
open_questions:
  - 首块短读、程序头完整读取、分派及回调数据/返回契约须由本批定点核实，不预设Linux标准实现。
---

# 第27批：进入ELF段装载前，哪些头部字节真正被读取和检查

## 1. 目标与现有对话操作

26按PASS_PENDING_LOCAL限定接收，必须合读R26-S01/S02/S03；没有单独返工任务，不另交26勘误，也不重做段映射。继续现有C03c / CHAT-EXEC-03、次一档GPT-6＋Extra；本批是该执行对话第三个工作包，不新建对话、不重放45号接收。

本批连接prepare_binprm首块读取、search_binary_handler对返回值的消费、ELF头识别、load_elf_phdrs/elf_read的完整长度要求及kernel_read直接分派。区分请求长度、实际返回长度、先行清零、缓冲区真实文件字节、字段检查和后继调用。目标不是完整ELF/解释器/动态链接审计、文件系统重跑、修复或样本运行。

Owner启动句：执行CORE-USER-ELF-HEADER-READ-27，完整读取47号固定任务及26主线回执，沿用C03c接收与GPT-6＋Extra，按H00–H04连续完成静态取证、自查、原PR17提交与远端回读；只交六件，旧件冻结，完成后停写交C03验收。

内部步骤连续推进，不为常规取证、记录和提交逐项请示。权限、关键输入身份、不明并行写入等真实阻断才停止相关动作；普通代码问题仅登记延期。证据缺失如实partial，不把文件数量和自查通过当语义证明；发布任务不自动唤醒对话。

## 2. H00：冻结输入、规则与旧结论

完整读本任务和895acd739f07中的26回执；特别沿用R26-S02“返回值未消费不等于存在非零生产者”与S03“短引文必须含实际调用/赋值/门”。源固定time a039d9803ade，前结果487936efec09；先核mykernel/scripts/options_flags.cmake，默认分支搜索只导航，源码结论回到固定对象。

定向合读05主线与user-vfs-baseline-05中exec/binfmt/文件读取相关条目，13文件缓存及相关已接受I/O接口限定，25/26的上游可达性和后置段检查边界。已有根因只追加全限定ID证据，不因新调用点重复建独立缺陷；未完成全仓检索就不宣称全仓去重。

主要导航：mykernel/sched/forkexec/exec.c、fs/vfs/binfmt_elf.c、fs/vfs/read_write.c、linux_binprm/linux_binfmt/file_operations与ELF64头类型、BINPRM_BUF_SIZE等真实定义。只补必要的formats注册/绑定、iovec/kiocb直接初始化与文件读取方法选择，不全量重读历史事实库。

## 3. H01：首块读取与格式分派

从实际bprm->file和bprm->buf的所选来源起，核prepare_binprm的局部pos初值、memset范围、kernel_read实参、返回类型及调用者转换。分别记录缓冲容量、此次请求数、实际返回数和已有清零前缀；短读后的零值不能称为从文件读到的有效字节。

完整核search_binary_handler对prepare_binprm返回的门：负错误、0、正短读、请求长度全部返回分别怎样进入或跳过后续步骤。未检查完整首块不自动等于接受任意短文件；还必须追到实际ELF字段门。对下层违约返回大于请求、溢出或无效指针，只标明确的假设性接口模型，不冒充合法生产路径。

为实际load_elf_binary间接调用补最小三要素：调用表达式、具体函数指针赋值/注册、所选对象/列表取得路径。核formats迭代、每个load_binary返回的覆盖/退出条件和活动/注释的区别。单处理器列表与假定多个处理器列表分开，不从源码含循环就声称已注册多种格式；不扩展完整脚本/解释器递归、模块加载、并发注册或exec全生命周期。

若发现成功返回、-ENOEXEC或point_of_no_return的退出逻辑被简化，给真实语句和有限后果；不按Linux习惯补全，不把此前某次handler的值默认作为最终值。底层列表有效性/初始迭代未闭合时保留该前提，允许单独讨论局部返回消费，不倒接为端到端执行证明。

## 4. H02：ELF头字段、程序头长度与缓冲对象

核load_elf_binary如何将bprm->buf解释为elfhdr_s，完整读取实际初始检查到load_elf_phdrs调用。对magic、e_type、e_machine、e_phentsize、e_phnum、e_phoff及必要身份字段，标明在哪里检查、在哪里使用、先后顺序和未核项。不要依据外部ABI常识声称代码已经检查某字段；也不要把不在本窄范围的验证一律判为全仓不存在。

完整核load_elf_phdrs：字段宽度、sizeof乘法、uint size转换、零值/上界、kmalloc、elf_read调用和失败释放。用当前类型与页大小列可接受/被拒数量的条件边界，区分算式结果、地址覆盖和真实文件内容。size<=一页不证明e_phoff到表尾在文件范围内；核必要的偏移转换与溢出/负值消费位置即可，不重做文件大小系统审计。

核elf_read的ssize_t rv与size_t len比较、rv<0和-EIO分流、局部pos按值传入的含义。指出读取不足、0、恰好len分别如何影响已分配buffer的保留/释放；不把kfree调用自动认定内存已回收。对返回类型的截断或符号转换，只在实际输入域内作条件推导，不泛化到不存在的超大请求。

如同一助手也用于解释器，只记录复用及相关输入差异；本批不完整验证PT_INTERP、GNU property、动态段或解释器映射。结束边界为主程序程序头表交付、字段有效性状态与26所选PT_LOAD循环接入，不再执行26段区间分析。

## 5. H03：kernel_read的直接返回与数据接口

完整核kernel_read、__kernel_read和rw_verify_area的活动流程；实际早返、注释校验、file mode门、read_iter/read优先顺序与缺失方法返回须回源，不凭名称推定已经进行了读区间验证。

分别跟踪read_iter的kvec/kiocb/iter初始化、截断到MAX_RW_COUNT的长度与普通read路径的实参；只对本批请求大小判断是否触及截断。记录局部ki_pos、传入loff_t *pos、file->f_pos之间的真实读写关系，不能把任一个偏移前进当其余全部同步。返回值、迭代器消耗、目标字节已写和文件位置更新分别列证。

必要时定向核本仓所选文件f_op绑定及直接read/read_iter适配器，足够说明接口方向和回传约定即可。下层页缓存/磁盘真实数据、异步完成、全部iov_iter及驱动不在本批重审范围；继承13或08等已核限制时明确作者/固定对象与未展开范围。无法证明完整数据链时标接口待核，不以返回计数等于请求自动认证文件内容准确。

至少形成以下矩阵：prepare前缀清零后读错误/0/短读/完整；格式门拒绝/继续；无handler和给定多handler的局部返回覆盖；phdr尺寸/数量拒绝、分配失败、elf_read负错/短读/完整；方法缺失、mode门、read_iter与read局部消费；返回后的缓冲归属与剩余条件。无真实生产者的行标helper model或not established，不混为运行场景。

## 6. H04：六件证据交付与自查

仅交allowed_files，不加脚本、第七件或通用验证框架。MANIFEST逐H00–H04列completed/partial及未做项；elf-header-read.md连贯说明缓冲、字节长度、字段门和返回；facts给类型/对象快照、分支矩阵与必要纸面例；DOT区分调用、绑定、宏、数据和写入；deferred对旧根因追加或新增条件候选；evidence记录实际读源和检查方法。新ID建议EH27-*，不设文件长度、引文量或问题数目标。

关键事实须真实path::symbol及1–5行逐字原文，空行也计行数；超长逻辑拆成有上下文的小段，不能手改代码再称逐字引文。调用者语句、具体返回生产者及其消费必须对应；常量返回函数不能虚构非零错误，未保存返回值不能画成活动数据依赖。调用点不是额外函数；折叠链明示被省略层。机械端点通过不是图语义通过。

可用现有临时沙箱作字符串/JSON或YAML/图ID/有限整数算术核对，不安装、不执行仓库脚本或C/ASM、不调用额外付费模型/API。清楚记录实际使用的工具/检查方式；没有执行解析器就不能用含糊“sandbox or tool validation”代替已执行说明。历史作者检查不冒称本轮复现。只申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING。

## 7. 单写者、回读与中断恢复

继续C03c、原PR17及claude/dazzling-cori-q0dnyt，首次写前期望487936efec09；核直接master/time/执行head、开放PR与路径交集、目标目录。已存在同名结果先核归属/交付状态，不重复起批、覆盖或重置。lead正常前进不算执行冲突，time变化记录但不静默换源。

全部旧件冻结；正式交付前仅可按当前blob修订自己本批六件。正常快进，支持expected-head时优先使用；没有则保留竞态限制，不称全局锁。禁止新建竞争分支/PR或改PR标题正文/Draft及他人评论。调用超时先读远端确定是否成功，再补余项。

最终固定提交全文读回六件，再核活动分支正文/blob/head；compare 487936efec09到交付头须恰本目录六件A新增，无历史净修改/删除和未知提交。实际后置核对与真实短提交写本人PR17评论，不为自引用结果反复改文件。正式交付后停写交C03独立验收，不自选28。中断记录仅在本批六件或本人PR17评论，不能承诺后台继续；Owner只转发47入口和通知完成。

## 8. 权限不扩展

继续阶段2、18延期、28同轮推进和45身份分离。禁止改内核/用户程序/旧成果/脚本、生成修复补丁，预处理/编译/运行C/ASM、内核/QEMU/硬件、真实ELF样本执行，安装/挂盘/读凭据、操作Owner本机或读仓库外libc/镜像；无master/time/lead写入、合并关闭PR、删改名/强推分支或阶段3权限。C03独占规划、lead、PR导航与最终验收；C03a/C03b/CC02不恢复，导览教学只读。普通问题登记延期，Owner当前无需修复选择。
