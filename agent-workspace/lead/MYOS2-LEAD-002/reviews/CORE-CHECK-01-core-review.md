---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-CHECK-01-CORE-REVIEW-001
record_type: core_evidence_review_and_bounded_return
conversation_display_name: "MYOS2-A-C02 内核分析主线"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-27
base_snapshot: "kernel=time；workspace=master；write=agent/MYOS2-LEAD-002（分支名）"
read_channel: connector
evidence_class: "远端原件与程序可读审查；执行结果来自 Claude 云会话；主线反例是未运行的静态推导"
status: CORE_REVIEW_COMPLETE_TARGETED_RECHECK_READY
disposition: RETURN
return_scope: "验证器的准入、观测完整性、结果生成及可重复执行；不是全部内核观察作废"
retained_evidence: PARTIAL_SCOPED_OBSERVATIONS
reusable_harness_accepted: false
reviewed_pr: 17
reviewed_branch: claude/dazzling-cori-q0dnyt
reviewed_commit_short12: a1e7c2277705
frozen_results_commit_short12: 7e2fa84a9823
pilot_head_short12: 0851af4fc08b
input_refs:
  kernel: {branch: time, short12: a039d9803ade}
  workspace: {branch: master, short12: de3bb1df906a}
  technical_taskbook: {branch: agent/MYOS2-LEAD-002, short12: 57a7c3e0eebf}
  pilot_review: {branch: agent/MYOS2-LEAD-002, short12: f79b3a281616}
inputs_read:
  - "PR17 core/MANIFEST.md、core/results.yaml、core/evidence.md 全文（均在本核验包结果根下）"
  - "PR17 core/fixtures/ 的全部二十个文本文件；具体名称见 §6"
  - "07-scheduler-wakeup-timer-audit.md 的 CA-04、CA-05 与 A46 段；旧技术任务及 pilot-review 沿用本会话已完整读取内容"
  - "time:mykernel/CMakeLists.txt 全文"
  - "time:mykernel/sched/scheduler/myos_rt.c 全文"
  - "time:mykernel/kactive/swait/swait.c 全文"
  - "time:mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h 的 arch_atomic_add_test_negative 注释及定义"
  - "time:mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h 的 init/is_locked/trylock/lock/unlock 定义段"
  - "PR16/17 元数据、PR17 全路径与提交比较、master/time/主线分支比较"
supersedes:
  - agent-workspace/lead/MYOS2-LEAD-002/07-scheduler-wakeup-timer-audit.md
supersedes_scope: "仅更正 A46 引文边界；限定 CA-02 的可达性消费方式；其余原件保留，不覆盖历史"
followup_id: CORE-CHECK-01-RECHECK-01
followup_taskbook: agent-workspace/lead/MYOS2-LEAD-002/12-core-check-recheck-contract.md
followup_execution_disposition: RUN_NOW_OPTIONAL_BY_OWNER_LAUNCH
allowed_execution_branch: claude/dazzling-cori-q0dnyt
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/
merge_required: false
kernel_acceptance_verdict: NOT_ISSUED
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待执行面；本主线未运行命令、解析器、哈希、夹具或反例。"
self_check:
  scope: this_file_only
  verified_claims: 2
  quotes_reconfirmed: 2
  downgraded_to_inferred: 0
  method: "模型逐项对照实际读取的 CMake 原文；非机械计数"
open_questions:
  - "下面 R01-R04 的负例尚须由执行面实跑；不得把主线推导冒称已复现。"
  - "idle 永不带睡眠状态切出的条件尚未全链证明；真实内核、ELF、IRQ/SMP 不在此次宿主核验结论内。"
---

# 结论：保留有用的内核观察；先堵住验证程序自己的假成功路径

**这次交付有实质内容，不是又一份清单。** 三份正文和二十个夹具/驱动文件已经读到，包含原函数抽取、替换依赖、实际输出、负控与远端回读。内核观察可以按下表有限使用；但当前验证程序会在某些缺失输入或失败路径上继续运行、甚至给出成功型结果，暂不能作为以后自动验收修复的可靠工具。PR #17 保持 Draft、暂不合并。

下一步是同一云会话完成一次有界的验证器修复与重核，不重跑九项研究、不修内核、不另做环境 pilot。主线自己的 A46 错误在本件更正，不转嫁给执行者。

## 1. 已取得并可有限消费的结果

[VRF] 以下数值来自被审执行者的 `core/evidence.md` §3–§5 与 `core/results.yaml`；主线检查了对应代码和记录的一致性，没有亲自重跑。

| 主题 | 保留的观察 | 不能升级成什么 |
|---|---|---|
| V12 原子加法判负 | 原 asm 片段的 (-1,1)、(1,2)、(2,-1) 得到 -2、-1、3；与减法相符，而非函数所述加法。 | 不是多核原子性或整个 atomic 子系统正确性证明。 |
| V13 trylock | 首次成功后锁字仍为 0；没有释放就再次成功。原 arch_spin_lock 持锁的对照则使 trylock 返回失败。 | 不是整个 spinlock 或 SMP 压测通过。 |
| V02/V03/V11 等待队列 | 单 waiter 被摘链后 count=1、实际链上节点=0；后续通知把 anchor 反推成 waiter；守卫在真实 try_to_wake_up 解引用错误 task 之前停止。 | 不能说已经观察到真实内核崩溃；无效容器的构造/取字段并非都发生在守卫之后。 |
| V04–V07 唤醒 | 有状态改变与自有队列入队，返回值仍为 0；本次五种状态不匹配案例有真实事件，串行重复唤醒没有重复入队。 | 不恢复“唤醒永不入队”旧误判，不证明并发安全。 |
| V09/V10 有限/无限等待 | 有限路径在 timer 替身下不递减，100 步后由界限终止；预先完成与 MAX 等待的局部路径必须另看。 | 不是实际 timer 到期/调度运行证明，也不是所有 completion 都失效。 |
| V14 时基 | 原文与宿主链接模型支持别名两次增量，对照为一次。 | 没有实际 MyOS2 ELF 或硬件计时证据，不称墙钟两倍。 |

执行者的“11 项 OBSERVED_AS_PREDICTED”表示所测现象符合待验证预测，其中许多预测本来就是缺陷；它绝不等于“11 项内核功能正确”。15 是顶层检查条目数，V11 真上下文切换层和 V14 真实 ELF 层仍未执行；不能用顶层 not_run=0 掩掉这些层次。

## 2. 接受对主线的纠正

### L01｜A46 确有定义体越界，主线负责修正

[VRF] 旧 A46 把以下两条不同 CMake 定义放在同一个 KERNEL_C_SRCS 标签下。执行者指出第二条越界是对的，不是其检查器误报。当前 CMake 原文已由主线重新打开；替换证据拆为：

A46-C｜C 源码收集定义：

[VERIFIED mykernel/CMakeLists.txt::KERNEL_C_SRCS]
```cmake
file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
```

A46-ASM｜汇编源码收集定义：

[VERIFIED mykernel/CMakeLists.txt::KERNEL_ASM_SRCS]
```cmake
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
```

这只更正证据组织，不证明最终编译/链接收进哪些目标。冻结的旧 47 条测试仍应报告旧 A46 失败；不得为迎合本件把旧结果改成 47 条全合格。上述两条另核，旧文件及旧 MANIFEST 的历史统计不改写。

### L02｜CA-02 保留函数层风险，但不称已证实正常路径可达

[VRF] V08 交付了正常 idle 重新入队与人为令 idle 阻塞切出的两组序列。主线重新读取 `myos_rt.c::pick_next_task_myos`，其 RUNNING idle 切出后确有回队分支。[INFERRED] 在串行、队列不变量成立且 idle 始终以 RUNNING 切出的限定模型中，非 idle current 运行时有 idle 可供选择；所以“阻塞 current + 空队列”不能直接当作实际启动后必然遇到的故障。

反过来也不能据一次正常序列宣告全局安全：kernel_thread/copy_process 等 idle 上下文路径并未在本批完成全链证明。CA-02 的运行可达性维持未闭合，而非从缺陷判定直接跳到通过。

V08 的额外回插序列 [3,4,5] → [4,5,2]，与期望的 vruntime 排列 [4,2,5] 不同。所读 myos_rt 原函数的循环只更新 tmp_list、不更新 tmp_rt，与此局部观察一致；作为后续候选保留，不在本轮自动修内核。anchor 判别之前读取容器字段的风险也只记静态线索，不冒称运行复现。

## 3. 验证器的四项定点问题

下述“推导”均来自实际提交的程序；本主线未运行它们。当前真实正例日志存在，所以这些缺陷不自动否定全部历史观测，但会妨碍安全重跑和后续自动验收。

### R01｜空观测可被 V05 判为符合预测

依据：`core/fixtures/evaluate.py::events_of`、`::v05`、`::judge`。

最小待实跑输入：`evaluate.v05({"cases": {}})`。

[INFERRED] events_of 在缺失案例时返回空事件；v05 循环不执行，checks 为空，却给 judge 一个必然不等的负控三元组 `(empty, 0, 1)`；judge 因此得到“正常检查零个不匹配、负控有不匹配”，返回 OBSERVED_AS_PREDICTED。这是检查器的空集合假成功，不是内核反证。一个已构建夹具即使产生空日志，也能进入这条路径。

同类风险：v05 不核五种 label 的完整集合；多个 evaluator 不先核运行状态、超时、正常/受控停止退出码；one 取第一项和字典覆盖可能隐藏重复/冲突事件。必须先判证据是否完整有效，再比较内核预测；缺日志不能写成成功或内核 COUNTEREVIDENCE。

### R02｜准入与安全检查被记录，却不控制执行

依据：`core/fixtures/run_all.py::main`、`::identity`、`::stage_script`。

[INFERRED] main 捕捉 identity 异常或得到 gate=false 后仍无条件调用 stages；H00 的失败/超时返回仅存进字典，仍调用 fixtures。最后也没有代表基础设施失败的汇总退出语义。这与“先满足准入/隔离，才运行动态夹具”的任务要求不符。

当前交付记录的门值确实为真，不能说本次已经越权；问题是程序自身不拒绝错误输入。应以假准入/安全探针失败的有限负测证明动态入口调用次数为 0；不实际碰任何禁止资源。有效核验发现内核反例不等于驱动异常，两种状态和退出意义必须分开。

### R03｜结果生成存在固定结论，不能可靠传播变化与缺失

依据：`core/fixtures/make_results.py` 与 `build_evidence.py`。

[VRF] V00/V01/V08 的顶层 result、host 的 EXECUTED，以及 CA-01…CA-07 的多项裁定/说明采用固定文本；部分输出直接下标读取必有的 run、事件或阶段。固定说明适合作为已标明的一次性解释，不适合作为任何重跑均有效的计算结果。[INFERRED] 例如改变 V01 的引用存在性为 false，生成器仍固定写 NO_FAILURE_IN_SCOPE；缺阶段或 blocked 案例还可能在成文前因缺键中断。

应把机械结果按本次完整数据计算，语义裁定另列依据、绑定本次输入。有效相反观察可为 COUNTEREVIDENCE；未执行、缺失、异常则是对应基础设施状态。生成器必须能为部分完成的批次交出完整但诚实的记录，不能套用本轮的 15/11/2/2 或七条固定裁定。

### R04｜冻结源版本与移动执行分支混用，已发布后重跑会误阻断

依据：`run_all.py::identity` 要求执行分支远端头等于 pilot_head；`h00_hardening.py::probe_readback` 用 pilot_head 调 `readback.remote_read`，后者要求远端分支头等于该旧提交。

[INFERRED] 当前已经发布 core 的分支头是 a1e7c2277705，不再是 0851af4fc08b；即使 pilot 原件完全没变，照抄这套入口也会把正常执行线推进报告为不匹配。R02 又会使它继续运行。不能用删除身份检查来补救；应分开校验“旧原件未改且为祖先”和“本次远端回读目标对象/文件一致”。

`readback.py` 的 CLI 还把 `--out` 的值留在位置参数数组里；无可选字段而给 --out 时会误当成 field=value。限定为入口修复一并测试。临时目录改用全新目录，不再把 evidence 中固定 canonical 路径配套的 rm -rf 命令当通用启动方式。进程组终止后的收集超时/快速退出也应保留明确终态；未证实回收不能填已回收。

## 4. 处置与下一次验收的范围

**接受本批为可追溯的部分研究/执行材料；验证器作为可复用交付定点退回 R01–R04。** 二十个辅助文本位于同一获准 core 子树，没有改内核，且提供了必要重建能力，保留；不因没有全塞入 evidence.md 而扔掉实质成果。

下一任务见 `12-core-check-recheck-contract.md`：先对冻结旧程序运行有限负例，再在全新 recheck-01 子目录补修；一次完成负测与同源 V00–V14 复跑。原函数语义、预测、源版本和历史红结果不改。旧核心实测可继续用于候选分析，不必等修完验证器才重新理解每条材料；但不据此签发内核修复授权或最终学习路线。

## 5. 交付保全与审查可信度

原件保持在执行分支及被审提交；主线只新增自己的审查/任务/检查点，没有把证据重新转录成伪原件。三份文档加二十个辅助文本是本次 core 实际范围，pilot 三件未在 core 提交中改写。

evidence 是由运行输出生成的汇编，部分前置命令明示为手工转录，脚本对嵌入文本使用了去尾换行处理；不称全量逐字日志备份。开发期失败以叙述保存，未交其全部原始输出；下一次只要求保留新负例/修复/复跑的真实记录，不伪造缺失的旧日志。

本轮是 GPT 主线对不同云会话的源码/日志作可读审查。模型选择仍未知，不认证特定后端或不可伪造执行；没有主线重跑、CI 运行或人类逐行审稿证明。时基/SMP/真实上下文等限制不因 provider 不同而消失。

## 6. 读取清单与可追溯入口

本轮完整读过的 core/fixtures 文件：build.py、build_evidence.py、evaluate.py、final_check.py、fx_common.h、fx_jiffies.c、fx_list.inc.c、fx_prims.c、fx_sched.c、fx_wait.c、h00_hardening.py、harness.py、locate.py、make_results.py、readback.py、run_all.py、static_checks.py、v00_anchors.py、v00_semantic_review.yaml、v01_structure.py。长文件按段补读；未将截断片段申报为全文。

- [被审 PR17](https://github.com/08822407d/MyOS2/pull/17)
- [冻结 core 结果](https://github.com/08822407d/MyOS2/blob/a1e7c2277705/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/results.yaml)
- [冻结执行证据](https://github.com/08822407d/MyOS2/blob/a1e7c2277705/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/evidence.md)
- [冻结验证代码](https://github.com/08822407d/MyOS2/tree/a1e7c2277705/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/fixtures)
- [A46 原文](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/CMakeLists.txt)
- [调度选择器原文](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/scheduler/myos_rt.c)

下一步仓库写入：是。执行者只写同一分支的 core/recheck-01/；主线只写自己的 lead 目录与审查评论。PR #17 暂不合并，执行分支保留至定点重核回收、主线审查完成且明确解除；PR #16 合并不是下一步前置。没有后台启动云执行。
