---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SIGNAL-SEND-WAKE-10
record_type: signal_send_wake_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: "MYOS2-A-C03a 代码分析执行"
produced_by: "ChatGPT普通对话，Owner选择GPT-6 / Extra；backend身份未独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；old_result=037b880e792b；instruction=0e2a53c3dcc3（短号）"
contract_ref: "agent-workspace/lead/MYOS2-LEAD-002/27-user-signal-send-wake-contract.md @ 065abf7e8677"
prior_review: CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01-REVIEW-001
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_made: false
old_results_changed: false
branch_created: false
PR_created: false
owner_decisions_requested_now: []
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-send-wake-10/"
deliverables: [MANIFEST.md, signal-send-wake.md, facts-and-boundaries.yaml, send-paths.dot, deferred-findings.yaml, evidence.md]
inputs_read:
  - "27号固定合同及本批S00固定GitHub元数据"
  - "time固定源码：options_flags.cmake、kill入口/系统调用表/包装器、signal发送与队列及直接头文件、PID查找/namespace、唤醒调度接口、缓存初始化"
  - "09报告/延期问题RS-03/04/12/13/14的定点材料、09-DOC-FIX-01修正与主线验收；不全量重读旧大型结果"
open_questions:
  - "未初始化override_rlimit所涉及的C语言实际表现及外部编译条件"
  - "全PID生命周期、并发同步与所有信号生产者未穷尽"
  - "真正唤醒、用户handler或硬件结果未运行；RS-01、GAP-U04/GAP-U10仍按旧限界开放"
---

# CORE-USER-SIGNAL-SEND-WAKE-10 交付清单

**结果：阶段2的信号发送、目标查找、pending/队列及唤醒断点已经完成有界静态取证；未实施内核修复或任何运行测试。** 当前状态为READY_FOR_MAINLINE_REVIEW，需C03对六件及其证据边界做独立核收，本执行方不签发主线PASS。

## S00–S04 完成与未完成

| 阶段 | 本批实际完成 | 未覆盖/不自动升级 |
|---|---|---|
| S00 绑定 | 核27号合同、09勘误验收、PR16/17、默认master/time及执行分支，按time先读编译选项 | 不复做25号接收，不核旧本地进程 |
| S01 入口/目标/输入 | 62号kill包装与debug宏、正PID查找、TGID共享pending分离；18类输入/条件矩阵 | 负数/非法信号的未定义前提不可给固定结果；不审PID全生命周期 |
| S02 pending/队列/返回 | 分配与未初始化参数、SIGKILL/PF_KTHREAD、普通/实时区别、siginfo/节点与位图分层、cache初始化和09定点消费 | 未运行分配器/接收测试，不证明用户实际可见返回 |
| S03 唤醒与旧RS | 当前complete_signal调用断点；signal_wake_up_state/wake_up_state/kick_process潜伏接口；与09五项旧RS核对 | 不做全调度、跨CPU延迟和handler保证 |
| S04 六件/自查/回传 | 六件按合同同一新目录追加；当前文件写入前完成源码引文、矩阵/图/ID及前五件diff自查；最终GitHub回读与交付证据在PR17后置评论 | 未做编译/QEMU、原09检查器、YAML/Graphviz完整解析；评论未提交前不声称其存在 |

## 六件交付和消费顺序

| 文件 | 内容与读法 |
|---|---|
| `MANIFEST.md` | 本清单：范围、读法、自查限界及主线验收入口 |
| `signal-send-wake.md` | 首读：面向C03的可读结论、分层、18类输入矩阵、09衔接、反证与重要未知 |
| `facts-and-boundaries.yaml` | 52段路径/符号/1～5行原文锚点，25节点/26边/18矩阵及5项有界查询范围；无全树不存在性保证 |
| `send-paths.dot` | 与facts的25节点、26边ID一一对应；commented/latent边不属于活动kill链 |
| `deferred-findings.yaml` | 六项新发现/潜伏风险及对RS-03/04/12/13/14的五项证据追加；errata_candidates为空；旧问题状态不改 |
| `evidence.md` | 真实读取、定点引用、文本与图机械自查、主动反证、未运行项及PR回读后置位置 |

旧结果037b880e792b及以前的文件全部冻结保留，不修改08/09/旧图/YAML或DOC-FIX-01。新成果仅供阶段2静态盘点消费，不自动变成新的执行源或真实测试PASS。

## 本批机械与语义自查

- **来源与文本：** 固定time对象生成52段锚点；可读正文20段[VERIFIED]与事实锚点逐字一致；evidence另有4段来源回读。不可用数目作为语义正确性证明。
- **结构：** 25个图节点和26条边的ID在YAML/DOT集合相等；18类输入分支覆盖正/缺失/自身/非正PID、0/合法边界/负值/越界信号、SIGKILL、普通/实时、PF_KTHREAD、分配成败和特殊info。问题ID全部指向存在的本批锚点。
- **反证：** 已专门核“返回0是否必有节点”“位图是否必唤醒”“唤醒是否必执行handler”“负向检索是否越界”“未初始化参数是否偷用默认值”，结论和限制写在evidence §4。
- **写区：** 发布本清单前，比较037b880e792b到09146a9f3914仅含本目录已有五文件A新增，PR17仍open/Draft。最后的六件比较、完整blob读回、实际提交短号写在PR17交付评论；本文件不预测未产生的Git对象。
- **运行：** 09旧检查器、C/ASM、预处理、构建、QEMU、内核、外部libc、硬件/调度验证均NOT_RUN；没有额外模型/API。由连接器进行固定源码回读和限定JS文本/集合自查，不宣称运行了仓库检验脚本或本机命令。

## 主线唯一后续

请C03从本文件进入可读说明，核矩阵与52锚点，再按远端固定对象与compare确认本批六件A新增；对未初始化参数、非法信号值和发送/唤醒断点作限定消费。旧RS-01仍needs_evidence，GAP-U04硬件/运行部分、GAP-U10继续开放。主线验收最高PASS_PENDING_LOCAL，实际内核正确性未签发。

本批交付后C03a停写，等待主线派发；不自行开始11或另一任务。
