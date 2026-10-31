执行模型：次一档（默认）。执行位置：现有 MYOS2-A-CC02 内核分析本地执行，不新建会话或分支。

---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-RETURN-SIGNAL-09
record_type: local_user_return_signal_static_analysis_contract
conversation_display_name: "MYOS2-A-C03 内核分析主线续建"
produced_by: "ChatGPT 普通对话（Owner报告当前选择：gpt pro）"
produced_by_source: "Owner本轮模型选择说明；不是直接观察界面或后端认证。"
model_per_owner: "gpt pro"
backend_identity: unknown_or_not_attestable
date: 2026-10-28
base_snapshot: "kernel=time；instructions=agent/MYOS2-LEAD-002；results=claude/dazzling-cori-q0dnyt"
read_channel: connector
evidence_class: "后继静态取证任务设计；不是09执行结果或内核正确性结论"
authorization_ref: "Owner要求审查08并设计下一步；工作令阶段2、18号约定及C02→C03的CONFIRMED。"
required_review_record: CORE-USER-SYSCALL-STDIO-08-REVIEW-001
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SYSCALL-STDIO-08-review.md
findings_policy_ref: agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
reviewed_execution_short12: 8983a9235c8a
kernel_source_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
execution_session_display_name: "MYOS2-A-CC02 内核分析本地执行"
initial_execution_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/
existing_files_frozen: "8983a9235c8a中的全部旧文件；包括08、07和06原件。"
status: READY_FOR_OWNER_FORWARDING
external_execution_started: false
launch_gate: "Owner将§1启动块转发至现有本地会话；仓库文件/PR评论本身不启动任务。"
receiving_model_tier: next_tier_default
receiving_model_tier_reason: "按明确问题逐项静态追踪和登记；开放冲突与最终验收交回主线，不授权在执行侧裁定架构。"
local_billing_requirement: subscription_only
paid_fallback_authorized: false
kernel_change_authorized: false
kernel_patch_production_authorized: false
host_fixture_execution_authorized: false
full_kernel_build_authorized: false
qemu_authorized: false
external_libc_read_authorized: false
phase3_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
owner_decisions_requested_now: []
inputs_read:
  - "08交付主件、相关结构化记录及本轮08主线回执，范围见回执inputs_read。"
  - "time的entry/common.c、arch/x86_64/entry/common.c、entry_64.S返回段、arch/x86_64/lock_IPC/signal/signal.c全文、signal_64.c建帧/返回段。"
self_check: {scope: task_design_not_execution, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
open_questions:
  - "09尚未执行；信号选择、帧字段来源、恢复和系统调用重启的组合需实际取证。"
  - "仓库外libc/restorer的真实行为与硬件返回、实际运行结果仍不在授权内。"
---

# CORE-USER-RETURN-SIGNAL-09：系统调用完成以后，怎样回到用户程序

## 当前目标

08已经把用户文件请求接到系统调用分发和fd路径，但没有展开返回用户态前的信号处理。主线回源发现，这段代码可能改寄存器帧，不能把分发时的ax无条件当作用户最终得到的值。本轮只把这段链路说明白，并处理08的两项小型文字退回；不修内核、不重跑08、不转入阶段3。

## 1. 启动块（Owner发给现有本地会话）

```text
执行 CORE-USER-RETURN-SIGNAL-09（packet仍为MYOS2-LEAD-002-CORE-CHECK-01）。
从08822407d/MyOS2的agent/MYOS2-LEAD-002读取23-user-return-signal-contract.md、
CORE-USER-SYSCALL-STDIO-08-review.md和18号延期约定。
沿用现有MYOS2-A-CC02、隔离clone、订阅额度、执行分支和Draft PR17。
先完成D00：按源码处置SR08-01/02，只追加文字勘误；然后同轮完成D01-D04，
追踪返回用户态前的信号选择、建帧、恢复、系统调用重启及与07/08的衔接。
次一档执行冻结清单；开放冲突只形成候选交回主线，不擅自启用额外模型/API。
不改内核、不出补丁、不构建、不运行C/ASM/QEMU、不读仓库外libc或镜像。
只在user-return-signal-09/新增，8983a9235c8a旧件全部不动，不合并或删分支。
普通问题入库，不征求修复意见；结果通过原PR17回传，不让Owner搬运正文。
```

上述三个文件均在agent-workspace/lead/MYOS2-LEAD-002/下；审查回执位于其reviews/子目录。启动前仍以GitHub实际头为准，不把提示词中的短号当永远最新。

## 2. D00：绑定核对与08文字处置

先读本任务、08回执全文、18号全文；确认回执record_id、被审头8983a9235c8a和本followup一致。沿用现有隔离clone，不触碰Owner开发工作树，不重新执行06交接或05/07/08检查器。

核远端time、执行分支和PR17：当前设计基线为time a039d9803ade、执行起点8983a9235c8a。time若变化，先按旧固定对象完成可复用的历史分析，并单列新头差异；不得把旧结论直接冒充新源码事实。若PR17被合并/关闭、执行头出现不明并行写入或写区冲突，停止写入，保留安全的只读进展并回报主线，不自建替代PR或重置分支。主线新增回执/任务书导致lead头前进是预期变化。

新增`stdio-08-disposition.md`，分别处置：

- **SR08-01**：按回执与原函数区分有效非空file、空槽、成功F_DUPFD、成功F_DUPFD_CLOEXEC以及失败/其他命令；说明哪些路径到达fdput。给出08主说明、US-01和相应facts摘要的定点替代口径，不把已有详细前提说成整条发现错误。
- **SR08-02**：把分发局部ax结果与返回工作处理后的帧分开；号检查限于实际eax/int/uint转换。给出旧表“用户看到的rax”的追加限定，而非宣称信号路径已经运行。

每项保留原位置、接受/部分接受/有证据异议、短引文、修正说明及影响到的结构化ID；不复制08全文、不改08文件、不运行旧检查器。源文支持且文字处置清楚后直接继续D01，不为勘误另停一轮。若发现实质冲突，标明该项未闭合，不把异议静默改成通过；继续不依赖它的安全条目。

同时遵守回执RC08-01：08 boundary退出码0不是独立安全门。新写入按真实diff和明确布尔条件核对；无需另造通用验证框架。

## 3. 输入范围与读取次序

配置面先读`mykernel/scripts/options_flags.cmake`。所有源码判断从固定time对象取得，默认分支搜索仅用于找路径。工作区公约与协议v2按项目适用范围继续执行，旧件只读。

| 层次 | 实际读取范围 |
|---|---|
| 既有说明 | 08主说明§3、§5-8与相关facts/US-01；07主说明的用户返回帧和相关事实条目；08/07主线回执。不全量重读大YAML。 |
| 返回入口 | `mykernel/arch/x86_64/entry/common.c::do_syscall_64`；`mykernel/entry/common.c`；`arch/x86_64/entry/entry_64.S`的entry_SYSCALL_64和syscall_return_via_sysret。07已完成的建帧细节以引用接回，不复制整轮。 |
| 信号/重启 | `mykernel/arch/x86_64/lock_IPC/signal/signal.c`中的arch_do_signal_or_restart、handle_signal、setup_rt_frame、get_sigframe、get_nr_restart_syscall；`mykernel/lock_IPC/signal/signal.c`中get_signal及本链直接调用的选择/出队辅助函数。 |
| 用户帧 | `mykernel/arch/x86_64/lock_IPC/signal/signal_64.c`中的x64_setup_rt_frame、restore_sigcontext、rt_sigreturn及它们直接使用的帧/寄存器访问宏。 |
| 设置接口 | `mykernel/lock_IPC/syscall.c`中的rt_sigaction、rt_sigprocmask；只沿实际调用定点读保存/取回动作和mask的函数，不扩大到全部信号发送、PID或进程生命周期。 |
| 定义与分发 | `mykernel/arch/x86_64/include/asm/syscall.h`、syscalls_64.h、unistd_64.h，以及上述源文件实际包含的必要sigframe/sigcontext/signal/uaccess定义。先定位实际路径，找不到就记unknown，不能按Linux路径猜。 |

必要时读取一层直接辅助函数以闭合当前问题；超出者列依赖边界。不要展开全部调度器、futex、FPU/SIMD、内存管理、设备、文件系统或所有中断入口。不要读取历史libc分支、仓库外musl、构建产物、磁盘镜像或联网下载外部源代码。

## 4. D01：返回用户态前的活动调用链

从`do_syscall_64`的分发结果写入开始，追到`syscall_exit_to_user_mode`、其实际活动调用与汇编返回落点。区分活动代码、注释掉的循环/标志检查、配置条件与只存在于注释中的Linux说明。

必须回答：哪些返回工作真实被调用；ti_work参数是否控制实际循环；调度/信号检查是否仍有活动门；C层返回的bool是否被调用方使用。对07已说明的内容给证据引用，不重复立项。只能说明源码流，不认证sysretq/swapgs硬件语义、生成偏移或真实中断时序。

建立紧凑的`return_frame_transitions`表，按实际阶段标记orig_ax、ax、ip、sp、flags等被读写的字段；只列实际变化，未读字段不能写“保持不变”。列出返回、信号处理入口、重启意图以及不返回/未知的分支。

## 5. D02：信号选择、建帧和恢复

按源码核实rt_sigaction/rt_sigprocmask保存哪些状态，get_signal如何消费pending/mask/action，哪些处理分支可接到handle_signal、哪些忽略/默认处理/退出。这里只追当前链路的接口和前提；不声称全局信号投递或唤醒可靠性已证。

追踪`get_sigframe`和`x64_setup_rt_frame`：帧地址、对齐表达式、保存字段、handler/restorer来源、SA_RESTORER/SA_SIGINFO等实际条件、成功/失败返回以及调用方如何消费失败。检查本链使用的用户内存访问宏的实际活动实现；不能只因名字含unsafe_put_user/get_user/access_ok就假定有故障恢复或完整验证。

追踪rt_sigreturn到restore_sigcontext：帧来源、所读字段、掩码恢复、寄存器写回、错误分支，以及返回值再次经过08分发和D01尾段的连接。用户态restorer真实实现若只给出函数指针，停在该输入边界，不补写libc行为。

输出一张字段来源/保存/恢复/未覆盖表及对应源码说明，避免列出大量没有路径证据的“支持特性”。发现普通缺陷只按18号入库。

## 6. D03：系统调用重启条件表与既有证据衔接

以实际实现建立条件表：是否被认为来自系统调用、是否选到信号handler、实际错误码分支、SA_RESTART条件、ax/ip如何变化、是否设置不同重启号。源码没有实现的组合标未实现/未知，不用通用Linux规则填空。

必须复核ERESTART相关分支使用的定义、syscall_get_nr/syscall_get_error的实际取值方式，以及get_nr_restart_syscall返回的号是否在当前活动分发表中。不要把“ip减2/号写回”的重启意图升级为已成功重新执行。

将D01-D03接到07的返回帧和08的分发局部表。单列：普通无改写返回的必要前提、信号交付对帧的改写、重启分支、rt_sigreturn恢复后的再处理、失败或不返回路径。GAP-U04只可追加静态证据，硬件/运行剩余部分继续NOT_RUN；GAP-U10不因本轮信号分析关闭。

## 7. D04：交付与有限核对

只向`user-return-signal-09/`新增：

- `MANIFEST.md`：D00-D04覆盖、实际模型/表面、读取通道、未完成项、无运行声明与消费顺序。
- `stdio-08-disposition.md`：SR08-01/02定点处置，不改原件。
- `user-return-signal.md`：连续可读的链路说明、返回阶段表、信号帧字段表、重启条件表、失败/未知边界与07/08衔接。
- `facts-and-boundaries.yaml`与`return-paths.dot`：有限能力、依赖、字段/分支事实及引用；图与事实使用同一ID。
- `deferred-findings.yaml`：只写本轮新发现、旧项证据追加和文档勘误，使用全限定ID；不复制全部技术债、不自动关闭旧项。
- `evidence.md`与必要`records/`：实际读过什么、运行了哪些只读/文本命令、核对结果、写区和远端回读。

规模上限是防失控而非凑数目标：能力15、边40、独立锚点75、未来验证候选4；正文以所问链路完整可读为准。超出时优先D00、返回链和分支条件，其次帧字段，再次直接设置接口；剩余如实登记，不扩大任务。

沿用公约的子系统前缀与05/07/08的边方向；间接边须同时给调用点、绑定和接收方证据。每条强制源码结论给实际路径、函数/宏/唯一符号和1-5行逐字引文；D01的返回工作调用、D02的建帧/恢复、D03的ax/ip重启分支不可用推测替代。否定判断必须有配置面、查询词、固定对象和范围。

可使用本机已有Git及Python/YAML/JSON做只读对象查询和本批文本处理；如确有必要，最多新增一个小型静态核对脚本，不追求通用框架。不编译/运行C或ASM，不预处理编译、不运行旧包检查器。机械计数、文本匹配、语义审查和运行验证分栏记录，不能用all_ok替代条件完整性。

提交前实际检查：8983旧文件无修改/删除；新增仅本前缀；远端执行头未被其他写入者改变。若内部比对Git对象，使用完整身份；公开记录仅写项目允许的短号。越界/缺件/输入漂移不得以退出码0掩盖，停止提交并报告具体阻断。推送采用普通快进；发布后从固定提交读回新件、核PR17仍open/Draft和实际头，结果短号放PR评论，不要求清单自引用自身提交。

## 8. 停止条件、模型和回传

同一现有会话完成D00后继续D01-D04，不重新开云任务，不重复做06接收。没有真实访问/授权阻断就不要求Owner回答普通问题。

次一档适用于已冻结的逐项取证、条件表和文本处置。若出现开放根因、多方案取舍、冲突裁定或需要重新解释权限，不把它伪装成机械工作：保存证据与候选，交回主线评估；不擅自切换主模型、调用额外稀缺子模型、转API或新增付费。主线最终语义验收独立于执行者自查。

禁止改内核/用户程序/原脚本、生成修复补丁、构建、QEMU、安装或挂盘、安装工具、提权、读取凭据、修改Owner原开发工作树、直写master/time/lead、合并/关闭PR、删除分支或force-push。普通缺陷仅入库；阶段3、真实运行、外部libc和修复权限仍未开放。

完成后只需通过原PR17给出提交短号、D00-D04完成范围、未覆盖项与真实阻断，并注明问题已入库、当前无需技术决策。本任务书的发布没有启动本地会话。主线之后回收09并复核SR08处置，不由执行者自行宣布08整包通过。
