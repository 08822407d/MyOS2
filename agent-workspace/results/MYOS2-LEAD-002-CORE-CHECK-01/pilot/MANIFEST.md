---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
phase: pilot
record_type: cloud_pilot_manifest
transport_marker: MYOS2-CLOUD-PILOT-20260925-K7P4
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "平台规则禁止在推送到仓库的产物中写模型标识；主线如需可向 Owner 核对会话界面所选项"
execution_surface: "claude.ai/code 托管云端会话容器（Ubuntu 24.04.4 LTS，x86_64）；平台分配的会话分支"
date: "2026-09-27"
base_snapshot: "workspace=master（分支名，短 SHA de3bb1df906a）；taskbook=agent/MYOS2-LEAD-002（分支名，短 SHA 57a7c3e0eebf）；kernel=time（分支名，短 SHA a039d9803ade）；短 SHA 均自本会话 git rev-parse --short=12 输出复制"
work_branch: claude/dazzling-cori-q0dnyt
read_channel: mixed
read_channel_detail: "源码与说明：git fetch 远端对象后 git show <ref>:<path>；10 号任务文件另经 raw URL 读取并与 Git 对象逐字节一致；open PR 经 GitHub MCP 读取；P03 回读经 raw.githubusercontent.com 与 api.github.com"
inputs_read:
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md"
  - "master:agent-workspace/conventions.md"
  - "master:agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "time:mykernel/scripts/options_flags.cmake"
  - "time:mykernel/debug/panic.c"
  - "master:mykernel/scripts/options_flags.cmake"
  - "master:mykernel/debug/panic.c"
status: final_for_pilot
pilot_conclusion: PILOT_CANDIDATE
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "写入目录 results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/ 与平台会话分支由 Owner 本次启动指令及 10 号文件 §2 授权；未写 agent/MYOS2-LEAD-002、master、time。"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
  note: "本批不作源码语义断言，未使用 VERIFIED 源码标签；金丝雀与负控以 p01_canary.py 的逐字节比对输出为准（见 evidence.md §3）。"
branch_canary_quotes:
  source: "agent/MYOS2-LEAD-002 MANIFEST.md front matter，PyYAML 解析，未重打"
  time:
    options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
    panic_c_panic: "\tthis_cpu = smp_processor_id();"
  cloud_mechanical_check: "DISCRIMINATES：两条均在 time 整行命中 1 次、在 master 子串命中 0 次；本次云端进程所作，非 Owner 本地"
kernel_modified: false
repo_scripts_run: false
full_build_run: false
qemu_run: false
tools_installed: false
acceptance_ceiling: "PILOT_CANDIDATE（待主线回执 ALLOW_CORE / RETURN_PILOT / BLOCKED）"
open_questions:
  - "执行模型名按平台规则未写入仓库；若主线需要，由 Owner 从会话界面转告。"
  - "正式阶段仅在主线回执 ALLOW_CORE 且 Owner 于同一会话转发继续指令后启动；本执行者未读取 11 号文件。"
---

# CORE-CHECK-01 pilot：P00–P03 均按预期，结论 PILOT_CANDIDATE

**云端会话能读到三个指定分支、真实编译运行并捕捉失败与超时，结果已推送到 GitHub 并经两个远端通道逐字节回读一致。** 未发现阻断项；正式 V00–V14 未执行，等待主线回执。

## 范围

本批只做 10 号文件 §3 的 P00–P03 小试跑。只读 time 源码（经 Git 对象，不切分支、不合并），任务说明读自 `agent/MYOS2-LEAD-002`，工作基分支为 master。只在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/` 新增三个文件，推送到平台分配的会话分支 `claude/dazzling-cori-q0dnyt`。

未改内核，未运行仓库脚本、完整构建或 QEMU，未安装工具，未输出凭据或完整环境。

## 三个文件

| 文件 | 内容 | 边界 |
|---|---|---|
| `result.yaml` | 识别串、三分支短标识、输入文件字节数与 SHA-256 分段、环境/工具版本、P00–P02 的实际值与是否符合预期 | 由脚本从运行输出生成；P03 前提交并冻结，不含回读结论。 |
| `evidence.md` | 全部脚本与 C 小程序全文、实际命令、stdout/stderr、退出码、提交与推送输出、P03 回读、提交范围检查 | 脚本与输出由组装脚本原样嵌入；§1 前置命令输出为手工转录并已标注。 |
| `MANIFEST.md` | 本清单：范围、结论、回读范围、阻断项 | 不替代主线回执。 |

## P00–P03 结论

| 项 | 实际结果 | 符合预期 |
|---|---|---|
| P00 环境与输入身份 | Ubuntu 24.04.4 LTS / x86_64；Python 3.11.15、gcc 13.3.0、clang 18.1.3、git 2.43.0、PyYAML 6.0.1 均存在；master `de3bb1df906a`、time `a039d9803ade`、任务包 `57a7c3e0eebf`，三者与 `ls-remote` 一致；五个输入文件均可取。脚本退出 0。 | 是 |
| P01 金丝雀 + 负控 | 两条 time 引文（含真实制表符）各在 time 整行命中 1 次、master 命中 0 次，判定 `DISCRIMINATES`；制表符换空格变体在 time 命中 0 次；篡改负控 `-mcmodel=kernal` 先确认不同且不在源文中，检查结果不命中，`NOT_HIT_AS_EXPECTED`。脚本退出 0。 | 是 |
| P02 ENVIRONMENT_SMOKE | gcc 实际编译；期望 5 → 输出 `actual=5`、退出 0；期望 6 → 输出 `actual=5`、退出 7；`sleep(2)` 在 0.2 s 限时下被捕获并 SIGKILL 回收（-9，`/proc` 条目已消失），`TIMEOUT_EXPECTED`。驱动退出 0。 | 是 |
| P03 GitHub 回传与回读 | `result.yaml` 于提交 `10ecb7dd0bdb` 推送；`ls-remote` 确认远端分支头即该提交；raw.githubusercontent.com 与 api.github.com 两通道均 HTTP 200、6741 字节、与已提交 blob 逐字节一致，识别串解析正确。脚本退出 0。 | 是 |

## 远端回读范围

P03 只回读 `result.yaml`（第一次提交后）。MANIFEST.md 与 evidence.md 在第二次提交中加入，未写入自身回读结果；其推送后的远端文件清单核对在 PR 说明中报告，不回填本文件。回读不证明执行真实性，识别串也不是密码。

## 阻断项与主线须知

- 阻断项：无。
- 模型身份：平台规则禁止在推送产物中写模型标识，故 `execution_model_selection: unknown_or_not_attestable`；不是抄任务书作者。
- 分支名：使用平台分配的 `claude/dazzling-cori-q0dnyt`，非 `agent/` 前缀，符合 10 号文件 §2 的本专项例外。首次推送显示 `[new branch]`，远端此前无该分支。
- 架构为 x86_64；P02 只跑普通 C，未测 x86 特权指令，正式 V12 能否运行原 asm 仍待正式阶段判断。
- raw.githubusercontent.com 对分支路径可能有缓存；本次回读的是首次出现的新路径，并用 API 通道交叉比对。

## 消费方式

主线从本会话分支或 Draft PR 读取三个文件；按 evidence.md 中的脚本与输出复核识别串、两条引文、负控、编译运行退出码、超时回收与远端回读。复核后在 `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md` 写 ALLOW_CORE / RETURN_PILOT / BLOCKED。执行分支保留至 core 回收与审查完成；PR 暂不合并。
