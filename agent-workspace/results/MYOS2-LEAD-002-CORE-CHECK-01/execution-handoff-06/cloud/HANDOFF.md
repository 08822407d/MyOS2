---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-EXECUTION-HANDOFF-06
phase: CLOUD_CLOSE
record_type: cloud_closeout_handoff
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
model: unknown_or_not_attestable
date: "2026-10-25"
base_snapshot: "收尾前：执行分支 claude/dazzling-cori-q0dnyt 头 293a9e3ef5f4（研究冻结点）；05 结果 22934124fe92；kernel=time a039d9803ade；master de3bb1df906a；主线 agent/MYOS2-LEAD-002 ad850ce5405b。均为短 SHA"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/20-cloud-to-local-handoff.md（agent/MYOS2-LEAD-002）"
review_ref: "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-VFS-BASELINE-05-review.md（CORE-USER-VFS-BASELINE-05-REVIEW-001，PASS_PENDING_LOCAL）"
inputs_read:
  - "20 号合同全文，只执行 §1–§3；§4–§5 的本地阶段未执行"
  - "05 审查回执的头部字段"
  - "C01：本工作树与远端分支的 git 只读命令输出，PR17 元数据"
  - "C02：本会话已知 scratch 目录的文件清单、大小与摘要；四件资产只复制，未执行"
status: CLOUD_CLOSE_DONE
scratch_preservation: PARTIAL
cloud_writer_state: STOPPED_AFTER_THIS_PUSH
owner_decisions_requested_now: []
kernel_change_made: false
---

# CORE-EXECUTION-HANDOFF-06 云端收尾（CLOUD_CLOSE）

本文件只做交接，记录三件事：收尾时的 Git 状态，scratch 中独有资料的去向，以及本机接手时不能继承的环境依赖。研究结论不在这里重写，一律以 GitHub 上已入库的正文为准。

## 1. 冻结点与入口

- **研究冻结点：** `293a9e3ef5f4`，即 05 的文档提交；05 结果提交为 `22934124fe92`。执行分支是 `claude/dazzling-cori-q0dnyt`，对应 PR17（open，Draft）。
- **本次写入范围：** 只在 `execution-handoff-06/cloud/` 下新增文件，293 中已跟踪的文件一个不改。
- **05 正式入口：** `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vfs-baseline-05/` 下的 MANIFEST.md（含恢复顺序）、user-vfs-baseline.md、mm-04-disposition.md、facts-and-dependencies.yaml、deferred-findings.yaml、evidence.md。
- **未完成项（只按 ID 引用）：**
  - `CORE-USER-VFS-BASELINE-05::UV-01`…`UV-16`，其中 UV-04、UV-09、UV-13、UV-14 为 needs_evidence；
  - GAP-U01…GAP-U10；
  - NVU-1…NVU-6，全部 NOT_RUN。

  这些问题都已入库，不需要 Owner 决策。

## 2. C01：收尾前的 Git 状态

| 项 | 结果 |
|---|---|
| 仓库与分支 | 08822407d/MyOS2，当前分支 claude/dazzling-cori-q0dnyt |
| HEAD | 293a9e3ef5f4，与远端分支相同；未推送的提交 0 个，落后远端 0 个 |
| 工作树 | 已跟踪文件改动 0，未跟踪文件 0，stash 0；HEAD 相对 293 无差异，293 的冻结文件均未修改 |
| 其他引用 | 本地 master 与 origin/master 均为 de3bb1df906a；time 为 a039d9803ade；主线头为 ad850ce5405b |
| PR17 | open、Draft、未合并，head 为 293a9e3ef5f4 |
| 与本任务无关的变化 | 未发现 |

## 3. C02：scratch 清点

只清点本会话已知的 scratch 根目录。05 的目录逐个文件看过；更早批次的目录只记下了目录名。

| 类别 | 去向 | 说明 |
|---|---|---|
| 05 负面探针驱动 `uv05/probes/run_probes.py` | 仅在云端，已保全（A1） | 12 条改坏规则和 p0 对照都在这个驱动里。运行结果早已入库：05 records/checker_probes.json |
| 05 探针副本 p0…p12 | 可重建 | 由驱动复制入库的 05 包后逐个改坏得到；未搬运 |
| 05 生成器 `uv05/gen/`（18 个文件，231,637 字节） | 仅在云端，部分保全 | 已保全 gen_facts.py（A2）、resolve.py（A3）、anchor_specs.py（A4）。下文“未保全的生成器文件”逐类说明其余文件 |
| 05 源码解包副本 `uv05/src/` | 可从 Git 重建 | `git archive a039d9803ade mykernel myinitramfs` |
| 05 检查、边界与回读输出（fc*.json、final/、scope_*.json、rb/、rb_docs/） | 大部分已在 GitHub | 正式版在 05 records/ 中。仅在云端的有两类，均未保全：docs 模式检查的输出 scope_docs.json，结论见 05 evidence.md §7；文档提交的三条回读记录，结论曾写在 PR17 顶部，该说明后来已由主线改写 |
| 回读工具 `integ03/rbtool/readback2.py`、`harness2.py` | 已在 GitHub | 与 core/recheck-01/fixtures/ 中的同名文件逐字节一致 |
| 任务书副本 c17_raw.md、c19_raw.md | 已在 GitHub | 对应主线分支上 17、19 号任务书的原件 |
| 更早批次的目录（pilot-work、core-work、raw、recheck-runs、recheck02、order02、integ03 的其余部分、mm04、reread-1001） | 未检查 | 只列了目录名，没有逐文件盘点，其中可能有 04 等批次的生成器和探针驱动。登记为 CLOUD_SCRATCH_NOT_INVENTORIED，不当作已经没有遗漏 |

**未保全的生成器文件：**

- gen_caps、gen_graph、gen_chains、gen_findings、query_specs 中的数据与入库的 YAML 等价。
- 三个 *_tpl.md 模板分别对应已入库的三份 Markdown。
- render_baseline、fill_docs、apply_review，以及四个小型试运行脚本（probe、q_new、run_queries、run_resolve），都没有保全。

## 4. 资产（cloud/assets/）

| 编号 | 文件 | 字节 | sha256（分段） | 原位置（会话 scratch） |
|---|---|---|---|---|
| A1 | assets/run_probes.py | 3,939 | 42b4e0caea82f132 23d2b7f222b8aeb9 a81615198800d843 56c99379124f8457 | uv05/probes/run_probes.py |
| A2 | assets/gen_facts.py | 12,453 | 1dda26dccb4b0e73 fb5cf62f072081a6 152c9238bc1688f9 620cc04c31563eae | uv05/gen/gen_facts.py |
| A3 | assets/resolve.py | 2,224 | f0cf757e1c3c3830 1f2efdcc6c1c5f3b ce8058f0fb863eac f78f5d1cd9211fe5 | uv05/gen/resolve.py |
| A4 | assets/anchor_specs.py | 18,883 | cec77291a771f5db 2faf84768af44ae8 67dc11ada720037b 447dd8e22112f4a4 | uv05/gen/anchor_specs.py |

四件合计 37,499 字节。

- **复制方式：** 原样复制，复制后用 cmp 核对过，与原文件逐字节一致，没有删减。
- **署名：** 原文件中本来就没有署名行。它们由本云端会话（Claude Code cloud session，模型 unknown_or_not_attestable）在 2026-10-02 为 05 编写，当时没有交付进 05 包。
- **未执行：** 本次收尾只复制，没有运行它们。

## 5. 复现说明

**探针驱动（A1）：**

- 脚本里的 PKG、CHK 分别写死为 `/home/user/MyOS2/…` 下的 05 包和检查器。
- 运行时它在自身所在的目录建 p0…p12：先复制整个 05 包，再在 p1…p12 中各改坏一处，最后逐个调用检查器。
- 预期结果：p0 返回码为 0 且 all_ok 为 true，其余 12 个返回码均为 1。
- 在本机使用前需要改 PKG、CHK，并按 20 号 L02 的方式重新绑定检查器的 REPO。副本目录不要建在仓库里。

**引文生成（A3、A4）：**

- resolve.py 按“函数名、首行文字、行数，必要时加上第几次出现”在 time 对象上定位引文，取出逐字行和 #if 栈。
- anchor_specs.py 是全部 108 条引文规格。从 108 条裁到 90 条的过程，记在 A2 的 DROP、FIX、ADD 三个列表里。

**组装（A2）：**

- gen_facts.py 依赖未保全的内容模块（gen_caps、gen_graph、gen_chains、gen_findings、query_specs），不能单独运行。
- 检索的期望值取自生成时的实际运行结果。
- 需要时可以从入库的 YAML 反推内容模块，也可以直接以入库 YAML 为准，因为检查器只读 YAML。

以上脚本当时都在 `PYTHONDONTWRITEBYTECODE=1` 下运行。

## 6. 本机不能继承的环境依赖

- **固定路径：**
  - 03 包 scan_index.py 中的 REPO 固定为 /home/user/MyOS2。
  - 四件资产也写死了 /home/user/MyOS2 和会话 scratch 的绝对路径。
  - scratch 位于本云端容器的 /tmp 下，容器回收后不会保留。
- **工具版本：** 本容器中实际为 git 2.43.0、Python 3.11.15、PyYAML 6.0.1。本机需要自备这些工具；本次没有安装任何工具。
- **GitHub 读写：** 经本云端会话的代理与凭据完成，这些不随交接转移。

## 7. 写入状态

`cloud_writer_state: STOPPED_AFTER_THIS_PUSH`

- 本次提交推送并完成远端回读后，云端不再修改执行分支。最终短号写在 PR17 的评论中，本文件不自引用。
- 就本会话可见的记录而言，没有订阅 PR17 的活动；Auto-fix 是否开启，以界面显示为准。
- scratch 保全状态为 PARTIAL，未保全的部分见 §3。
