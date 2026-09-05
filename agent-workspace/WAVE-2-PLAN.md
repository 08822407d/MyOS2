# 第二波计划与发射清单

```yaml
record_type: wave_plan
wave: 2
date: 2026-09-01
based_on: WAVE-1-REVIEW.md（17 代理评审：8 精读＋8 对抗核查＋1 综合）
principle: 严格分流——需要 grep/编译/git 的走本地 Claude；纯外部文献调研走 GPT 深度研究；有学习价值的实现由 Owner 亲手做
```

## 0. 分流原则（第一波的教训）

第一波最大的浪费是把需要仓库访问的工作派给了深度研究模式：006 把仓库里现成的 `myinitramfs/myshell/tests.c`（126 行，含 fork+sigaction 的 signal_test）与 `dbg-qemu.sh.bak` 的 QEMU 命令行当成了"未知"，反过来推高成本。同时三份深度研究的引用全是内部句柄、无一可解析。

因此第二波：

| 工作类型 | 派给谁 | 理由 |
|---|---|---|
| 外部文献/实践调研 | **GPT 深度研究 Pro** | 不需仓库访问，正是其强项；强制要求可解析 URL/DOI |
| 需要 grep / 编译 / 跑 git / 逐行核对 | **本地 Claude** | 第一波证明远程读码的行号与路径错误率过高 |
| 有学习价值的内核实现 | **Owner 亲手**（本地 Claude 陪跑写回归） | Owner 的目的是学习，代做等于剥夺目的 |
| 只有 Owner 知道的事实 | **Owner 一句话回答** | 见 §3 决策清单 |

## 1. 派给 ChatGPT Pro 的任务（本波仅两个，都是纯外部调研）

| 任务 | 标题 | 模式 | 优先 |
|---|---|---|---|
| MYOS2-DR-010 | 补做外部测试实践综述（006 未达标部分） | **深度研究 Pro** | P1 |
| MYOS2-DR-011 | lockdep-lite 设计调研 | **深度研究 Pro** | P2 |

任务书见 `tasks/MYOS2-DR-010-*.md`、`tasks/MYOS2-DR-011-*.md`。两者并行安全、互不依赖、都不需要读仓库源码。

**启动提示词（两个任务共用，把 NNN 换成 010 或 011）：**

```text
你将执行 MyOS2 项目的外部调研任务 MYOS2-DR-NNN。这是一个纯外部文献调研任务。
第一步：读取任务书与写入公约（仓库 08822407d/MyOS2 是 public 仓库，master 分支）：
https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
任务书在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/tasks/ 下以 MYOS2-DR-NNN 开头的文件。
若 GitHub 连接器可用也可直接用连接器读取。
第二步：严格按任务书的研究问题、出处纪律、交付物与验收判据执行。
出处纪律是硬性要求：每条外部结论必须附可解析的 URL 或 DOI；内部检索句柄（fileciteturn 之类）一律不计为出处；拿不到出处的结论不要写，列进 open_questions。
写入规则：深度研究期间连接器只读。研究完成后回到普通对话回合，若能写库则在新分支 agent/MYOS2-DR-NNN 上、agent-workspace/results/MYOS2-DR-NNN/ 目录内新增文件并向 master 开 PR；不能写库就在对话里逐文件完整输出交付物，每个文件前注明目标路径。
不要申报任何 commit SHA——本任务不分析源码。若确需引用仓库状态，只写分支名。
开始前用三句话复述：你的任务目标、出处纪律的要求、你的交付物清单。复述无误后直接开工，过程中不要向我提问。
```

**注意最后那条新增纪律**：第一波有两个任务（001、006）编造了 40 位 commit SHA（前 8 位对、后面编的），006 甚至把假 SHA 写进了 ci.yml 的环境变量。本波两个任务都不需要 SHA，故直接禁止申报。

## 2. 本地 Claude 承担的任务（不烧 GPT 额度）

按优先级：

| # | 任务 | 优先 | 说明 |
|---|---|---|---|
| L1 | 工作区副本同步 ＋ 冻结「已核实事实基线」文档 | P0 | 把评审确证的硬事实一次性冻结，免去后续重复扫描 |
| L2 | 配置面真相表：读 `options_flags.cmake`，重验全部"未启用/死代码"结论 | P0 | 九份产出无一读过配置面，已实证造成一条假 [VERIFIED]、一条本可闭合的悬案 |
| L3 | **重跑 MYOS2-DR-004R**（启动检查点桩与调试 instrumentation） | P0 | 第一波唯一结构性空洞；008 的 32 条嫌疑全靠它锁定。任务书已就绪：`tasks/MYOS2-DR-004R-*.md` |
| L4 | 事实重锚：按 time 快照校正 001/002/003/008 的行号与证据路径 | P0 | 纯机械可验证工作，本地跑脚本比让 GPT 重述便宜一个数量级 |
| L5 | 永久自旋全树穷尽清点（32 处/23 文件）与错误返回改造方案 | P1 | 既是安全修复也是 CI 前置：不改则 CI 只能报"挂了" |
| L6 | QEMU 无头冒烟落地：拆用 006 的 harness，修 CRLF 与进程组 kill | P1 | 006 的 `run-qemu-smoke.sh` 经实跑五场景通过，是全波次唯一真能用的工程件 |
| L7 | 能力节点细化 v2 ＋ 三套分级口径统一（口径需 Owner 拍板一次） | P1 | 002/003/007 三份 YAML 现状无法安全叉乘 |
| L8 | 构建 feature 边界第一刀：开编译诊断 ＋ GLOB 换显式清单 | P1 | 仅开 `-Wreturn-type` 一项即可暴露 008 的五条 S1 |
| L9 | 001 的 `proposed/` 拆件复用（**不整包采纳**） | P2 | 整包会静默破坏可启动性；但 `common.sh` 的 fail-closed 双确认原语值得单独提取 |
| L10 | 009 的 53 ID 教学映射表过滤后转 YAML | P2 | 需与 002 成熟度叉乘，给规模不足者打 `not_teachable_yet` |
| L11 | 存储栈与生命周期治理（ATA 根盘路径） | P2 | 需 L3、L5 先就位 |

## 3. Owner 决策清单（每条一句话即可关闭，多项后续工作卡在这里）

1. `myinitramfs/mylib` 下 17 份 CMakeLists 存在但顶层只 `add_subdirectory(myinit)` 与 `(myshell)`——自制 libc 子树是**有意停用**还是**漏接**？
2. `BOOTX64.EFI` 的权威来源是仓库内预编译件，还是 `myloader/uefi` 的 EDK2 工程？（现有两条互不统一的产物链）
3. 是否接受构建输出统一到 `out/`？（涉及改 `.vscode` 配置）
4. 物理盘同步是固定目录集合还是整根文件系统？
5. **CI/调试的 console 通道选哪条**：新写 8250/16550 串口驱动 / QEMU debugcon 0xE9 / isa-debug-exit？——当前打印走 `klib/printk.c` 的 `color_printk` → framebuffer，无串口驱动，这是冒烟测试的前置决策。
6. 共享词汇表是否补 `time.misc`（313 行实码却无 ID）、并新增 `repo.build`/`deploy.scripts` 类目？
7. 三套分级口径（002 的 risk_level / 007 的 tier / 008 的 severity）如何映射统一？
8. 主攻清单第 1、4、6 项（并发原语修复、唤醒链与调度状态机、lockdep-lite）——确认由你亲手做、本地 Claude 只陪跑写回归？
9. `-DROOTBLK_NVME` 当前被注释（根盘走 ATA）——这是有意选择还是遗留？NVMe 路径是否还要维护？
10. `arch/aarch64/` 仅有 CMakeLists（0 源码）——是规划中还是已放弃？
11. SMP（AP trampoline 原地自旋、无 INIT/SIPI 闭环）在你的学习路线里的优先级？
12. 是否同意 007 推荐的**路线 A（并发正确性优先）**作为主线？（另两条候选：存储栈打通优先、用户态生态优先）

## 4. 硬性纪律（第二波新增，针对第一波暴露的问题）

1. **SHA 机械校验闸门**：任何产出入库前自动跑 `git cat-file -e <sha>`，不通过即拒收。第一波两处伪造 SHA 都是"前缀正确、尾部编造"，人工阅读发现不了。
2. **删码需证明**：任何删除动作必须先给出"该符号在当前 CONFIG 组合下不可达"的证明（配置面依据来自 L2）。
3. **行号降级使用**：重锚（L4）完成前，第一波的证据一律当"函数级事实断言"而非"行级定位索引"。
4. **实施类任务的硬前置**：L3（instrumentation）与 L6（冒烟 harness）未就位前，不开展依赖运行时验证的实施任务。
5. **UP/SMP 分标**：所有并发结论显式区分"UP 下成立"与"SMP 下待验"——当前 SMP 未上线，第一波所有"基本路径可跑"只对单 BSP 成立。

## 5. 补充（2026-09-03，MYOS2-LEAD-001；只增不改上文）

**背景**：Owner 2026-09-03 指示"找回上一会话对编造情况的分析，再设计一次补充/重做任务书"。分析在本文件上方引用的 WAVE-1-REVIEW.md；机械取证与重做计划见 `agent-workspace/lead/MYOS2-LEAD-001/05-wave1-fabrication-ledger-and-redo-plan.md`。

### 5.1 新增任务书（草案，发射由 Owner 亲手）

| 任务 | 标题 | 模式 | 优先 | 说明 |
|---|---|---|---|---|
| （协议） | `tasks/00-gpt-task-protocol-v1.md` | — | 前置 | 所有 GPT 任务共用的反编造协议：分支名代替 SHA、引文＋函数名代替行号、自检回合、本地机械闸门 |
| MYOS2-DR-003R | 依赖图与初始化序列重锚（函数级） | 普通对话 Pro | P0 | supersedes 003 的锚点与 rtc 判定；新增 config 边 |
| MYOS2-DR-001R | 外围工程审计复跑（勘误优先） | 普通对话 Pro | P1 | supersedes 001；只交 errata＋两件改进件；建议在决策 2/3/4 后发 |
| MYOS2-DR-005R | x86 查表资料包勘误与出处补全 | 普通对话 Pro（可开浏览） | P2 | supersedes 005 部分 |

004R/010/011 不变（004R 的执行者归属待 Owner 裁决：任务书 mode 为普通对话 Pro，§2 的 L3 却派给本地 Claude）。006 的假 SHA 由本地机械 -v2 替换处理，不另开 GPT 任务。

### 5.2 启动提示词 v3（001R/003R/005R 共用；NNN 换成 001/003/005）

```text
你将执行 MyOS2 内核分析的重做任务 MYOS2-DR-NNNR。仓库 08822407d/MyOS2 是 public 仓库。
第一步：读 master 分支的 agent-workspace/conventions.md、agent-workspace/tasks/00-gpt-task-protocol-v1.md（反编造协议，硬性）、agent-workspace/WAVE-1-REVIEW.md，以及你的任务书 agent-workspace/tasks/ 下以 MYOS2-DR-NNNR 开头的文件。连接器读不到就用 raw URL：https://raw.githubusercontent.com/08822407d/MyOS2/master/<path>。
第二步：开工自检——在回复中逐字引用 conventions.md §1 第 2 条与协议 P2 的原文；引不出来就停止并报告"读取失败"，不要继续。
分支分工：工作区文件在 master；内核源码以 time 分支为准（raw URL 用 https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>）。
硬性纪律：不写 40 位 commit SHA（只写分支名 time，或从连接器输出复制的 12 位短 SHA 并标"短 SHA"）；每条 [VERIFIED] 断言附 路径＋函数名＋逐字引文，不用行号定位；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；交付前按协议 P6 自检并在 MANIFEST 写 self_check 结果；编造一条即整份作废。
写入规则：新分支 agent/MYOS2-DR-NNNR（从 master 建）、只在 agent-workspace/results/MYOS2-DR-NNNR/ 内新增文件、完成后向 master 开 PR；不能写库就在对话里逐文件完整输出交付物，每个文件前注明目标路径。
开始前用四句话复述：任务目标、源码分支、唯一可写目录、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 5.3 发射顺序建议

先合并承载任务书的 PR → 003R（P0）先发，本地同时做引文闸门脚本与配置面真相表 → 004R（若仍由 GPT）与 010/011 并行 → 001R 在决策 2/3/4 后 → 005R 最后。回收件先过本地闸门（协议 P9），不过整份退回原对话返工。
