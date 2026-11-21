执行模型：次一档，沿用现有C03c的GPT-6＋Extra。执行位置：原「MYOS2-A-C03c 代码分析执行续建」，不新建对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-OPEN-FLAGS-30-DOC-FIX-01
record_type: bounded_additive_evidence_and_documentation_correction_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通对话；C04主线
model_per_owner: gpt pro（Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
parent_review: CORE-USER-OPEN-FLAGS-30-REVIEW-001
parent_review_verdict: RETURN
returned_scope: [R30-01, R30-02]
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-OPEN-FLAGS-30-review.md
required_review_binding: 与本53号最终固定启动提交一致
original_contract_short12: 1412a1991aee
original_result_short12: da292d44b40c
source_pin_short12: a039d9803ade
result_base_short12: da292d44b40c
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-open-flags-30-doc-fix-01/
allowed_files: [MANIFEST.md, errata.md]
existing_files_frozen: da292d44b40c及此前全部文件，包括原30六件
activation_gate: Owner向原C03c发送本53号执行指令，固定输入及现场预检通过
external_execution_started: false
executor_rotation: false
batch31_authorized: false
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
authorization_ref: 当前30独立验收与28号同轮后继约定；C04主线写权及原执行者分工
---

# 53 · 第30批证据资格及跨件表述的定点文档纠正

## 1. 目标与固定输入

本任务不是重做30的全部分析，不是内核修复，也不是31批。第30批当前RETURN，须处理R30-01/R30-02；主要静态结论可按主线回执的限定复用，但纠正完成前不得自签PASS_PENDING_LOCAL。继续现有C03c，原接收和角色有效。

完整读取本53号及同提交30主线回执；完整读取1412a1991aee中的52-user-open-flags-contract.md，定点合读同提交29回执R29-S03。完整读取da292d44b40c的user-open-flags-30/原六件：MANIFEST.md、open-flags.md、facts-and-boundaries.yaml、open-flags-paths.dot、deferred-findings.yaml、evidence.md。它们是纠正对象，不重写原件。

固定源码仍为time a039d9803ade。直接源限原80锚点对应的14文件：mykernel/fs/vfs/open.c、mykernel/include/linux/fs/internal.h、mykernel/include/linux/kernel/fcntl.h、mykernel/include/uapi/asm-generic/fcntl.h、mykernel/include/uapi/linux/fcntl.h、mykernel/include/uapi/linux/openat2.h、mykernel/include/linux/kernel/stat.h、mykernel/include/uapi/linux/stat.h、mykernel/include/linux/fs/fs.h、mykernel/include/linux/fs/namei.h、mykernel/fs/vfs/namei.c、mykernel/fs/syscall.c、mykernel/sched/forkexec/exec.c、mykernel/include/linux/kernel/types.h。读取所需定义体及必要前后文，不扩审整个文件的其他子系统。

最先读固定options_flags.cmake和panic.c金丝雀；继承已加载的工作令、18/24/28/45和适用Mnemosyne指导，不导入外项目维护。本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条，特别P2/P6/P8/P9；本任务采用追加勘误，不覆盖旧目录。

## 2. D00：身份和边界

核PR17及直接master/time/lead/执行头、开放PR和路径交集、新目录是否存在。首次写前预期da292d44b40c。若已有正式纠正交付，先核现状而不是重复写；未知头变化或竞争写入停写核归属。主线合法前进不构成执行冲突，固定源变化不静默换源。

记录本次只纠正文档，30原始回执仍RETURN；只允许原执行分支的新目录两件和本人PR17进度/交付评论。不能修改原六件、历史评论、PR正文/标题/Draft、lead、源码或其他仓库。

## 3. D01：逐锚点处置，不用文本命中替代定义归属

建立OF30-A001至A080的完整处置表，每个旧ID恰一行：保留（已重核）/保留（未重核，原因）/修正/撤回；附实际path::symbol、问题类型和替代锚点或导航位置。它是80个既有条目的核对，不是新增80个分析任务。关键材料不能在未重核状态下宣称纠正完成。

A038声明和A058包含链应明确转为导航，不能继续作为定义体VERIFIED；它们支持的关键语义须由真实定义证据承担。对单标签夹多个宏、typedef、外部注释或#ifndef/#endif的项目逐项裁定，不笼统降级整片源文件，也不把函数体内合法注释误当定义外内容。

每个新的关键锚点应只定位一个真实函数、宏、typedef或结构体定义，逐字片段1–5行，空行计数；多符号结论用多个锚点。需要拆分时保留旧ID映射，可加后缀，不重编号历史OF30问题。长宏允许多个各不超过5行且确属同一宏定义的片段，语义以完整定义为准，不能为行数拼接不连续源文本。

补齐30回执指明的直接证据：open_flags的lookup_flags成员、__O_TMPFILE数值、S_IALLUGO所需特殊权限位及S_IRWXU、LOOKUP_IN_ROOT/CACHED、do_open_execat的LOOKUP_EMPTY赋值。标题必须与片段实际内容一致；不能用相邻行号或文件已读来替代关键语句。

## 4. D02：跨六件的同义纠正与小型值表

errata中给出R30-02要求的图标注替代：build_open_flags(how,&op)而非&how；O_TMPFILE包含O_DIRECTORY位而非同值别名；getname有效结果/错误结果的数据来源；lookup_flags写入在没有前置返回时才发生。按原图节点/边定位说明，不增加独立DOT文件，也不要求重建整个图。

将A003术语改为局部聚合指定初始化。明确op字段“本次未写”与“调用者本来未初始化”两层条件；后者只在实际未初始化局部op的模型成立。保持getname与fd的前提、UV-04边界、raw-helper与legacy区分，以及OF30-F02不是仅凭字段差异即成立的bug。

补必要的小型对照：实际未知且未被strip消去的flags位经过/不经过build_open_how、O_PATH携带O_CREAT的legacy与raw-helper差异、非法mode经掩码前后；数值从固定定义复算，必要时附直接宏证据。只做有限纸面模型，不运行C或真实open样本。不要新增漏洞数量指标，不做全仓去重或完整openat2/权限分析。

逐件列出原六件“保留/需合读本errata/局部替代”的消费关系。处理MANIFEST及摘要中80/80归属全通过的过强说法，准确保留作者历史文本核对声明，不反向编造“当时没有做检查”。任何未闭合项明确留pending，不能把RETURN作为已经自动关闭。

## 5. D03：恰两件交付、自查与机械门

仅MANIFEST.md和errata.md。MANIFEST包含任务/执行身份、Owner模型选择、read_channel: connector、base_snapshot、精确inputs_read、startup_selfcheck_quote、branch_canary_quotes、self_check的verified_claims/quotes_reconfirmed/downgraded_to_inferred。不可独立得知的界面/后端信息写unknown，不凭产品自报认证。新自查统计针对本次两件实际标签，原80条的保留/替换/导航分类另列，不把历史计数与新标签计数混算。

errata头部supersedes指向真实原evidence.md路径，同时用supersedes_scope及逐件表限定为本次纠正范围；不宣称替换未审过的其他历史材料。保留原qualified_id、错误历史、source_ref和分支。正文须有逐锚点处置表、合格替代引文、跨件/图标注替代、必要纸面值例、未闭合项及R30-01/R30-02状态。无须把六份原报告完整复制一遍。

范围上限为两文件、原80条资格复核及上述直接缺口；不增第三件、通用检查器或脚本。可在现成临时环境做纯文本、计数、JSON/YAML、图标注与有限算术检查，不安装工具、不运行仓库程序或外部付费模型。机械工具不可用时如实说明，不虚报运行结果。

重点验收按P9编号：1零完整40位提交串；2每个保留的新VERIFIED片段逐字存在且属于所标定义体，声明/include不混入；3所有源路径和supersedes路径真实；4附带行号有效但不代替符号；5两件标签与自查计数一致；7/8固定源金丝雀、公约原句及read_channel；11不声称内核已运行；13下列强制证据必须直接闭合。其余P9项目按真实适用性登记，不自签全局通过。

强制直接证据：do_sys_openat2两次fd赋值及getname门；build_open_flags的TMPFILE错误门、mode/open_flag/lookup_flags赋值；file_open_name错误返回门；do_filp_open的lookup_flags读取；open_flags五字段；O_TMPFILE/O_DIRECTORY/__O_TMPFILE/O_TMPFILE_MASK各实际定义；S_IALLUGO组成；do_open_execat的LOOKUP_EMPTY赋值。不得以INFERRED或导航替代这些关键证据。

## 6. D04：提交、回读与停止

在原PR17、claude/dazzling-cori-q0dnyt正常快进，支持expected-head即使用，禁止force/reset及另建分支或PR。调用超时先查远端再重试，不能把未知状态当失败盲写。最终固定提交全文回读两件，再按活动分支核正文/blob/head；compare da292d44b40c至结果头须恰两件A新增、无旧件净修改/删除及未知提交。将真实短号及回读结果写本人PR17交付评论。

仅申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，明确30的RETURN仍待C04关闭。正式交付后STOP_WRITING，不自行进入31或再修旧件。Owner只需向C04报告“30-DOC-FIX-01已完成”和真实短提交，无须搬运长报告。

模型仍次一档GPT-6＋Extra；包内D00–D04连续推进，不为普通内部步骤逐项等待确认。真正访问、授权、材料身份或竞争写入阻断才停相应动作。禁止修改内核/用户程序/旧成果、生成补丁、预处理/编译/C/ASM、仓库脚本、QEMU/硬件、真实用户输入、安装/挂盘/凭据、仓库外libc/镜像、额外API、合并关闭PR、强推/删改分支、跨仓维护或阶段3。发布本任务不等于已经投递或启动。
