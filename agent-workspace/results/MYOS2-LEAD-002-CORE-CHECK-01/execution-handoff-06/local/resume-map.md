---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-EXECUTION-HANDOFF-06
phase: LOCAL_ACCEPT
record_type: resume_map
produced_by: "Claude Code desktop app (Code tab), local session"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-25"
base_snapshot: "执行分支头 9c36fb3ba7ff；主线 agent/MYOS2-LEAD-002 ad850ce5405b；time a039d9803ade（短 SHA）"
index_ref: resume-map.yaml
provenance: "只读子代理按包定点读取，独立核查代理对抗式修正，本会话汇总并机械抽查路径/短号/判定原值（records/l03_map_spotcheck.json，all_ok）。摘录与意译；原文以所指文件为准"
evidence_class: "导航与消费限定；不是语义审查、不是新研究、不修改任何原报告"
owner_decisions_requested_now: []
---

# 续接工作地图（L03）

## 1. 怎么接着做

先核实际 PR17 与执行分支头是否仍为本地接收后的头，再按顺序读：

1. 主线 `agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md`；
2. 主线最新检查点（写本文件时为 `checkpoints/2026-10-25-user-vfs-reviewed-local-handoff-ready.md`）；
3. 下表中对应包的主线回执；
4. 包内原件（只按 ID 或行号定点读）；
5. 当时最新的 time 源码。

本目录 `RECEIPT.md` 是本地接收证据，`resume-map.yaml` 是本表的逐字段索引（含每包 uncertain 列表）。本地接收不替代主线最终审查。

## 2. 各包入口

结果目录都在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/` 下；主线回执都在 `agent-workspace/lead/MYOS2-LEAD-002/reviews/` 下。

| 包 | 入口与冻结 | 主线回执与判定 | 已接受范围 | 未运行层（摘要） | 后继勘误/收窄 |
|---|---|---|---|---|---|
| pilot | `pilot/` MANIFEST、result.yaml、evidence.md；结果 10ecb7dd0bdb（只含 result.yaml），被审头 0851af4fc08b | `CORE-CHECK-01-pilot-review.md`：ALLOW_CORE；内核 NOT_ISSUED | 云端环境、金丝雀与负控、单子进程超时、远端回读足以开始有界核验 | 正式 V00–V14；真实内核/ELF/IRQ/计时/SMP；原 asm | core 回执 §3 R04 要求改校验方式（core 驱动问题，不改 pilot 结论；recheck-01 在其范围内关闭）；09-27 core 检查点取代其状态 |
| core | `core/` MANIFEST、results.yaml、evidence.md、fixtures/；结果 7e2fa84a9823，被审头 a1e7c2277705 | `CORE-CHECK-01-core-review.md`：RETURN（验证器准入、完整性、生成与可重复执行）；PARTIAL_SCOPED_OBSERVATIONS；reusable_harness_accepted: false | 回执 §1 表所列 V02–V07、V09–V14 的局部观察有限消费；V08 按 §2 L02 限定 | V11 真实切换、V14 ELF 层、V06 并发；CA-02 全局可达性；到期分发链 | recheck-01 回执撤回 swait 两函数“文件外无调用者”；recheck-02 关闭读取层；integration-03 回执 §3（工作安排判断）：旧结果所示的未修机制不能作动态验证、功能承诺或真实时基/ELF/SMP 结论的可靠前提，静态盘点与局部模型分析可继续 |
| recheck-01 | `core/recheck-01/`；结果 efb9846b88ec，被审头 b843d475367a | `CORE-CHECK-01-recheck-01-review.md`：RETURN（R01/R03 读取与完整性传播）；R02/R04 在所审范围关闭；reusable_verifier_accepted: false | M01–M12 变体记录与同源复跑的受限观测 | V04…V14 的 11 个真实层；CA-02；V08 计费时点 | recheck-02 回执关闭 S01–S03；consumer-fix-02 MANIFEST §3 S04；14 号 §3 |
| consumer-fix-02 | `core/recheck-01/consumer-fix-02/`；结果 77faf51e9a43，被审头 0d62c4d19711 | `CORE-CHECK-01-recheck-02-review.md`：PASS_PENDING_LOCAL；consumer_use: FROZEN_SCHEMA_AND_SCOPED_REVIEW | 13 号读取层修订、N01–N06、冻结观测的有限消费；不含内核验收 | C 夹具与 H00 未重跑；真实 ELF/IRQ/SMP/切换；CA-02 | 15 号冻结 0d62 原件；ORDER-02 回执确认收口继续有效 |
| scheduler-order-02 | `scheduler-order-02/`；结果 5078686e8267，被审头 f36b89b8a53a（MANIFEST/evidence 在此加入） | `CORE-SCHED-ORDER-02-review.md`：PASS_PENDING_LOCAL（W01–W08 宿主原函数见证与三项控制） | 有限实验记录；不是调度器正确性 | CA-02；真实时钟/IRQ/SMP/切换；修改后函数；优化构建 | integration.md §1 C4 与 §7（DV-11 源码推导）、DV-06/07/08 细化、W04 收窄；idle 键差异待 NR-4（NOT_RUN） |
| scheduler-integration-03 | `scheduler-integration-03/`；结果 b7fa83583e35，被审头 ee9e6a738224（MANIFEST/evidence 在此加入） | `CORE-SCHED-INTEGRATION-03-review.md`：RETURN（IR-01/IR-02 两处规格）；部分接收 | D01 索引形态、D02/D03 局部生命周期与计费分析、已有 W/V 引用与开放缺口；不是 151 条语义认证 | NR-1…NR-6；A/B 未实施；OD-1/OD-2 未采用；DV-11/13/19/23 运行 | 回执 §4 IR-01/02、§5 收窄；`mm-baseline-04/integration-03-disposition.md`（M00）；05 `mm-04-disposition.md` §1 MR-01 |
| mm-baseline-04 | `mm-baseline-04/`；结果 eb75b3100601，文档在 de7c96ede546 | `CORE-MM-BASELINE-04-review.md`：RETURN（MR-01/MR-02 两处表述）；部分接收 | 四个子系统 21 个代表能力、39 条依赖的静态资料；M00 处置 | NV-1…NV-6；构建/QEMU；A/B/键下限 | 05 `mm-04-disposition.md`（33 处取代/收窄/撤回，7 处复查保留）；05 deferred status_changes；05 回执 §2 关闭两项文字（旧 RETURN 保留） |
| user-vfs-baseline-05 | `user-vfs-baseline-05/`；结果 22934124fe92，文档 293a9e3ef5f4 | `CORE-USER-VFS-BASELINE-05-review.md`：PASS_PENDING_LOCAL；runtime_evidence: NOT_RUN | U00 处置及 U01–U04 代表静态资料（20 能力、6 链、43 边） | NVU-1…NVU-6；构建/QEMU/C/ASM | 回执 §3 消费限定 |

05 另有本地记录（不是勘误，待主线审查）：本次 L02 静态复核两份输出与冻结记录逐字节一致，见 `records/l02_compare.json`。

**问题登记入口：** 18 号 §3 导航覆盖 core（含 consumer-fix-02 复判，须同读 recheck-02 回执；recheck-01 未单列）、order-02、integration-03、mm-04 四组；pilot 没有导航行，限制在 pilot 回执 §2/§3 与其 MANIFEST open_questions。recheck-01、consumer-fix-02 没有独立 deferred 文件，问题在各自 MANIFEST open_questions 与 results.yaml。05 在 `user-vfs-baseline-05/deferred-findings.yaml`（UV-01…UV-16、勘误 2、状态变更 10、交叉链接 13）。本次接手的普通问题在本目录 `deferred-findings.yaml`（LA-01…LA-04）。18 号 §3 与主线 MANIFEST 都还没有 05 的行，05 入口以 05 MANIFEST、05 回执和 10-25 检查点为准。

## 3. 云端仅存资产

| 类别 | 本地状态 | 重建方法 |
|---|---|---|
| 05 探针驱动 run_probes.py（A1） | 已接收，字节数与 sha256 同 HANDOFF 表；未执行 | 复跑须改 PKG/CHK 并按 L02 方式重绑 REPO，副本不建在仓库内；本轮不要求 |
| 05 探针副本 p0…p12 | 未搬运 | 由 A1 复制 05 包后逐个改坏得到 |
| 05 生成器 gen_facts.py、resolve.py、anchor_specs.py（A2–A4） | 已接收，同上；未执行 | gen_facts 依赖未保全的内容模块，不能单独运行 |
| 05 生成器其余 15 个文件（5 个内容模块、3 个模板、render_baseline/fill_docs/apply_review 三个脚本、4 个试运行脚本） | 缺失 | 按 HANDOFF：内容模块可由入库 YAML 反推，模板对应入库 Markdown；检查器只读入库文件与固定 Git 对象，不读 scratch |
| 05 docs 模式 scope_docs.json、文档提交三条回读 | 缺失 | scope_docs 结论见 05 evidence.md §7；三条回读结论曾写在 PR17 顶部，后已被主线改写（本地推断：原记录无法逐字重建） |
| 05 源码解包副本 | 未搬运 | `git archive a039d9803ade mykernel myinitramfs`（本轮未运行） |
| 回读工具 readback2.py、harness2.py；任务书副本 c17/c19 | 已在 GitHub | `core/recheck-01/fixtures/`；主线 17、19 号 |
| 更早批次目录（pilot-work、core-work、raw、recheck-runs、recheck02、order02、integ03 其余、mm04、reread-1001） | CLOUD_SCRATCH_NOT_INVENTORIED | 未知，不当作无遗漏（LA-02） |

## 4. 消费提示（本轮接触的旧口径）

这些是读旧报告时的限定，不是对原报告的修改。

- **公式结果不是机器终态：** VT-FR01 是公式结果，不是实际复制字节数或返回值；R3 的负长度支持越界或非预期复制的风险，未运行时不能解释成某种必然的机器表现。
- **局部无调用不等于全树不可达：** 单个源码条件、词法负检索、`visible_active_text` 分类、范围内的判定（如 PageBuddy 判假只在已查初值与 Q34 范围内，msleep 未命中只在原文本扫描范围内），都不升级为全局可达性结论；swait 两函数“文件外无调用者”已整条撤回，与“永不入队”一样不得恢复。
- **Python 检查执行过，内核 NOT_RUN：** 05（及 04）MANIFEST 里的“没有运行”只指内核、C/ASM、构建/安装与旧测试；云端执行方与本机 L02 都运行过 05 的 Python 文本检查器。pilot、core、recheck、order-02 等早期包另在宿主上编译运行过 C/原 asm 片段，那些是各包回执所限定的宿主观察，不是内核运行。`all_ok` 只表示机械一致性。
- **链与运行：** 六条链里 endpoint/broken 段不能画成运行闭环；候选 A/B、键下限、OD-1/OD-2 都不是已采用政策。

## 5. 下一次安全分析可用的资料缺口（3 条）

只给入口、范围与所需授权；不是本轮研究，也不是修复建议。下列定义位置为本次只读 `git grep` 的符号定位，没有读正文，也不是 05 的结论。

| 缺口（05 原状态） | 端点与范围 | 证据入口 | 所需授权 |
|---|---|---|---|
| GAP-U03（open）：set_init_mount、set_init_taskfs、init_mount(devtmpfs) 正文未逐行读，找不到根分区时行为未知 | 调用点 `fs/vfs/myos_vfs.c::myos_switch_to_root_disk`；定义 `myos_vfs.c` 第 42、49 行，`fs/vfs/init.c` 第 16 行 | capability fs.vfs.root_switch；anchors A-C1-05…07；queries Q07/Q18/Q37；UV-01 unknown | 读正文只需静态只读；实际行为能否静态确定未核；若要运行观察需构建与 QEMU 授权（当前没有） |
| GAP-U04（open）：syscall_return_via_sysret、ret_from_fork 与 pt_regs 来源未追 | `arch_task.c::kjmp_to_doexecve` → `entry_64.S::syscall_return_via_sysret`（第 184 行）；`entry_64.S::ret_from_fork_asm`（第 255 行）→ `process_arch.c::ret_from_fork`（第 63 行；entry_64.S 第 272 行 call，grep 所见，未读正文）；`process_arch.c::copy_thread`（第 82 行） | edge E07（hits 为空）；anchors A-C1-10/11；chains C1/C2 coverage_limits；UV-01 unknown | 追汇编与 pt_regs 来源只需静态只读；确认实际寄存器值需构建与 QEMU 授权（当前没有）；与 GAP-U09 的 copy_thread 范围重叠 |
| GAP-U05（open）：devtmpfs 与 /dev/console 的 f_op 未追 | `init_mount("devtmpfs", "/dev", …)` 调用点；用户侧 `myinitramfs/myinit/initd.c` 的 freopen；`drivers/base/devtmpfs.c` 存在，console f_op 所在文件未定位 | capability fs.vfs.read_syscall not_covered；chain C4 coverage_limits；OLD-U13；Q16 不覆盖 freopen | 追绑定只需静态只读；05 未为 /dev/console 实际读写设运行候选，若要观察，所需授权由 Owner 另定（本地推断可能涉及宿主夹具或构建/QEMU，当前都没有） |

## 6. 不在本地图内

本地图只覆盖上面 8 个包；没有 53 个子系统的全量盘点，也不把 007R 整项写成已完成。阶段 3 学习路线与裁剪决策不在本任务范围。
