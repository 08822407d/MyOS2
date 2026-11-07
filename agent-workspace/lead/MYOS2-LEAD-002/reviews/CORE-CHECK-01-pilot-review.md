---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-CHECK-01-PILOT-REVIEW-001
record_type: cloud_pilot_admission_review
conversation_display_name: "MYOS2-A-C02 内核分析主线"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
verification_packet_id: MYOS2-LEAD-002-CORE-CHECK-01
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
produced_by_source: "沿用 Owner 告知的网页端显示名；不是本轮读取客户端或认证后端。"
date: 2026-09-27
base_snapshot: "kernel=time；workspace=master；review_write=agent/MYOS2-LEAD-002（均为分支名；核对用短标识见下）"
evidence_class: "GitHub 远端原件读取、脚本与输出可读复核、准入裁定；主线未重跑程序"
read_channel: connector
status: PILOT_REVIEW_COMPLETE_CORE_LAUNCH_READY
disposition: ALLOW_CORE
reviewed_pr: 17
reviewed_branch: claude/dazzling-cori-q0dnyt
reviewed_commit_short12: 0851af4fc08b
pilot_result_commit_short12: 10ecb7dd0bdb
transport_marker: MYOS2-CLOUD-PILOT-20260925-K7P4
reviewed_input_refs:
  workspace: {branch: master, short12: de3bb1df906a}
  kernel: {branch: time, short12: a039d9803ade}
  taskbook: {branch: agent/MYOS2-LEAD-002, short12: 57a7c3e0eebf}
short_identifiers_source: "从连接器 PR/compare 返回与被审记录原样复制；未补全。"
inputs_read:
  - "PR17@0851af4fc08b: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/MANIFEST.md"
  - "PR17@0851af4fc08b: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml"
  - "PR17@0851af4fc08b: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/evidence.md"
  - "taskbook@57a7c3e0eebf: agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md"
  - "taskbook@57a7c3e0eebf: agent-workspace/lead/MYOS2-LEAD-002/11-core-verification-cloud.md"
  - "taskbook@57a7c3e0eebf: agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md（文件头及范围）"
  - "time@a039d9803ade: mykernel/scripts/options_flags.cmake（全文）、mykernel/debug/panic.c（定义起点至金丝雀及其后段）"
  - "master@de3bb1df906a: mykernel/scripts/options_flags.cmake（全文）、mykernel/debug/panic.c（定义起点至金丝雀及其后段）"
  - "PR16/17 元数据与全部变更路径；远端分支比较"
input_scope: "pilot 三文件、10/11 全文；没有重新审阅 07 全部源码锚点，没有读取其他项目的误发材料。"
core_execution_started_by_lead: false
owner_continuation_required: true
merge_required_before_core: false
kernel_acceptance_verdict: NOT_ISSUED
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待本地；本主线未运行命令、解析器、哈希或测试。已发生的云端试跑由执行者记录，见本文限定审查。"
open_questions:
  - "云端执行模型选择未能认证；所述平台限制只记作执行者的解释，不推广为提供商通用政策。"
  - "真实内核、Owner 本机 ELF、IRQ/计时/SMP 均未由 pilot 验证。"
---

# 结论：试跑证据足以进入正式核验，不必重做 pilot

**ALLOW_CORE。** PR #17 的三个实际文件已经完整读到：环境与输入、两条真引文和故意错误引文、C 程序正负控、单子进程超时以及 GitHub 远端回读都有对应脚本和结果。主线还独立从 GitHub 取得这些文件，复查了源文件中的两条分支差异及提交范围。

这是对“现有执行环境与回传方式足以开始有界核验”的裁定，不是十五项正式检查已经完成，不是 MyOS2 已可运行或通过验收。当前只缺 Owner 在原云会话发送一次继续指令；不需要合并 PR，不需要下载上传附件，不重新开研究。

## 1. 对应 P00-P03 的核查

| 项 | 实际读到的材料 | 主线结论及边界 |
|---|---|---|
| P00 | evidence §1/§2 的预检、版本查询、Git 对象读取、分支短标识和 ls-remote 比较；result.yaml 同步记录 x86_64、Python、GCC/Clang、Git、PyYAML | 足以证明此执行者提供了可检查的环境记录。当前 master 与被审起点一致，time 比较亦一致；不是主线亲自运行版本查询。 |
| P01 | 从原 MANIFEST 解析引文；按字节整行/子串比较；真引文 time 各命中 1 次、master 为 0；篡改的 kernal 引文不命中 | 脚本没有预写匹配结果。主线重新打开对应源文件，所见分支差异与报告一致。只有两条金丝雀，不等于 47 个正式锚点全部合格。 |
| P02 | 完整 smoke.c、编译参数、run_limited 和驱动；实际 stdout、退出码、时间、进程回收字段 | 成功用例返回 0，故意不相等返回 7；睡眠探针被限时结束，记录 -9、回收和 /proc 不存在。源码逻辑与日志相符，足以开展普通用户态片段核验；未测试真实内核。 |
| P03 | result.yaml 先在 10ecb7dd0bdb 提交；两个 HTTP 200 回读与该提交比较；后续提交 0851af4fc08b 只新增两份文档 | 已形成实际远端交付。主线远端比较确认第二次提交没有改 result.yaml，并读到了全部三件。SHA-256/逐字节比对是执行者日志，主线没有自行算哈希。 |

PR17 相对当前 master 的净变更恰是三份 pilot 文件：MANIFEST.md 93 行、evidence.md 1363 行、result.yaml 187 行，均为新增，合计 1643 行、零删除。这里的行数来自连接器 diff 统计，不是源码锚点或主线运行的计数程序。未发现内核、公共规则、其他任务路径或主线任务文件被该 PR 修改。

PR 正文另报告第二次推送后的三文件 API 回读；该段未附完整驱动/原始输出，故不把它升格为主线独立机械认证。P03 已有的 result.yaml 回读与当前可读取的三份 GitHub 原件足以满足本次准入。

## 2. 保留的限制，不要求 Owner 为此补一轮问答

**执行身份。** 产物将具体模型选择记为 unknown_or_not_attestable，没有冒用主线 GPT 作者。关于“平台不许写模型名”的说法，主线仅记录为执行者自述，不当作已核实的产品通用限制；本次准入不需要 Owner 补截图或再询问模型。后续沿用现有会话的允许配置，受限制的字段继续明确未知。

**输入与证据保全。** evidence §1 明示部分命令输出为手工转录；build_evidence.py 还对嵌入文本做 rstrip 换行处理。因此该文件是可重建检查逻辑的证据汇编，不是整个会话日志的逐字节备份。其现有原件不改。core 的源文逐字检查仍须直接针对冻结 Git blob，不能改用这些围栏摘要作源文；若导出时规范化了换行，要记录变换，不能称原字节。

**有限放行。** 旧核心结论仍是待验证假设。pilot 成功不排除正式核查反证，不能将所有案例预填为通过，也不能据云环境架构字段宣布 x86 原 asm 已运行。

## 3. 正式执行时在同一轮内处理的小型加固

以下针对已经读到的驱动边界，属于落实 11 §4 的有界执行与交付要求；不另开 pilot、不改变 V00-V14 的业务判据，不修改冻结的 pilot 原件。

1. **超时覆盖范围。** 现有 run_limited 的超时分支只调用 p.kill 后无超时 communicate；本次探针只包含一个 sleep 子进程，没有验证派生子进程和继承管道的收尾。core 不得把它视为完整进程树隔离保证。对编译器/可能派生子进程的命令，使用本任务独立进程组或等价隔离、有限的终止与输出收集，并保留一次有限的父子睡眠负控及实际回收证据。不得杀会话中无关进程。无法可靠限制时，该动态例 BLOCKED，继续独立的静态检查。
2. **失败不能继续运行旧产物。** 当前 P02 驱动在编译失败后仍会进入运行循环；本次编译实际成功，未影响既有结果。core 编译失败或超时后，应记录对应 BLOCKED/失败，不运行旧二进制，不让未捕获异常吞掉整批结果；独立案例继续完成。
3. **远端回读判据。** P03 的最终 ok 主要合并字节相同和 marker，未把 curl 退出码、HTTP 状态与超时全部纳入布尔判据。本次日志这些值确为 0/200、无错误，因此不退回既有成功观察；core 驱动须显式同时检查传输成功、未超时、目标 Git 对象、字节和解析结果。禁止用缓存的旧工作树冒充远端回读。

改进只写 core/ 的证据与夹具文本，或新临时目录；不写 pilot/。可在本轮正式工作开始时完成加固及小负控后，直接继续 V00-V14，无需再等主线单独批准驱动。

## 4. 准入绑定、冻结输入与续做规则

首次 core 启动时，先核对本回执的 packet_id、PR #17、执行分支 `claude/dazzling-cori-q0dnyt` 及已审 pilot 头 `0851af4fc08b`。若已存在 core 的后续提交，则必须证明它是同一执行线的续做，且三份 pilot 原件未改；不能因正式提交推进了 HEAD 就机械要求重新做 pilot，也不能接受无法解释的别处输入。

本轮主线会在旧任务包分支上新增本回执和续接检查点。**新增回执导致主线分支头改变是预期动作，不是任务输入漂移。** 以 `57a7c3e0eebf` 的 07 报告、07 YAML、原 MANIFEST、09、10、11 为原技术输入；回执另取最新已发布版本。开始时按文件比较这六份原技术输入，新头若仅增加本次回执/续接记录，不构成阻断。任何更改原技术输入内容的差异才需要逐项评估；影响案例语义则暂停该项并保留差异。

源码仍取 time。本次主线比较 time 与 `a039d9803ade` 相同，workspace master 与 `de3bb1df906a` 相同；执行时再次记录实际身份，读取期间固定到取得的对象，不用移动 ref 拼接证据。全过程只输出复制的短标识。

工作分支仍为 `claude/dazzling-cori-q0dnyt`；既有 PR #17 继续作为唯一执行 PR，base=master。结果只新增于 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/`，按 11 输出 MANIFEST.md、results.yaml、evidence.md。已交 pilot 三文件保持不变。主线只写 lead 目录，双方不得替对方写入。

原 10/11 中“等待主线回执”的门由本件闭合；创建时写下的“尚无回执/试跑未收到”是历史状态，不要求执行者因此自我阻塞。十五项技术要求、主线后续审查、禁止改内核和需要 Owner 转发继续的条件均不变。旧 09 的手工附件回传指令不恢复生效。

## 5. Owner 一次操作（现在可执行）

打开生成 PR #17 的原 Claude Code 云会话，保持原模型/effort/费用配置，不另开 Research、不启用自动修复、不增加付费。发送：

```text
继续 MYOS2-LEAD-002-CORE-CHECK-01 的 core 阶段。
先完整读取主线分支 agent/MYOS2-LEAD-002 下：
agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md
确认 ALLOW_CORE 对应 PR #17、分支 claude/dazzling-cori-q0dnyt、pilot 头 0851af4fc08b。
再读取同一主线目录的 11-core-verification-cloud.md，并按回执和 11/09 完成 V00-V14。回执第3节的驱动加固在本轮内先做，不另停下来等待；新增回执造成主线头变化按第4节处理。
只在原执行分支的 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/ 新增完整结果、证据和可重建夹具，更新同一个 Draft PR #17。保留 pilot 原件；不向 master、time 或主线分支推送。
不改内核，不运行原仓库脚本、完整内核构建或 QEMU，不安装工具、不提权、不输出凭据。逐案如实记录执行/未执行、反证及保真限制；单项受阻不停止其他独立检查，不用自编模型冒充原函数运行。
完成后只回 PR #17 链接、结果提交短标识和简短结论，不让我搬运文件；不合并、不删除分支。
```

原会话入口见 PR #17 正文中的 Claude Code session 链接。若原会话不能恢复/不能推回原分支，保留已有 PR，报告具体限制；不私自新开竞争分支或迁移产品。读取本回执失败、输入身份冲突或越出白名单也须停止受影响动作并返回可检查证据，不索取 token。

结果完成后 Owner 将同一 PR 链接或“PR17 已更新”发回当前主线即可。本主线直接读库检查，不要求报告附件、全部对话转录或重复完整回复文件。

## 6. 发布与审查边界

本回执由 GPT 主线对另一执行会话提交的代码/日志作可读复核；来源产品与上下文分离，但精确模型身份未知，不据此认证模型独立性或程序实际执行的不可伪造性。没有由本主线重跑、没有人类逐行审阅证明。

PR #17 必须保持 Draft、暂不合并；分支保留到 core 回收、主线审查完成并明确解除。PR #16 是主线文档提交，与执行 PR 不相交，是否合入不是继续门。本件没有签发内核 PASS，也没有启动云端会话或消耗其额度。

可追溯入口：
- 被审 PR：https://github.com/08822407d/MyOS2/pull/17
- 冻结结果：https://github.com/08822407d/MyOS2/blob/0851af4fc08b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml
- 冻结证据：https://github.com/08822407d/MyOS2/blob/0851af4fc08b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/evidence.md
- 技术任务：https://github.com/08822407d/MyOS2/blob/57a7c3e0eebf/agent-workspace/lead/MYOS2-LEAD-002/11-core-verification-cloud.md
