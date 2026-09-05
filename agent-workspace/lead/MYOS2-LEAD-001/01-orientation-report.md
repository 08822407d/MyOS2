# 定向报告（MYOS2-LEAD-001 · 阶段 0 交付件 1）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: orientation_report
evidence_class: B（现状盘点）；决策转述部分为 C（引自 WAVE-2-PLAN §3）
produced_by: claude-fable-5.1@claude-code-vscode（主导会话；启动时 myos2-62 [c480bd]，现 myos2-93 [b18176]；UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-09-03
base_snapshot: 工作区 origin/master @ d231708c77e101dd38280ec0dc74e73d16a0446a；内核 time @ a039d9803ade2a1613d620bda375e028530d5242；本分支 head 见 checkpoints/
inputs_read: [00-owner-work-order-verbatim.md, 00-startup-receipt.md, predecessor/*, origin/master:agent-workspace/{README,conventions,WAVE-1-REVIEW(提取件),WAVE-2-PLAN,LAUNCH,repo-map}.md, results/{ARCHIVE-RECEIPTS,EXTRACTION-NOTE,README}.md, results/*/MANIFEST*.md, tasks/*（12）, Mnemosyne 三份规范记录, time 检出的若干只读核对（见 05 文件）]
status: final（阶段 0 门前版本；门后变更以 checkpoints/ 与新文件记录）
open_questions: 见 §3 open items 与 §4 决策清单
```

## 0. 一句话

接手完成：工作区、前任记录、第一波产出与评审都已读通并落盘；Owner 要的"编造分析找回＋重做任务书"已交（05 文件＋三份任务书＋协议）。现在停在阶段 0 的门，等 Owner：合并 PR #6、答 §3/§4 中想答的、说一声"进阶段 1"。

## 1. 现状盘点

### 1.1 仓库与分支

| 项 | 值 | 备注 |
|---|---|---|
| 内核分析基线 | `time @ a039d9803ade2a1613d620bda375e028530d5242` | = origin/time = 主检出 HEAD；工作树干净 |
| 工作区基线 | `origin/master @ d231708c77e101dd38280ec0dc74e73d16a0446a` | 相对本地 master（63f0785c，过期）多 5 个提交，全部只动 agent-workspace/ |
| 内核内容分叉 | 无：`git diff time...origin/master -- mykernel/ myloader/ myinitramfs/` 为空 | time 领先 44 提交（内核），master 领先 5 提交（工作区） |
| 本会话工作副本 | `/home/cheyh/projs/myos2-agent-ws` @ 分支 `agent/MYOS2-LEAD-001`（自 origin/master） | draft PR #6 |
| 远端 agent/ 分支 | MYOS2-DR-001/002/003/004/005/008（第一波 GPT 所建）＋ MYOS2-LEAD-001 | 004 为空分支 |
| 本地遗留 | Mnemosyne 会话的 worktree（/tmp/…/myos2-master @ d231708c, 分支 archive-receipts-wave1，目录仍存在）＋ 本地分支 agent-workspace-bootstrap、agent-wave-001-integration、archive-receipts-wave1 | 未动 |
| 危险脚本 | `scripts/phys_nvme_install.sh`、`scripts/part_vdisk.sh`、根目录 `make_install.sh` | 未读全文、未运行（工作令 §4.5） |

### 1.2 工作区清单（origin/master d231708c）

`agent-workspace/`：README.md、conventions.md、LAUNCH.md（第一波历史）、repo-map.md、WAVE-1-REVIEW.md、WAVE-2-PLAN.md、tasks/（12 份：001~009、004R、010、011）、results/（001~009 九个目录＋ARCHIVE-RECEIPTS.md、EXTRACTION-NOTE.md、README.md）。本分支新增 `lead/MYOS2-LEAD-001/`、`tasks/` 四份新文件，README/ARCHIVE-RECEIPTS/WAVE-2-PLAN 各追加一节。

### 1.3 第一波九任务状态

| 任务 | 交付 | 可靠性 | 编造 | 对话原件归档 | PR |
|---|---|---|---|---|---|
| 001 | PR #2（5 md＋14 proposed） | low | 是 | Alaya 批次 1 | #2 OPEN |
| 002 | PR #3（MANIFEST、矩阵、YAML） | medium | 否 | 批次 1 | #3 OPEN |
| 003 | PR #1（MANIFEST/v2、deps.yaml/dot、init-sequence、trim） | low | 是 | 批次 1 | #1 OPEN |
| 004 | 零交付（received/ 故障报告） | — | 否 | 批次 1 | 空分支，无 PR |
| 005 | PR #5（12 卡＋atlas＋普查＋OQ） | low | 是 | 批次 1 | #5 OPEN |
| 006 | 对话降级（received 50,963 B；拆出 MANIFEST、01-survey、proposed/3 件；02-adoption-plan 未拆） | low | 是 | 批次 1 | — |
| 007 | 对话降级（拆出 MANIFEST、01-evidence-survey、importance.yaml、roadmap） | medium | 否 | 批次 1 | — |
| 008 | PR #4（MANIFEST/v2、01-summary/v2、02-correctness-suspects、debt-register.yaml） | medium | 否 | 批次 1 | #4 OPEN |
| 009 | 对话降级（received 61,623 B；全部五份未拆） | medium | 否 | 批次 1 | — |

全部内容已通过集成分支进入 master（Mnemosyne 01 记录 §7）；五个 PR 只是残留。

### 1.4 任务书状态

就绪未发射：004R（P0，执行者归属待裁）、010（P1）、011（P2）。本会话新起草（draft）：003R（P0）、001R（P1）、005R（P2）＋ 共用协议 `tasks/00-gpt-task-protocol-v1.md`。

### 1.5 本地任务 L1~L11

全部未启动。本会话在 05 文件把与编造处置直接相关的部分重编为本地 A（引文闸门）、B（假 SHA -v2）、C（L4 重锚）、D（L2 配置面）、E（L6 冒烟）；其余 L3/L5/L7/L8/L9/L10/L11 维持 WAVE-2-PLAN 原定义。取号从 012 起，每项单独请 Owner 批示。

### 1.6 归档

批次 1（九件＋任务书）已入 Alaya `research/MYOS2/`（archive_commits 76ef190 → 1c4f253），双侧哈希本会话抽 3 条复核一致。批次 2 待第二波回收；Alaya 写入权仍需 Owner 单独同意（§4.9）。ARCHIVE-RECEIPTS 维护权已在本分支改记为本轨道。

### 1.7 本会话阶段 0 做了什么

1. 启动仪式十二项 PASS（00-startup-receipt.md）；Owner 补充 2 条逐字登记（00-owner-work-order-verbatim.md §13）。
2. 建 worktree 与分支；四批提交（19802097 → 3fa6f5e9 → 3f48a175 → af00db0e → 本文件所在提交）；draft PR #6。
3. 镜像 Mnemosyne 两份记录（predecessor/，双侧哈希）；README 登记 lead/；ARCHIVE-RECEIPTS 维护权移交。
4. 03 暂定执行源清单（S1~S13 逐条核出处）；05 编造台账（机械取证）＋重做计划；三份重做任务书＋协议；WAVE-2-PLAN §5；02 维度台账 v0.1；本报告。
5. §10 接手确认：Mnemosyne 主导会话（UUID f0e60aba…）不在线，投递到 mnemosyne-0c 失败、mnemosyne-89 回复"我是维护线会话（UUID 0f041ee3…），非收件方，已留存未执行"。已按兜底把确认件写到 `~/Downloads/startup-packages-2026-09-02/receipts/MYOS2-LEAD-001-handover-confirmation-to-mnemosyne.md`，请 Owner 转交。
6. 自动记忆只写了两条 Owner 偏好（沟通方式、Owner 画像），无任务状态（§4.10）。

### 1.8 未动的受保护对象（机械证明）

`git diff --name-only origin/master...HEAD | grep -v '^agent-workspace/'` → 空；README/ARCHIVE-RECEIPTS/WAVE-2-PLAN 的 diff 删除行数 = 0；主检出始终在 time、干净；本地 master ref 未动；PR #1~#5、遗留 worktree/分支未动；内核本体未改、未构建、未运行脚本；Alaya、Mnemosyne 仓未写。

## 2. 分工补齐（WAVE-1-REVIEW 主攻清单 8 项；评审只明示了 1/4/6）

| # | 项 | 执行者（建议） | 依据 |
|---|---|---|---|
| 1 | atomic add/sub 反转＋spin_trylock CAS（<20 行） | **Owner 亲手**；本地 Claude 写回归 | 评审 §7.7；S7 |
| 2 | 验证 substrate：重跑 004＋冒烟 harness | 004R（GPT 或本地，待裁）＋本地 E | S7 |
| 3 | 永久自旋 32 处改受控错误返回 | 本地 Claude 出补丁进 results/，采用由 Owner | 改内核须 Owner 决定（§4.1） |
| 4 | 唤醒链与调度状态机 | **Owner 亲手** | 评审 §7.7 |
| 5 | 定时到期→唤醒→调度（与 4 同病灶） | **Owner 亲手**（与 4 并做）；本地陪跑 | 评审 §6 建议并做 |
| 6 | lockdep-lite | **Owner 亲手**；011 供设计输入 | 评审 §7.7 |
| 7 | 构建诊断第一刀（-Wreturn-type 等＋GLOB 换显式清单） | 本地 L8（需隔离副本＋逐项授权） | §4.5 |
| 8 | ATA 根盘存储栈治理 | 本地 L11（需 2、3 先就位；决策 9） | WAVE-2-PLAN L11 |

## 3. Open items（登记；裁决权在 Owner；不答按"默认"）

| # | 事项 | 建议 / 默认 |
|---|---|---|
| OI-1 | **004R 执行者矛盾**：任务书 `mode: 普通对话 Pro`（GPT 读仓库），WAVE-2-PLAN §1 只列 010/011 为 GPT 任务、§2 的 L3"重跑 004R"派给本地 Claude | 建议：004R 仍由 GPT 执行（任务书已加固；可再补一句引用协议 v1），L3 改为"本地核验 004R 产出并落地桩"。默认＝按建议 |
| OI-2 | PR #1~#5 去留（内容早已进 master） | 默认不碰；答"关"＝说明谁关 |
| OI-3 | Mnemosyne 遗留 worktree（目录仍在）与三条本地分支；主导会话不在线 | 默认不碰；由 Owner 或 Mnemosyne 会话清 |
| OI-4 | 本地 master ref 过期（63f0785c） | 默认不更新；需要时 Owner 说"更新本地 master"我再做 `git branch -f master origin/master`（不影响 time） |
| OI-5 | 脱敏：工作令仓库副本里 gitee 备份镜像地址已替换为占位符 | 默认维持脱敏；答"保留"＝改回逐字节 |
| OI-6 | Mnemosyne 侧移交记录（§10 约定的 MNEMOSYNE-262）尚未发生；接手确认走了兜底 | 请 Owner 转交 Downloads 里的确认件，或告诉我主导会话的当前名字 |
| OI-7 | 镜像所据 Mnemosyne 本地 origin/master（c319397）未 fetch | 默认维持；答"可以 fetch"＝我 fetch 后如有更新出 -v2 镜像 |
| OI-8 | **独立复核层未跑**：阶段 0 的提取/取证由本会话单独完成（子代理复核因 API 529 过载全部失败）；05 文件里标 [VRF] 的是机械事实，标 [REVIEW] 的未独立复核 | 默认：API 恢复后补跑一轮对抗复核并出 05-v2；不阻塞门 |
| OI-9 | Alaya 写入授权：Owner 2026-08-31 归档原话含授权，本轨道工作令 §4.9 要求单独同意 | 默认按工作令（不写 Alaya）；批次 2 时再问 |
| OI-10 | README"未来接管协议"是否追加第 6 条"交接自足" | 默认不改；答"加"＝追加一节 |
| OI-11 | 001/006 落盘文件里的 27 处假 SHA（含 ci.yml 环境变量）需要本地机械 -v2 替换（本地 B） | 默认列入阶段 1 首批取号 |
| OI-12 | 三个危险脚本是否先加保护（默认 dry-run） | 与决策 4 一起答；默认不动原脚本，只出 proposed-v2 |
| OI-13 | 重做任务书编号用 R 后缀（沿 004R 先例）还是新号 012/013/014 | 默认 R 后缀 |
| OI-14 | S 表内部矛盾：S9 与 §4.9（Alaya）、S1 五条与 §1.3 六条、S6 的"master @ 63f0785c"表述 | 见 03 文件 §3；默认按工作令 |

## 4. Owner 的 12 条待决问题（人话版；每条：问题｜不答＝默认｜答了的影响）

1. **自制 libc 接不接**：`myinitramfs/mylib` 下有 17 份 CMakeLists，但顶层只编 myinit 和 myshell——是故意停用还是漏接？｜不答＝当停用、不动｜答了＝用户态完成度、教学映射、后续用户态任务都据此改。
2. **引导器以哪个为准**：仓库里 2024 年 4 月的预编译 `BOOTX64.EFI`（CMake 只安装它），还是 `myloader/uefi/` 的源码？｜不答＝当预编译件为准｜答了＝构建链只留一条，001R 和构建诊断据此定。
3. **构建输出要不要统一到 out/**：现在 `.vscode/launch.json` 八处写死 `build/kernel`、`build/sh`。｜不答＝不动｜答"统一"＝001R 提案含迁移，.vscode 由你自己改。
4. **物理盘同步范围**：固定几个目录，还是整个根文件系统？（现在 `phys_nvme_install.sh` 是无条件清空 /mnt1）｜不答＝脚本提案只加保险不改语义｜答了＝按你的语义重写提案。
5. **CI/调试输出走哪条通道**：新写串口驱动、QEMU 的 0xE9 调试口、还是 isa-debug-exit？（现在只有 framebuffer 打印）｜不答＝冒烟测试挂起｜答了＝冒烟测试和调试桩的输出口定了。
6. **词汇表补不补**：`time.misc`（313 行实码没 ID）、`repo.build`/`deploy.scripts` 类目、007 提的 net/security/extensibility。｜不答＝只补 time.misc｜答了＝按你的补。
7. **三套分级口径怎么统一**（002 的风险级 / 007 的 T0~T3 / 008 的 S1~S3）。｜不答＝我先出映射提案再问你｜答了＝直接用你的。
8. **主攻清单 1、4、6 你亲手做**（原子/自旋锁修复、唤醒链与调度、lockdep-lite），我只陪跑写回归——确认？｜不答＝当确认｜答"你来"＝我出补丁进 results/，采不采用你定。
9. **根盘走 ATA 是有意还是遗留**：`-DROOTBLK_NVME` 被注释。NVMe 还维护吗？｜不答＝当 ATA 为现行、NVMe 保留不动｜答了＝存储栈治理范围据此定。
10. **aarch64 目录**只有 CMakeLists 没源码：规划中还是放弃？｜不答＝当占位、不分析｜答了＝可移植性维度是否立项据此定。
11. **SMP 在你学习路线里的优先级**（AP 引导现在原地 `jmp .`）。｜不答＝当晚于并发正确性｜答了＝主攻顺序和 011 的落地映射据此调。
12. **同意 007 推荐的路线 A（并发正确性优先）作主线吗**？另两条：存储栈打通优先、用户态生态优先。｜不答＝按 A 准备材料，不做实施｜答了＝按你的。

## 5. 阶段 0 的门：请 Owner 做三件事

1. 看一眼并合并 PR #6（任务书要在 master 上 GPT 才读得到；合并前可先答 OI-5 脱敏去留）。
2. §3、§4 里想答的答一两条即可（尤其 OI-1 与决策 5，它们卡着 004R 与冒烟测试）。
3. 说一声"进阶段 1"（或改顺序：先发 GPT 任务）。阶段 1 首批本地任务＝A 引文闸门、B 假 SHA 替换、D 配置面真相表（取号 012 起，每项再请一字批示）。

## 6. 同族局限声明

本报告对 Mnemosyne 主导会话（同族同模型）产出的引用与判断不构成独立复核；标 [VRF] 处为本会话在仓库上的只读机械实测；原计划的子代理独立复核层因 API 过载未运行（OI-8）。
