# 第一波编造台账与补充/重做计划（MYOS2-LEAD-001 · 阶段 0 交付件 5；响应 Owner 补充说明第 1 条）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: fabrication_ledger_and_redo_plan
evidence_class: B（本会话机械取证）＋ C（WAVE-1-REVIEW 的评审结论，标注来源）
produced_by: claude-fable-5.1@claude-code-vscode（主导会话，UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-09-03
base_snapshot: 工作区 origin/master @ d231708c77e101dd38280ec0dc74e73d16a0446a；内核 time @ a039d9803ade2a1613d620bda375e028530d5242
inputs_read:
  - origin/master:agent-workspace/WAVE-1-REVIEW.md（经子代理全文提取，本会话复核其 key facts 的仓库侧证据）
  - origin/master:agent-workspace/results/**（全部非 received 文件的 SHA 扫描；received 原件只计数）
  - origin/master:agent-workspace/results/MYOS2-DR-003/{deps.yaml,deps.dot,init-sequence.md,trim-analysis.md,MANIFEST*.md,FINAL-RESPONSE.md}（行号引用抽样）
  - origin/master:agent-workspace/results/MYOS2-DR-001/02-problems.md（H/M 条目定位）
  - origin/master:agent-workspace/results/MYOS2-DR-005/{00-usage-census.md,cards/*.md,cpuid-atlas.md,open_questions.md}（错值定位，grep）
  - origin/master:agent-workspace/results/*/MANIFEST*.md（9 份）；tasks/ 全部 12 份；LAUNCH.md；repo-map.md；EXTRACTION-NOTE.md
  - /home/cheyh/projs/MyOS2（time 检出）：mykernel/init/main.c、mm/misc/mm_init.c 等被引文件（只读 sed/wc）
status: final（台账）／draft（计划：任务书为草案，发射与顺序待 Owner）
open_questions:
  - 独立复核层未跑：本文件的取证由本会话单独完成（API 529 过载导致子代理复核全部失败），待 API 恢复后补一轮对抗复核
  - 005 的"PTE 物理地址自 bit13"错值本会话未在交付件中定位到具体行（grep 未命中），以 WAVE-1-REVIEW 所述为准
```

## 0. 一句话

Owner 问"能不能找回上一个会话对编造情况的分析，再设计一次补充/重做任务书"。答：找回了（§1），并用机械手段复核了其中能复核的部分（§2）；重做方案见 §3~§5：三份 GPT 重做任务书（001R / 003R / 005R）＋一份所有 GPT 任务共用的反编造协议＋若干本地配套任务；006 的编造处置不需要 GPT，是本地机械替换。

## 1. 找回的分析在哪里（C 类）

| 材料 | 位置 | 内容 |
|---|---|---|
| 第一波回收评审报告 | `origin/master:agent-workspace/WAVE-1-REVIEW.md`（14,183 B，2026-09-01，提交 11a8d8f6 引入） | 17 子代理（8 精读＋8 对抗核查＋1 综合）在本机 time 检出上实做核查；§1 九任务合规/可靠性表；§3 编造与失实专节；§3.4 行号降级纪律；§4 十一条独立发现；§5~6 交叉综合与主攻清单；§7 八项风险 |
| Mnemosyne 侧回收记录 | 本目录 `predecessor/mnemosyne-01-wave-1-intake-and-review.md`（逐字节镜像） | 回收方式、004 诊断、方法论回授（"溯源字段被编造"是新失效模式）、§7 未做清单 |
| 拆分状态说明 | `origin/master:agent-workspace/results/EXTRACTION-NOTE.md` | 006/007/009 降级交付的拆分情况与使用前必读 |
| 第二波计划 | `origin/master:agent-workspace/WAVE-2-PLAN.md` | 分流原则、010/011 提示词、本地 L1~L11、12 条决策、硬纪律 |

评审的九任务裁定（[EXTERNAL WAVE-1-REVIEW §1]，本会话未逐条复算"实/伪/无法验证"计数）：

| 任务 | 交付 | 合规 | 可靠性 | 实/伪/无法验 | 编造？ |
|---|---|---|---|---|---|
| 001 外围工程审计 | PR #2 | PARTIAL | low | 9/14/1 | **是**：假 SHA＋成段虚构 |
| 002 完成度盘点 | PR #3 | PARTIAL | medium | 20/5/1 | 否（证据路径失实） |
| 003 依赖图 | PR #1 | PASS | low | 15/4/1 | **是**：行号系统性偏移 |
| 004 调试桩 | 零交付 | — | — | — | 否（诚实报告连接器故障） |
| 005 x86 资料包 | PR #5 | PASS | low | 20/8/0 | **是**：SDM 位域错值、假 [VERIFIED] |
| 006 测试与 CI | 对话降级 | PARTIAL | low | 10/12/1 | **是**：假 SHA ×14＋写进 ci.yml |
| 007 重要度与路线 | 对话降级 | PASS | medium | 11/2/1 | 否（tier 计数错） |
| 008 技术债台账 | PR #4 | PASS | medium | 21/3/0 | 否（行号越界） |
| 009 教学蓝本预研 | 对话降级 | PARTIAL | medium | 15/3/0 | 否（零源码接触） |

## 2. 编造台账（本会话 2026-09-03 机械取证；`[VRF]`＝本会话实测，`[REVIEW]`＝仅据 WAVE-1-REVIEW）

| # | 任务 | 编造类型 | 具体位置（origin/master，agent-workspace/results/ 下） | 机械证据 | 对下游的影响 | 处置 |
|---|---|---|---|---|---|---|
| F1 | 001 | 伪造 40 位 commit SHA `a039d9803ade94c67918930525530d2a1b46e9f0` | 非 received 文件共 **20 处/19 文件**：01-inventory.md、02-problems.md、03-target-structure.md、FINAL-RESPONSE.md 各 1；MANIFEST.md 2；proposed/ 下 14 个文件各 1（cmake/MyOS2Common.cmake、dbg-qemu.sh、make_install.sh、myinitramfs/CMakeLists.txt、myloader/CMakeLists.txt、scripts/{common,prepare_env,map_vdisk,part_vdisk,phys_nvme_install,make_install_kernel,make_install_initranfs,make_install_bootloader}.sh） | `git cat-file -e` 失败 [VRF]；前 12 位 `a039d9803ade` 是真提交 a039d980 的唯一前缀，第 13 位起编造 [VRF] | 任何按该 SHA checkout 的自动化必败；proposed/ 脚本头部带假溯源 | 重做（001R，§4）＋本地机械 -v2 替换（§5 本地 B） |
| F2 | 001 | 成段虚构：H-03 称 map_vdisk.sh 固定 `/dev/nbd0`、`qemu-nbd`、`losetup -D`；H-05 称 `install_boostloader()` 用 `\|\| true` | 02-problems.md 第 47~49 行（H-03）、第 63~65 行（H-05）[VRF 定位] | 全仓 .sh 对 nbd0/qemu-nbd/losetup -D 零命中；真实为 `losetup --show -f`＋`kpartx`；函数真名 `make_install_bootloader` [REVIEW，子代理已在 time 检出复核] | 问题清单两条整条作废 | 001R 勘误表逐条撤回 |
| F3 | 001 | 半真半假与判反：H-04（util_cmds.sh 只有一行 cloc）、M-02（称 make_install.sh 默认无操作，实际无参即 `rm -rf ./build/*`＋挂盘安装）、M-11、M-12 | 02-problems.md 第 55~57、96~98、164、172 行 [VRF 定位] | [REVIEW] | M-02 判反且危险：把最危险的脚本说成"无害" | 001R 勘误；危险脚本进 Owner 决策 4 |
| F4 | 001 | MANIFEST 称 `bash proposed/make_install.sh help` 可直接运行 | MANIFEST.md "使用 proposed 的边界"段 | 评审实测 exit=1（`MYOS2 ERROR: set MYOS2_ROOT`）[REVIEW] | "可运行"声明不可信 | 001R：proposed 一律标 UNTESTED |
| F5 | 003 | 行号系统性偏移＋越过文件末尾 | 非 received 文件共 **65 个不同的 `路径:行号` 引用** [VRF]；抽 30 处实读 time 检出：多数落在空行/注释行/无关行（例：`init/main.c:276`→`*/`、`:193`→空行、`:76`→` *`、`scheduler_core.c:1500`→被注释行）；`init-sequence.md` 第 117 行引 `mm_init.c:430-650`，该文件仅 430 行 [VRF]；`setup_arch()` 实际在 `init/main.c:138` [VRF] | 逐行实读 | 行级索引不可用；函数级断言仍可用 | 重做（003R，§4）＝函数级重锚；本地引文闸门校验 |
| F6 | 003 | 误判 `drivers.rtc` 可裁剪 | trim-analysis.md / MANIFEST 核心结论 7 | 主链 `timekeeping_init → read_persistent_clock64 → rtc` [REVIEW] | 裁剪方案错一项 | 003R 撤回并附配置面依据 |
| F7 | 005 | 架构错值 4 处 | `cards/apic-ioapic.md:114`"17:16 destination shorthand"（应 19:18；apicdef.h `APIC_DEST_SELF 0x40000`=bit18 [VRF]）；`cards/msr-control-registers.md:43` CR4 "19–28 Reserved"（bit19 为 Key Locker，且 `cpuid-atlas.md:68` 自己列了 leaf 0x19 Key Locker）[VRF 定位]；能力字 11 vs `get_cpu_cap()` 15 槽 [REVIEW]；PTE 物理地址 bit13 vs bit12 [REVIEW，未定位] | grep 定位＋apicdef.h 实读 | 查表包最忌错值 | 重做（005R 勘误，§4） |
| F8 | 005 | 溯源失实：inputs_read 列不存在文件 | `00-usage-census.md:61` 列 `lapic.c`（time 上 `mykernel/arch/x86_64/kernel/apic/` 无此文件）、`:50` 列 `insns/special_insns.h`（真名 `special_insns_arch.h`）[VRF 定位，存在性 REVIEW＋本会话 ls] | ls | [VERIFIED] 标签可信度下降 | 005R：inputs_read 只列实际存在路径 |
| F9 | 005 | 假 [VERIFIED]：称 kvmclock "未见调用者"/stub | `cpuid-atlas.md:219`、`cards/timers.md:36`、`open_questions.md:49`（OQ-027）[VRF 定位] | `kvm.c` 有 `kvmclock_init()` 活动调用链 [REVIEW] | 一条"可裁剪"暗示错 | 005R 撤销并修正 |
| F10 | 006 | 伪造 40 位 commit SHA `a039d9803893e0ce54164413aa8a0c29eacf5c78` | received 原件 **14 处**；落盘文件 **7 处/5 文件**：MANIFEST.md 2、proposed/ci.yml 2（含 `BASE_SNAPSHOT` 环境变量）、01-survey.md 1、proposed/run-qemu-smoke.sh 1、proposed/test-contract.md 1 [VRF] | `cat-file -e` 失败；只有前 8 位 `a039d980` 是真前缀 [VRF]；同文件里的 tree 哈希 `fa8d26ffa2cf…` 却是真的（a039d980 的 tree 对象）[VRF] | ci.yml 按此 SHA checkout 必败 | 本地机械 -v2 替换（§5 本地 B）；不需 GPT |
| F11 | 002 | 证据路径不存在（5 条被引 16 次）、4 处假 [VERIFIED] | completeness.yaml [REVIEW] | — | 降权 | 本地 L4 重锚（函数级校验），不重做 |
| F12 | 008 | 69 条证据 23 条行号越界；热点图 52/60 静默截断 | debt-register.yaml、01-summary.md [REVIEW] | — | 函数级采信 | 本地 L4 重锚，不重做 |
| F13 | 007 | MANIFEST tier 计数错（T1 实 23、T2 实 11；MANIFEST 自报 T0 14/T1 25/T2 9/T3 5） | MANIFEST.md [REVIEW；MANIFEST 数字 VRF] | — | 按重算值 | 本地 L7 统一口径时重算，不重做 |
| F14 | 006/007/009 | 外部引用全是内部检索句柄（175 处，0 URL/DOI） | received 原件 [REVIEW] | 外部结论不可复核 | 006 外部综述已由 010 补做；007/009 的外部结论保留降权 | 010（已就绪） |

排除的假阳性：007 的 `9780672329463` 是 ISBN 非 SHA [VRF]；EXTRACTION-NOTE.md 引用的假短 SHA是评审自身的举例 [VRF]。

**取证方法**（可复现，全部只读）：`git ls-tree -r --name-only origin/master agent-workspace/results/` 枚举 → 每个文件 `git show | grep -oE '\b[0-9a-f]{12,40}\b'` → 逐个 `git cat-file -e <sha>^{commit}`；行号引用用 `grep -oE '(mykernel|…)/[…]+:[0-9]+'` 提取后在 time 检出 `sed -n`/`wc -l` 实读。

## 3. 重做的设计原则（据 S7 分流原则与第一波教训）

1. **编造的根因是"溯源字段与引文可以凭记忆写"**。所以重做任务书不再要求 GPT 写 commit SHA，改为要求**逐字引文＋函数名**作为唯一定位手段，并把"引文是否真的存在"做成本地机械闸门（grep 即可校验）。这把"人工阅读发现不了"的失效模式变成机器可拒收的失效模式。
2. **GPT 仍可读仓库**（第一波 8/9 在普通对话模式下读到了 time 分支），所以 Owner 想用 GPT 额度重做是可行的；但按 S7，需要 grep/编译/git 的机械校验必须留在本地——GPT 做语义重做，本地做闸门核验，两者不互相替代。
3. **不整包重写、只勘误**：重做产出以 errata（勘误表）为主件，每条对旧产出的判定写"保留/修正/撤回"并附证据；旧目录不改（S3 修订不覆盖）。
4. **一件事一份任务书**：001/003/005 各自的问题性质不同（脚本事实虚构 / 行号锚点腐烂 / 手册错值），分别成书；006 的编造是纯 SHA 替换，不值得一个 GPT 对话。
5. 所有 GPT 任务共用一份**反编造协议**（`agent-workspace/tasks/00-gpt-task-protocol-v1.md`），任务书只引用不重复。

## 4. 起草的任务书（草案；发射由 Owner 亲手；顺序建议见 §6）

| 任务号 | 标题 | 模式 | 优先 | supersedes | 文件 |
|---|---|---|---|---|---|
| — | GPT 任务反编造协议 v1 | — | 前置 | — | `tasks/00-gpt-task-protocol-v1.md` |
| MYOS2-DR-003R | 依赖图与初始化序列重锚（函数级） | 普通对话 Pro | P0 | 003 的锚点与 rtc 判定 | `tasks/MYOS2-DR-003R-dependency-graph-reanchor.md` |
| MYOS2-DR-001R | 外围工程审计复跑（勘误优先） | 普通对话 Pro | P1 | 001 | `tasks/MYOS2-DR-001R-build-and-scripts-audit-rerun.md` |
| MYOS2-DR-005R | x86 查表资料包勘误与出处补全 | 普通对话 Pro（可开浏览） | P2 | 005 部分 | `tasks/MYOS2-DR-005R-x86-reference-pack-errata.md` |
| MYOS2-DR-004R | 调试桩复跑 | 普通对话 Pro | P0 | 004 | 已就绪（前会话起草）；执行者归属待 Owner 裁决（见 01-orientation-report open item） |
| MYOS2-DR-010 | 补做外部测试实践综述 | 深度研究 Pro | P1 | 006 部分 | 已就绪 |
| MYOS2-DR-011 | lockdep-lite 调研 | 深度研究 Pro | P2 | — | 已就绪 |

编号说明：沿用前会话的 `004R` 先例——"R"＝同题复跑并 supersedes 旧目录，不占新序号；新题（如 010）才取新号。Owner 若不喜欢 R 后缀，一句话改为 012/013/014。

## 5. 本地配套任务（不烧 GPT 额度；开工后取号，从 012 起；每项单独请 Owner 一字批示）

| 代号 | 内容 | 前置 | 对应 |
|---|---|---|---|
| 本地 A | **引文闸门脚本**：解析产出中的 `[VERIFIED path::function]`＋引文块，在 time 检出 grep 逐条校验；顺带 SHA `cat-file -e`、路径存在性、行号越界 | 需 Owner 同意建 `agent-workspace/tools/`（工作令 §7.2）或放 results/<取号>/ | S8 第 1 条的推广；001R/003R/005R/004R 的验收前置 |
| 本地 B | **006/001 假 SHA 机械替换 -v2**：对 F1/F10 的 27 处落盘假 SHA 生成 -v2 文件（`supersedes:` 原件），ci.yml 的 `BASE_SNAPSHOT` 改为分支名 | 无 | F1、F10 |
| 本地 C | **L4 事实重锚**：002/008 的证据路径与行号改函数级并校验 | 本地 A | F11、F12 |
| 本地 D | **L2 配置面真相表**：options_flags.cmake 逐行→生效宏表，供 003R/004R 引用 | 无 | 评审独立发现 1、2 |
| 本地 E | L6 冒烟 harness 落地（006 的 run-qemu-smoke.sh 修 CRLF/进程组）——需决策 5 与隔离副本授权 | 本地 B、决策 5、§4.5 逐项授权 | 主攻清单 2 |

## 6. 发射顺序建议（Owner 可改）

1. 先合并本 PR（任务书与协议要在 master 上 GPT 才读得到）；
2. **003R** 先发（P0，002/007/裁剪都靠它的锚点）；同时本地做 A、D；
3. **004R**（若 Owner 裁定仍由 GPT 执行）与 **010/011** 可并行发（互不依赖）；
4. **001R** 在决策 2/3/4 有答案后发（否则它只能再问一遍同样的问题）；
5. **005R** 最后（P2，纯勘误，可随时补）。
回收后每份先过本地闸门（A），闸门不过整份退回原对话返工；过了再进对抗核查与归档（批次 2）。

## 7. 与工作令 §12 第 3 条默认顺序的关系

默认顺序是"先冻结事实基线，再发第二波"。本计划把"冻结事实基线"的本地部分（A、B、D）与第二波任务书起草并行推进，并不冲突：GPT 任务书已起草完毕但**发射仍在基线冻结之后**（§6 第 1~2 步）。若 Owner 想先发 GPT 任务、后做本地基线，一句话即可。

## 8. 同族局限声明

WAVE-1-REVIEW 由 Mnemosyne 主导会话（claude-fable-5 族）产出；本会话为同族同模型，§2 中标 [REVIEW] 的条目未获独立复核；标 [VRF] 的条目是本会话在仓库上用只读命令实测的机械事实，不依赖评审的判断。原计划的子代理独立复核层因 API 过载未能运行，待补。
