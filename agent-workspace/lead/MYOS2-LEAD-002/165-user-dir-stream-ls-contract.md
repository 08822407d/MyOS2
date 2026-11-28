---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-DIR-STREAM-LS-116"
record_type: "bounded_shell_directory_stream_contract"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（沿用 Owner 在本主线已明确选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "a5b40470646a"
execution_pin_short12: "452444a1c4b4"
status: "READY_FOR_OWNER_FORWARDING_NOT_STARTED"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
executor_display_name: "MYOS2-A-C07c 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-17"
receiving_model_tier: "次一档 GPT-6＋Extra"
activation_gate: "OWNER_FORWARDS_FIXED_165_AND_C08_RECEIPT_THEN_FRESH_G00"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/"
allowed_files: ["MANIFEST.md","dir-stream-ls.md","facts-and-boundaries.yaml","dir-stream-ls-paths.dot","deferred-findings.yaml","evidence.md"]
execution_status: "NOT_STARTED"
mainline_acceptance: "PENDING"
batch114_composite: "PASS_PENDING_LOCAL"
batch115: "RETURN"
R115_01: "OPEN"
R115_02: "OPEN"
R115_03: "OPEN"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"task_local","refresh_completed":"pending_executor"}
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/164-user-memdev-scalar-io-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIRECTORY-ENUMERATION-59-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-directory-enumeration-59/directory-enumeration.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c","myinitramfs/mylib/myglibc/dirent/dirent.h","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/fs/vfs/readdir.c","mykernel/include/linux/kernel/dirent.h"]
---

# 第116批任务书 · shell 的 `ls` 怎样取出并显示目录项

从固定 time 源码的 shell 内建 `ls` 出发，说明 `opendir` 如何建立目录流，`readdir` 如何把一次系统调用取得的一批记录逐条交给 shell，以及枚举何时结束、关闭时释放了哪些资源。技术范围包括本仓 myglibc 与内核 getdents64 的交界。第59批已查内核记录生产，本批新增用户态消费和资源交接。第115批的三个文档退回项作为本批首项。[内建 ls](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L214-L242) · [59主线裁定](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIRECTORY-ENUMERATION-59-review.md) · [59内核记录](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-directory-enumeration-59/directory-enumeration.md)

## 接收与 fresh G00

1. Owner 把固定165与本轮 C08 正式评论转给原 C07c／CHAT-EXEC-17 后启动。沿用159身份、原执行分支、原PR17。全文读本任务、同提交的115验收、114补正裁定和检查点；读固定115六件及作者评论、164合同与发布评论。按159在原对话复用有效的九份项目约束并补读变更，核110/124，按任务局部刷新适用的 Mnemosyne 指导；如实记录读本与范围。[159身份](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md) · [164合同](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/164-user-memdev-scalar-io-contract.md) · [164发布](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7043832344) · [115交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7044709177) · [固定六件](https://github.com/08822407d/MyOS2/tree/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115)
2. 本任务适用 `agent-workspace/tasks/00-gpt-task-protocol-v2.md` 全部十三条。完整读63号及28、79、110、124适用要求。核 direct master 公约§1第2条原句，time/master 的 options_flags.cmake 与 panic.c 两对金丝雀、time 的有效 C 配置。实际模型按可见信息填报。[协议](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [28号](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md) · [110号](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)
3. 分别查四个 direct refs，lead 应为本轮回执所列真实发布头，exec=452444a1c4b4，time=a039d9803ade，direct master=e8e60cd19540。开放列表仅PR16/17，原head，base master，16 Ready、17原题 Draft、均未合并。115固定/活动六件全文及blob须逐件相同，初次启动116目标目录与六路径均缺席。PR17顶层不带since分页至空，核164正式发布、115正式作者、C08本116正式回执各唯一，本批全体作者交付初始为零。记实测ID、分页、路径与头。断点重入如已有本人提交或评论，查父链、内容、blob与标记后只补缺项。未知并行写不覆盖。

## D00 · 追加115三项补正

115原六件和评论冻结。以同提交的主线验收为准，每项记录原文件和行键、替代表/图/标签、固定依据、下游消费者和关闭条件。D00交作者状态仅 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，R115-01/02/03继续 OPEN。第114批已由主线文档闭合。

| 退回项 | 必交差量 |
|---|---|
| R115-01 | 用真实字段重制正文83–89、95–97、111–116行三表。核七个漏计值0、1、4095、4096、1、4096、1与七个哨兵，逐轮余量0／1／0，六格均明确有无回调。三个模式位组合写成不会分裂Markdown单元格的文字。逐行与facts比对，交付表内占位符零、列数与表头一致。 |
| R115-02 | 区分70个逐字窗、69个原标签归属正确与72次旧前缀，保留原账；为E001补正确 `set(CMAKE_C_FLAGS)` 及行58/65。按115验收逐字段修复 A.read_VFS、A.write_VFS、A.MAX_RW_COUNT、C.operations.zero、C.installation 的证据指向，列出每个动作和支持它的窗口。新六件按协议独立计数。 |
| R115-03 | 用真实 ksys 节点替代旧图84–89行中错误的直接调用主体；审查83、97、107、109–120及132行的分派/截断/存储、零次循环和回调身份。正文53、facts.A.iter_detail补同步迭代特殊返回门；引用有效配置及助手原行。图中零次、一次和跨页路径均能读到相应返回。保留115已有 A9/B7/C6 正确有限结果。 |

## A · `ls` 如何打开目录并得到第一个可用缓冲区

从 shell_internal_cmd 的 ls 表项到 ls_command，列默认路径与显式路径的选择。核 opendir_oflags、invalid_name、open、__alloc_dir 的实际分支、分配大小和 DIR.fd/allocation/size/offset/filepos/errcode 初值。列正常打开、空字符串、open 返回负数但分配成功、正常 fd 而分配失败这四种源码分支。每例只到本函数的返回，分别记所持 fd、DIR、是否执行 close/free、errno是否由所审语句赋值。负 fd 模型到保存该值为止，不向内核提交无效对象。固定开销按 sizeof(DIR) 表达，32768与注释中的上限分别核源。[命令选择](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L214-L229) · [打开和分配](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L33-L121) · [目录流字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h#L30-L51)

## B · 一次取数能供多少次 `readdir`

沿 readdir→getdents64→活动系统调用映射，固定源码证明公开符号、计数上限和返回通路；每个宏定义与映射调用分别归类。核缓存已耗尽和仍有余量两条路径，size/offset 的复位、按 d_reclen 前进、filepos 取 d_off、返回的指针指向哪块存储。区分内核 file.f_pos、DIR.filepos 与用户缓冲 offset。结构声明 `struct dirent64` 与 shell 暂存的 `struct dirent *` 分开核头文件、feature 条件、对应字段及通常 x86-64 布局假设，只签源码层。[消费函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L28-L83) · [内联汇编包装](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L66-L82) · [映射](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L67) · [内核记录](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/dirent.h#L7-L13) · [用户记录](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h#L22-L46)

做一次连续调用的纸面演算：同一正向有效批次包含 `.`、`..`、`alpha`，采用59的19字节名字偏移和8字节对齐模型，三条记录分别24/24/32，共80字节。第一次 refill 返回80，耗尽后的下一次返回0；另设该次为普通负返回作对照。逐次列 syscall 次数、记录起点、消费后 offset/size、指针所属缓冲、shell是否打印、最后停止点。若演算需要读 d_off，显式声明所有被读字节已有确定初值，或对尾记录只列符号值／未证明，不能从 FAT 的 actor offset=0 推断尾记录必为0。保留59单条与多条尾记录差别。补一个缓冲已有完整条目的例子，核该次无新系统调用；表内数值与独立复算逐行相等。所有输入限正常、完整、正记录长度，无运行输入、异常记录或故障注入。

## C · shell 显示、枚举结束和资源释放

列 ls 对 `.`、`..` 和普通名字的过滤、输出顺序、换行，以及 opendir 返回NULL、readdir 返回NULL时各自的分支和 shell 返回值。核 `bytes<=0` 的两种普通结局，是否更新 size/offset/filepos、二者是否在 shell 显示上可区分，以及已取记录指针何时可能被同一缓冲重新填充。对 `closedir` 完整活动函数逐句核 fd 关闭、内存释放、显式返回与调用方是否使用返回值。分开写接口注释、活动代码、C返回值消费条件与可观察结果，避免为没有明示的返回赋数值。若某段缺运行/布局证据，在该边停止。[过滤、输出与收尾](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L224-L242) · [非正返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L39-L68) · [关闭函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c#L26-L51) · [内核返回记录字节](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L150-L168)

复用59原验收 R59-S01/S02，尤其是结构布局假设、完整记录条件、内核返回字节与真实运行的区别。输入限本仓以上函数、直接消费头文件、配置与D00指定位置。本批不展开FAT簇链、分配器或设备生命周期；源码问题依18号记录延期。本包重点是用户程序的普通路径。

## 六件、验收和发布

唯一执行分支 `claude/dazzling-cori-q0dnyt`、原PR17，仅可在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/` 新增六件：`MANIFEST.md`、`dir-stream-ls.md`、`facts-and-boundaries.yaml`、`dir-stream-ls-paths.dot`、`deferred-findings.yaml`、`evidence.md`。MANIFEST 填 task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check、write_retry_log、G00及实际未运行项。报告含D00/A/B/C；facts 给分支、前提、持有资源、记录大小、字节/位置、返回、源ID；DOT 按真实主体分出调用边和缓冲字段；deferred 写首个未证前提与恢复入口；evidence 分列新逐字引文、既有证据、普通位置与推论。

强制锚涵盖D00三项、A路径/标志/分配和初值、B refill调用与返回/游标/取字段/缓冲指针、C过滤/调用和实际收尾。空函数或整段缺动作从固定完整体作普通定位，有效动作另取短锚；宏实例不充定义。每个新增P2引文为所标活动符号内的连续1–5行。P9①扫40位串为零，②逐字、准确标识及支持关系，③④查所有路径/行界/消费ID，⑤核正式标签及全六件前缀、三数自洽，⑥外部来源可回查，⑦金丝雀，⑧公约与元信息，⑨⑩文档范围，⑪排查虚报运行，⑫如需降级逐件核路径和围栏，⑬强制锚全覆盖。另核报告所有表列数、占位、值域与facts，图节点/边归属及零次分支。执行文本比对与纸面模型，完整本地P9、编译、链接、仓库程序、Graphviz、QEMU/实机均 NOT_RUN。软上限为新增源码约16份、每专题约5个有限情形，既有D00源按需复用，无引文或缺陷数配额。使用原执行对话独立完成，保持阶段2。

以 `452444a1c4b4` 对应的完整执行提交为唯一父，从原树单提交六新增。移动ref前重读真实头，expected_sha=真实父、force=false。核差异1提交，6新增，0改删；固定与活动六件正文和blob逐件相同。原PR17顶层唯一作者正式交付，正文放下列标记一次。

MYOS2-LEAD-002-CHAT-EXEC-17 / CORE-USER-DIR-STREAM-LS-116 / FORMAL_DELIVERY

按ID两读评论并与无since全分页至空的正文核对，枚举116所有作者正式标记恰一次，复核两PR、四个直接头。报 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，三项R115仍 OPEN，然后 `STOP_WRITING`。将提交与正式评论交回 C08 使用 Pro 同轮独立验收并安排后继。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。

按10/20/40秒或服务端 Retry-After，记每次尝试、重试前头、远端已有结果、间隔与最终回读。无故障记 not_needed。
