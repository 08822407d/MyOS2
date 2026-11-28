---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MM-FAULT-12
record_type: bounded_user_mm_fault_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: "MYOS2-A-C03a 代码分析执行"
produced_by: "ChatGPT普通对话，Owner选择GPT-6 / Extra；backend身份未独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
result_base: "2c410132e42d"
instruction_ref: "agent-workspace/lead/MYOS2-LEAD-002/30-user-mm-fault-contract.md @ ec5ab09e3c2d"
instruction_base: "e7ca3dbbcc06"
required_review_record: CORE-USER-PID-LIFETIME-11-REVIEW-001
sr11_01_local_disposition: accept_with_bounded_historical_qualification
sr11_01_mainline_state: RETURN_PENDING_C03_REVIEW
prior_sr10_01_mainline_state: CLOSED_IN_BOUNDED_REVIEW_WITH_SUPPLEMENT
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
kernel_change_made: false
old_results_changed: false
branch_or_pr_created: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mm-fault-12/"
deliverables: [MANIFEST.md, pid11-disposition.md, user-mm-fault.md, facts-and-boundaries.yaml, fault-paths.dot, deferred-findings.yaml, evidence.md]
inputs_read:
  - "30号固定任务书、11主线审查回执；10/11/05定点历史摘要与源码"
  - "固定time中的arch/x86_64/mm/fault.c、mm/fault/fault.c、vm_map.h、mmap.c、filemap.c、pgalloc系列和常量/类型定义"
  - "time固定源码中相关VM fault回退与函数指针实际定义；options_flags.cmake先读"
open_questions:
  - "真实IDT/ASM入口、页表权限/CPU异常返回/硬件重试没有运行或证明"
  - "外部文件读、完整VMA锁、MM与folio生命周期未穷尽"
  - "SR11-01及历史UV-15限定仍由主线裁定"
---

# 第12批 · M00–M04 七件交付总入口

**阶段2有界静态分析已按本包交付，状态仅为READY_FOR_MAINLINE_REVIEW。** 本次没有实施内核修改、代码补丁、编译预处理或运行；C03主线负责独立验收与后续派工，执行者不自签PASS。

## 1. 七件文件（按建议消费顺序）

| 文件 | 实质内容 |
|---|---|
| `MANIFEST.md` | 本包身份、固定源、M00–M04范围、自查和交回约束 |
| `pid11-disposition.md` | **M00先读**：接受SR11-01的OWN11整数API返回/E38调用证据修正，补ST14调用锚点及已关闭SR10-01的引用限定；05 C6与UV-15.claim历史陈述逐字区分并给追加替代文本 |
| `user-mm-fault.md` | **M01–M03次读**：从C缺页入口到VMA、匿名/文件/PTE/写保护分流，以及VM_FAULT非零/0/不返回的架构层消费；含代表输入与反证 |
| `facts-and-boundaries.yaml` | 202段固定源1–5行引用（11个源码文件；细分片段非独立证明）、41节点/49边、25类分支、9类effects及有限查询范围 |
| `fault-paths.dot` | 和facts的41节点/49边ID完全一致；直接、条件、函数指针绑定、稳定VMA下不可达的分支分别标明 |
| `deferred-findings.yaml` | 7项本批有前提的静态/语言层风险、3项向旧05/09的问题证据追加；旧状态不更改，无内核修复 |
| `evidence.md` | 实际读取、源码逐字核对（202/202）、正文59段引用（59/59）、图/ID、主动反证、NOT_RUN、写前与后置回读规则 |

所有文件仅在YAML头`allowed_write_prefix`内新建；第11批及之前全部文件冻结保留，不修改源码、旧图、PR17正文/标题/Draft状态或主线lead。

## 2. M00–M04完成和保留限制

| 阶段 | 已完成 | 必须保留的限制 |
|---|---|---|
| M00 | 完整读取11主线审查回执；先提交SR11-01定点补正，区分kill_proc_info整数返回与内部裸指针、E38真实调用锚点、ST14/A84b、SW10-A20消费；回读05 C6及UV-15两处措辞并限界 | 执行者accept不自动将SR11-01从RETURN改PASS |
| M01 | 核exc_page_fault C入口、地址与访问权限来源、user_mode/err_code标志、mm指针/VMA包含关系、非VMA与扩栈分支 | C入口不等于IDT/ASM已绑定；VMA锁和外部构建条件未验证 |
| M02 | 完整读取myos_handle_mm_fault，定点跟踪页表分配、原PTE、匿名read/write、文件缓存hit/miss、private-COW、shared写、现存PTE写保护；实际generic文件vm_ops绑定 | 页表/文件缓存/权限/folio并发及实际映射正确性未运行；函数指针仅在所核绑定下可确定 |
| M03 | vm_fault_t位集合与ENOERR=0定义回源；逐层核非零不变while忙等和0返回，区分现有PTE为空/已写、失败、不返回；必要衔接09信号帧的用户地址边界 | 没有活动SIGSEGV/OOM/retry成功保证；不能把异常C返回等同系统调用返回尾 |
| M04 | 七件交付、自查、限定问题入库；图与事实关系、源码与正文引用、固定GitHub源端点与写区交叉核查 | 最终七件compare及固定提交/分支全文/Blob身份回读在本清单之后执行，并只放PR17交付评论 |

## 3. 本轮确实完成的校验

- **源文：** facts固定time的202段引文分两批按11个文件回读，178＋24，202/202文本及行范围匹配；每段1–5行。没有把这些短片段冒充202项独立正确性结论。
- **正文：** 59处[VERIFIED]的路径、符号、行号与逐字内容再次对照facts，59/59匹配；后续推论按具体条件与反证消费。
- **结构：** facts与DOT的41节点、49边ID集合相同，全部边的from/to和锚点存在；25类分支、9个effects，函数指针分派保留真实绑定和helper中间节点。
- **问题与旧件：** 7新项、3旧项证据追加；对11的SR11-01与05历史表述的追加纠正只作为待主线核收材料，不修改原件。
- **主动反证：** VMA存在不保证权限；调用分配器不保证有PTE；返回0不保证fault已修；非零不会在此源码自动发信号；注释与函数名不构成活动边；supervisor访问用户范围与用户态缺页有不同标志。
- **已用工具：** GitHub连接器的固定源读取/远端比较与有界JavaScript字符、引用、ID一致性核对。正式YAML/Graphviz解析、源码编译/预处理、C/ASM、QEMU、内核、硬件和旧检查器均NOT_RUN。

## 4. 远端写入历史与最终回读说明

首次写入前：执行分支`claude/dazzling-cori-q0dnyt`头`2c410132e42d`，PR17 open/Draft；开放PR仅16/17且路径不重叠，master=`e8e60cd19540`，time=`a039d9803ade`。先提交M00，然后新增facts，交付前仅对本批facts作两个授权范围内的图/锚点修订，之后新增DOT、正文、延期与evidence。**本清单发布前compare**从`2c410132e42d`到`6ccfd6ca3e02`为8个连续提交，只有本目录6个A新增、0旧件净修改/删除；PR17仍open/Draft。

最后`MANIFEST.md`的提交号、完整七件A diff、固定提交/当前分支全文与blob相等核对，以及PR17交付评论须在本文件创建后真实检查。结果只记录于PR评论，避免本文件虚构未来的SHA或回读。

## 5. 停止与交回

C03请先看M00对SR11-01的接受与05的历史消费限定，再读`user-mm-fault.md`和facts的分支矩阵、vm_ops绑定、返回/不返回分层，最后核七件远端diff。**旧11原RETURN由C03独立裁定，12本批只有PENDING，没有内核正确性PASS。** 不重开SR10-01、SR09-01或普通内核修复；提交后C03a停写，不自行发起第13批、阶段3、补丁或运行。
