# MYOS2-DR-004R · 调试桩与可观测性方案（004 复跑；任务书 v2）

```yaml
task_id: MYOS2-DR-004R
supersedes: agent-workspace/tasks/MYOS2-DR-004R-debug-instrumentation-rerun.md（v1 任务书，保留不改；本 v2 经 3 视角对抗评审后重写）；原任务 MYOS2-DR-004 因连接器故障零交付（results/MYOS2-DR-004/received/），未编造
mode: 普通对话 Pro（GitHub 连接器；读 time 分支源码）
priority: P0（Owner 目的③；第一波唯一结构性空洞；008 的 32 条嫌疑全靠它锁定）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-004R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v2.md（全部十三条适用，硬性；与本任务书冲突时以协议为准）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §2（004 诊断）、§3.4（行号不可靠）、§4.1~4.8（配置面与已证实缺陷）；再按 §输入清单 读第一波产出的指定节
drafted_by: MYOS2-LEAD-001（v2：2026-09-05）
status: draft（发射由 Owner；执行者归属见 WAVE-2-LAUNCH.md §2 备注）
```

## 0. 一句话

给 Owner 一套"基本调试桩"：让已实现的部分能自证清白、出错时能快速缩小范围。不是完整 tracing 框架，不是 lockdep-lite 实现（那是 011 与 Owner 的事）。

## 1. 复跑前须知（原次故障的加固；按顺序执行）

1. 先用连接器读 `agent-workspace/conventions.md`；读不到改用 raw URL（本仓库 public）；两条通道都读不到才停止并报告"读取失败"，不猜内容（协议 P3、P7）。
2. **开工自检**写进 MANIFEST（闸门只认 MANIFEST）：`startup_selfcheck_quote:` 逐字复制公约 §1 第 2 条整句；`branch_canary_quotes:` 逐字引用协议 P3 指定的两行（`options_flags.cmake` 的 `-mcmodel=…-fno-pie` 行、`panic.c::panic` 的 `this_cpu = ` 行）。
3. raw URL 不能列目录；浏览目录用 `https://github.com/08822407d/MyOS2/tree/time/mykernel/<dir>`。

## 2. 输入清单（协议 P11：只取结构、函数名、ID；不转述其断言）

| 输入 | 只读哪部分 | 用途 | 可靠性提示 |
|---|---|---|---|
| `results/MYOS2-DR-008/02-correctness-suspects.md` | 三张表：当前 S1 嫌疑表（只取 ID＋symbol 列）、"历史正确性线索"表（4 条）、"建议给 MYOS2-DR-004 的调试桩"表（桩类别 / 首批覆盖 ID / 最小观测量） | 桩设计的强制输入 | 其证据 URL 带行号且 23/69 越界——不转抄 |
| `results/MYOS2-DR-003/init-sequence.md` | 仅"阶段 3"表格的**函数名列**（`setup_arch()`…`rest_init()` 共 19 项） | 启动检查点候选集 | 行号已证实失效，一个不得转抄 |
| `results/MYOS2-DR-002/completeness-matrix.md` | 子系统级汇总表＋ mm / sched / lock_IPC / time / kactive 五章的"风险点"条目 | 判断哪些地方最需要自检 | 证据路径不可信，只取风险描述；不读 completeness.yaml（67 KB） |
| `results/MYOS2-DR-001/proposed/dbg-qemu.sh` | 仅参考其脚本形态 | gdb/QEMU 辅助件的命名参考 | 001 含编造（20 处假 SHA、整条虚构问题）；其任何字段（尤其文件头 `commit:`）不得复制；001R 正在重做 |
| `bugs_record.md` | 全文（12 行） | 历史线索原句 | 历史材料，不当现状 |
| `mykernel/scripts/options_flags.cmake` | 全文 | 协议 P4 前置；现有开关只有 `-DDEBUG`、`-DCONFIG_BUG`，无 `CONFIG_DEBUG_*` | — |

**源码起点清单**（按函数跳读，≤14 处；大文件只读被点名的函数）：`mykernel/debug/panic.c::panic`（该目录仅此一个源文件＋0 字节 CMakeLists.txt）；`mykernel/include/linux/debug/panic.h`；`mykernel/include/linux/kernel/asm-generic/bug.h`（`BUG()`/`BUG_ON` 生效，`WARN_ON`/`WARN_ON_ONCE`/`WARN_ONCE` 被退化为 `(condition)`——以引文证实）；`mykernel/klib/printk.c::color_printk`（当前实际输出路径，framebuffer）；`mykernel/printk/printk.c::printk` 与 `::vprintk`（838 行 Linux 移植件，logbuf 锁三处被注释）；`mykernel/lock_IPC/lockdep/`（7 个文件共 58 行，`lockdep.c` 仅 2 行）；`mykernel/arch/x86_64/sched/context/process64_arch.c::__show_regs`（寄存器打印）；`mykernel/init/main.c::start_kernel`、`::rest_init`、`::kernel_init`；`mykernel/arch/x86_64/myos/interrupt.c`（中断/异常分发函数，自行核实函数名）；`mykernel/arch/x86_64/mm/fault.c`（缺页处理入口）；`mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk`（DR008-060 相关）；`mykernel/lib/list/double_list.h`（只看 list_add/list_del 一族）；调试脚本现状基线：`.vscode-kdbg/run-qemu-gdb-myos2.sh`、`.vscode-kdbg/kill-qemu-myos2.sh`、根目录 `dbg-qemu.sh.bak`、`dbg-vmw.sh`（所读快照中未见任何 .gdb/.gdbinit 脚本——请核实）。

## 3. 强制 [VERIFIED] 锚点（不得降级、不得省略；闸门核标签与引文）

`mykernel/debug/panic.c::panic`；`mykernel/klib/printk.c::color_printk`；`mykernel/printk/printk.c::printk`（或 `::vprintk`）；`mykernel/lock_IPC/lockdep/lockdep.c`（全文 2 行整体引用）；`mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON` 与 `::WARN_ON`；`mykernel/scripts/options_flags.cmake` 中含 `-DCONFIG_BUG` 的那一行；`mykernel/arch/x86_64/sched/context/process64_arch.c::__show_regs`。凡"无寄存器转储 / 无栈回溯 / lockdep 无实现 / 无串口"一类否定断言，措辞限定"所读快照中未见"并引 options_flags.cmake（协议 P4）。

## 4. 目标

1. **诊断现状**（`01-current-observability.md`）：现有 panic / printk（两处入口，判定哪处在启动路径生效）/ 断言宏 / lockdep 目录 / 调试脚本各能提供什么；崩溃时 Owner 现在能看到什么（`__show_regs` 打印哪些寄存器、有无栈回溯、panic 信息质量）。每条断言按协议 P2 引文；本任务书不预设结论。
2. **调试桩体系设计**（`02-instrumentation-plan.md`）：按投入产出排序给"前十件快赢"，每件固定字段：`title` / `catches`（能抓住哪类 bug）/ `target_path`（须在 `mykernel/` 下且父目录在 time 上存在）/ `hook_site`（`path::function`＋引文）/ `dr008_refs`（DR008-NNN 列表或 `none`）/ `config_switch`（`CONFIG_DEBUG_*` 名，粗粒度即可，须与未来 menuconfig 兼容）/ `est_lines`。候选面至少覆盖：
   - 启动阶段检查点（与 003 的 19 个函数名对齐，落点自行在 `start_kernel`/`rest_init`/`kernel_init` 函数体内以引文锚定）；
   - 断言框架：在 `bug.h` 现状上补全 `WARN_*` 路径、编译开关、`__show_regs` 接入——**不另起炉灶**；
   - 内存类：页分配器一致性自检、kmalloc 毒化、越界金丝雀、双重释放检测；
   - 链表/IDR 损坏检测（`double_list.h` 是全内核地基）；
   - 锁类**只做三件**：lockdep 目录现状评估（引文证明其为空壳）、IF 状态成对断言（直击 DR008-060）、计数型中断上下文违规断言；**lockdep-lite 的数据结构与钩子设计归 MYOS2-DR-011，本任务不给 lockdep 实现代码**；
   - 调度/时钟活性观测：jiffies 前进、上下文切换计数、HPET/LAPIC 中断计数（直击 DR008-061）；
   - panic 路径强化：寄存器＋栈回溯＋最近检查点回放；
   - 日志规范：等级/子系统前缀/时间戳的宏层设计须**与输出后端无关**；当前唯一输出是 `klib/printk.c::color_printk` → framebuffer，无串口驱动（WAVE-2-PLAN 决策 5）。Owner 若未答决策 5，按 [INFERRED] 假设 QEMU `0xE9` debugcon，另给一个 ≤30 行的 `outb` 后端作可选件；**不写串口驱动**；
   - QEMU/GDB 辅助：gdb 脚本放 `proposed/tools/`，脚本访问的每个内核结构体成员列入 `symbols_referenced`；每条 QEMU monitor / gdb 命令按协议 P5 附包含该命令名的具体文档页面 URL。
3. **实现件**（`proposed/`）：**前五件**快赢给完整代码（整文件优先；改现有文件用协议 P10 的改动块，不手写 diff）；**后五件**只给落点＋接口签名＋≤15 行伪码。每个代码文件首部标 `UNTESTED — generated without execution`；MANIFEST 列 `symbols_referenced`。
4. **交叉回应与回溯对照**（`03-debt-crosswalk.md` ＋ `03-debt-crosswalk.yaml`）：
   - §A 对 008"建议给 MYOS2-DR-004 的调试桩"表逐类回应，YAML 每条固定字段：`category`（与该表"桩类别"列**逐字相同**，条数等于该表行数）/ `ids`（等于该行"首批覆盖 ID"展开后的集合）/ `observables`（覆盖该行"最小观测量"每一项）/ `probe_file`（proposed/ 路径，或对"磁盘脚本护栏"一类写 `n/a — 转交 MYOS2-DR-001R`）/ `hook_site` / `hook_quote`；
   - §B 对 008"历史正确性线索"表的**全部四条**（DR008-058/060/061 为 S1，DR008-059 为 S2；不得改写 severity、不得新增第五条）逐条写"若当时有本方案的哪个桩，如何更快定位"：(a) `[VERIFIED bugs_record.md]` 逐字引用该事件原句；(b) 对应桩的 `probe_file`；(c) 桩落点 `path::function`＋引文。

## 5. 交付物（放入 write_zone）

- `MANIFEST.md`（必交）：`base_snapshot: time（分支名）`、`read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`、`symbols_referenced`、覆盖表（十件快赢哪些有代码、哪些只有伪码）、一句"原次 MYOS2-DR-004 零交付，errata 免交，文件名不带 -v2"；
- `01-current-observability.md`；`02-instrumentation-plan.md`；`03-debt-crosswalk.md` ＋ `03-debt-crosswalk.yaml`；`proposed/…`（≤5 件代码＋`proposed/tools/` 脚本）。
- 降级交付按协议 P12：每条回复一个文件，顺序 MANIFEST → 01 → 02 → 03 → proposed。

## 6. 边界

不直接改内核源码（改动以改动块交付）；不写 lockdep-lite 实现（011）；不出任何写盘/分区脚本（001R）；不写串口驱动；不写 40 位 SHA；不基于行号断言；不声称可编译/可运行；不读 `todo.txt`/`changelog.md`/`documents/` 当现状；尊重现有代码风格（先读几个文件感受）；不引入外部依赖。

## 7. 验收判据（本地闸门按协议 P9 执行；任一项不过整份退回）

1. MANIFEST 五字段齐全（P9-5、7、8）；`startup_selfcheck_quote` 在 master 的 conventions.md 命中；金丝雀两行在 time 命中、master 不命中。
2. §3 七组强制锚点齐全，引文 `grep -F` 100% 命中且位于符号定义体内（P9-2、13）；全文 `[VERIFIED` 计数 = `self_check.verified_claims`。
3. `02` 恰有 10 条快赢，每条七个字段齐全；`target_path` 父目录在 time 上存在；前五件的 `proposed/` 文件存在且首部含 `UNTESTED`。
4. `03-debt-crosswalk.yaml` 的 `category` 集合与 008 表"桩类别"列逐字相等；`ids` 集合相等；`probe_file` 存在（或为转交 001R 的 `n/a`）；`hook_quote` 命中。
5. §B 覆盖 DR008-058/059/060/061 四条，各有 bugs_record.md 逐字引文（命中）＋ `probe_file`（存在）＋落点引文（命中）。
6. `symbols_referenced` 100% 在 time 上存在；改动块 before 引文命中（P9-9、10）。
7. 正则 `[0-9a-f]{40}` 0 命中；无"可编译/可运行/已验证"字样；`[EXTERNAL]` 均带 URL（P9-1、6、11）。
8. `02` 与 `proposed/` 中不出现 `init-sequence.md` 的任何 `:行号` 形式引用。
