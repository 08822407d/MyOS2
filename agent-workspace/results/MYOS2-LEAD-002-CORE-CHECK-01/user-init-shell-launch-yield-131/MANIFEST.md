---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_executor_manifest
followup_id: CORE-USER-INIT-SHELL-LAUNCH-YIELD-131
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: "MYOS2-A-C08c 代码分析执行"
produced_by: "GPT-6（会话自述；实际后台与推理档UNATTESTED）"
model_per_owner: "GPT-6＋Extra"
actual_backend_model: UNATTESTED
actual_backend_effort: UNATTESTED
model_selection_source: "Owner本轮明确发令"
date: 2026-11-23
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_release_short12: 5a22d13876bc
execution_parent_short12: 7e52edee26b4
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  - "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  - "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 48
  quotes_reconfirmed: 48
  downgraded_to_inferred: 0
read_current_this_batch:
  fresh_G00: true
  handoff_receive: "REUSED_PREVIOUS_130_SAME_CONVERSATION"
  mnemosyne_guidance_refresh: "REUSED_PREVIOUS_130_SAME_CONVERSATION"
  project_guidance: "REUSED_WITH_FRESH_CONTRACT_AND_CANARIES"
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/183-user-init-shell-launch-yield-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-diagnostics130-reviewed-init-shell131.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/181-user-shell-error-diagnostics-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md"
  - "agent-workspace/README.md"
  - "agent-workspace/conventions.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot"
  - "myinitramfs/myinit/initd.c"
  - "myinitramfs/CMakeLists.txt"
  - "myinitramfs/myinit/CMakeLists.txt"
  - "myinitramfs/myshell/CMakeLists.txt"
  - "myinitramfs/myshell/myshell.c"
  - "mykernel/arch/x86_64/include/asm/unistd_64.h"
  - "mykernel/arch/x86_64/include/asm/syscalls_64.h"
  - "mykernel/arch/x86_64/entry/syscall_64.c"
  - "mykernel/sched/syscall.c"
  - "mykernel/sched/scheduler/scheduler_types.h"
  - "mykernel/sched/scheduler/myos_rt.c"
  - "mykernel/sched/scheduler/scheduler_core.c"
  - "myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h"
  - "myinitramfs/mylib/myglibc/stdio/errlist_internal.c"
  - "myinitramfs/mylib/myglibc/stdio/errlist.c"
  - "myinitramfs/mylib/myglibc/include/stdio.h"
  - "myinitramfs/mylib/myglibc/string/string.h"
  - "myinitramfs/mylib/myglibc/stdio/printf.c"
  - "myinitramfs/mylib/myglibc/stdio/vsnprintf.c"
  - "myinitramfs/mylib/myglibc/string/wordcopy.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00:
  R127_02: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R130_01: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R130_02: CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW
  R129_01: CLOSED_DOCUMENTATION_ONLY_BY_C09_NOT_REOPENED
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
open_questions: []
---
# 第131批MANIFEST · 原执行者、原分支、六个纯新增结果

**交付目标：**新的init启动shell/fork/execve/sched_yield静态取证，与R127-02/R130-01/R130-02三条文档定点补正。状态READY_FOR_MAINLINE_REVIEW不等于C09主线裁定。全部原源码、旧制品、原评论及PR16/17元数据保持不变。

## 目录与六件
| 文件 | 内容与消费约束 |
|---|---|
| MANIFEST.md | 真实身份、输入、沿用的130接收/指导、fresh G00、P6/P9、发布限界 |
| init-shell-launch-yield.md | D00/A/B/C中文报告，静态事实与条件给定分离 |
| facts-and-boundaries.yaml | JSON兼容YAML：历史12标签重放、130证据勘误/五图、fork三态、execve指针对象与yield条件 |
| init-shell-launch-yield-paths.dot | 81/91历史选择图、五错误模型、init三态、调度条件图，Graphviz NOT_RUN |
| deferred-findings.yaml | 3项等待C09的退回，另记execve/fork/yield/链接未知 |
| evidence.md | 新定义短窗48、独立定位12；旧130的44项不计入131 P6 |

## 会话延续与现行授权
前131已在本**同一**「MYOS2-A-C08c 代码分析执行」/CHAT-EXEC-20会话完成130接收：Mnemosyne交接包与指导刷新从固定e581eb9df6c4读取，MyOS2九项原约束及110/124已经独立加载；本轮没有伪称跨新会话重新接收或重新运行无关指导，也未承接Mnemosyne项目写入。现在Owner新发令使183号生效，本批额外独立读取183固定全文、C09独判130、历史D00、检查点及发布评论#7089219709，直接重核四refs与两反向金丝雀，并从固定time读取本批新源码。现有MyOS2项目规则优先于交接材料，所有写入仅限原exec本131新六件与PR17本人唯一顶层作者回件。

## G00 · 固定读件与实际权属
1. 原执行固定父`7e52edee26b4`。当前四条直接refs初核为lead `5a22d13876bc`、exec `7e52edee26b4`、time `a039d9803ade`、直接master `e8e60cd19540`。原开放PR列表第一页PR16/17共2、次页0：PR16 Open/Ready，原PR17 Open/Draft，base均master，尚未合并；无新增分支/PR。
2. C08→C09正式03与#7088796995均已追溯，C09已自报TAKEOVER_CONFIRMED并发布183号；第131作者仍为EXEC-20，不获lead写权。C09正式发布评论唯一#7089219709，130作者正式评论唯一#7088362162，按ID核正文。
3. PR17无since的顶层评论写前分页100/100/50/0，共250条，131本人全身份FORMAL_DELIVERY标记为0。原131目标目录不存在；若发现并行新写或重复评论，须先按63保全现场。
4. 第130执行父`824ce6b5607a`、唯一新提交`7e52edee26b4`，compare领先1落后0、六件纯新增3,947行/121,585字节。六件从130固定提交与活动exec独立双读，文本、Git Blob身份及UTF-8字节均6/6同一；原130作者评论在本批之前唯一。C09第130审查独立重算Blob的历史证明不冒充本批执行者自己的实测。
5. 183发布lead提交`5a22d13876bc`唯一父`bbbe03f75da1`，四份lead纯新增342行；第130审查为RETURN，R129-01已文档关闭，R127-02/R130-01/R130-02尚OPEN。
6. 直接time内核C金丝雀为`-m64 -mcmodel=kernel -fno-pie -fno-pic`，panic为`this_cpu = smp_processor_id();`；直接master反向分别为`-m64 -mcmodel=large -fPIE`与`this_cpu = raw_smp_processor_id();`。公约§1第2条从直接master原文件核实。固定time源未被master/PR缓存替代。

## D00与新技术工作机器自检
- R127-02以128→129→130→131顺序恢复：六个B选择器重绑到129旧值、六个被130覆盖的B/C标签恢复；三组do0保留，历史投影81节点/91边，已核43对寄存来源及旧18替代边/19新增边原样不动。
- R130-01给出E130-003/014/015/016/017/031六个准确新引文。130原六件裸前缀45（MANIFEST 1、evidence 44），正式源窗44；在追加虚拟MANIFEST第24行替代后恢复旧包44/44/0。结构化补入130虚拟读件的三个字段（加上inputs_read的有证据来源重建）；旧原件不覆盖。
- R130-02五ERR案例由函数调用者→字符串数据→vsnprintf rv→write请求与printf返回的两个用途、被弃的write结果、各命令自己的return来源及run_cmd弃用逐项明确；补rmdir/rm argc门、0槽[0]选择器与两个活动extern及一个注释。
- A：init三个freopen、welcome、一次fork、rv=0/正PID/−1三态，返回/不返回界限；B：execve六种字符串内容，按7次引用86/93字节与按6内容78/84字节两口径（各自不含/含NUL），2个指针空终止；C：syscall号24、myos_rt条件类、resched_curr同CPU双状态及注释锁解锁边界。
- graph机器件有172个选择模型节点/190条有类型边（另2个独立说明节点），各边端点有效，六历史图仍81/91。evidence本批48条带路径::符号的正式单定义/活动宏短窗、12条来源定位，P6为48/48/0；没有把130的44条和本批混算。未声称Graphviz执行。

## P1–P13与运行边界
P1仅真实连接器短SHA、40位十六进制正文扫描应为零；P2正式窗1–5物理行且位于函数/宏/初始化器，注释、独立extern、配置与分派条目作定位，不假冒定义体；P3 read_channel、原公约句、双向金丝雀均提供；P4不对其他代码作全仓否定断言；P5外部结论零（不用无源外链）；P6三数与新短窗字面计数匹配；P7读不到不猜；P8历史原件保全且三项退回只追加；P9-1/2/3/4/5/6/7/8/11/13尽GitHub文本能力核对，P9-9符号全树本地确认、P9-10本地diff执行、P9-12降级围栏各按NOT_RUN或N/A；P10没有生成补丁或脚本；P11第一波不作权威源；P12未发生聊天降级；P13署名不认证后台型号。

**完整本地P9、预处理、编译链接、仓内程序/脚本、Graphviz、QEMU、实机、真实进程/调度/文件操作均NOT_RUN。** 仅GitHub文件读取、程序化文本核对与Git对象写入。写入使用原分支单父快进、expected_sha与force=false，按63先判断远端效果再安全重试，不反复创建已成功的作者评论。六件全部固定/活动/UTF-8/Blob及评论双读/全分页终检后STOP_WRITING；由C09 Pro负责独审，作者不提前宣布三个RETURN闭合。

write_retry_log: 本轮素材准备过程中出现过脚本语法及单次工具调用预算错误，均在执行分支更新前发生；改用有界Git Blob分段生成。前次错误不代表GitHub拒绝写入，也不证明曾成功更新分支。本manifest成文时尚未做本批正式ref快进，最终Git对象与评论需以后置远端读回为准。
