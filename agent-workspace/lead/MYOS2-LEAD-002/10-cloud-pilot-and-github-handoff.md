---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
verification_packet_id: MYOS2-LEAD-002-CORE-CHECK-01
phase: pilot
record_type: cloud_execution_pilot_and_transport_contract
evidence_class: "Owner 当前执行面/回传偏好；官方产品文档；主线试验设计，未执行"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-25
base_snapshot: "workspace=master；taskbook=agent/MYOS2-LEAD-002；kernel=time（均为分支名）"
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md
  - agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md
  - "Mnemosyne：本轮定向复读的操作布局、跨对话执行意图、文件交付、PR 谱系与运行归属指导；未接管其建设任务"
  - "本文 §7 官方页面"
status: READY_FOR_OWNER_PILOT_LAUNCH
execution_disposition: RUN_NOW_OPTIONAL
external_execution_started: false
formal_execution_disposition: READY_NOT_SELECTED
supersedes:
  - agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md
supersedes_scope: "本专项的执行地点和回传方式：云端优先、GitHub 回传；不替代旧包 V00-V14 的技术检查项，不修改公共协议。"
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待执行面；本主线未运行命令、编译、脚本或云端会话。"
open_questions:
  - "Owner 账号的赠额适用范围、余额和有效期未由本主线认证；不在公开仓库记录账单细节。"
  - "具体云端会话的架构、工具与 GitHub 推送权限须由本试跑验证。"
  - "正式阶段仅在主线检查试跑结果、写出 ALLOW_CORE 回执，并由 Owner 转发继续指令后启动。"
---

# 当前只做一次小试跑：读对版本、真正运行、经 GitHub 回传

**现在只启动 pilot，不运行正式 V00-V14，不修内核。** 本文件与后续 `11-core-verification-cloud.md` 属于同一个既有核验包的两个阶段；不是两次深度研究。试跑结果直接提交 MyOS2 的独立执行分支，本主线读取远端文件检查。Owner 不搬运报告附件。

## 1. Owner 操作流程（现在可选执行）

1. 在浏览器打开 https://claude.ai/code，登录获得赠额的账号。先在赠额通知/账号用量界面确认本入口适用及未过期；不为试跑新增付费、自动充值或 API key。官方页面不能证明个人账号实际享有哪项优惠。
2. 完成该页面的 GitHub 连接，选择且仅附加 `08822407d/MyOS2`，启动基分支选 `master`。不选本主线写分支，不需要先合并 PR #16。若授权界面要求选择仓库，只开放本任务需要的仓库。
3. 环境选 Anthropic 托管的 Cloud / Default，不选 Local 或 Remote Control。保留 Trusted 网络；环境变量、API credentials 和 Setup script 留空，不启用 Auto-fix 或定时运行。本任务不需要安装依赖。
4. 建议模型选界面中的 `Opus 5.5`，effort 有选项时选 `medium`；若赠额限定其他模型或界面没有该项，使用赠额适用的默认编码模型并记录实际选择，不额外购买。权限模式选 `Accept edits`，不是会停在计划阶段的 Plan。权限模式不替代下文路径白名单。
5. 将下面的试跑指令完整发送到这个新云端 Code 会话。无需上传历史聊天、九份研究附件或私仓资料。试跑与正式阶段继续用同一个会话，不另开同题并行任务。
6. 完成后将返回的 PR 链接发回 `MYOS2-A-C02 内核分析主线` 即可。若分支已推送但自动创建 PR 失败，可在任务 diff 顶部点 Create PR，核对 base 为 master、head 为执行分支，选择 Draft；也可只把实际分支名发回，本主线能先读分支。不要合并或删除执行分支。

可复制启动指令：

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 pilot 阶段，暂不执行正式核验。
先完整读取：
https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md
按该文件完成 P00-P03。源码只读 time；任务说明从 agent/MYOS2-LEAD-002 读取；工作基分支为 master。
本次允许使用平台分配的独立会话分支，只在 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/ 新增结果并推送、开一个 Draft PR 到 master。不得向 agent/MYOS2-LEAD-002、master 或 time 推送。
不改内核，不运行原仓库脚本、完整构建或 QEMU，不安装工具、不输出凭据。结果和实际命令/输出经 GitHub 回传，不要求我下载上传文件。
完成后只告诉我 PR 链接（或已推送的真实分支名）、试跑状态和阻断项，然后停止，等待主线检查。
```

若授权、工具或网络失败，只作一次同原因重试并保存明确错误；不要反复开新任务或要求 Owner 提供 token。若连结果也不能推送，只回报失败阶段、非敏感错误及现有会话/分支信息，不声称 GitHub 回传完成。

## 2. 权限、基线与数据安全

本次 Owner 转发上面的指令才启动外部执行。其明确限定的结果写入授权，取代旧 09 包“不提交、不建 PR、要求 Owner 上传附件”的运输条款；仅对本专项允许平台分配的会话分支，不修改公约的通用命名规则。主线自己的分支仍是 `agent/MYOS2-LEAD-002`。

执行前读取 `master:agent-workspace/conventions.md` 和 `master:agent-workspace/tasks/00-gpt-task-protocol-v2.md`。保留其证据、原件不改、禁止完整提交标识与源码保护要求。无执行环境的 GPT 专属措辞不能被反过来用作禁止记录真实云端执行的理由；所有执行描述必须限定为本次云端进程，不宣称内核运行通过。

执行者先确认实际工作分支与基分支，枚举本仓 open PR 和相交路径。PR #16 是主线文档交付，不是本执行任务的结果 PR；它不得被云端执行者写入。若已有本核验包的执行 PR，优先回原会话续做；平台不能推那个分支时停止，不创建竞争分支。首次试跑可使用平台自动分配的会话分支；不要为了满足旧 agent/ 前缀而改名或绕过平台推送限制。

工作区只在指定 pilot 结果目录新增文件。源码通过读取 time 的 Git 对象或解出独立临时副本取得，禁止将工作分支切换成 time，禁止把整棵 time 合并到结果分支。任务包分支也只读，不合并到执行分支。保留 master、time、任务包三份本次读取身份，用工具给出的 12 位短标识和读取时间记录；不以分支名替代取数一致性检查。

不运行仓库安装、分区、写盘、原 QEMU 启动脚本；不构建完整内核，不启动服务/容器，不提权、不安装软件、不修改 `.github/`、`.claude/`、CLAUDE.md、其他任务成果或本地用户工作树。临时试验仅用新建目录，目录不自动删除。读取仓库自动执行配置时发现会触发上述动作则记录阻断，不主动执行。平台自身在提示词前发生的初始化不得冒称已由执行者审计。

不打印完整环境、认证 token、认证配置或私有资料，不给 GitHub 扩权。仅使用平台已有 GitHub 认证。公开产物和复制的日志不得包含 40 位连续十六进制串；Git 内部对象数据库不是报告。需要精确文件摘要时可将 SHA-256 按四段 16 位记录为数组，注明拼接规则，不能删改原始证据后冒称逐字。命令 stdout/stderr 的敏感内容须明确标为脱敏，不称原样完整日志。

## 3. P00-P03：简单、有限、可检查的试验

### P00｜环境与输入身份

记录操作系统、CPU 架构、Python 3、GCC/Clang、Git、可用 YAML 解析器的实际版本/是否存在；只调用选定工具的安全版本查询，不枚举凭据。不安装缺少的工具。记录实际云端工作分支及 master/time/任务包的短标识。预检必须能获取指定三个分支的材料；文档所列工具不等于本会话已经实测可用。

### P01｜两个真实分支引文，加一个故意不匹配的负控

只读取 time 和 master 的 `mykernel/scripts/options_flags.cmake` 与 `mykernel/debug/panic.c`，以及任务包分支的 `lead/MYOS2-LEAD-002/MANIFEST.md`（路径前加 `agent-workspace/`）。从清单读取 branch_canary_quotes，不凭提示词重新造预期。

用小程序按字节/连续行检查 time 的两条引文是否命中，是否与 master 区分；真实制表符不能换为空格后声称逐字一致。对 time 文件另用一条人为篡改的引文作负控，预期不命中。必须先确认篡改文本确实不同并不在源文中。

报告逐项实际值、源路径、引文、比较方法及退出状态。若 master 后来同步了源码而金丝雀失去区分能力，写 CANARY_NO_LONGER_DISCRIMINATES，不能因此猜测读错分支，更不能伪造“不命中”。这是正式阶段准入待解决项。

### P02｜真正编译运行一个小程序，并检查失败能否被捕捉

此项是 ENVIRONMENT_SMOKE，不是 MyOS2 原函数验证。由执行者在临时目录写一个小 C 程序：用两个 volatile 整数 2 与 3 相加，打印实际值；通过参数传入期望值，实际相等时返回 0，不等时返回 7。完整保留该小程序及执行驱动代码在 evidence.md 的围栏内。

使用已安装的 GCC 或 Clang 实际编译。运行两次：传期望值 5 应打印实际值 5 且退出 0；传期望值 6 应打印实际值 5 且退出 7。驱动必须核对输出与退出码，不用“编译成功”替代运行，不用吞掉非零退出码的做法伪造成功。

用 Python 的子进程超时或已存在的等价工具限制每个编译/运行；编译上限 30 秒，每个小程序上限 5 秒。另用一个仅睡眠 2 秒的 Python 子进程、0.2 秒超时验证驱动确实捕捉超时并回收子进程，记录 TIMEOUT_EXPECTED；不制造永久循环。缺编译器/驱动则如实 BLOCKED，仍完成能做的取数与回传。

本试跑不做 x86 特权指令、内核代码执行或并发压力测试。CPU 架构不是 x86-64 时，P02 仍可验证普通 C，但正式 V12 的 x86 原 asm 暂不具备执行前提。

### P03｜GitHub 回传与独立读回

此次传输识别串固定为 `MYOS2-CLOUD-PILOT-20260925-K7P4`，不是密码或执行真实性证明。

先生成并提交 `pilot/result.yaml`，含识别串和 P00-P02 的真实结果；只推到本次会话分支。随后从远端 GitHub API/raw 或重新获取的远端对象读取该文件，与已提交版本比较字节。报告读取通道、是否一致和真实分支；不能把工作树本地重读当远端回读。

`result.yaml` 在这次比较后冻结，不再补写回读状态。将 P03 的回读结论写入另外的 MANIFEST.md/evidence.md，再提交推送；这样避免“写入回读结果又改变被核对文件”的自引用问题。最后重新检查 PR 净变更只含允许目录。无需把远端最终提交号写回自身文件；在会话最后回报短标识即可。

创建或复用唯一执行 Draft PR，base=master，标题以 `MYOS2-LEAD-002-CORE-CHECK-01: pilot` 开头。Draft 是因为正式核验尚未执行且该 PR 将继续承载 core 结果，不是要求 Owner 逐行审稿。PR 内注明：暂不合并；保留执行分支至 core 回收、审查完成并明确解除。推送失败不扩大路径或更换凭据绕过。

## 4. 固定交付：只需三个公开、可回读的文件

目录：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/`。

| 文件 | 必须包含 |
|---|---|
| MANIFEST.md | 范围、三个文件清单、实际模型/执行面、P00-P03 结论、远端回读范围、阻断项；结论仅 PILOT_CANDIDATE 或 BLOCKED。 |
| result.yaml | 识别串、实际输入短标识/字节信息、架构/工具、正负控与超时结果；P03 在本文件形成后才做，不在此预填成功。 |
| evidence.md | 所有试验脚本/小程序的完整内容、实际命令、stdout/stderr、退出码、P03 回读与提交范围证据；精简但足以复核。 |

所有文档有 YAML 头，YAML 文件本身从元数据字段开始。执行者的 produced_by 按实际可见模型选择记录；不可见写 unknown，不能抄任务书作者。`model_per_owner: gpt6` 若保留，必须同时写 `model_per_owner_scope: originating_lead_only_not_executor`，并另写 `execution_model_selection` 与其来源。实际运行记录属于 Claude 执行者，不属于 GPT 主线；未知后端保持 unknown_or_not_attestable。

结构化结果至少有 `packet_id`、`phase`、`transport_marker`、`source_refs`、`environment`、`checks`、`evidence_refs`、`limitations`。每个 check 写实际结果与预期是否相符；失败与未执行不混为一谈。不需要导出整个对话或再造一份 complete-response 文件，最后的聊天回复只作 PR/分支导航。

## 5. 主线如何检查；为什么必须在这里停一次

本主线收到 PR 链接或真实分支名后，直接从 GitHub 读取三个文件、diff、提交范围及日志，核对识别串、两条引文、故意失败案例、真实编译/进程退出和远端回读。可读审查不是本主线重新执行了程序，也不是硬件级运行证明。

之后新增主线回执：`agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md`。本文件目前只是规划该路径，不声称回执已存在。回执应含 packet_id、被审执行分支、pilot 提交短标识、输入身份、ALLOW_CORE / RETURN_PILOT / BLOCKED 及理由；不使用无范围的 PASS。

只有该回执明确 ALLOW_CORE，且 Owner 在原云端会话转发继续指令，才读取并执行 `11-core-verification-cloud.md`。不依赖试跑 PR 或 PR #16 合并，不让执行者自审即自动放行。本次停点是用户明确要求的跨执行面实测与主线检查，不是把可自动做的子步骤拆成多次 Pro 回合。

## 6. 设计取舍与现行入口变化

旧“待本地”的实质是需要有命令/编译能力的执行面，不是必须使用 Owner 的物理电脑。当前试跑验证云环境能否承担其中的机械核验；真实用户 ELF、启动盘、物理硬件/IRQ/SMP 行为仍不能由云端普通进程代证。

选择 GitHub 文件+独立执行 PR，而不是手工附件，符合 Owner 本轮明确偏好；不使用自动 PR 监听/Auto-fix，避免审核评论意外变成改内核授权。源文档与执行结果分属两个不相交写区，双方都只写自己的区域。公共协议、旧 09 与旧检查点原文保留，当前运输与操作入口以本文为准。

本主线本轮仅准备任务并写入指定分支，没有启动 Claude、使用外部额度、执行命令或声称赠额已验证。关于个人额度的说明仅留在当前对话，不写入本公开技术任务。

## 7. 官方来源及读取界限

[EXTERNAL https://code.claude.com/docs/en/web-quickstart] 官方 quickstart 说明在隔离云环境中从 GitHub 开始任务并推送分支；本文据此选择浏览器 Code 入口。模式与按钮以该页面的 Start a task、Review and iterate 为参照。

[EXTERNAL https://code.claude.com/docs/en/cloud-environments] Installed tools 列出 Python、C/C++ 编译器和 Git 工具；GitHub proxy 段明确：`git push` 只适用于会话当前工作分支。原文短句："git push works only against the session’s current working branch"。这是保留平台分支而不要求云端写主线分支的原因；具体会话仍须试跑。

[EXTERNAL https://claude.com/blog/claude-opus-5-5-built-for-coding-sessions-that-use-more-context] 用于确认本轮所建议模型的公开名称，不用其一般性能描述证明本任务可通过，也不据此推测个人赠额条款。

上述页面访问日期为 2026-09-25；仅保留链接和限定摘录，没有建立网页逐字节长期快照。试跑设计及阈值是本主线的选择，不冒称为平台承诺。
