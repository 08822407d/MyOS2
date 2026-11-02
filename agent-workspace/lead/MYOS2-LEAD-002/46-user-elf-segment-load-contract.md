---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-SEGMENT-LOAD-26
record_type: bounded_elf_load_segment_mapping_and_zero_tail_contract
evidence_class: 后续静态执行授权及验收条件；不是已完成的代码分析
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告25完成并要求验收；28号同轮验收与后继派发持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: e9aac6625cb5
required_review_record: CORE-USER-ELF-STACK-TABLES-25-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-STACK-TABLES-25-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: 897c3a935aa8
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-elf-segment-load-26/
allowed_files: [MANIFEST.md, elf-segment-load.md, facts-and-boundaries.yaml, elf-segment-load-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 897c3a935aa8及此前全部文件
writer_assignment: C03c仅本批六件及本人PR17进度/交付评论；正式交付后停写
activation_gate: Owner向现有C03c转发本任务；输入绑定与现场写前检查通过
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
  - 25固定六件、44号及25主线回执；18延期、28同轮派发和45身份分离继续适用。
  - 主线本轮固定time binfmt_elf.c中的elf_map/elf_load/padzero和所选load_elf_binary调用片段，仅作验收与后继范围预检。
  - 05和07主线回执定点核对范围；13/14/20/21既有接口材料按主题消费，不宣称重做其整批审计。
open_questions:
  - 主程序段映射、部分页清零、匿名扩展与错误消费由本批取证；主线预检不替代执行结果。
---

# 第26批：ELF程序段的文件映射、尾页清零与失败副作用

## 1. 目标与现有对话操作

25按PASS_PENDING_LOCAL限定接收，须合读R25-S01/S02/S03；没有待单独返工的25报告，不重做栈表或重放45号接收。继续现有C03c / CHAT-EXEC-03，次一档GPT-6＋Extra；这是该执行对话第二个工作包，不需要新开对话。

本批仅追主程序PT_LOAD→elf_load/elf_map→文件映射、页尾清零与必要匿名扩展。回答：文件字节和内存区间怎样换算，哪些检查发生在映射之前/之后，返回地址怎样与错误区分，后续失败会留下哪些已发生的局部副作用。不是修复、完整ELF/解释器/重定位审计、全系统uaccess研究或真实程序装载实验。

Owner启动句：执行CORE-USER-ELF-SEGMENT-LOAD-26。完整读取46号固定任务及25主线回执，沿用C03c既有接收和GPT-6＋Extra；按E00–E04连续完成静态分析、自查、原PR17提交和远端读回。只交指定六件，旧件冻结；普通问题入库，不修内核，不重做24/25。正式交付后停写交C03验收。

工作包内部自主推进，不为已授权步骤重复请示；实际访问、输入身份、权限或不明并行写入冲突才停止相应动作。证据不足如实标partial，不用文件数、引文数或自查通过代替语义证明。发布文件不会自动启动另一个对话。

## 2. E00：固定输入、历史问题和范围边界

完整读本任务及e9aac6625cb5中的25验收回执。源码time固定a039d9803ade，结果旧基线897c3a935aa8，指令基线分别记录；先核mykernel/scripts/options_flags.cmake，默认分支搜索只用作导航。

主文件mykernel/fs/vfs/binfmt_elf.c；定点读取主程序PT_LOAD调用、elf_map、elf_load、padzero、必要ELF字段/宏与make_prot/total_mapping_size等实参生产者。若同一帮助函数也服务解释器，仅标接口复用，不扩大完整解释器装载、PT_DYNAMIC或用户库重定位。

25的R25-S01/S02/S03及23/24的上游返回、VMA与参数页缺口仍为条件边界。不要因为这些缺口尚在就宣称无需分析其他局部函数；也不能把独立有效输入模型回接成已经证明的全链运行。

定点合读05主线回执和原user-vfs-baseline-05/中与ELF/exec相关条目；13文件页缓存、14解除映射/brk、20权限、21选址主线限定按实际被调用接口使用，不全量重读大型事实库。复现已有根因只追加全限定ID证据。没有全仓去重就不得宣称新候选全部独立。

## 3. E01：PT_LOAD实参、类型和检查顺序

从load_elf_binary真实PT_LOAD调用点开始，分别记录p_vaddr、p_offset、p_filesz、p_memsz、p_flags、load_bias、prot、type、total_size的阶段值和单位。只核实解释本批调用所需的ET_EXEC/ET_DYN、首段/后续段及可选解释器对这些实参的影响；不展开全部地址随机化和解释器状态机。

必须标明输入验证、调用elf_load、BAD_ADDR/IS_ERR_VALUE消费和后置字段检查的真实先后。不能因为函数后面出现p_filesz<=p_memsz或地址检查，就把它们当作前面的映射已经满足的前提。任一否定结论限完整读到的函数/分支和固定配置；不以注释替代活动调用。

从实际定义核ulong、ELF字段、int错误值、页偏移/页对齐及字节与页数单位，处理加减回绕和类型转换的必要条件。对尚未证明可由真实程序头生产的极端输入，明确“帮助函数直接模型/生产待证”，不凭纸面溢出构造宣布运行漏洞。

## 4. E02：elf_map文件范围、整体预留与裁剪

完整核elf_map的size=p_filesz+页内偏移、off=p_offset-偏移、地址向下对齐、size向上对齐，以及!size早返。区分p_filesz为0、对齐后size为0和调用者根本未调用elf_map；这些不是同一条生产路径。

分别跟踪total_size为0与非0的vm_mmap实参和返回。非0路径核整体映射后对余段的vm_munmap调用、实参单位、是否检查其返回以及何时才返回map_addr。调用发生不等于解除成功；返回有效起点不等于整个剩余区间已经裁剪。total_size-size的算例须说明关系由谁保证，不能凭函数注释假定不会下溢。

区分可用地址、编码在无符号返回中的错误和BAD_ADDR判定；只在必要时核真实包装/宏定义。MAP_FIXED_NOREPLACE分支的日志是否改变返回值，以实际语句判断；21既有选址/冲突限制继续存在，不能在ELF这一层重新宣告固定映射可靠。

## 5. E03：elf_load的清零、匿名扩展和返回消费

逐分支记录map_addr、zero_start、zero_end何时取得有效来源：p_filesz非0的文件映射路径、p_filesz为0的纯内存模型，以及p_memsz与p_filesz大于/等于/小于的局部条件。不能将没有经过赋值的变量默认设0；也不能在未使用的分支凭变量声明误报缺初始化。

对尾页padzero完整取证：它根据哪个地址算页内偏移、clear_user实际尝试的字节区间、错误怎样映射为返回值。核padzero调用与PROT_WRITE条件的求值顺序，区分“是否尝试清零”与“是否因其失败而提前返回”；不要把忽略错误解释为字节已清零。clear_user只核直接包装、检查与返回接口，底层CPU/异常恢复不在本批认证范围。

对整页尾部扩展，核zero_start/zero_end再次对齐、vm_brk_flags的长度和标志实参、返回值怎样转入map_addr。区分VMA/映射描述建立、物理页已经存在、数据为零、权限正确及全局资源回收；不按BSS名称、GFP名称或注释补齐这些性质。沿用13/14/19/20相应已知边界，仅补证本调用所需接口。

形成有前提的状态矩阵，至少覆盖：文件映射错误、合法返回后裁剪错误/未消费、页尾已对齐无需清理、页尾清理正常/非零且可写性条件不同、仅匿名尾段、扩展错误、后置程序头检查失败及正常返回。记录此前已经发生的映射/字节/字段副作用与是否有局部撤销，不把没有正常返回写成没有副作用，不从局部未撤销断言永不回收。

回到load_elf_binary核真实错误判断及后继界面；AT_PHDR等25相关值只补其段来源与范围前提。任务到所选段帮助函数及必要调用者消费为止，不重审create_elf_tables、寄存器交接或外部libc。

## 6. E04：六件交付与验收标准

仅allowed_files：MANIFEST逐E00–E04登记completed/partial；elf-segment-load.md给连贯时序与区间账；facts给实参快照、分支/返回/副作用矩阵及必要纸面算例；DOT给真实调用/宏/数据/效果；deferred区分旧根因追加与新增条件候选；evidence给实际读源、逐字锚点、自查及未做项。建议新ID EL26-*，不设长度、问题数量或引文数量指标。

关键源码断言须有真实path::symbol及1–5行引文，调用句、赋值和门都要对应，不用被调定义或标题替代调用者证据。图中数据对象不能充当函数调用者；折叠调用须写明中间层；未保存的返回值用ignored_result说明，不画成活动数据消费。源码前缀、正常完成、条件模型和运行结论分层，R25的同类错误纳入自查。

允许现成临时沙箱文本工具作字符串/JSON或YAML/图ID/有限算术核对，不安装、不执行仓库脚本或C/ASM、不使用额外付费模型/API。文本核对和历史作者检查分别记录，不冒充运行或全配置认证。只申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING。

## 7. 唯一写者、远端核对与中断恢复

沿用C03c、原PR17及claude/dazzling-cori-q0dnyt，首次写前期望897c3a935aa8。核直接master/time/执行head、开放PR、路径交集和目标新目录；已有结果先核归属/交付状态，不重复起批或盲覆盖。lead合法前进不算执行冲突，time漂移记录但不静默换源。

旧件冻结；正式交付前可按当前blob身份修订自己本批新文件。仅普通快进，工具支持expected-head时优先使用，否则如实保留竞态限制，不宣称全局锁。不得新建竞争分支/PR或改PR标题正文/Draft及他人评论；调用超时先读远端确定是否已成功，再补余项。

写后从最终固定提交完整读回六件，再按活动分支核正文/blob/head；compare 897c3a935aa8到交付头须恰本目录六件A新增，无历史净修改/删除或未知提交。实际后置核对和短提交记本人PR17交付评论，不为自引用最终提交反复改成果。

正式交付后停写，交C03独立验收并同轮派发后继。中断材料仅在本批六件或本人PR17评论，不新增第七件、不承诺后台继续。Owner只转发46号固定入口及通知完成，不搬运正文。

## 8. 保留边界

阶段2、18延期、28同轮推进和45身份分离不变。禁止修改内核/用户程序/旧成果/脚本，生成修复补丁，预处理/编译/运行C/ASM、内核/QEMU/硬件，安装/挂盘/读凭据、操作Owner本机或读仓库外libc/镜像；无master/time/lead写入、合并关闭PR、删改名/强推分支或阶段3权限。C03独占主线与验收，C03a/C03b/CC02不恢复，教学/导览只读。普通问题入库，Owner当前无需修复决定。
