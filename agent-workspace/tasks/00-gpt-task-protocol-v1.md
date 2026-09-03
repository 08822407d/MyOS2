# GPT 任务反编造协议 v1（第二波起所有派给 ChatGPT Pro 的任务书共用；任务书只引用本文件，不重复）

```yaml
record_type: gpt_task_protocol
version: 1
track_id: MYOS2-LEAD-001
drafted_by: claude-fable-5.1@claude-code-vscode（主导会话 MYOS2-LEAD-001）
date: 2026-09-03
applies_to: MYOS2-DR-001R、003R、005R 起草时即引用；004R、010、011 可经 Owner 一句话追加引用；以后所有 GPT 任务默认引用
status: draft（待 Owner 追认；追认后改 final 并进 conventions.md 的 supersedes 链）
why: 第一波九份产出中四份含编造——两例 40 位 commit SHA 前缀真尾部编造（001 20 处、006 21 处，其中一处写进 ci.yml 环境变量）、一例成段虚构脚本内容、一例行号系统性偏移、一例手册位域错值与假 [VERIFIED]；共同点是"溯源字段与引文可以凭记忆写、人工阅读发现不了"。本协议把这些字段改成机器可拒收的形式。
```

## P1 · 快照申报只写分支名

- `base_snapshot` 写 **`time（分支名）`**；工作区文件写 `master（分支名）`。
- **禁止书写 40 位 commit SHA**。若连接器输出里确实显示了 commit，只允许写从该输出**原样复制**的前 12 位，并标注"短 SHA，自连接器输出复制"。凭记忆补全一个字符都算编造。
- 理由：第一波两例假 SHA 都是"前缀真、尾部编造"；本地入库前会 `git cat-file -e` 校验，不过即整份退回。

## P2 · 引文锚定（每条 [VERIFIED] 的最低要求）

每条 `[VERIFIED …]` 断言必须同时给出：
1. 文件路径（相对仓库根，如 `mykernel/init/main.c`）；
2. **函数名或唯一标识符**（如 `start_kernel`、宏名、变量名）；
3. **逐字引文 1~5 行**，原样复制，不改写、不缩写、不"整理"。

写法：`[VERIFIED mykernel/init/main.c::start_kernel]` 后跟引文代码块。**行号不得作为唯一定位手段**（可以附带，但闸门只校验引文与函数名）。理由：第一波 003/008/002 的行号大量偏移或越过文件末尾，而引文可以被本地 `grep -F` 机械校验。

## P3 · 读取通道与降级

- 首选 GitHub 连接器；读不到时用 raw URL：工作区 `https://raw.githubusercontent.com/08822407d/MyOS2/master/<path>`，内核源码 `https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>`。
- MANIFEST 必须申报实际使用的通道（`read_channel: connector | raw-url | mixed`）。
- **两条通道都失败＝停止并报告**，不猜内容、不做"根据常识"的交付（004 的零交付是正确示范）。

## P4 · "不存在/未调用/可裁剪"断言的前置

- 说任何"未见 X / X 不存在 / X 无调用者 / X 可裁剪"之前，必须先读 `mykernel/scripts/options_flags.cmake` 并引用相关行（P2 格式）——它定义了实际生效的 CONFIG 宏（第一波无人读过，已导致一条假 [VERIFIED]）。
- 措辞限定为"**所读快照中未见**"，不得写"仓库中不存在"。

## P5 · 外部结论

- 每条外部结论附**可解析 URL 或 DOI**；内部检索句柄（`fileciteturn…` 之类）、"据某文档"、无链接的项目名一律不计为出处。
- 引用 Intel SDM / AMD APM 时给出**卷、章、表/图编号**＋URL；给出手册修订号。
- 拿不到出处的结论不写，列进 `open_questions` 并说明查过哪里。

## P6 · 交付前自检回合（强制）

交付前对自己产出中的**每一条** [VERIFIED] 引文重新打开源文件核对是否逐字存在；在 MANIFEST 写：
```yaml
self_check:
  verified_claims: <n>
  quotes_reconfirmed: <m>
  downgraded_to_inferred: <k>   # 核不过的必须降级为 [INFERRED] 或删除，不得保留
```
`m + k` 必须等于 `n`。没有 self_check 段的 MANIFEST 视为未完成。

## P7 · 零交付优于假交付

读不到输入、找不到证据、拿不到出处——都写成 open_questions 或直接停止报告。**编造一条＝整份产出作废**（公约 §1 违反＝产出作废的延伸）。

## P8 · 重做/勘误的文件纪律

- 重做任务号用 `<原号>R`；产出目录 `results/<原号>R/`；旧目录**一个字节不改**。
- 勘误主件 `errata.md`：对旧产出逐条写"保留 / 修正 / 撤回"＋证据；修正后的文件在头部写 `supersedes: results/<原号>/<旧文件>`。
- 没改的旧结论不必重抄，只在 errata 写"保留，未重核"或"保留，已重核"。

## P9 · 本地机械闸门（GPT 不做，但要知道会被这样验收）

回收后本地会自动做：引文 `grep -F` 校验、SHA `cat-file -e`、路径存在性、行号越界、self_check 数字是否自洽、[EXTERNAL] 是否带 URL/DOI。**任一项不过＝整份退回原对话返工**，返工时引用具体判据。

## P10 · 脚本与代码件

- GPT 无法运行任何脚本/构建；`proposed/` 下每个脚本/代码件头部必须标 `# UNTESTED — generated without execution`，MANIFEST 不得声称"可直接运行"。
- 涉及删除、格式化、挂载、写盘的脚本必须默认 dry-run，破坏性动作需双确认（环境变量＋与目标完全一致的路径），否则不交。

---

**给任务书作者的备注**：任务书中只需写一句"本任务适用 `agent-workspace/tasks/00-gpt-task-protocol-v1.md` 全部十条"，并在验收判据里引用 P2/P6/P9。
