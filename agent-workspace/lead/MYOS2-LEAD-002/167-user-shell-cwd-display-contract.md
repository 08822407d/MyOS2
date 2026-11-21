---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-CWD-DISPLAY-118"
record_type: "bounded_shell_cwd_display_contract"
status: "READY_FOR_OWNER_FORWARDING_NOT_STARTED"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（Owner选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "b2f563a67814"
execution_pin_short12: "52c51a3752d3"
batch115: "RETURN"
batch116: "RETURN"
batch117: "RETURN"
R115_01: "CLOSED_DOCUMENTATION_ONLY"
R115_02: "CLOSED_DOCUMENTATION_ONLY"
R115_03: "OPEN"
R116_01: "OPEN"
R116_02: "CLOSED_DOCUMENTATION_ONLY"
R117_01: "OPEN"
R117_02: "OPEN"
R117_03: "OPEN"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
executor_display_name: "MYOS2-A-C07c 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-17"
receiving_model_tier: "GPT-6＋Extra"
activation_gate: "OWNER_FORWARDS_FIXED_167_AND_C08_RECEIPT_THEN_FRESH_G00"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/"
allowed_files: ["MANIFEST.md","shell-cwd-display.md","facts-and-boundaries.yaml","shell-cwd-display-paths.dot","deferred-findings.yaml","evidence.md"]
execution_status: "NOT_STARTED"
mainline_acceptance: "PENDING"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"task_local","refresh_completed":"pending_executor"}
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-D00-115-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-21-dir-stream116-reviewed-shell-cat117.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md","agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/open.c","mykernel/fs/vfs/d_path.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/arch/x86_64/include/asm/syscalls_64.h","myinitramfs/mylib/myglibc/posix/unistd.h","myinitramfs/mylib/myglibc/string/strcmp.c","myinitramfs/mylib/myglibc/stdio/printf.c"]
---

# 第118批任务书 · shell 的 cd、pwd 与提示符从哪里取得目录

追踪两个不同的对象：内核 `curr->fs->pwd` 和 shell 的字符数组 `current_dir`。从 shell 启动、成功或失败的 cd，解释到下一次 pwd 和提示符分别请求输出什么。第60批已检查内核路径拼接与工作目录对象，本批的增量是 shell 何时请求更新、怎样使用返回和缓冲，以及哪种状态会显示给用户。D00只做117独立验收留下的五处追加补正。[启动和提示符](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L58-L88) · [cd](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L188-L212) · [pwd](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L245-L250) · [60主线裁定及两项限定](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md)

## 接收及 fresh G00

1. Owner 将固定167与本轮C08正式PR17回执转给原「MYOS2-A-C07c 代码分析执行」，沿用 CHAT-EXEC-17 和 GPT-6＋Extra。读同一固定提交另三件、固定166与发布评论、117固定六件与作者正式交付、针对D00的115/116原裁定及定位。复用159中在原对话有效的九份项目约束、补读变更，核110与124，按本任务局部刷新适用 Mnemosyne 指导。记实际已读来源与模型。[159身份](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md) · [166合同](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md) · [166发布 #7056768574](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056768574) · [117固定六件](https://github.com/08822407d/MyOS2/tree/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117) · [117作者交付 #7057046328](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057046328) · [116原裁定](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md) · [115原裁定](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-D00-115-review.md)

2. 全部 P1–P13 适用。完整读63，并按28/79/110/124执行。核 direct master 公约§1第2条原句、time/master 的 options_flags.cmake 与 panic.c 两对金丝雀、time 实际 C 配置。所有工件引用提交时仅用连接器实得短12位。[协议](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [28号](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md) · [110号](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

3. 分别核四条 direct refs。lead 为本轮C08正式发布提交，exec=`52c51a3752d3`，time=`a039d9803ade`，direct master=`e8e60cd19540`。开放列表仅原 PR16 Open/Ready、PR17原题 Open/Draft，原head，base master，均未合并。固定与活动117六件全文和blob逐件相等。初次118目录及六条路径缺席。PR17顶层无since由首页读至空，核166发布、117作者、C08本轮117验收与118发布标记及评论ID唯一，初次所有118作者正式交付为零。断点重入先核既有父链、六件及评论，只补缺项，记实际分页与头。

## D00 · 五项退回一次补齐

原件与旧评论冻结。以同提交的[117独立验收](reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md)与[115/116补正裁定](reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-D00-115-116-review.md)为准，记原行/字段、替代内容、新旧证据、下游消费者及实测。三组技术差量共用一套六件。R116-02使用本轮主线补记后的结论，R115-01/02已闭合。

| 退回项 | 在118六件中可独立核查的结果 |
|---|---|
| R115-03 | 正确定名的实际R115子图补 read/write 真实 switch 映射和入口、两侧 ksys 的 file_ppos 与局部 pos 来源、方法及外层写回。限长、绑定、queued门、零次/一次/跨页的既有正确边可直接复用。沿实图查0、4096、4097，到清零数/返回/最终位置。核映射实例是4–5行，历史getdents64和lseek位置分列。 |
| R116-01 | 保留已经核对的单条及多条尾 d_off 模型，只补源索引。判空条件、return true 和 return false 按实际源窗归位；用户汇编三个标签分别取各自定义内的窗口，完整段另列普通位置，补必要的跳转/退出依据。将三处旧P29逐项关联到 closedir 完整体、公开声明、ls 调用，区分 closedir 与 close 两个接口。 |
| R117-01 | 生成七行八列完整替代表。直接以有效缓冲求首NUL，检查事实表两个真实字段、零起始值3/1/2/2/0/0/0、请求字节及 printf 返回。对最终成品按表头解析、拒绝缺值和占位符，逐行比对。 |
| R117-02 | 历史账写88个逐字窗、六件前缀89次，列多出的一处准确位置。新包P6按六件最终文本独立计数，并逐项对证据账。修P05 read/write/open/close/lseek 映射的具体源行；验定义归属与每条实际支持的命题。 |
| R117-03 | 在本批实际CAT117补正图分清 cat 对 malloc/memset/strcpy/read 的调用与其先后/数据关系。接通 read 的真实入口、ksys、vfs_read、new_sync_read、通过方法表选择的 generic_file_read_iter；显示零计数/正数及 caller 模型止点。表绑定、调用、条件模型分别有类型。逐边走普通、短读、open失败及零计数。 |

## A · 启动、内建命令与目录缓冲

核本仓 myshell.c 的 PATH_MAX、homedir、current_dir 的类型、存储期与容量。跟 main 的 memset、getcwd 与第一次提示符，记录调用实参、谁拥有缓冲、是否使用返回值、何处消费字符串。核命令表 cd/pwd 绑定和 run_cmd 间接调用，以及 shell 文件自身活动声明、myglibc 的系统调用宏实例和共享返回段。区分本地活动声明、头文件注释、宏定义、调用实例。有限模型为初始全零后假设复制入 `/` 加终止符，以及本次调用没有写入时数组仍为空。实际屏显止于格式化写请求。[本地声明及容量](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L15-L40) · [表和缓冲](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L44-L60) · [初始化与使用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L73-L88) · [间接调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L139-L142) · [用户桩](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L44-L52)

## B · cd 是否改变内核 pwd，何时刷新用户缓存

列无额外参数取 homedir、显式 `.` 的早退、相对或绝对路径传 argv[1] 三种选路。跟 `strcmp`、`chdir`、成功后的 memset/getcwd，以及失败时使用 strerror(errno) 和原 path 的格式。区分内建函数返回1、chdir 返回、getcwd 返回和 run_cmd 是否消费。核用户桩与编号映射、内核 user_path_at 和后置 pwd 赋值。只在有效稳定、查找成功的条件下写内核更新结果。失败时不从本地分支外推 errno、权限检查或整个查找无副作用。[cd函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L188-L212) · [strcmp定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcmp.c#L29-L44) · [chdir系统调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/open.c#L272-L293) · [映射](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L41-L42)

由已有 shell 缓冲 `/old` 出发做四例：显式 `.`；无参回家且两次调用均完成、复制出 `/`；相对 `docs` 的 chdir 成功且给定有效新路径 `/old/docs` 被复制；普通 chdir 失败保留原缓存。逐例列选定实参、系统调用次序、内核 cwd 更新、shell 缓冲变化、错误/正常输出请求与返回。若此前缓存已陈旧，同一单字符点名 `.` 会不会触发刷新，也按这条早退路径核实。路径解析内部和目录项生命周期沿第60批边界停止。

## C · pwd、提示符与缓存未刷新

比较 pwd_command 的 `if (current_dir)`、`printf("%s\n",current_dir)` 与 main 的提示符格式。数组在条件表达式中转为地址；其首字节可为NUL。分别给缓存 `/`、`/old/docs`、空串的精确 pwd 输出请求和提示符内容，指出 main 对提示符之前另有一次单独换行请求；记录 pwd 返回1和 run_cmd 的处理。作为跨专题对照，核同一已终止缓冲如何供117的 cat 构造一个普通文件名。

补一例调用方模型：chdir 已成功改变内核 pwd，随后 shell 已将缓存清零，getcwd 没有写入。明确这是所给调用结果，写出内核对象、空缓存、pwd 和下一次提示符分别能推出什么。若只知道 getcwd 返回或发生部分复制而不知写入了哪些字节，在缓冲内容处停止。结合60的R60-S01/S02，核 getcwd 成功分支中复制动作、`int error` 接 `(long)buf` 及返回表达式。对已复制有效终止串的支路，shell 忽略返回值；不得由一次返回值反推拷贝出的字符串。限正常、短、已终止且可读的字符串与有效对象，不展开极端长度、并发、故障注入、引用计数或整棵目录树。[pwd与cat](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L245-L269) · [提示符](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L83-L87) · [getcwd退出段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c#L196-L230) · [第60批限定](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md)

## 六件、检查与停写

沿原执行分支 `claude/dazzling-cori-q0dnyt`、原PR17，只在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/` 新增 `MANIFEST.md`、`shell-cwd-display.md`、`facts-and-boundaries.yaml`、`shell-cwd-display-paths.dot`、`deferred-findings.yaml`、`evidence.md`。MANIFEST 记录协议要求、真实模型/输入/G00、公约原句、金丝雀、自检三数、write_retry_log。报告含 D00/A/B/C。facts 以模型ID列每一步动作、持有对象、缓冲真实值、调用返回及请求输出。DOT 给实际的115及117补正子图和118三专题，明确每个调用者、绑定、局部顺序、条件和数据。deferred 给首个未证前提与恢复入口。evidence 分列新定义内引文、既有引文、普通位置与推论。

强制锚涵盖D00上述实际源与 A 缓冲初始化和首次消费、B 点名比较/成功刷新/失败处理/内核pwd写入、C pwd与提示符实际格式、getcwd复制与返回。每个新P2引文在单一定义内逐字1–5行，声明、宏调用和完整否定结论另用普通位置。按P9逐项做当前可行的文本核查：①40位串，②逐字和符号/命题，③④路径、行界及消费ID，⑤六件前缀与P6三数，⑥外部来源，⑦金丝雀，⑧公约及元信息，⑨⑩文档与写区，⑪运行状态，⑫若有降级核出处和围栏，⑬强制锚。发布前从最终六个正文重新提取所有表、簇名、端点和调用边，按字段逐格复算；正文中引用旧错例要同新表当前有效值分开。新的专题软限约12份源码、每专题约3–5例，D00按需复用。完整本地P9、预处理、编译、链接、Graphviz、仓库程序、QEMU/实机一律 `NOT_RUN`。保持阶段2，普通源码问题依18号延期。

以 `52c51a3752d3` 对应的完整执行提交为唯一父，沿原树单提交六新增。移动 ref 前核真实头、expected_sha=实得父、force=false。回读差异1提交、6新增、0改删，以及固定和活动六件全文及blob。PR17顶层作者正式交付一次，正文含下列标记一次。

MYOS2-LEAD-002-CHAT-EXEC-17 / CORE-USER-SHELL-CWD-DISPLAY-118 / FORMAL_DELIVERY

按ID两读评论，与无since从首页至空所得正文一致；核所有118作者标记恰一次、四个直接头及原两PR形态。报 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，R115-03、R116-01、R117-01/02/03 继续 `OPEN` 后 `STOP_WRITING`。将提交与正式评论交回原 C08 使用 Pro 同轮独立验收和安排后继。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。

按服务端 Retry-After 或10/20/40秒，记录真实尝试、异常、重试前头、远端效果、间隔与最终回读。无重试记 not_needed。
