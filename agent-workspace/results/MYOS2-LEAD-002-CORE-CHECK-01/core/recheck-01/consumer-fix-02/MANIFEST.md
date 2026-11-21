---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-CHECK-01-RECHECK-02
phase: evidence_consumer_recheck
record_type: consumer_fix_manifest
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）；不认证具体后端"
execution_surface: "claude.ai/code 托管云端会话容器；普通用户态 Python 数据层实验，每例独立进程、30 s 上限；与 pilot/core/recheck-01 同一会话、同一执行分支"
lead_attribution:
  source: "13 号任务书与 recheck-01 审查回执的 YAML 头"
  model_per_owner: gpt6
  effort_per_owner: pro
  scope: originating_lead_only_not_executor
date: "2026-10-01"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；agent/MYOS2-LEAD-002 b0aa54db1b70（13 号任务书与 recheck-01 回执，开工时固定）；被审输入 b843d475367a；recheck-01 results 冻结于 efb9846b88ec。均为短 SHA"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
reviewed_input_short12: b843d475367a
results_commit_short12: 77faf51e9a43
read_channel: mixed
read_channel_detail: "13 号任务书与回执经 raw URL 读取并与 git 对象逐字节比较；b843 程序与观测按提交对象解出；PR 状态经 GitHub MCP；results.yaml 回读经 raw.githubusercontent.com 与 api.github.com"
inputs_read:
  - "agent/MYOS2-LEAD-002 @ b0aa54db1b70：13-core-evidence-consumer-recheck.md（全文）"
  - "agent/MYOS2-LEAD-002 @ b0aa54db1b70：reviews/CORE-CHECK-01-recheck-01-review.md（全文）；同批新增的检查点全文；14 号文件只读开头，未执行"
  - "claude/dazzling-cori-q0dnyt @ b843d475367a：recheck-01 的 MANIFEST、evidence 与 S01-S03 涉及的 evaluate2/make_results2/meta_tests 及观测"
  - "agent/MYOS2-LEAD-002 @ 57a7c3e0eebf：07 报告头、07-core-audit-map.yaml、MANIFEST.md（S02 预期对象集合）"
  - "core/fixtures/v01_structure.py @ a1e7c2277705（V01 对象选择规则）"
status: final_for_recheck_02
recheck_conclusion: S01_S03_REPRODUCED_ON_B843_CONSUMER_FIXED_N01_N06_MET_RECORDED_OBSERVATIONS_UNCHANGED
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_acceptance_verdict: NOT_ISSUED
reusable_verifier_self_certified: false
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "只在 consumer-fix-02/ 新增，由 Owner 本轮指令、13 号任务书与 recheck-01 回执授权（allowed_write_prefix 由 guard3 机械核对）。b843 中全部文件不变；未写 agent/MYOS2-LEAD-002、master、time。"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
  note: "本批不新增源码断言标签。data_summary 与 delivered_files 由 fixtures/final_check3.py 对照 results.yaml 与暂存文件核对（evidence.md §9.2）。"
data_summary:
  S01_S03_reproduced_on_b843: true
  N_met: {N01: true, N02: true, N03: true, N04: true, N05: true, N06: true}
  all_N_met: true
  S04_old_new:
    N02.control_complete_hex40_hit_found:
    - [ERROR, null]
    - [VALID, COUNTEREVIDENCE]
    N05.valid_but_different_from_prediction_v04_ret_1:
    - [ERROR, null]
    - [VALID, COUNTEREVIDENCE]
  revalidation_consumer_status: COMPLETE
  revalidation_cases_different_from_efb: []
  revalidation_source_run_status: COMPLETE
delivered_files:
  - MANIFEST.md
  - evidence.md
  - fixtures/build_evidence3.py
  - fixtures/common3.py
  - fixtures/consumer3.py
  - fixtures/consumer_tests.py
  - fixtures/expected_objects.json
  - fixtures/expected_objects3.py
  - fixtures/final_check3.py
  - fixtures/frozen_b843.py
  - fixtures/guard3.py
  - fixtures/make_results3.py
  - fixtures/make_summary3.py
  - fixtures/old_side.py
  - observations/after_results/guard_after_push.json
  - observations/after_results/readback_results.branch.json
  - observations/after_results/readback_results.object.json
  - observations/consumer_tests.json
  - observations/frozen_b843_manifest_start.json
  - observations/frozen_b843_manifest_tests.json
  - observations/guard_pre_results.json
  - observations/guard_pre_results_closed_stray_pyc.json
  - observations/guard_start.json
  - observations/n06_revalidation.yaml
  - observations/old_side_s01_s03.json
  - results.yaml
kernel_modified: false
c_fixtures_compiled_or_run: false
h00_rerun: false
full_tree_scan: false
repo_scripts_run: false
qemu_run: false
tools_installed: false
privilege_escalation: false
credentials_output: false
merge_or_branch_delete: false
open_questions:
  - "CA-02 全局可达性；V08 回插顺序候选（14 号 CORE-SCHED-ORDER-02 未在此执行）；真实 ELF、IRQ、SMP、上下文切换。"
  - "run.json 不可用时，本读取层把依赖阶段完成记录的静态项一并判为不完整；是否另行接受独立阶段文件，待主线约定。"
---

# RECHECK-02：读取层的三处缺口已复现并修好，另修一处同类缺口，已交观测复判结果不变

**S01–S03 先用未改的 b843 程序和已交观测实跑，主线三项推导全部复现，没有反证。新读取层（consumer-fix-02/fixtures/）在 N01–N06 中全部满足。N05 另外暴露出一处主线未列的同类缺口 S04，已一并修正。用新读取层复判 b843 的全部已交观测，结果与 efb9846b88ec 的冻结结果逐项相同。** 本轮只做数据层检查，验收上限仍为 PASS_PENDING_LOCAL。

## 1. 限制（先读）

- **只判读已交观测。** 没有编译或运行 C 夹具，没有重做 H00、全树扫描或研究，没有改内核。N06 叫 RECORDED_OBSERVATION_REVALIDATION，不是新的宿主实跑。数据完整时得到的仍是 recheck-01 已有的受限结论。
- **结构检查只覆盖读取层实际使用的字段与对象集合**，不是任意损坏或恶意输入的通用证明。
- **预期对象集合**（S02）从 taskbook 57a7c3e0eebf 的冻结输入提取，选择规则沿用写出记录的 producer 程序（a1e7 的 v01_structure.py）。如果 producer 的选择规则本身有误，预期集合会继承它。
- **重复运行一致性**只能读 producer 写下的布尔摘要和第二次的退出码与终态；第二次运行的原始 stdout 没有保存，本轮不补造。
- **run.json 不可用时**，阶段完成记录随之缺失，依赖它的静态项（V00、V01、A46、V08 限定模型层、V14 源码层）一并判为不完整。这是保守选择（见 open_questions）。
- **S04 的修正**只改变负控判定的一种情形；冻结 evaluate2 中各案例的预测与负控取值没有改动。
- **R02/R04** 在 recheck-01 回执的范围内已关闭，本轮没有重跑。
- 执行模型未知；没有 CI 或人工逐行复核。内核问题（CA-02 全局可达性、V08 排序候选、真实 ELF/IRQ/SMP/上下文切换）继续开放。

## 2. S01–S03：旧侧实际结果（修订前）

程序与观测都从 b843d475367a 逐字节解出；每例独立进程。记录：observations/old_side_s01_s03.json。

| 项 | 输入（HARNESS_META_TEST） | 冻结程序实际返回 | 主线预测 | 复现 |
|---|---|---|---|---|
| S01 | 实际 V04 记录删去 run 中 collect_timed_out、terminal_state、reaped_confirmed、stderr | VALID / OBSERVED_AS_PREDICTED | 同左 | 是 |
| S02 | 实际 v01.json 分别清空 referenced_paths、report_inputs_read、hex40_hits_in_scope_files，及三处同时清空 | 四种都是 VALID / NO_FAILURE_IN_SCOPE | 同左 | 是 |
| S03a | 完整复制观测但省略 static.json，冻结 make_results2 CLI | exit 0；顶层仍写 generated_from_run_status_COMPLETE；V14 INCOMPLETE_EVIDENCE；V08 仍为缓存的 VALID/COUNTEREVIDENCE，限定模型 COUNTEREVIDENCE_IN_LIMITED_MODEL；CA-02 保留限定模型判定 | V14 缺证据而 V08 沿用缓存 | 是 |
| S03b | static.json 内容只有 `{` | exit 1；JSONDecodeError；没有报告 | 错误收集前抛异常、无部分报告 | 是 |
| 对照 | 完整复制 | exit 0；报告与 efb 的 results.yaml 逐字节相同 | - | - |

## 3. 新处理

| 问题 | 修订（consumer3.py / make_results3.py） | 验证 |
|---|---|---|
| S01 必需终态信息被默认补成成功 | RAN 案例必须带 producer 已写、consumer 要用的字段（case：status、repeat_identical、repeat_returncode、repeat_terminal_state；run：returncode、timed_out、collect_timed_out、output_lost、terminal_state、reaped_confirmed、stdout、stderr）。缺键 → INCOMPLETE_EVIDENCE [MISSING_FIELD]；类型错 → INVALID_EVIDENCE [WRONG_TYPE]；冻结检查漏掉的显式失败（output_lost=True、重复运行终态非 exited）→ INVALID_EVIDENCE [EXPLICIT_FAILURE]。不再默认 exited/True/无错误 | N01 |
| S02 空对象清单仍全真 | referenced_paths、report_inputs_read、hex40_hits_in_scope_files、parse、completion_flags、MANIFEST 计数表都与 expected_objects.json 比对。缺成员 → INCOMPLETE_EVIDENCE；多余、重复、改名或成员类型错 → INVALID_EVIDENCE。集合完整时交给冻结 v01：空错误列表仍是合法结果，成员 exists=False 仍是 VALID 发现 | N02 |
| S03 缓存判定掩盖缺证据 | 每个输入有读取状态（OK、MISSING、EMPTY、JSON_SYNTAX_ERROR、NOT_UTF8、WRONG_TOP_TYPE）与结构检查；按当前可用原件重算全部项目，不把 run.evaluation 的缓存当作当前结论；源运行状态保留在 source_execution，消费状态单列在 consumer_validation；报告列出文件名、错误类别和受影响项。退出码 0 完整、2 部分（报告已写）、4 读取器自身失败 | N03、N04 |
| S04（本轮发现）有效观测等于负控错误值时被记为 ERROR | 冻结 judge 用观测值比一个固定错误值；V04 ret=1、V01 hex40 命中 1 这类完整有效观测会得到 ERROR/COMPARATOR_INVALID。修正后只在负控错误值等于预测值（负控无意义）时保留 ERROR，否则按发现判定 | N02、N05 |

冻结文件本身没有改动。新读取层在自己的进程里包装 `evaluate2.validate_case` 与 `evaluate2.judge`：包装先取冻结函数的判定，再追加本轮检查。

## 4. N01–N06 覆盖

| N | 内容 | 关键观察 | 满足 |
|---|---|---|---|
| N01 | V04 真实记录：四个键分别删除与合并删除；另删 repeat_terminal_state、output_lost；显式失败与类型错；完整对照 | 旧侧删键全部 VALID；新侧全部 INCOMPLETE_EVIDENCE 并带 MISSING_FIELD；显式失败仍拒绝；完整对照 VALID | 是 |
| N02 | v01 三种集合分别缺一项、清空、合并清空；重复成员；改名成员；完整对照；exists=False 对照；hex40 命中对照 | 旧侧全部 NO_FAILURE；新侧缺项为 INCOMPLETE_EVIDENCE，重复/改名为 INVALID_EVIDENCE；完整对照 NO_FAILURE；exists=False 为 VALID/COUNTEREVIDENCE；hex40 命中 1：旧 ERROR，新 VALID/COUNTEREVIDENCE | 是 |
| N03 | 完整 CLI：省略 static.json、省略 v01.json、完整对照 | 新侧 exit 2；V08 只保留函数层，限定模型层不再有效，CA-02 只剩函数层；V14 源码层未执行；缺 v01 时只有 V01 不完整；其余项与对照相同；源运行 COMPLETE 作为历史保留 | 是 |
| N04 | static/v01 各三种损坏（`{`、空文件、`[]`）；run.json 不可解析；读取器失败对照 | 旧侧损坏时崩溃或沿用缓存；新侧 exit 2 并写出部分报告，受影响项没有行为结论，其余项 VALID；run.json 坏时动态证据不可用；冻结模块被改时 exit 4，只写错误记录 | 是 |
| N05 | M01–M05 变造逻辑在真实记录上重做；0/42/43 正控；有效但与预测不同（V04 ret=1）；经 CLI 的 M08、M09（含阶段失败变体）及 N02 变换 | 旧拒绝仍拒绝；正控仍有效；ret=1 为 VALID/COUNTEREVIDENCE（旧侧 ERROR）；CLI 路径结论随输入变化 | 是 |
| N06 | b843 全部已交观测复判 | consumer COMPLETE；15 项、A46 更正、CA、计数、R/M 摘要与 efb 冻结结果相同；缓存与当前判定无差异；旧 A46 失败保留，A46-C/ASM 分列，未执行层未升级 | 是 |

逐项旧/新返回见 results.yaml `new_tests` 与 evidence.md §4。

## 5. 文件清单

路径相对于 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/`，共 26 个文本文件，没有二进制。

| 文件 | 内容 |
|---|---|
| `MANIFEST.md`、`evidence.md` | 本清单；由 build_evidence3.py 生成的证据（手工转录处已标注） |
| `results.yaml` | 由 make_summary3.py 从 observations/ 生成；旧负例、新负测、复判与差异、输入身份；源运行事实与消费检查分列 |
| `fixtures/common3.py` `frozen_b843.py` `guard3.py` | 固定对象与辅助；b843 逐字节解出；保护门 |
| `fixtures/old_side.py` | S01–S03 旧侧（修订前运行一次，之后未改） |
| `fixtures/expected_objects3.py` `expected_objects.json` | S02 预期对象集合及其提取程序 |
| `fixtures/consumer3.py` `make_results3.py` | 新读取层与完整入口 |
| `fixtures/consumer_tests.py` | N01–N06 |
| `fixtures/make_summary3.py` `final_check3.py` `build_evidence3.py` | results.yaml 生成、发布前检查、evidence 生成 |
| `observations/old_side_s01_s03.json` | S01–S03 旧侧原始记录 |
| `observations/consumer_tests.json`、`n06_revalidation.yaml` | N01–N06 记录与完整复判报告 |
| `observations/guard_start.json`、`guard_pre_results.json`、`guard_pre_results_closed_stray_pyc.json` | 开工时、结果提交前的保护门；一次因本会话生成的缓存文件而关门的真实记录 |
| `observations/frozen_b843_manifest_start.json`、`frozen_b843_manifest_tests.json` | 两次 b843 解出清单（字节数与 SHA-256） |
| `observations/after_results/`（3 个） | results.yaml 的 object/branch 回读与推送后保护门 |

复用的 b843 模块（recheck-01/fixtures/ 下）：evaluate2.py、make_results2.py、harness2.py、identity2.py、readback2.py。运行时从 b843 对象解出，并在导入前逐字节核对，字节数与 SHA-256 记录在两份解出清单中。

## 6. 重建命令

```bash
git clone https://github.com/08822407d/MyOS2 && cd MyOS2 && git checkout claude/dazzling-cori-q0dnyt
export MYOS2_REPO=$PWD PYTHONDONTWRITEBYTECODE=1
cd agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/fixtures
P=$(mktemp -d)                                     # 新建空目录；程序不复用、不清理已有目录
python3 frozen_b843.py $P/frozen                   # b843 程序与观测逐字节解出
python3 guard3.py $P/frozen $P/guard.json          # 退出码 0 开门，3 关门
python3 old_side.py $P/frozen $P/work $P/old_side.json                              # S01-S03（冻结程序）
python3 consumer_tests.py $P/frozen $P/work $P/consumer_tests.json $P/n06.yaml      # N01-N06
python3 make_results3.py --frozen $P/frozen/fixtures $P/frozen/observations $P/consumer.yaml   # 任一观测目录
python3 make_summary3.py ../observations $P/results.yaml && cmp $P/results.yaml ../results.yaml
python3 expected_objects3.py $P/expected.json && cmp $P/expected.json expected_objects.json
```

- `make_results3.py --frozen <解出目录>/fixtures <观测目录> <out.yaml>`：退出码 0 表示消费完整（可含核验发现），2 表示部分（报告已写，受影响项无行为结论），4 表示读取器自身失败（只写 `<out>.error.json`）。
- `guard3.py` 要求在执行分支上、b843 是 HEAD 的祖先，并且相对 b843 只在 consumer-fix-02/ 下新增。
- 需要 git、Python 3 + PyYAML；回读需要访问 raw.githubusercontent.com 与 api.github.com。不需要 gcc。

## 7. 回读与交付

- 结果批 `77faf51e9a43`：results.yaml 33576 字节。object 与 branch 两种模式、raw 与 api 两个通道都满足条件：curl 退出 0、未超时、HTTP 200、与提交 blob 逐字节相同、packet_id 正确。推送后保护门打开，相对 b843 的 20 项变化全部是 consumer-fix-02/ 下的新增。
- 文档批：本清单、evidence.md、build_evidence3.py 与 after_results/ 三份记录。提交前运行 `final_check3.py docs 77faf51e9a43`，输出嵌入 evidence.md §9.2；推送后的核对写在 PR 正文中。
- 对“无调用者”说法的撤回导航见 evidence.md §7。本轮没有为它重新扫描源码。
- PR #17 保持 Draft，不合并；执行分支保留到主线审查完成并明确解除。
