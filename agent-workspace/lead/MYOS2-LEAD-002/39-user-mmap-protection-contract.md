---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MMAP-PROTECTION-20
record_type: bounded_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-01
read_channel: connector
status: READY_FOR_OWNER_FORWARDING
authorization_ref: Owner报告19完成；28号同轮验收及下一工作包约定
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-02
executor_display_name: MYOS2-A-C03b 代码分析执行续建
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: 7b7e80263748
required_review_record: CORE-USER-ANON-ZERO-19-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ANON-ZERO-19-review.md
source_pin_short12: a039d9803ade
result_base_short12: 6166b3e2f5b9
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mmap-protection-20/
allowed_files: [MANIFEST.md, mmap-protection.md, facts-and-boundaries.yaml, mmap-protection-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 6166b3e2f5b9及以前全部文件
writer_assignment: C03b仅本批六份分析文档及自己的PR17交付评论；交付后停止
activation_gate: Owner向已有C03b转发本任务，执行者完成固定输入与现场写前检查
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
---

# 第20批：mmap权限参数到VMA保护模板的静态传递

## 1. 任务与角色

第19批已获限定静态接收，必须合读R19-S01/S02/S03；不重做19、不单独提交旧批勘误。本批继续由已有C03b使用GPT-6＋Extra执行，沿用其25/36接收及项目约束，不新建对话，不恢复C03a或CC02的执行委派。

目标：只阅读固定MyOS2源码，解释mmap的prot、flags和文件模式怎样经过检查与转换，成为VMA的vm_flags及vm_page_prot。区分请求、中间值、VM_MAY*、当前VM_*、保护模板与实际PTE；不把某层返回成功当作完整映射或硬件权限验证。

任务内P00–P04连续推进，普通问题记录入库，不要求Owner逐项确认。实际访问失败、授权冲突或不明并行写入时停止相应操作，如实保全安全完成部分。本文件仅授权本批文档产出，不授权修改或运行内核。

## 2. P00：输入绑定

完整读取本任务和19主线回执。指令基线7b7e80263748、结果基线6166b3e2f5b9、源码time固定a039d9803ade分开。先核options_flags.cmake；所有源码判断回固定对象，默认分支搜索只能导航。

18号问题延期、24/25执行验收分离、28号同轮派发与36号C03b身份继续生效。19只复用匿名判定、VMA模板及PTE构造限定；14只取解除旧映射和失败副作用的既有边界；08只取系统调用入口和返回层的相关记录。需要时定点读取，不搬运全部历史YAML。

主要入口为mm/vm_map/mmap.c、util.c、vm_map.h及其实际依赖、arch/x86_64/mm/vm_map/pgprot.c和PTE模板。系统调用表、宏、mman与文件模式常量路径须按真实树定位。

主线预检已读mmap.c相关主体、util.c和pgprot.c；系统调用定义目前只由搜索定位，尚未闭合固定绑定。这是范围设计，不算本批执行结果或执行者的VERIFIED数量。

## 3. P01：真实入口与参数

核固定源码的MYOS_SYSCALL_DEFINE6(mmap)及实际系统调用表/包装绑定，追addr、len、prot、flags、fd、off的传递。只展开该入口必要的宏和实参，不重做完整ABI或09返回信号链；未取得活动绑定就标明未闭合。

对照vm_mmap、vm_mmap_pgoff和do_mmap，明确offset字节与pgoff页号、对齐与必要错误检查。核匿名请求如何选择file=NULL、文件请求如何取得fd对应对象，以及本次临时file引用与VMA持有的区别。不要把f_flags、f_mode、O_*与FMODE_*当同一字段。

包装中的锁与解锁按实际直接定义和出口说明，只确认本链保护条件，不开展完整读写锁或SMP审计。

## 4. P02：权限转换与文件访问检查

完整阅读do_mmap相关控制流，逐阶段记录输入prot/flags、局部修改、vm_flags组合及错误出口。特别核PROT_READ相关的PROT_EXEC增位，注释中的READ_IMPLIES_EXEC/personality条件是否活动；不得用Linux惯例替代当前表达式。

核calc_vm_prot_bits、calc_vm_flag_bits、mm->def_flags及VM_MAYREAD/MAYWRITE/MAYEXEC的实际组合；区分许可上限标志与当前读写执行标志。分别记录file/no-file和MAP_PRIVATE、MAP_SHARED、MAP_SHARED_VALIDATE的实际分支及支持掩码。

文件FMODE_READ、FMODE_WRITE、append、path_noexec和缺失mmap回调等条件必须回到真实定义。读取一个代表性活动文件回调绑定即可，不展开完整FAT、块I/O或设备驱动。若直接帮助函数为桩、注释或无明确返回，保留语言和控制流前提，不推断确定运行结果。

## 5. P03：VMA、回调及保护模板

区分simple_mmap_region的既有VMA合并出口与新建VMA赋值路径。记录vm_ops、vm_file、vm_flags与vm_page_prot分别在哪一步建立；不得将新建分支的赋值推广到所有合并情况。

核vm_get_page_prot的实际模板、VM位掩码、配置附加位与__supported_pte_mask。模板RW/NX/PRESENT/USER和筛选后的结果分列；支持掩码来源未闭合时保留符号条件，不用名字代替位运算。

检查call_mmap前后字段可能变化的处理，至少核一个真实回调目标。是否重新合并、刷新vm_page_prot或保留原值，只依据活动语句；不能由回调成功推出PTE已填好。未穷尽回调不得作全局结论。

与19交接只说明最终VMA字段及其缺页消费前提，不重新分析整个do_anonymous_page。用户PROT_READ请求可能已被上游转换，不能用仅有VM_READ的模板示例替代所有实际请求。

正常返回地址、错误值、字段赋值、VMA入链及CPU访问分开。地址选择只作为必要输入接口；不展开全部空洞搜索、MAP_FIXED覆盖算法、VMA分割回滚、mprotect、exec、完整W^X或硬件隔离证明。

## 6. P04：六件文档与检查

仅交allowed_files。MANIFEST逐阶段写completed/partial；正文连贯说明请求→转换→检查→VMA/回调→模板→19接口；facts给分层权限账、条件矩阵、调用/data/effect；DOT对应；deferred登记增量或历史evidence_append；evidence列实际读取、引文、自查和未做项。不额外创建脚本、日志或第七件。

新ID建议MP20-*。矩阵覆盖有意义的PROT_NONE/READ/WRITE/EXEC组合、匿名/文件、private/shared、只读fd的私有写与共享写差异、noexec、缺回调、新建/合并、回调改flags及保护更新。每行给可证明值或符号条件；没有生产者证据不认定场景实际可达，不设问题或引文数量目标。

源码断言需真实path::symbol及1–5行逐字引文，包含实际条件、调用、赋值或掩码。读完整相关控制流后才能作局部否定。引文相符不等于语义、覆盖或运行通过。

直接call与嵌套实参求值分开；call_path列真实中间调用；data按消费方→提供方，layout独立，输出赋值及引用变化用effect。调用条件与副作用条件分别标注。

允许使用自己临时沙箱已有的文本工具作字符串、JSON/YAML、图ID及有限算术检查；不安装工具、不运行仓库旧脚本或C/ASM、不使用额外付费服务。人工、机械及未做事项分层，只申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING。

## 7. 写区、回读与停止

C03b仅使用既有PR17和claude/dazzling-cori-q0dnyt，首次写前应为6166b3e2f5b9。现场核master/time/执行头、开放PR、路径交集和本批目录；已存在成果先核归属，不覆盖或重复起批。time变化记录但不静默换源；lead合法前进不等于执行冲突。

全部旧件冻结；本批交付前仅可按当前blob身份修订自己的新文档。只普通快进，不新建分支/PR，不改PR标题、正文、Draft状态或他人评论。expected-head比较不是全局锁，写入结果不明先查远端再补余项。

最终固定提交完整读回六件，再与活动分支核blob/头；compare 6166b3e2f5b9到交付头仅本批六件A新增，无历史文件净修改/删除或不明提交。后置结果记自己的PR17交付评论和实际短提交，不为自引用反复改文件。

交付后停写，由C03独立验收并安排下一工作包。中断进度保存在既定六件或自己的评论，不承诺后台续跑。Owner仅转发本任务入口及通知完成，不需要搬运报告。

## 8. 权限不扩展

继续阶段2静态分析。禁止修改内核、用户程序、旧成果或旧脚本；禁止生成修复补丁、预处理、编译、C/ASM/内核/QEMU或硬件运行；不操作Owner本机、凭据或仓库外libc/镜像；不写master/time/lead，不合并、关闭PR、删除或强推分支，不进入阶段3。普通问题按18号延期，Owner当前无需修复决定。
