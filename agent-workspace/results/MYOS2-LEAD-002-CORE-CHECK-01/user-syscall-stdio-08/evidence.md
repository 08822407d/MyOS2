---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SYSCALL-STDIO-08
record_type: user_syscall_stdio_evidence
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-28"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 ab3b4b86662c；主线 agent/MYOS2-LEAD-002 91753f8155ee；master e8e60cd19540（短 SHA）"
inputs_read: "见 §2"
status: final_for_user_syscall_stdio_08
runtime_evidence: NOT_RUN
open_questions:
  - "推送后的远端读回写在 PR17 评论与最终回复中，本文件不自引用其所在提交。"
---

# CORE-USER-SYSCALL-STDIO-08 证据与方法

本文记录本批实际执行的绑定核对、读取范围、命令与核对结果。所有命令只读 git 对象，或用 Python 处理文本、YAML、JSON；没有编译，没有运行 C/ASM、cmake、内核、QEMU、宿主安装/挂盘/分区脚本或仓库原有脚本，没有重跑 05/06/07 的核验。Python 运行时都设置了 `PYTHONDONTWRITEBYTECODE=1` 和 `-B`，交付脚本在 import 之前设置 `sys.dont_write_bytecode = True`；运行前后 clone 内都没有 `__pycache__`。

## 1. C00 绑定

| 项 | 所见（工具输出） |
|---|---|
| 主线对象 | 在 clone 中 `git fetch` 了 `agent/MYOS2-LEAD-002`、`time`、`master` 与执行分支四个引用（只更新远端跟踪引用）；主线头 `91753f8155ee`，相对 07 时所读 `f5d0423da491` 只新增 22 号任务书、07 审查回执与 10-28 检查点三个文件，属于主线正常追加 |
| 任务书 | 22 号任务书从主线对象全文读取（106 行） |
| 07 审查回执 | `record_id: CORE-USER-BOUNDARIES-07-REVIEW-001`，`reviewed_head_short12: ab3b4b86662c`，`next_followup: CORE-USER-SYSCALL-STDIO-08`，`next_taskbook` 指向 22 号，与任务书一致 |
| 18 号约定 | 全文读取，按其字段要求写 `deferred-findings.yaml` |
| 源码 | 远端 `time` 仍为 `a039d9803ade`，与任务书 `kernel_source_short12` 相同，无需另列差异 |
| 工作区 | 远端 `master` 为 `e8e60cd19540` |
| 执行分支 | 本地与远端头都是 `ab3b4b86662c`（含 07 结果）；工作树干净 |
| PR17 | open、Draft、未合并，头 `ab3b4b86662c`，基 `master` |
| 隔离 clone | 沿用 06 建立的 `/home/cheyh/projs/myos2-handoff06-local/MyOS2`，所有仓库命令以它为根；没有触碰 Owner 的开发工作树 |
| 配置面 | 先读 `mykernel/scripts/options_flags.cmake`：`-D__KERNEL__`、`-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER` 在内核编译选项中；`-DROOTBLK_NVME` 一行被注释；`CONFIG_INTEL_X64_GDT_LAYOUT` 不在其中（QS-12） |

## 2. 实际读取范围

- **主线：** 22 号任务书全文；07 审查回执全文；18 号全文；10-28 检查点（07 已审、08 就绪）全文；`agent-workspace/conventions.md` §3–§4（共享词汇表与事实纪律，master 上的工作区文件）；`agent-workspace/repo-map.md` §4–§6（分支、构建流程与历史材料）。
- **07（本会话所写）：** `user-entry-console.md` §4 的两张 f_op 表与读写落点；`facts-and-boundaries.yaml` 中 console、syscall、fd 相关能力与 SB-01；`deferred-findings.yaml` 的 UB-01、UB-05 及涉及 GAP-U10 的条目。
- **05：** `deferred-findings.yaml` 的 UV-09、UV-10、UV-11、UV-15；`facts-and-dependencies.yaml` 中 GAP-U06、GAP-U10 的定义，锚点 A-C2-14、A-C2-15、A-C4-12、A-C4-25、A-C4-28、A-C4-31，检索 Q14，能力 ID 列表。
- **源码（time a039d9803ade，定点读取）：**
  - C01：`myinitramfs/` 文件清单；`myinitramfs/CMakeLists.txt`、`myinit/CMakeLists.txt`、`myshell/CMakeLists.txt`、`mylib/CMakeLists.txt` 全文；`myinit/initd.c` 全文；`myshell/myshell.c`、`tests.c`、`tests.h`、`externs.h` 全文（`command.h` 没有被任何源文件包含，只看了首行）；`mylib/myglibc/syscall/syscall.c` 全文、`myglibc/CMakeLists.txt`、`syscall/CMakeLists.txt`；`make_install.sh`、`scripts/make_install_initranfs.sh`、`scripts/prepare_env.sh` 全文（只读文本）；`changelog.md` 开头、`documents/useful_cmds.md` 第 10–26 行、`user-guide/User-Application.md` 与 `Syscall-Table.md`。
  - C02：`arch/x86_64/kernel/cpu/common.c` 530–610；`arch/x86_64/entry/entry_64.S` 144–184；`arch/x86_64/entry/common.c` 1–86；`arch/x86_64/entry/syscall_64.c` 全文；`asm/syscalls_64.h` 全文；`asm/unistd_64.h` 的相关定义与末尾；`asm/unistd.h` 1–40；`asm/syscall_wrapper.h` 1–30、60–135、225–254；`include/linux/kernel/syscalls.h` 1383–1397；`arch/x86_64/myos/syscall.c` 1–60、125–191；`sched/syscall.c` 66–80、131–142；`lock_IPC/syscall.c` 91–102；`klib/printk.c` 20–40（color_printk）。
  - C03：`fs/syscall.c` 全文；`fs/vfs/file.c` 1–241；`include/linux/fs/file.h` 1–90；`fs/vfs/file_table.c` 30–135；`fs/vfs/read_write.c` 120–200、312–407；`fs/vfs/open.c` 196–332；`fs/vfs/fcntl.c` 1–128（含 default 分支与 check_fcntl_cmd）；`fs/vfs/namei.c` 的 `path_init`、`set_nameidata` 与 660–764；`sched/forkexec/fork.c` 718–747、`copy_signal`、`kernel_thread`、1580–1619；`sched/forkexec/exec.c` 880–890、944–949；`init/main.c` 86–100；`init/init_task.c` 20–60 与 112–126；`include/linux/kernel/fdtable.h` 20–70；`include/uapi/linux/fs.h` 与 `include/uapi/asm-generic/resource.h` 的相关定义；`sched/task/task.h` 100–122；`drivers/char/tty_io.c` 109–119。
- **没有读：** 仓库外的 musl（`/home/cheyh/projs/musl`）、历史主题分支 `libc`、构建产物与镜像、mylib 的其余实现、color_printk 的格式化、信号与 `syscall_exit_to_user_mode` 内部、slab 内部。

## 3. 方法

- 源码一律用 `git -C <clone> show a039d9803ade:<path>` 与 `git -C <clone> grep ... a039d9803ade -- <paths>` 读取，不检出、不构建。
- **锚点引文**由会话内一次性辅助程序按“路径、起止行”从固定对象逐字取出（并用 03 包的 `analyse()` 计算 #if 条件），再由交付脚本 `scripts/ss_check.py` 独立复核。正文中的 `[VERIFIED]` 代码块由一次性渲染程序从同一批引文填入，同样由 `ss_check.py` 复核。`facts-and-boundaries.yaml` 与 `deferred-findings.yaml` 由会话内撰写，用一次性组装程序写成 YAML（只负责格式，不生成事实）。检索的期望值取自实际运行结果，并逐条看过命中内容后才写入。这些一次性辅助程序没有交付，复核不依赖它们。
- **两张值表**的记录值由 `ss_check.py` 中同样的函数从源码算出后写入，复核时重算比对。它保证记录与源码文本一致，不独立证明这两个函数本身没有错；函数只做文本解析（`#define` 取值、表头展开、定义行与函数体的正则计数），口径写在值表的 assumptions 中。
- **汇编**不用 C 词法器判断函数边界：以 `SYM_*_START(sym)` 到 `SYM_*_END(sym)` 为范围，没有 END 时以下一个符号或段切换为界（`entry_SYSCALL_64` 属于这种情况）。
- **构建文本**（CMakeLists.txt、.cmake、.sh）不经 C 词法器：锚点只要求所标符号出现在引文中（允许 `-D` 前缀）且没有 #if 条件；检索中以 `#` 开头的行算注释。
- **否定结论**都带检索词、路径范围、返回码与未排除项（`records/queries.json`）。“未见”只表示所读快照与检索范围内未见。
- **不做的事：** 不读仓库外 libc，不把 Linux/glibc 的通常做法当作 MyOS2 现状，不为填表编译或运行任何东西。

## 4. 核对结果

最终核对在 clone 根目录运行（完整命令、返回码、起止时间与输入摘要见 `records/run.json`）：

```text
PYTHONDONTWRITEBYTECODE=1 python3 -B -P <本目录>/scripts/ss_check.py <clone 根> check <本目录> <本目录>/records/ss_check.json
```

返回码 0，`all_ok: true`。逐项（数字取自 `records/ss_check.json`）：

| 项 | 结果 |
|---|---|
| 锚点 | 70/70：引文与 `time a039d9803ade` 对应行逐字一致（每条 1–5 行），落在所标函数/初始化器/类型/宏/汇编范围内或在构建文本中点名该符号，记录的 #if 条件与重新计算的相同，且至少一行是活动代码。按类型：in_body 45、macro 10、build 8、initializer 3、asm_range 3、type_member 1 |
| 正文引文 | `user-syscall-stdio.md` 的 34 处 `[VERIFIED 路径::符号]` 代码块 34/34 是源码或构建文本中的连续原行，且落在所标符号范围内 |
| 检索 | 38 个检索全部执行，0 次工具失败；每个检索的返回码、总命中数、活动命中数都等于期望（30 个正向、5 个要求无活动命中、3 个要求只有注释命中）。间接边中 18 处“检索 + 路径 + 行号”引用都是对应检索的活动命中 |
| 结构检查 | 23/23（函数体正则计数） |
| 值表 | VT-SYSTAB：41 个活动表项、12 个被注释表项，号全部唯一且小于 453，每项恰有一处活动定义，逐行（号、符号、定义位置、参数个数、提示宏、return 个数）与重算一致；VT-FDLIMITS：NR_OPEN_DEFAULT=256、INR_OPEN_CUR=1024、FDPUT_FPUT=1、`__to_fd` 返回 flags=3、`myos_fdget_pos` 返回 0、init_files.fd_count=0、f_dupfd 以 RLIMIT_NOFILE 为上界，三项推论（fdget 系列隐含 fput、读路径不隐含、上界超过表长）与重算一致 |
| 记录结构 | 0 项错误：ID 不重复；证据、边端点、间接边三项、refines 与 05/07 外部 ID（从 `ab3b4b86662c` 读出 542 个）都能解析；上限为能力 12/15、边 23/40、锚点 70/75、候选 3/4；counts 与重算一致 |
| 延期字段 | 0 项错误：5 条发现的必填字段齐全、qualified_id 前缀正确；每条的 `future_verification.candidates` 与候选的 covers 一致；5 项追加的目标都能在 05/07 原件中解析 |
| 40 位十六进制 | 包内 0 处 |

运行前后 clone 内都没有 `__pycache__`。这些核对只证明记录与源码文本之间机械一致，不证明语义正确。

## 5. 自查与更正

本批在交付前发现并改正了以下问题（都在本批写区内改正，05/06/07 原件未动）：

1. **锚点取行。** AS2-19 起初取了配置面 59–65 共 7 行，超出 1–5 行的规定，拆成 AS2-19（第 65 行）与 AS2-20（第 59 行），并让构建文本的符号检查接受 `-D` 前缀；AS3-01 第一次取到第 92 行的注释，改为第 93 行；AS3-13 起初把两条独立的 `#define` 当作一个宏，改为单行；AS3-05 的类型范围在 scan_index 中名为 `struct files_struct`，据此改正。
2. **与 05 重复的锚点。** AS3-12（fput 释放行）与 AS3-36（close 两行）与 05 的 A-C4-28、A-C4-25 是同一组行，删去后直接引用 05，编号留空不补。
3. **检索口径。** QS-08 的命中全部是注释，期望改为 comment_only；QS-26 在 `check_fcntl_cmd` 的白名单中有活动 case，改为正向并在用途中写明；QS-27 写明只有声明；QS-29 收窄为对 dfd 字段的访问（只有写入，没有读取）。
4. **结构一致性。** ES-22 没有被其端点能力列出，已补。
5. **一次独立审查。** 草稿完成后启动了一个只读审查子代理，限定只用 `git show`、`git grep` 读固定对象和本包文件。它提出 12 条意见，逐条回到源码核对后全部采纳：
   - US-01 对 fcntl 说过头：成功的 F_DUPFD 先 get_file，与 fdput 相抵；只有 F_GETFL、失败的 F_DUPFD 与不支持的命令（default 分支，新增 AS3-39）才只剩一次 fput。标题、前提、影响与正文都已收窄。
   - fork 继承来的 file 来自 kzalloc，US-01 的那次 fput 走 05 UV-09 的跨缓存路径；US-01 的 related 补上 UV-09，“回 filp 缓存”限于 open 建立的 file；“重新分配后被释放”改为“减一，原为 1 才释放”。
   - kernel_thread 除 CLONE_FS 外还加 CLONE_VM 与 CLONE_UNTRACED，正文“只带 CLONE_FS”已改。
   - US-02：得到的 fd ≥ 256 时 close、lseek 返回 -EBADF，无法关闭，退出时也不扫描；只有 read、getdents64、write、writev、fcntl 越界（新增 QS-38）。
   - GAP-U04 属于 05（07 只收窄），两处归属已改。
   - sh 启动时先调用 tests.c 的 test_PrintDateTime（新增 QS-36）；用户源码中没有 writev（新增 QS-37），三栏表已改。
   - “do_sys_openat2 只在错误指针时不装 fd”改为不止这一种情形。
   - `CMAKE_C_COMPILER` 写在 `project()` 之后（新增 AS1-12），实际生效的编译器只凭文本不能确定：“链接的是 musl”改为基于构建文本的推断，并补成“无论哪个生效，libc 都在仓库外”。
   - RLIMIT_NOFILE 的初值链补上 `init_task.signal = &init_signals`（新增 AS3-38），AS3-06 的用途改写；QS-10 补上 User-Application.md 的命中；QS-16 写明其中 1 处是宏定义本身。
   - US-05 补上 color_printk 把 fmt 原样交给 vsnprintf（新增 AS2-21），未知项只留 vsnprintf 的内部。
   - US-01…US-05 的 source_ref 改为指向 `deferred-findings.yaml`。
6. **措辞。** 分派表的“提示宏”一列只表示函数体含 ALERT_DUMMY_SYSCALL，不等于空实现（exit、exit_group、kill 在宏之后还有实际处理）；没有活动 return 的处理函数写成“若返回，返回值没有定义”（exit 在 do_exit 之后）。
7. **本批执行过的 git 命令。** 除 `git show`、`git grep`、`git ls-tree` 外，在 clone 中执行过 `git fetch`（四个引用，只更新远端跟踪引用）、`git status`、`git log`、`git diff --stat`、`git merge-base` 与 `git rev-parse`，用于绑定核对与写区核对；没有执行任何改动工作树内容的命令，提交与推送见 §6。

## 6. 写区与回传

- **只新增：** 本批只在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-syscall-stdio-08/` 下新增 10 个文件（五个主件、`scripts/ss_check.py` 与 `records/` 下的四个记录）。
- **提交前边界核对：** 暂存后运行 `ss_check.py <clone 根> boundary records/boundary_staged.json --staged`。它把暂存区与 `ab3b4b86662c` 的树逐文件比较：基线文件的模式与对象逐个相同、没有缺失；相对基线的变更全部是本前缀下的新增；没有未暂存或未跟踪的文件；同时记下远端执行分支头，确认推送前没有被别人更新。这个记录先生成一次、加入暂存区后再生成一次，所以记录里的变更清单包含它自己。
- **提交与推送：** 在 `claude/dazzling-cori-q0dnyt` 上做一个普通提交，不改写历史、不 force-push；推送前再次确认远端头仍是 `ab3b4b86662c`。PR17 保持 Draft，不改 PR 描述，只追加一条评论，写明提交短号与远端读回结果。
- **不自引用：** 本文件与 MANIFEST 不写自己所在提交的标识，提交短号与读回结果在 PR17 评论与最终回复中给出。

## 7. 限度

- 这是静态源码说明与机械一致性核对，不是语义证明，也不是内核或用户程序的运行验证；所有运行层仍为 NOT_RUN。
- 用户程序从 libc 符号到系统调用号这一步在仓库外，本批没有、也不能在仓库内闭合（GAP-U10 open）。
- US-01、US-02 的后果依赖释放或越界后内存的实际内容，只推导到源码表达式为止。
- pid 1 文件表各槽的初值取决于 slab 对象原有数据，没有追到分配器内部。
