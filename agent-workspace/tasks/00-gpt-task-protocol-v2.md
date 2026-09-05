# GPT 任务反编造协议 v2（所有派给 ChatGPT Pro 的任务共用；任务书只引用本文件，不重复；与任务书冲突时以本协议为准）

```yaml
record_type: gpt_task_protocol
version: 2
supersedes: agent-workspace/tasks/00-gpt-task-protocol-v1.md（v1 保留不改；v2 自足，读 v2 即可）
track_id: MYOS2-LEAD-001
drafted_by: claude-fable-5.1@claude-code-vscode（主导会话 MYOS2-LEAD-001）
date: 2026-09-05
applies_to: 第二波全部 GPT 任务（001R/002R/003R/004R/005R/007R/008R/009R/010/011）及以后
status: draft（待 Owner 追认）
why_v2: v1 经对抗评审（3 个独立视角对 004R 任务书的评审）发现六个可绕过闸门的编造入口：引用真实存在但与断言无关的行；把该核的降级为 [INFERRED]；self_check 三个数字自报；读错分支（master 与 time 的同名文件多数行相同，引文照样命中）；转抄第一波产出里的编造字段（假 SHA、坏行号）；代码件凭记忆写 diff 上下文与不存在的 API。v2 逐条堵上，并把闸门写成可执行清单。
```

## P1 · 快照申报只写分支名；40 位十六进制串一律拒收

- `base_snapshot` 写 **`time（分支名）`**；工作区文件写 `master（分支名）`。
- **禁止书写 40 位 commit SHA**。若连接器输出显示了 commit，只允许写从该输出**原样复制**的前 12 位，并标注"短 SHA，自连接器输出复制"。
- 闸门：全部交付文件用正则 `[0-9a-f]{40}` 扫描，**命中即整份退回**（不管它是真是假）。
- **不得转抄第一波产出（results/MYOS2-DR-001~009/）文件头里的任何溯源字段**（`commit`、`base_snapshot`、`inputs_read`、带 SHA 的 GitHub URL）——其中两份是编造值。

## P2 · 引文锚定（每条 [VERIFIED] 的最低要求）

每条 `[VERIFIED …]` 断言必须同时给出：
1. 文件路径（相对仓库根）；
2. **函数名 / 宏名 / 唯一标识符**（写作 `path::symbol`）；
3. **逐字引文 1~5 行**，原样复制，不改写、不缩写、不"整理"；
4. **引文必须取自该符号的定义体内部**（函数体、宏定义、结构体定义）。函数体外的引文视为锚定失败。

写法：`[VERIFIED mykernel/init/main.c::start_kernel]` 后跟引文代码块。**行号不得作为定位手段**（可附带，闸门不认）。引文必须**支持**断言：闸门核引文存在，对抗核查抽样核"引文是否真能推出断言"。

**强制锚点**：每份任务书列有"强制 [VERIFIED] 锚点"，这些锚点**不得以 [INFERRED] 替代、不得省略**；闸门核其标签存在且引文命中。

## P3 · 读取通道、目录浏览与分支自检

- 顺序：先连接器；读不到用 raw URL（工作区 `https://raw.githubusercontent.com/08822407d/MyOS2/master/<path>`，内核源码 `https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>`）；**两条都失败才停止并报告**（不猜内容）。
- raw URL 不能列目录；浏览目录用 `https://github.com/08822407d/MyOS2/tree/master/agent-workspace/...` 或 `https://github.com/08822407d/MyOS2/tree/time/mykernel/...`。
- MANIFEST 必须有 `read_channel: connector | raw-url | mixed`。
- **分支自检（金丝雀）**：凡读内核源码的任务，MANIFEST 必须有 `branch_canary_quotes:` 字段，逐字引用以下两行（它们在 master 与 time 上内容不同，本地据此判定你实际读的是哪个分支）：
  - `mykernel/scripts/options_flags.cmake` 中含 `-mcmodel=` 且含 `-fno-pie` 的那一行；
  - `mykernel/debug/panic.c::panic` 中以 `this_cpu = ` 开头的那一行。
  闸门：两行在 time 检出上 `grep -F` 命中且在 master 上**不**命中；判定结果与 `base_snapshot` 不一致即整份退回。

## P4 · "不存在 / 未调用 / 可裁剪 / 未启用"断言的前置

说任何否定性断言之前，必须先读 `mykernel/scripts/options_flags.cmake` 并按 P2 引用相关行（它定义实际生效的 CONFIG 宏；第一波无人读过，已导致假 [VERIFIED]）。措辞限定为"**所读快照中未见**"。

## P5 · 外部结论

- 每条外部结论附**可解析 URL 或 DOI**；内部检索句柄（`fileciteturn…` 之类）、"据某文档"、无链接的项目名一律不计。
- 引用 Intel SDM / AMD APM 时给出**卷、章、表/图编号**＋URL＋手册修订号。
- 引用工具命令语义（QEMU monitor、gdb 等）时，URL 必须是**包含该命令名的具体文档页面**，并逐字引用该页面中出现该命令的一句；凭记忆写的命令标 [INFERRED] 并列 open_questions。
- 拿不到出处的结论不写，列进 `open_questions` 并说明查过哪里。

## P6 · 交付前自检回合（强制；闸门只认 MANIFEST，不认聊天记录）

MANIFEST 必须有：
```yaml
startup_selfcheck_quote: "<逐字复制 conventions.md §1 第 2 条整句>"   # 闸门在 master 的 conventions.md 上 grep -F
self_check:
  verified_claims: <n>          # 必须等于本地对全部交付文件 grep -o '\[VERIFIED' | wc -l 的合计（闸门计数，不等即退回）
  quotes_reconfirmed: <m>       # 交付前重新打开源文件逐条核对过的引文数
  downgraded_to_inferred: <k>   # 核不过的必须降级为 [INFERRED] 或删除，不得保留
```
`m + k` 必须等于 `n`。强制锚点不计入可降级范围。没有这两个字段的 MANIFEST 视为未完成。

## P7 · 零交付优于假交付

读不到输入、找不到证据、拿不到出处——写成 open_questions 或直接停止报告。**编造一条＝整份产出作废**。

## P8 · 重做/勘误的文件纪律

- 重做任务号用 `<原号>R`；产出目录 `results/<原号>R/`；旧目录**一个字节不改**。
- 勘误主件 `errata.md`：对旧产出逐条写"保留（已重核）/ 保留（未重核，原因）/ 修正 / 撤回"＋证据；修正后的文件头部写 `supersedes: results/<原号>/<旧文件>`（闸门核该路径存在）。
- 原次为零交付（如 004）的，MANIFEST 注明"原次零交付，errata 免交"，文件名不带 -v2。

## P9 · 本地机械闸门清单（GPT 不做，但按此验收；任一项不过＝整份退回原对话返工）

1. 正则 `[0-9a-f]{40}` 全文扫描为 0 命中；
2. 每个 `[VERIFIED path::symbol]` 标签后紧跟引文块，引文在 time 检出对应文件上 `grep -F` 命中，且位于该符号定义体内（本地按符号定义范围核对；无法定位符号者视为不命中）；
3. 引文所指路径、`supersedes` 所指路径、`inputs_read` 所列路径在对应分支上存在；
4. 附带的行号若给出，须 ≤ 文件总行数；
5. `self_check.verified_claims` 等于本地 `[VERIFIED` 标签计数；`quotes_reconfirmed + downgraded_to_inferred = verified_claims`；
6. 每个 `[EXTERNAL …]` 带 `http(s)://` URL 或 `doi:`；内部检索句柄计数为 0；
7. `branch_canary_quotes` 两行在 time 上命中、在 master 上不命中（读源码的任务）；
8. `startup_selfcheck_quote` 在 master 的 conventions.md 上命中；`read_channel` 存在；
9. `symbols_referenced` 列出的每个头文件路径 / 内核符号 / 结构体成员在 time 上存在（`find` / `grep -rw`）（有代码件的任务）；
10. 改动块的 before 引文在目标文件 `grep -F` 命中；unified diff（如有）`git apply --check` 通过；
11. 交付文件中不出现"可直接编译 / 可直接运行 / 已验证 / 已测试"字样（无运行环境）；
12. 降级交付时每个文件独立代码围栏且围栏前一行为目标路径（本地机械拆分成功）；
13. 任务书列出的强制锚点全部存在。

## P10 · 代码件与脚本件

- 优先交**整文件**（`proposed/<目标路径>`）。确需改现有文件时用**改动块**：`目标路径::函数名` ＋ 逐字 before 引文（≥3 行，作为本地定位锚）＋ after 代码块 ＋ 一句改动说明。**不手写 unified diff**（其 `@@` 行号与上下文行是第一波行号偏移的同型失效）；若坚持给 diff，须能在 time 检出 `git apply --check` 通过。
- MANIFEST 必须有 `symbols_referenced:`：新代码 `#include` 的每个头文件路径、调用的每个既有内核符号、访问的每个结构体成员。
- 每个代码/脚本文件首部标 `UNTESTED — generated without execution`；MANIFEST 不得声称可编译/可运行。
- 涉及删除、格式化、挂载、写盘的脚本默认 dry-run，破坏性动作需双确认（环境变量＋与目标完全一致的路径），否则不交。

## P11 · 第一波产出的使用纪律（输入可靠性声明）

第一波 `results/MYOS2-DR-001~009/` 全部是 S1 级证据且各有已证实缺陷（`agent-workspace/WAVE-1-REVIEW.md` §3）：001 含编造（假 SHA、整条虚构）；002 证据路径不存在；003/008 行号系统性失效；005 位域错值与假 [VERIFIED]；006 假 SHA；007/009 外部出处不可解析。因此：
- 只取它们的**结构、函数名、ID（如 DR008-NNN）、节点名**；**不转述其断言为 [VERIFIED]**，不转抄其行号、路径、溯源字段；
- 任务书只给**文件级/节级**输入清单，不给目录级；未列出的第一波文件不必读。

## P12 · 规模与降级交付

- 任务书对输入（文件与节）和输出（条目数、代码件数）设上限；读不完按任务书的优先顺序弃尾，并在 MANIFEST 的覆盖表如实声明。
- 降级到对话内交付时：**每条回复只含一个文件**，每个文件独立代码围栏，围栏前一行写目标路径；按 MANIFEST → 主件 → 其余 → proposed/ 的顺序输出；长文件可按节分回复，每段首行写 `<!-- continued: <path> part N -->`。

## P13 · 署名

MANIFEST 的 `produced_by` **原样填写你界面上显示的模型名称**，不猜测后端。回收登记时主导会话按 Owner 告知另记 `model_per_owner`，GPT 自报值不改。

---

**给任务书作者的备注**：任务书写一句"本任务适用 `agent-workspace/tasks/00-gpt-task-protocol-v2.md` 全部十三条"，并至少包含：输入清单（文件/节级）、强制锚点清单、MANIFEST 必备字段、输出上限、按 P9 编号写的验收判据。
