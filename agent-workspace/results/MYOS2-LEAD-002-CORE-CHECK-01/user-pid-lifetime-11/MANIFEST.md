---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PID-LIFETIME-11
record_type: bounded_pid_lifetime_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: "MYOS2-A-C03a 代码分析执行"
produced_by: "ChatGPT普通对话；Owner指定GPT-6 / Extra；backend不可独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；results_base=848e2acdf7e1；instruction=69523af0507e（短提交）"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/29-user-pid-lifetime-contract.md @ 04fc4f26ba3d"
prior_review_record: CORE-USER-SIGNAL-SEND-WAKE-10-REVIEW-001
prior_return: {SR10-01: PENDING_MAINLINE_ACCEPTANCE}
executor_disposition: {SR10-01: accept_as_local_documentation_erratum}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
kernel_change_made: false
phase3_authorized: false
merge_authorized: false
old_results_changed: false
owner_decisions_requested_now: []
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/"
deliverables: [MANIFEST.md, signal10-graph-disposition.md, pid-lifetime.md, facts-and-boundaries.yaml, pid-lifecycle.dot, deferred-findings.yaml, evidence.md]
inputs_read:
  - "29号固定任务书04fc4f26ba3d、10主线回执、28号同轮规则及18号延期政策"
  - "10号冻结结果848e2acdf7e1的facts/DOT和局部说明；05的C2/C6、UV-14/15按既有审查限制消费"
  - "time固定源码：fork及用户syscall、alloc_pid/attach_pid/IDR、各PID查询引用API、namespace直接原语、init任务、exit_notify/do_exit/do_task_dead、当前kill查询桥"
open_questions:
  - "SR10-01须由C03据P00处置独立核收，旧10回执RETURN尚未由本执行者更改"
  - "PID数字冲突、越界、资源引用、exit未定义前提等均无运行验证"
  - "05 C6无条件改挂1号的措辞疑点仅为PID11-ER01候选，不修改旧05"
---

# 第11批交付总入口

**本批按阶段2有界静态证据交付，状态仅为 READY_FOR_MAINLINE_REVIEW。** 完成P00–P04的授权执行；所有旧件原样保留，主线C03负责最终核收与后继工作选择。

## 一、七份文件及消费顺序

| 文件 | 本批实质内容 |
|---|---|
| `MANIFEST.md` | 身份、固定输入、进度、自查、边界与主线接收入口 |
| `signal10-graph-disposition.md` | **先读：P00** 接受SR10-01定点图/引文关联修正，五处处置、旧26条边有界核查、替代YAML/DOT；旧10仍RETURN待主线 |
| `pid-lifetime.md` | **后读：P01–P03** 普通fork、IDR数字/对象/任务三时点、namespace和持引用、退出/摘链、所有权及状态矩阵的可读说明 |
| `facts-and-boundaries.yaml` | 125个固定源码短锚点（17个文件）、47节点、54边、16条生命周期状态、11条接口所有权、7个effects及有界查询/unknown |
| `pid-lifecycle.dot` | 与facts的47节点/54边ID一致；有条件/潜伏/注释边均有区分，当前exit未强连detach/free |
| `deferred-findings.yaml` | 新增6项有条件问题、向旧05/10追加3条证据、05 C6的1项文档勘误候选；不改旧状态 |
| `evidence.md` | 完整读取、125/125固定源码文本及43/43正文引文比对、26边审阅、主动反证、预检和NOT_RUN范围 |

以上路径一律在本文件YAML头的唯一`allowed_write_prefix`下；未新增第八份工具/脚本/日志库或修改10历史facts/DOT。

## 二、P00–P04完成情况

| 阶段 | 已完成 | 未覆盖或待C03 |
|---|---|---|
| P00 | 读取29号/10回执及固定旧图；先提交`signal10-graph-disposition.md`，更正E01/E12/E16/E21/E24、规范data方向、补`__dequeue_signal`中间节点/条件；逐条核原26边的同类引用问题 | 执行者的accept不自动关闭旧10的SR10-01 RETURN |
| P01 | 核实际fork入口、PID数字槽/NULL预留、`idr_replace`对象发布、`attach_pid`任务挂接及随后唤醒/编号；16阶段状态矩阵及早期错误路径 | 未验证并发占号、实际调度、任意clone/namespace组合 |
| P02 | 定点核11个查询与引用接口：`find_vpid`、`pid_task`、`get_pid/put_pid`、`get_task_pid`、`find_get_pid`、`get_pid_task`、`find_get_task_by_vpid`等；对接10正PID kill的裸目标指针 | 不证明全局任务生存、RCU/锁或完整PID namespace正确性 |
| P03 | 核`exit/exit_group→do_exit→exit_notify/do_task_dead`与潜伏`detach_pid/free_pid/idr_remove`、错误参数及未初始化前提，区分退出和实际摘链 | 未运行内核；不能推出退出后一定查不到/一定重用PID |
| P04 | 七件依约在同一目录顺序新建，facts在本批交付前做了一次授权范围内的引文定位校正；完成有界结构与固定源码比对 | 最终GitHub七件回读、基线diff与PR17交付评论将在本清单之后执行，并如实记录于评论 |

## 三、本批自查证据和反证结论

- **源码引文：** `facts-and-boundaries.yaml` 125段，每段1～5行，按17个固定源码文件从time再读原文，分两批80+45条进行**125/125逐字核验**；符号及控制流语义仍按正文上下文限定。
- **正文：** 43处`[VERIFIED]` 的路径、符号、逐字引文与facts锚点**43/43一致**；不把引文数当作独立语义证明数。
- **图：** 47节点/54边的两份ID集合一致；全部54个边的端点、引用锚点存在，状态`active/conditional/latent/commented`分别消费；data消费方→提供方，写入变化放effects；未运行Graphviz。
- **问题：** 新问题6项，旧问题证据追加3条，勘误候选1项；P00处置10退回不更改旧10状态，05 C6、UV-14/15均未自行裁定。
- **主动反证：** 空指针数字槽不可当作排他占号；PID对象可见但任务链可能为空；`get_pid`不等于持task引用；`free_pid`定义存在不代表退出实际调用；`group_dead`未赋值与`exit_notify`的错误namespace实参不支持固定返回或成功重挂；超范围PID索引不保证安全，具体后果未知。

## 四、写入及回读约束

**写前初始基线**：PR17 open/Draft且执行头`848e2acdf7e1`；time=`a039d9803ade`，master=`e8e60cd19540`；仅PR16、PR17开放、与本目录无交集。后续提交前按最近执行头作非原子写前复查，并保留GitHub contents API没有原子分支锁的竞态限制。

**本清单发布前已实际核对**：从`848e2acdf7e1`至`2590ce21304f`为**7次后继提交、只含本批6个A新增路径、没有旧件净修改或删除**；PR17 open/Draft，主线lead合法前进不属于执行者源码/成果漂移。第七件本清单及最终提交需再从真实GitHub状态回读，不在本文件预测尚未产生的结果提交。

固定源`time`以GitHub连接器读回；工具处理为有界JavaScript字符串、正则、ID集合和比较。**NOT_RUN**：PyYAML/Graphviz正式解析或渲染、任何仓库旧检查器、预处理/编译/运行C/ASM、完整内核/QEMU/硬件、外部libc、额外模型/API、真实并发、宿主机或原CC02进程。没有生成代码修补方案，不修改master/time/lead、旧文件、PR标题/正文/状态，不创建新PR/分支。

## 五、验收交接与停止

请C03先按P00审`SR10-01`及其新旧图证据，再读`pid-lifetime.md`和facts状态/所有权表，核关键前提与图边、延期条目，最后对本批七件执行远端diff。**当前依然是RETURN待验收的旧10、PENDING的本批11**；若主线接受，最高只可按既定`PASS_PENDING_LOCAL`有限静态层消费，非内核运行正确性PASS。

C03a将本批最终增量与七件固定对象回读记入PR17交付评论后停写；后继任务须由C03独立授权，不自行启动12或阶段3。
