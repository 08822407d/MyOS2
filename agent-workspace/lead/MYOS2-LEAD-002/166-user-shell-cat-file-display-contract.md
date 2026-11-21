---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-CAT-FILE-DISPLAY-117"
record_type: "bounded_shell_cat_file_display_contract"
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
lead_before_publication_short12: "e96b1079ff0d"
execution_pin_short12: "145d6781c75e"
batch115: "RETURN"
R115_01: "CLOSED_DOCUMENTATION_ONLY"
R115_02: "CLOSED_DOCUMENTATION_ONLY"
R115_03: "OPEN"
batch116: "RETURN"
R116_01: "OPEN"
R116_02: "OPEN"
executor_display_name: "MYOS2-A-C07c 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-17"
receiving_model_tier: "GPT-6＋Extra"
activation_gate: "OWNER_FORWARDS_FIXED_166_AND_C08_RECEIPT_THEN_FRESH_G00"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/"
allowed_files: ["MANIFEST.md","shell-cat-file-display.md","facts-and-boundaries.yaml","shell-cat-file-display-paths.dot","deferred-findings.yaml","evidence.md"]
execution_status: "NOT_STARTED"
mainline_acceptance: "PENDING"
receiver_guidance_load: {"project_guidance":"required","mnemosyne_guidance":"task_local","refresh_completed":"pending_executor"}
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md","agent-workspace/lead/MYOS2-LEAD-002/165-user-dir-stream-ls-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MEMDEV-SCALAR-IO-115-review.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/evidence.md","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/strcat.c","myinitramfs/mylib/myglibc/string/strcpy.c","myinitramfs/mylib/myglibc/posix/unistd.h","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/fs/vfs/read_write.c","mykernel/mm/vm_map/filemap.c","mykernel/fs/fat/myos_fat32.c"]
---

# 第117批任务书 · shell 的 cat 怎样读取并显示小文件

从内建命令表跟到一次具体的文件显示请求。说明 shell 怎样组成实际路径，如何用两次定位得到读取范围，以及一次 read 所得字节怎样经 `%s` 成为输出。已有第114批给出位置语义，第13批有文件读取链，本批落实到调用方、请求的输出字节与资源状态。D00先补第115、116批尚未闭合的三项。[cat_command 完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L252-L286) · [114位置分析原裁定](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FILE-SEEK-POSITION-114-review.md) · [13文件缓存裁定](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FILE-CACHE-13-review.md)

## 接收与 fresh G00

1. Owner 将本固定166与同轮C08的PR17正式评论转给原「MYOS2-A-C07c 代码分析执行」，使用次一档 GPT-6＋Extra。沿用 CHAT-EXEC-17 与159身份。读本提交另外三份文件、固定165及发布评论、116固定六件及正式作者评论、115 D00涉及的原件和原C08裁定。按159复用本执行对话有效的九份项目约束、补读变更，核110和124；按任务局部刷新适用的 Mnemosyne 指导，记实际读本、范围、模型。[159接收身份](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md) · [165合同](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/165-user-dir-stream-ls-contract.md) · [165发布 #7056268192](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056268192) · [116固定六件](https://github.com/08822407d/MyOS2/tree/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116) · [116作者交付 #7056533291](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056533291) · [115固定六件](https://github.com/08822407d/MyOS2/tree/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115) · [115原裁定](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MEMDEV-SCALAR-IO-115-review.md)

2. 适用 v2 协议 P1–P13。读完整63及以下约束。用 direct master 公约§1第2条原句和 time/master 的 options_flags.cmake、panic.c 两对金丝雀作分支核验；另核 time 实际 C 配置。仓内文件的提交标识只记连接器输出的前12位。[协议](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [28号](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md) · [63号](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md) · [79号](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md) · [110号](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md) · [124号](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)

3. 核四个 direct refs。lead 应为166正式回执给出的发布提交，exec=`145d6781c75e`，time=`a039d9803ade`，direct master=`e8e60cd19540`。只开放原 PR16 Ready、原题 PR17 Draft，原 head 分支、base master、均未合并。核116固定/活动六件全文与blob，初次117目标目录与六条路径均缺席。PR17顶层从第1页无 since 翻至空，核165发布、116作者、C08本轮116验收/117发布标记及ID各唯一，117所有作者正式标记为零。续入时核已有提交父链、内容、blob及评论，只补缺项。

## D00 · 在本批六件中追加三项纠正

以本提交的[116独立验收](reviews/CORE-USER-DIR-STREAM-LS-116-review.md)与[115补正裁定](reviews/CORE-USER-DIR-STREAM-LS-116-D00-115-review.md)为准。115/116原件及旧评论冻结。每项给出原位置、替代文字/表/图边、固定源ID、受影响的消费者与逐例复查结果。已闭合的 R115-01/02 引用裁定。

| 项目 | 可检查的关闭条件 |
|---|---|
| R115-03 | 在本批实际 DOT 新增115子图，落实116 facts 所列六组图修复，包括真实 ksys 调用者、验证与限长、外层写回、zero_fops 绑定、零次/一次/跨页分支。新同步助手保留 queued 特殊返回门。沿图走 n=0、4096、4097，分别核清零数、返回与位置。仅提交拟改边清单不足以闭合。 |
| R116-01 | 补单条、两条及三条成功记录的尾 d_off 来源，按 prev_reclen 初值、prev/current 地址、actor 写入、退出段及用户 filepos 依序推导。多条尾值的任何使用须有独立已初始化前提。修三处P29、仓根路径、宏中转、汇编标签/退出段和 invalid_name 证据边界，重核消费ID与定义内短窗。 |
| R116-02 | 在本批实际 DOT 新增116子图。改正表绑定与 run_cmd、opendir 内两个顺序调用、分配结果分支、maxread 与系统调用、内核有/无产出返回、过滤/打印/继续/NULL退出及实际 closedir 调用者。给出明确条件，覆盖正常、打开失败、缓存命中、0与负返回。A03分列 DIR.fd=−2 和有效fd数0，A04记录有效fd7仍未在此关闭。修复 B01/02“跳过/停止”，表、facts 与图逐例相等。 |

## A · 命令名和打开的路径

跟踪 shell_internal_cmd 的 cat 表项、run_cmd 的间接调用及 cat_command 的路径构造。限定当前目录为已终止字符串、argv[1]存在、文件名很短且分配成功。列 len、i、malloc 容量、清零区间、strcpy 写入结尾、条件写入斜杠、strcat 的位置、新结尾和 open 参数。用 `/` + `note` 与 `/docs` + `note` 逐字节推演路径，另检查 `/docs` + `/note` 拼出的字节，文件系统解析止于 open 接口。列成功得到有效fd及普通负返回两支。失败时给出错误格式的实际前缀 `ls:`、参数顺序、返回值与当时仍分配的对象。错误文本只按已知 errno 前提给值。[命令表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L44-L60) · [间接调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L139-L142) · [路径和 open](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L260-L275) · [追加](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcat.c#L26-L32) · [复制与结尾](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcpy.c#L27-L32)

## B · 两次定位与一次读取

限有效普通文件、长度0–32字节，位置及长度在所存类型中可表示，分配和用户缓冲有效。分别写 END/SET 的实参、返回类型、f_pos、i 的覆盖和第二次返回的消费方式，再接 malloc(i+1)、全范围清零和唯一的 read(fd,buf,i)。区分预估文件长度、请求字节、read 返回及后续 len 用途。核对用户库声明、生成宏及调用实例，核 VFS 进入条件、read_iter 优先及 FAT32_file_ops 的两个方法。用114和13作为已有依据；普通非空 FAT 读在选择 generic_file_read_iter 后止于其有界入口，不重开磁盘或整条页缓存分析。[两次 seek 和 read](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L276-L281) · [用户接口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/posix/unistd.h#L335-L370) · [FAT seek](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L278-L308) · [FAT 操作表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L354-L364) · [VFS 选择](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L147-L165) · [迭代入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/mm/vm_map/filemap.c#L263-L305)

在两次定位成功和普通有效缓冲前提下，分别列完整、短读、零返回和普通负返回的调用方模型。每例明确哪些字节由 read 提供、哪些保留预清零；负返回例另设缓冲没有变化。纸面返回由模型给定，内核实际可达性单列。未证的文件状态、失败定位或分配在首个缺失前提处登记，不由此构造运行输入。

## C · 格式化、输出请求与资源收尾

核 `printf("%s\n", buf)` 的 `%s` 长度、复制循环、格式中换行及 printf 向 stdout 对应 fd 提交的 write 长度。列 printf 的返回来源、write 返回是否消费、cat 自己的返回、run_cmd 是否接收。有效小输入确保格式化结果装入 printf 的缓冲区。依次演算 `ABC`、三个字节 `A/NUL/B`、预计4字节仅读到 `AB`、已有结尾换行的 `A\n`、空文件。写出读取长度、首个 NUL 位置、请求输出的精确字节序列和数量；另给 B 中普通负返回在已知缓冲未变时的结果。区分请求输出与终端实际显示。[收尾](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L278-L286) · [printf](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L11-L25) · [格式的字面字符](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L557-L574) · [字符串长度与复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783-L808)

资源表按阶段区分 filename、buf 与有效 fd。对 open 失败、普通成功及一次 read 普通负返回，列本函数是否调用 close/free、是否消费 close 返回、到达哪个 return；释放状态仅签本函数所见。源码问题依18号延期。阶段2只做静态取证和纸面计算。

## 六件、验收与发布

仅用原执行分支 `claude/dazzling-cori-q0dnyt`、原PR17。在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/` 严格新增 `MANIFEST.md`、`shell-cat-file-display.md`、`facts-and-boundaries.yaml`、`shell-cat-file-display-paths.dot`、`deferred-findings.yaml`、`evidence.md`。MANIFEST 含协议规定元信息、真实 G00、自检三数、来源清单、金丝雀、引用的公约原句及 write_retry_log。报告含 D00/A/B/C。facts 给前提、逐格状态/字节和源ID；DOT 将115、116补正和117新路径画成可单独追踪的子图，给出动作主体、边类型与守卫；deferred 给首个未证前提与恢复入口；evidence 区分新引文、历史引文与普通位置。旧材料用ID和固定位置，避免混入新账。

强制锚覆盖 D00 关键修复、cat 表绑定与调用者、路径构造/字符串助手/open分支、seek和一次read、FAT方法表与 VFS 选择、%s 长度和复制、字面换行、printf 的 write 及返回、close 和 cat 返回。每个新P2窗口在所标签定义内逐字1–5行；宏实例、公共声明、缺动作的完整函数用普通位置补足。P9①扫40位串，②逐字、归属与证明关系，③④核仓根路径、行界及每个消费ID，⑤重数六件正式前缀及三数，⑥核外部链接，⑦金丝雀，⑧公约和元数据，⑨⑩写区与文档范围，⑪实际运行状态，⑫降级路径和代码围栏，⑬强制锚覆盖。另按 D00 和 A/B/C 逐格复算、核表宽与实际图边。新来源软限约16份，旧证据按需复用，无引文或缺陷数量配额。完整本地P9、预处理、编译、链接、Graphviz、仓库程序、QEMU/实机均 `NOT_RUN`。

以 `145d6781c75e` 对应完整执行提交为唯一父，沿原树单提交六新增。移动ref前查真实头，expected_sha=已核父、force=false。核差异1提交、6新增、0改删，固定及活动六件全文和 blob 逐一相同。原PR17顶层仅一次作者正式交付，正文含下列标记一次。

MYOS2-LEAD-002-CHAT-EXEC-17 / CORE-USER-SHELL-CAT-FILE-DISPLAY-117 / FORMAL_DELIVERY

按ID两读评论，与无 since 分页至空所得正文核对，枚举所有117作者标记恰一次。复核四个直接头、PR16/17形态及写入结果。交回 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，R115-03、R116-01/02 仍 `OPEN`，然后 `STOP_WRITING`。交回原 C08 用 Pro 独立验收并同轮安排后继。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。

按服务端 Retry-After 或10/20/40秒记录实际尝试、回读、重试前头及等待。无重试则记 not_needed。
