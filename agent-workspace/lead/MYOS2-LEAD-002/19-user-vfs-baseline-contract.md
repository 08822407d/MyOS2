---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: user_process_and_vfs_static_baseline_taskbook
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "kernel=time；workspace=master；instructions=agent/MYOS2-LEAD-002（分支名）"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VFS-BASELINE-05
phase: phase2_bounded_static_baseline
required_review_record: CORE-MM-BASELINE-04-REVIEW-001
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-MM-BASELINE-04-review.md
findings_policy_ref: agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
reviewed_execution_short12: de7c96ede546
memory_results_frozen_short12: eb75b3100601
kernel_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
initial_execution_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vfs-baseline-05/
existing_files_frozen: "de7c96ede546中的全部文件；仅新前缀新增，不改mm-baseline-04等原件。"
status: READY_FOR_OWNER_LAUNCH
execution_disposition: RUN_WHEN_OWNER_FORWARDS
external_execution_started: false
kernel_change_authorized: false
kernel_patch_production_authorized: false
host_fixture_execution_authorized: false
full_kernel_build_authorized: false
phase3_authorized: false
candidate_A_adopted: false
candidate_B_adopted: false
owner_decisions_requested_now: []
acceptance_ceiling: PASS_PENDING_LOCAL
read_channel: connector
evidence_class: "有限静态调查任务规格；未执行，不把拟查内容当作现状"
inputs_read:
  - "MM-BASELINE-04本轮回收范围详见对应主线审查。"
  - "time:options_flags.cmake；filemap.c的simple_filemap_fault、generic_file_mmap；init/main.c的kernel_init及相邻定义。"
  - "18号Owner延期约定；本会话既有工作令与第二波范围。"
supersedes: null
continues_from: agent-workspace/lead/MYOS2-LEAD-002/17-memory-baseline-contract.md
self_check: {scope: this_file_only, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
local_validation: "待云端静态执行；主线未运行命令。"
open_questions:
  - "实际首个用户程序、文件回调与后端必须由time源码取证，不预填。"
  - "遇未决定修复/策略只登记并绕过，不把分析任务变成Owner问卷。"
---

# 把内存基线接到用户程序与文件访问，不进入修复阶段

**一次完成U00–U04：小型报告处置之后，连续盘点进程创建/程序装载/退出与VFS文件访问，并形成以后新任务可消费的问题增量。** 默认发现问题入库，不在最终对话或PR顶部列问题让Owner决定。Owner近期无时间处理，未答复不授权任何修复；详见18号。

本任务直接服务于原计划的“实际完成度与依赖关系”，不要求现有内核已经正确，不以扩大bug数量或验证器规模为目标。已有MM、调度证据用作明确限定的输入，不重新跑全部旧任务。

## 1. Owner只转发这一段（后续不搬运正文）

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 CORE-USER-VFS-BASELINE-05。
先完整读取：
https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/19-user-vfs-baseline-contract.md
再按其顺序读18号问题收存约定和MM-BASELINE-04审查回执。
沿用原云会话、claude/dazzling-cori-q0dnyt分支，一次完成U00–U04。
先追加MR-01/MR-02处置，再直接完成用户程序与文件访问静态基线，
不要只交勘误或问题清单就停止。
默认把缺陷、风险和待决项写进仓库延期记录，不在完成回复中展开或征求我的意见。
我近期不处理这些问题；不答复不授权修复。继续所有独立安全工作。
不改内核、不出补丁、不编译运行C/ASM/内核/QEMU，不重跑旧测试，不安装工具。
仅在任务书user-vfs-baseline-05/新增，de7c96ede546全部旧文件保持不变。
PR17仍开放就保持Draft复用；已合并按任务书核原件后为新内容开唯一Draft。
不合并、不删分支、不索取凭据。结果经GitHub交回。
最后只返回实际PR链接、提交短标识、完成范围和“问题已入库，当前无需技术决策”；
只有无法继续任何安全工作的真正阻断才说明最小人工操作。
```

原会话/分支不可恢复、PR关闭未合并、源码身份不符或不明相交修改时，保留工作并报告具体阻断，不建竞争PR、不自动更换收费产品。任务文件或评论不自动启动云会话；合并不是本次启动条件。

## 2. 读取、身份及允许动作

顺序：本文件 → 同目录18-deferred-findings-and-resume.md → reviews/CORE-MM-BASELINE-04-review.md → de7执行对象中mm-baseline-04/MANIFEST.md与memory-baseline.md → U00所需M00处置片段及MM YAML。验证回执record_id、被审头de7、冻结结果eb75、returned_items为MR-01/MR-02、next_followup与本任务相符。本任务明确允许在局部RETURN未关闭时先处置并继续独立分析，无需额外准入问卷。

开工记录实际主线任务对象短号。主线后续新增文档属于正常前进，不要求其停在c217或某个发布头。源码固定time a039d9803ade，工作区规则从master读。读首批旧研究时只取下文规定的ID/结构，不采用其成熟度、错误断言或旧证据。

允许：Git只读对象查询、定点文本检索、现有Python/YAML/JSON处理及值表算术；在指定执行分支/新前缀新增、提交、推送和维护唯一PR、回读。禁止：改内核/原脚本/旧原件、生成候选补丁、C/ASM编译或运行、cmake、QEMU、安装/挂盘/分区、内核或硬件探针、网络扫描、提权、输出凭据、安装工具。

复用已读懂的静态读取原语即可，不重跑旧H00/V/W/M/N、不修改旧检查器前缀、不新增通用闸门平台。临时产物置于新建隔离目录；Python禁写旧目录字节码缓存。所有旧源码与交付只读，不清理未知工作树。

## 3. U00：处置两项AI文档问题，不再扩展成研究

交 `mm-04-disposition.md`，supersedes_scope只指对应旧文字：

- MR-01：键下限分支明确区分原键大于/等于/小于队首。用35与10、5与10两个值表核对；不要总括所有被唤醒任务都会到队首。无非idle队首时单列。A/B及OD-2仍未采用，不能把算术例子当作运行结果。
- MR-02：MC-01/RF-01/LB-08与其他总括必须保留实际检索范围和初值前提；未知写入者/别名未排除就不说全局恒假、永不合并、全局不可达。可按已交资料直接收窄，不新增page_type全树专项，不反向宣称代码正确。去掉无测量的发生概率与“先碎片化再耗尽”确定时序；保留真实局部疑点。

如主线推导被反证，附理由入库；证据不足就保留unknown。U00不运行原函数，不重建调度或内存测试，不改用户代码。完成后直接继续U01–U04，不等待Owner确认。

## 4. U01：稳定对象与六条链的实际入口

主要汇总对象固定为 `sched.forkexec`、`fs.vfs`；`user.initramfs`、mm、具体文件系统/块设备和entry仅作为必要交界端点，不宣称一并完成其全量盘点。代表能力至多20个，两主对象各优先4–10个；有依据才建节点。依赖边至多45条、关键锚点至多90条、未来验证候选至多6项。超限按六条主链优先弃尾，未覆盖明确记录，不硬凑数量。

旧输入仅允许master的 `results/MYOS2-DR-002/completeness.yaml` 中这两个对象的ID/能力名，以及 `results/MYOS2-DR-003/deps.yaml` 的相交端点。只用作命名起点，旧断言不继承。新细粒度ID标proposed_id，保留映射，不修改公共词汇表。旧输入缺失不阻塞新源码盘点。

六条主链均须交实际入口、正常路径、失败或清理边界、证据与覆盖限制：

1. **启动到首个用户程序。** 从已读 `init/main.c::kernel_init` 中myos_switch_to_root_disk、kjmp_to_doexecve两处出发，查询真实定义和调用链；确定程序名/参数/装载入口从何而来。不要预设Linux的init搜索顺序，也不凭旧课题默认路径。涉及myinitramfs时只读对应源码/构建文本，不运行其中程序或脚本。
2. **创建/fork。** 追kernel_clone/copy_process及当前实际clone入口，说明任务、mm与文件表是复制还是共享、失败清理到哪里。SMP、并发可达性不自动成立，不复扫调度算法。
3. **exec/ELF装载。** 追实际binfmt注册/分派、文件读取、段映射、参数栈、mm切换与失败返回。映射内部直接引用MM包限定证据，不重查所有页分配/缺页实现。
4. **文件打开/读取/关闭。** 跟踪fd、file、inode/dentry等实际对象及f_op绑定位置；分别标清引用获取/归还、失败返回、短读/EOF和文件位置语义。不要因同名Linux函数存在就补齐其职责。
5. **文件映射到读完成边界。** 从generic_file_mmap、simple_filemap_fault的f_op->read往下找到至少一个由实际对象绑定支持的文件后端。到首次I/O提交/等待/完成端点为止，最多追出具体后端两个局部调用层；若闭合不了保留未知，不把函数指针名字当作调用图闭合。不得由当前配置文本直接升级为全系统唯一后端。
6. **退出/回收。** 找到实际exit、mm/file引用处理与等待/通知接口；区分函数空壳、活动释放、仅减引用与真正归还资源。MM已知断点引用原ID并附本次连接证据，不重复改号报新bug。

源码从实际目录/头文件定位。已知候选文件仅作读取起点：mykernel/init/main.c、sched/forkexec/{fork.c,exec.c}、fs/vfs/{myos_vfs.c,binfmt_elf.c}、mm/vm_map/filemap.c。其他路径先查到再打开，不能猜文件名或把工作区master当内核time。

## 5. U02：实现、依赖与边界分轴

沿用MM包的局部证据字段：implementation_evidence（五种值）、correctness_evidence、runtime_evidence=NOT_RUN、concurrency_evidence和source_condition；不要换算成旧002的0–4，不把20个能力的局部样本当作全量完成度。

每个能力有用途、实际入口/helper、正常路径、失败/清理路径、已读范围与未覆盖。依赖分call/data/init_order/config/build；方向显式声明，init_order沿用“后依赖先”。间接调用必须给调用点、对象/回调绑定和接收方证据，少任一项就断开登记。include不是call，GLOB不是运行可达，词法active不是预处理/链接确认。

特别防止将以下内容写成全局结论：没有搜到同名函数、一个调用点被注释、一个引用减为0、父结构复制、配置宏存在。所有否定断言带查询范围/返回状态、已读定义与未排除项。不要为清除unknown而扩展成全树形式化调用图。

## 6. U03：默认问题记录与以后新任务接手

按18号默认不询问Owner修复/策略意见。本批新增 `deferred-findings.yaml`，其中每项按qualified_id记录证据类型、前提、影响、反证、未知、适用源、后续验证/授权和重新开启条件。普通事项 `owner_action_now: none`，默认 `status: deferred_owner_not_ready` 或 `needs_evidence`。关联旧MC/DV/V等时使用完整包名与路径，避免同ID混用和重复计数。

把MR-01/02处置作为documentation_erratum，与内核缺陷分开；A/B、OD-1/OD-2只引用旧deferred项，不再提出选择。至少为本次接触的旧记录建立交叉链接，不要求机械复制所有历史问题到新文件。

仓库技术正文保留真实失败和未知，面向Owner的MANIFEST/PR顶部/最终聊天不倾倒缺陷清单、不问意见；只写成果、范围、记录入口。当前资料质量不合格时明确partial/缺口并自行补正，不能用“Owner无暇”掩盖失败。仅真正无法继续任何安全工作时，提出一次最小操作。

给新任务的恢复顺序写进MANIFEST：18号入口 → 最新主线回执 → 本包基线 → 被选qualified_id → 原证据与当时最新time。未来修复前重核快照与权限，当前不生成实施代码或激活修复任务。

## 7. U04：可用性与一次交回

交一张两层表：六条链哪些段已被静态证据连接、哪些断开；能直接支持源码讲解的范围与未来运行所需前置分开。最多6项未来核验候选都写NOT_RUN并保存到文件，不作为Owner当前作业，不启动宿主实验、QEMU或更大研究。

主件六份：MANIFEST.md、mm-04-disposition.md、user-vfs-baseline.md、facts-and-dependencies.yaml、deferred-findings.yaml、evidence.md。实质正文与记录一次交齐；另附最少的查询/核对记录，新增小脚本最多2个。不复制整包旧验证器、不交ZIP或二进制。

核对：ID唯一、引用可解析、旧ID映射、六条链和两对象都回应、条件前提与unknown不被抹去；实际计数与上限相符；每条源码锚点按类型核1–5行逐字引文，正文/宏/类型/构建/注释分开；检查器不充当语义证明。没有需要回答的技术问卷，任何建议均未授权实施。

## 8. 回传与收口

开工和推送前核执行分支包含de7；de7所有旧文件不变；新增仅本前缀；time仍对应固定源；两写区无相交修改。主线追加和master合并前进可按事实核对，不盲套旧scope脚本的固定master限制。

PR17仍open保持Draft复用；若已合并，先确认de7旧原件都在master、同分支无其他open PR，再为本批创建唯一Draft。关闭未合并则停止报告。禁止合并、force-push、删分支或写入master/time/主线分支。

结果先提交冻结，再按对象远端回读；回读证据后追加避免自引用。所有文件写执行者真实产品及可知模型，未知则unknown_or_not_attestable，不冒用主线gpt6。禁止完整40位提交号，必要摘要分段。记录实际命令、结果与限制，不写“已通过真实内核运行”。

完成后只交实际PR链接、短提交、U00–U04完成或受限范围及“问题已入库，当前无需技术决策”。没有问题报告也必须明确记录空集合与已检查范围，不凭沉默表示安全。主线直接读库审查；两个分支保留至明确解除，Owner不承担文件搬运或全文技术审阅。
