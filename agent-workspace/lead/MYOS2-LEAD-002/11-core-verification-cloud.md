---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
verification_packet_id: MYOS2-LEAD-002-CORE-CHECK-01
phase: core
record_type: cloud_core_verification_taskbook
evidence_class: "既有 V00-V14 的云端适配与 GitHub 交付设计；未执行"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-25
base_snapshot: "workspace=master；taskbook=agent/MYOS2-LEAD-002；kernel=time（均为分支名）"
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md
  - "07 报告、07 YAML 和本批 MANIFEST 的既有已读内容；本轮不宣称重新全文审查内核"
  - "官方云端工具、GitHub 和执行环境文档，见 10 文件 §7"
status: READY_NOT_SELECTED
execution_disposition: RUN_AFTER_GATE_OPTIONAL
external_execution_started: false
required_gate: "主线试跑回执 ALLOW_CORE + Owner 在原执行会话转发继续指令"
supersedes: agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md
supersedes_scope: "覆盖旧包执行地点、旧启动块、禁止结果提交与附件回传方式；V00-V14 技术要求及证据分类继续引用旧包，不改旧文件。"
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "全部待执行面；本主线没有运行本包。云端可检项与真实本机/内核运行项分列。"
open_questions:
  - "试跑未实际回收；当前不能宣布正式云端核验获得准入。"
  - "用户本机 ELF、真实内核上下文切换、硬件计时和 SMP 不由云端宿主夹具认证。"
---

# 正式核验：保留十五项检查，不把云端结果冒充本机内核结果

**当前不要启动本文件。** 先完成 `10-cloud-pilot-and-github-handoff.md`，由主线读库检查；主线回执允许后才进入本阶段。本文把已冻结的技术检查与执行地点分开，所以不用等试跑回来再花一轮 Pro 重新写十五项要求。

## 1. 准入和 Owner 操作流程（试跑获准后）

仍用试跑的 Claude Code 云端会话、工作分支与唯一执行 PR；不另开同题会话，不先合并试跑 PR。模型建议 Opus 5.5、medium；沿用试跑中赠额适用且实际记录的选择，不静默切换或升级计费。

主线预定回执路径：`agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md`，读取分支 `agent/MYOS2-LEAD-002`。在当前写作时该回执尚未生成，不能视作已通过。必须核对回执的 packet_id、pilot 执行分支、被审提交短标识、输入身份和 disposition。仅 `ALLOW_CORE` 可继续；缺失、退回、无法核对或身份不一致时只回报，不执行 core。

回执获准后，Owner 在原云端会话发送：

```text
继续 MYOS2-LEAD-002-CORE-CHECK-01 的 core 阶段。
从 agent/MYOS2-LEAD-002 分支先读 reviews/CORE-CHECK-01-pilot-review.md，再读 11-core-verification-cloud.md；两者均在 agent-workspace/lead/MYOS2-LEAD-002/ 下。
仅在回执 ALLOW_CORE 且对应本次 pilot 分支/提交和输入身份时执行。按 11 与旧 09 的 V00-V14 一次做完能够可靠完成的检查，不修内核，不启动虚拟机。
在同一执行分支的 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/ 新增结果并推送，更新同一个执行 PR；不向 master、time 或主线分支推送，不合并、不删除。
结果、逐例证据和可复现夹具直接入 GitHub；最终只给我 PR 链接和简短结论，不让我下载上传报告。
```

完成后 Owner 只需把同一个 PR 链接发回本主线。本主线直接查 diff、结果与证据并回写审查；无需用户手工复制日志或解释结果。评论/任务完成不会自动触发本主线在后台运行。

若原云会话丢失，先保留已推送分支与 PR；不能推回同一分支时停止并报告，不另造竞争 PR，不自动迁移到本地或消耗第二种产品额度。

## 2. 输入读取与角色边界

正式读取顺序：主线准入回执 → 本文件 → 10 的权限/运输规则 → 09 全文 → 07 报告 → 07 YAML → 本批 MANIFEST → 需要核查的 time 源码。文档均在 `agent-workspace/lead/MYOS2-LEAD-002/`，前三类来源从 `agent/MYOS2-LEAD-002` 取得；规则文件用 master；源码只用 time。

需要的三个原始分析文件是：

- `07-scheduler-wakeup-timer-audit.md`
- `07-core-audit-map.yaml`
- `MANIFEST.md`

旧 09 的 §3 证据类型与 §4 V00-V14 为技术合同；§1 的旧发射块和 §5 的单附件运输要求不执行。旧“不能写 GitHub”改为“只允许本专项 core 结果白名单及执行 PR 元数据”；其余不改内核、不安装、不运行完整构建/原脚本/虚拟机等边界继续有效。

默认不读十八个旧聊天、不催旧 DR 正文、不读私仓 Mnemosyne/Alaya、不重新做全部九课题。主线的七组推导是待验证假设，不是必须迎合的 oracle。发现反证就记录反证。

所有输入在阶段开始时固定到实际取得的 Git 对象。报告只写工具复制的 12 位短标识；进程内部读取使用固定对象，不反复用移动分支名拼片段。若与 pilot 准入身份不同，比较受影响文件；只要影响检查语义，暂停受影响项并报告，不能自称沿用旧准入。文档自身变化也须核对。

工作分支始终是平台的执行分支。通过读取对象或临时解出源码进行核验，不 checkout 到 time，不将 time 或主线任务分支 merge 到执行分支。开始与最终推送前均核对 open PR、写路径相交和基分支变化；任何越出白名单的净改动都必须停止发布，不能用 reset/clean 掩盖。

## 3. 每项在云端能证明到哪一层

| 项目 | 本阶段要求 | 不可外推部分 |
|---|---|---|
| V00、V01 | 逐字引文、符号边界、元数据/YAML、引用和计数；先做独立工具核查 | 不证明内核行为正确，也不直接接受自报 47。 |
| V02、V03 | 单等待者、二次通知/多等待者的原函数片段和数据结构不变量 | 需隔离/有界，不在错误 anchor 上继续非法访问，不证明真实抢占。 |
| V04-V08 | 串行唤醒、状态匹配、防重、CPU 元数据、选择器与 idle 边界；可保真时执行 | 不能用人为构造但实际不可达的状态宣告运行故障，不证明 SMP。 |
| V09、V10 | 有限 timeout 进展与无限/已完成快路径分开核对；可保真时执行 | stub timer 不等于真实到期分发，不能说所有 completion 失效。 |
| V11 | 等待、通知、收尾及对象生命周期的局部序列 | 无真实上下文切换则其动态层 NOT_RUN，保留有根据的静态/局部结果。 |
| V12 | CPU 架构和编译器满足时执行原 x86 算术 asm 片段及指定向量 | 非 x86-64 或约束不支持则只做源文核查；不引入模拟器冒充原执行。 |
| V13 | 原 trylock 片段的返回值、所有权状态、重复尝试；公开全部 stub | 串行片段不能代替多核压力或全套 spinlock 正确性证明。 |
| V14 | 核查两次计数操作、链接别名与构建引用；已有可信 ELF 时另列观察 | 云端没有 Owner ELF 就标未提供，不要求上传，不新构建内核，不认证时钟倍率。 |

云 VM 只是宿主进程的运行地点；可以产生真实编译/进程执行证据，但这不是 MyOS2 自己在 CPU 上运行的证据。本任务不要求 QEMU/KVM、嵌套虚拟化、UEFI 固件、用户启动盘或任何硬件接口。

## 4. 防止“检验程序通过了，但检错了对象”

### 引文与语义

V00 对每个 A 编号分别给：源路径、源类型、原样连续引文是否命中、符号定义体范围是否成立、引文是否支持对应命题。不得用单次字符串搜索成功替代函数/宏/结构/配置定义边界证明。C/汇编/CMake/链接脚本/顶层 shell 的定位方式不同；无法可靠定位就单列失败或阻断，不自行放宽 P2。

07 中 A 编号按语义出现而非严格数值排序，计数应按实际标签与 ID 集合去重/查重，不能按最后一个编号推总数。引文中的 tab、换行、反斜杠不做“看起来等价”的归一化。语义近似可作说明，不能填逐字命中。

V01 按原 MANIFEST 的 `scope_files` 六文件统计；该清单明确是旧批次，不是整个 lead 目录。不得把本 10/11 任务书、后续执行报告或历史其他文件混入旧自检分母。对执行产物另做独立自检。解析 YAML 使用已安装解析器；缺失则标相应项 BLOCKED，不把手工挑出的字段当完整解析成功。

### 可复现执行与替换依赖

每个 HOST_ORIGINAL_SLICE 案例必须保存可重建的完整测试夹具、原函数抽取方式、所用类型/常量及所有替换依赖。可将完整代码放 evidence.md 围栏，由脚本在临时目录提取执行；不要只交一段“关键代码”而丢掉能改变结果的 stub。

声明原函数/宏保留范围以及任何签名、链接属性、include、类型布局、内存访问和锁语义改动。保真性不足时降为 MODEL_ONLY 或 STATIC_COUNTEREXAMPLE；修改了待测函数的算法就不再是原函数测试。不能先修错再跑，然后说旧代码没有问题。

记录命令、工具版本、输入、stdout/stderr、退出码、关键实际值；可以重复一次以检验稳定性。案例驱动同时检查正常与故意失败的预期，使用 pilot 已验证的有界子进程机制。单个夹具编译上限 60 秒、运行上限 5 秒；有界状态序列不超过 100 步。任何超时先终止该案例、保留证据并继续独立案例，不无限重试，不以超时自动推断某个具体 bug。

V12 的算术值和布尔返回都需检测，不能只看到 sub 指令名即填运行结果。V13 要记录首次/再次调用前后的原始锁状态；锁初始化或读取 stub 不可预先把预期现象写死。V06 允许得到无失败的串行结果；V08 要主动检查 idle 前提可能构成的反证。

V09/V10 的调度替代函数若只增加调用计数，要明确不能表示真实睡眠、切换或中断返回。V14 的别名算术模型只能解释逻辑，不是原 ELF 或硬件证据。

### 不突破安全和额度边界

沿用 10 的临时目录、无安装、无提权、无凭据输出和原文件保护要求。不启动子 Agent 群、不做新的大规模研究；一个阶段顺序做完能可靠完成的检查。技术失败不导致自动更换付费模型或自动充值。不得修改内核或原评审来迎合通过标准。

## 5. 固定结果与 GitHub 回传

新增且仅新增于：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/`。保留 pilot 原件。首次交付三个文件：

| 文件 | 内容 |
|---|---|
| MANIFEST.md | 本次执行身份、实际范围、覆盖计数、文件清单、反证/阻断摘要；不声称全任务 PASS。 |
| results.yaml | V00-V14 每项真实结果与 CA-01 至 CA-07 对应裁定；每项有输入身份、证据类型、前提、实际值和 evidence 引用。 |
| evidence.md | 完整夹具/抽取与替换说明、实际命令/输出、47 引文检查逐项明细、范围保护和远端回传检查。 |

必要修订新增 `-v2` 文件并写 supersedes，不覆盖已交的 pilot 或 core 证据。不要提交二进制、原始磁盘、整个源码副本、机密日志或完整聊天历史。只需以上具名成果，不要求完整最后回复的重复导出件。

结果 YAML 的每个案例至少有：

```yaml
case_id: V00
execution_status: NOT_RUN
result: NOT_RUN
evidence_types: []
source_refs: []
preconditions: []
observations: []
evidence_refs: []
limitations: []
```

上例是 schema 示例，不是本轮预填结果。实际使用 09 定义的 SOURCE_MATCH / STATIC_COUNTEREXAMPLE / HOST_ORIGINAL_SLICE / MODEL_ONLY / EXISTING_ELF_EVIDENCE 等类型，以及 OBSERVED_AS_PREDICTED / COUNTEREVIDENCE / NO_FAILURE_IN_SCOPE / BLOCKED / NOT_RUN 结果。未执行和未发现失败不等价；静态匹配与语义支持也不等价。

各文件标明实际执行者 produced_by、模型选择来源、Cloud 执行地点。若保留 `model_per_owner: gpt6`，加 `model_per_owner_scope: originating_lead_only_not_executor`，防止把主线历史模型信息错贴给 Claude。后端不可认证时如实写 unknown_or_not_attestable。

推送前对新增产物作路径白名单、YAML、引用完整性、连续 40 位十六进制、意外凭据与无范围 PASS 声明检查。与当前 master 的 PR 净差异只能落在本核验包的 pilot/core 结果路径。输入文件已知旧错误不能通过复制进结果绕过检查；必要引文只取支持本次判断的最小片段。

沿用同一执行分支和执行 PR，标题改为 `MYOS2-LEAD-002-CORE-CHECK-01: core verification`。仍保持 Draft，等主线审查，不触发 Auto-fix，不合并不删除。冻结 results.yaml 后远端读回作字节比较，随后在其他文件登记该明确范围；不得将工作树读回称作远端核查。PR 正文给三个文件入口、执行/未执行数及反证重点。

## 6. 主线回收与范围结束

主线从 GitHub 直接读取本执行 PR 的实际文件及提交范围，优先检查反证、边界失败和工具检查矛盾，再判定可以消费哪些结论。云端日志由执行者产生，不因换了提供商就自动成为完全独立、不可伪造的证据；本主线不会把可读审查写成自己实际重跑了夹具。

本主线验收上限仍为 PASS_PENDING_LOCAL。可另给范围明确的 `cloud_checks_reviewed` 和案例消费裁定，但不把云宿主结果当作用户本机运行通过。真实 ELF、内核时序、设备和 SMP 等剩余门继续单列。

若试跑失败证实该云环境不适用，主线可依据同一 V00-V14 合同准备本地 Codex 适配；本文件不授权执行者自行切换产品、变更既有本地工作树或重复花费额度。不能仅因云端缺少 Owner 的启动盘就把可以独立完成的引文/宿主检查全部阻塞。

当前停止原因应是具体技术/权限边界或完整结果交付，不是等待 PR 合并或要求 Owner 逐项确认内部安排。能独立继续的案例在同一次执行中完成；不可完成的保留准确原因。最后的用户可见回复只给 PR 链接、完成/未完成范围及主线需要注意的反证，不要求文件搬运。
