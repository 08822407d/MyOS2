---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FORK-RETURN-IDENTITIES-135
record_type: phase2_author_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: "MYOS2-A-C08c 代码分析执行"
produced_by: "GPT-6 self-reported; actual backend model and effort UNATTESTED"
model_per_owner: "GPT-6＋Extra"
actual_backend_model: UNATTESTED
actual_backend_effort: UNATTESTED
date: 2026-11-23
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
lead_release_short12: 676e4169f14c
exec_parent_short12: f0d46c448367
direct_master_short12: e8e60cd19540
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  - "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  - "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 66
  quotes_reconfirmed: 66
  downgraded_to_inferred: 0
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/187-user-fork-return-identities-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134-D00-133-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-pid-view134-reviewed-fork-returns135.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/186-user-process-id-namespace-view-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/conventions.md"
  - "agent-workspace/README.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/process-id-namespace-view-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-process-id-namespace-view-134/evidence.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-yield-task-selection-132/yield-task-selection-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-first-run-frame-67/first-run-frame.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-clone-group-72/clone-group.md"
  - "mykernel/sched/forkexec/fork.c"
  - "mykernel/arch/x86_64/sched/context/process_arch.c"
  - "myinitramfs/myinit/initd.c"
  - "mykernel/sched/task/pid.h"
  - "mykernel/sched/task/pid.c"
  - "mykernel/sched/task/pid_api.h"
  - "mykernel/sched/task/task.h"
  - "mykernel/sched/task/task_types.h"
  - "mykernel/sched/syscall.c"
  - "mykernel/lock_IPC/signal/signal.h"
  - "mykernel/arch/x86_64/entry/syscall_64.c"
  - "mykernel/include/linux/kernel/syscalls.h"
  - "mykernel/arch/x86_64/include/asm/syscall_wrapper.h"
  - "mykernel/arch/x86_64/include/asm/syscalls_64.h"
  - "mykernel/sched/scheduler/scheduler_core.c"
  - "mykernel/sched/scheduler/scheduler_const.h"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
guidance_provenance:
  original_C08c: "workorder, conventions, README, protocol v2, 18/24/28/63/79/110/124; source and forbidden-run directives inherited and relevant files reread"
  mnemosyne: "previously accepted limited behavior guidance in same original C08c; no new cross-project task and no substituted C09 reviewer"
  new_G00: "187, C09 independent reviews/checkpoint and PR17 comments read via connector; original exec fixed six rehashed against active"
D00:
  R133_01: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R134_01: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R134_02: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R134_03: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R134_04: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
closed_historical_not_reopened:
  - R133-02
  - R133-03
  - R132_COMBINED_PASS_PENDING_LOCAL
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
open_questions: []
---
# 第135批 · 六件清单、G00与源级自检

本次作者仍为原「MYOS2-A-C08c 代码分析执行」/CHAT-EXEC-20。Owner把C09发布的187号合同及正式PR17发布回执#7094597734转发后，任务授权限定为G00、R133-01最后一条虚拟源映射、R134-01至04四项勘误、普通fork的A/B/C及G04。**没有授予修改内核、启动运行、切换阶段3、代行C09验收或变更PR16/17导航元数据的权限。**

## 六件限定新增产物

只可新增于`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/`：

| 文件 | 验证重点 |
|---|---|
| MANIFEST.md | 本清单、身份/读取、反向金丝雀、自检账及止写 |
| fork-return-identities.md | G00、五项D00逐项映射、A父方、B子帧、C两成功一失败、源级与运行的边界 |
| facts-and-boundaries.yaml | JSON兼容YAML：五项勘误、47旧窗逐ID窄断言、十旧查询200字段保留、真实新图ID、普通fork源码拥有者、父子三个样本的输入/步骤/输出及全图 |
| fork-return-identities-paths.dot | 由同一facts.graph按group直接投影的有类型节点/方向/标签图，未执行Graphviz |
| deferred-findings.yaml | 五项作者待复核、真实用户返回/errno/时序/PID生命周期和运行禁令 |
| evidence.md | **66条本135新增正式定义体短窗**、另15条独立定位；每条有窄断言，独立于134冻结47及其替代视图47 |

这六件固定源码交付，不是源码补丁。Git Blob制作只使用连接器的文本和本会话隔离的JS算术校验；未编译、未执行仓内程序/脚本、未真实分配任务或PID。

## Fresh G00与写入前状态

四条直接ref写前为lead`676e4169f14c`、exec`f0d46c448367`、time`a039d9803ade`、直接master`e8e60cd19540`。187固定lead发布提交唯一父`3251b8f68697`，只新增四个主线文档共355行。原PR16 Open/Ready、原题PR17 Open/Draft、均base master且未合并；开放PR分页2/0，PR17顶层评论无since分页100/100/58/0共258条，134作者#7094240400和134独审/187正式发布#7094597734各有且仅有一次，本135正式作者标记写前0。新135目录写前404；C08→C09接管签认182/03和PR17 #7088796995已核对。

134执行固定提交`f0d46c448367`唯一父`5e979654dc92`，GitHub compare显示仅六个新增、4480行/170040字节，固定/活动原执行分支全部六件逐件全文、UTF-8及自建Git Blob SHA-1回算6/6一致，134 evidence.md短SHA前12为`66dfb0a00a20`。本作者没有重新做C09的正式独审或将旧RETURN自签PASS。

直接master公约§1第2条的精确句置于frontmatter；固定time金丝雀CMake行含`-m64 -mcmodel=kernel -fno-pie -fno-pic`、panic.c含`this_cpu=smp_processor_id()`。直接master反向分别为`-m64 -mcmodel=large -fPIE`和`raw_smp_processor_id()`。两条对比按对应源位置直接读到，不混用master源码。协议v2的P2标题为“引文锚定（每条正式标签的最低要求）”，只引用规范含义而不制造额外的机械引文前缀。

## D00逐项结果与当前裁定

- **R133-01（仅余虚拟源）**：133 D00的`S132_class_iter -> S132_picker / INDIRECT_CALL_INSTANCE`是旧ID名字空间里的虚拟边，不是133 DOT真实声明。唯一旧源对应两条实际新边`D133_CLASS_owner -> D133_CLASS_call / INDIRECT_CALL_INSTANCE`以及`D133_CLASS_call -> D133_CLASS_callee / TARGET_RESOLUTION_IF_MYOS_RT`，二者在133固定facts/DOT各唯一；其他循环及静态边已在C09旧裁定核收，不重开。
- **R134-01**：将134旧D00里的`D134_CLOCK_GUARD/RETURN/ADD/OVERWRITE`四个虚拟名精确映射到实际`PID134_CLOCK_guard/early/add/caller`。四个虚拟边到实际边均核端点、kind和两载体基数；后两条回合类型实际是`RETURN_TO_CALLER_THEN_FLAGS_OVERWRITE`和`NORMAL_RETURN_THEN_FLAGS_OVERWRITE`，不是旧字串。
- **R134-02**：134十条`*_target -> *_observer / EVALUATION_ORDER`各自存在，但是假定目标PID槽先于默认ns读取的错误解释。本包将caller的target/type/ns视为参数数据、不假定C实参求值次序；在`__task_pid_nr_ns`体内严格先执行`if(!ns)`及有条件的current默认链，**之后**在独立源语句才对target/type取槽。五个缺省链明确`P -> P.thread_pid=T -> T.numbers[T.level=1].ns=N1`；五个显式ns保持原指针而不调用默认链。十条旧完整PID134主键、正文简称和新Q135实际子图ID一对一交叉，C09已经核过的20×10=200字段全部保留，level失败与ns指针不等两种零值仍不同。
- **R134-03**：旧134从switch直达查询体的`CONDITIONAL_DISPATCH_39/110`是合成可达性。固定宏条件下应分为x64_sys_call的case直接调用`__x64_sys_getpid/getppid`、存根再直接调用`__se_sys_getpid/getppid`、后者直接调用`__do_sys_getpid/getppid`、最后一层就是查询函数的定义体（不是又一次调用）。同一syscalls_64.h在11/21/30三处分别展开extern原型、静态sys_call_table数据和switch分派；条件来自固定CMake与syscalls.h，实际预处理/链接仍UNKNOWN。
- **R134-04**：134的E134-021把pid.h的函数签名124与活动返回125混在一个正式窗；本135 E135-001仅取第125行`return task->thread_pid;`作为一对一虚拟替代。134其他E134-001至047共47条均在机器件按完整旧ID逐条附原符号、原位置和具体窄断言，不复制47个旧代码窗。134冻结47裸/47正式/0降级、16定位不变，一对一更正的完整旧视图也还是47；135新增66正式另有15定位，两个账严格隔离。

五项状态一律`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，须交C09 Pro独判。R133-02、R133-03历史CLOSED_DOCUMENTATION_ONLY和132组合PASS_PENDING_LOCAL保持，133/134原RETURN未被作者改变。

## A/B/C 源级模型核对

A普通fork的`MYOS_SYSCALL_DEFINE0(fork)`只显式设置`args.exit_signal=SIGCHLD`，其余聚合成员在普通C初始化规则下为0，故flags0、fnNULL、kthread0、stack0。先由kernel_clone调用copy_process；成功copy_process先copy_thread，再alloc_pid。将`pid_nr(KPID)=KPID.numbers[0].nr`写入K.pid，非CLONE_THREAD时K.tgid=K.pid；普通非CLONE_PARENT时K.real_parent=P；在已给定的p.pid非零、thread_group_leader(K)=true前提下，PID与TGID槽均指同一KPID。注意copy_process的`p.exit_signal=args.exit_signal`是注释，不能把SIGCHLD直接当成谓词为真的依据。

父方kernel_clone对IS_ERR成功分支调用get_task_pid(K,PIDTYPE_PID)并增一个PID引用，通过pid_vnr(KPID)使用**current=P**的活动PID命名空间求子线程编号nr，随后请求wake_up_new_task(K)、归还临时引用、返回已保存的nr。编号源与唤醒/归还分开；后者不会重算nr。给定count2且无其他变化，引用2→3→2，非最终释放。子方copy_thread的PF_KTHREAD=false、fnNULL、stack0走普通用户分支，先复制父pt_regs（题设ax57），再**在K的childregs上写ax=0**；`ret_from_fork`非空fn分支另有ax写点，本例不经过。父方fork整数、子帧的零、后续getpid是三个不同数据产生点。

| 字段 | C1 P/K于N0 | C2 P/K于N1 | C3 dup失败 |
|---|---:|---:|---|
| K.pid / K.tgid标量 | 5200 / 5200 | 5200 / 5200 | 无新K |
| 父方kernel_clone返回 | 5200 | 30 | -ENOMEM |
| K子帧ax（假设创建有效） | 0 | 0 | 未到达copy_thread |
| K.getpid（条件运行） | 5200 | 30 | 不存在本次子任务 |
| K.getppid（条件运行） | 5100 | 20 | 不到达 |
| P.getpid（条件运行） | 5100 | 20 | 若额外给定用户rv=-1则P仍20 |
| 取得/归还临时PID引用 | 2→3→2 | 2→3→2 | 未到达 |
| wake_up_new_task调用 | 源级到达 | 源级到达 | 未到达 |

C1/C2各19条连续有界源级事件，C3按dup_task_struct返回NULL拆为retval=-ENOMEM→goto fork_out→ERR_PTR(-ENOMEM)→IS_ERR/PTR_ERR→内核返回-ENOMEM的六步。C3不运行真实内存分配或诱发错误；用户libc是否转换成-1/errno未知，若另给用户rv=-1才进入init的非零else分支。K.childregs ax=0不等于K的PID为0，也不等于用户已经成功fork。真实父子先后、实际CPU切换和动态链接均NOT_RUN/UNKNOWN。

## P1–P13与最终止写边界

- P1：在成品正文与机器件只使用由实际连接器输出复核的短SHA和分支名，不允许完整40位SHA。
- P2：新增证据仅取固定time单一函数体或有效宏内部1–5行连续短窗；声明、函数头、宏表项、注释和金丝雀另作定位，每条正式短窗有同一窗口支持的单独窄断言。E135-001必须只有pid.h:125返回。
- P3：本MANIFEST具备`startup_selfcheck_quote`、两条`branch_canary_quotes`、`inputs_read`、`read_channel`、`self_check`及四ref短号；master反向金丝雀符合。
- P4：对“没有运行”“未出现活动赋值”等负判断只限明确读取的源文件与函数和合同路径；活动配置体取自time CMake，并不证明全仓或编译结果。
- P5：对比与函数体来自固定GitHub文件，不把旧对话内部意向当执行证据。
- P6：**135六件新正式窗66、新裸前缀66、降级0；另15个独立定位**。旧134冻结47/47+16定位和虚拟替代47不进入135计数。
- P7–P8：旧文档只追加定点勘误，旧分支/别批不改；发生不完整证据则STOP而非虚构成功。
- P9-1/2/3/4/5/6/7/8/11/13：按GitHub可达固定/活动文本、源窗及机器/DOT完成有界闸门；P9-9/10/12对应有代码件、补丁/差分、降级的部分标`N/A`或`NOT_RUN`。完整本地P9仍不执行。
- P10无源码补丁，P11检查成品禁语/内部句柄，P12不降级，P13执行者身份/Owner选档与后台不可认证项分开。

**阶段2全部运行禁令：完整本地P9、预处理、编译、链接、仓库程序/脚本、Graphviz、QEMU、实机、真实文件/PID/命名空间/进程/调度/时钟操作均`NOT_RUN`。** 本批所有数字来自给定对象和连接器文本，既没有实际内核运行，也没有测试程序、修复补丁或阶段3授权。

write_retry_log（写分支前）：先期静态文本合成曾有两次JavaScript草稿语法/结构错误，均在ref更新前发现并改为分块生成；已建立若干独立未挂载Git Blob帮助文本构造，不代表旧分支已变化。最终G04仍须先核远端父、目标目录和正式标记，随后只做一次单父expected_sha/`force=false`快进、比较限定六新增及六件固定/活动字节和Git Blob。PR17唯一正式作者标记为`MYOS2-LEAD-002-CHAT-EXEC-20 / CORE-USER-FORK-RETURN-IDENTITIES-135 / FORMAL_DELIVERY`，只发一条；按ID双读、全部顶层评论无since分页到空、四ref和两原PR再核。合格即`STOP_WRITING`、交C09用Pro独立验收并同轮发布后继。
