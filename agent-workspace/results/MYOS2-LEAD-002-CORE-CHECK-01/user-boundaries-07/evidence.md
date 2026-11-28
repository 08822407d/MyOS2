---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-BOUNDARIES-07
record_type: user_boundaries_evidence
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-28"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 b12572f14eae；主线 agent/MYOS2-LEAD-002 f5d0423da491；master e8e60cd19540（短 SHA）"
inputs_read: "见 §2"
status: final_for_user_boundaries_07
runtime_evidence: NOT_RUN
open_questions:
  - "推送后的远端读回写在 PR17 评论与最终回复中，本文件不自引用其所在提交。"
---

# CORE-USER-BOUNDARIES-07 证据与方法

本文记录本批实际执行的绑定核对、读取范围、命令与核对结果。所有命令只读 git 对象，或用 Python 处理文本、YAML、JSON；没有编译，没有运行 C/ASM、cmake、内核、QEMU、宿主安装/挂盘脚本或仓库原有脚本，没有重跑 05/06 的核验。Python 运行时都设置了 `PYTHONDONTWRITEBYTECODE=1` 和 `-B`，交付脚本在 import 之前设置 `sys.dont_write_bytecode = True`；运行前后 clone 内都没有 `__pycache__`。

## 1. B00 绑定

| 项 | 所见（工具输出） |
|---|---|
| 任务书 | 主线 `21-user-entry-console-contract.md`，raw 下载与主线对象逐字节一致 |
| 06 审查回执 | `record_id: CORE-EXECUTION-HANDOFF-06-REVIEW-001`，`reviewed_execution_short12: b12572f14eae`，`next_followup: CORE-USER-BOUNDARIES-07`，与任务书一致 |
| 主线对象 | `agent/MYOS2-LEAD-002` 头 `f5d0423da491`（相对上次所读 `ad850ce5405b` 只新增 06 回执、21 号任务书与 10-28 检查点，属于主线正常追加） |
| 源码 | 远端 `time` 仍为 `a039d9803ade`，与任务书固定对象相同，无需另列差异 |
| 工作区 | 远端 `master` 为 `e8e60cd19540` |
| 执行分支 | 本地与远端头都是 `b12572f14eae`；它是执行头的祖先；工作树干净 |
| PR17 | open、Draft、未合并，头 `b12572f14eae`；PR16（主线）也为 open |
| 隔离 clone | 沿用 06 建立的 `/home/cheyh/projs/myos2-handoff06-local/MyOS2`，所有仓库命令以它为根；没有触碰 Owner 的开发工作树 |
| 配置面 | 先读 `mykernel/scripts/options_flags.cmake`：`-DROOTBLK_NVME` 一行被注释，`CONFIG_DEVMEM`、`CONFIG_DEVPORT` 不在其中 |

## 2. 实际读取范围

- **主线：** 21 号任务书全文；06 审查回执全文；18 号全文（与上次所读相同）；10-28 检查点全文。
- **执行分支 06：** `local/RECEIPT.md`、`resume-map.md` §5、`session-naming-record.md`（均为本会话所写，内容已知）。
- **05：** MANIFEST 全文；`user-vfs-baseline.md` 的 §0、C1、C2、C4 与 §8；`facts-and-dependencies.yaml` 中 `fs.vfs.root_switch`、`sched.forkexec.first_user_launch`、`sched.forkexec.fork_entry` 三个能力，A-C1-02/03/05/06/07/09/10/11/14、A-C4-10 等锚点与 Q16、Q21、E07；`deferred-findings.yaml` 的 UV-01、UV-02。
- **INTEGRATION-03：** `facts-and-regressions.yaml` 的 SF-C5-26 与 switch_in_kinds 段。
- **源码（time a039d9803ade，定点读取）：**
  - B01：`fs/vfs/myos_vfs.c` 全文；`fs/vfs/init.c` 全文；`fs/vfs/namespace.c` 1–689 中挂载相关函数；`fs/vfs/namei.c` 140–935 中路径解析、打开、mknod 与 creat 部分；`fs/vfs/open.c` 的 chdir；`fs/vfs/fs_context.c` 55–181；`fs/vfs/filesystems.c` 35–153；`fs/vfs/super.c::vfs_get_tree`；`fs/fat/myos_fat32.c::read_fat32_superblock` 与 `FAT32_fs_type`；`fs/fat/myos_fat32_namei.c::FAT32_lookup` 返回路径与 `vfat_dir_inode_operations`；`drivers/base/devtmpfs.c` 全文；`drivers/base/base_init.c`；`sched/misc/fs_struct.c` 1–140；`sched/forkexec/fork.c::copy_fs`、`copy_process` 中 PF_KTHREAD 与 copy_thread 调用、`kernel_thread`；`sched/forkexec/kthread.c::create_kthread`；`init/main.c` 80–365；`fs/vfs/dcache.c::__d_lookup`；`fs/vfs/libfs.c::simple_lookup`；`include/linux/fs/GPT.h` 表项定义；`arch/x86_64/include/obsolete/device.h` 的 ROOTBLK_TRANSFER。
  - B02：`arch/x86_64/myos/arch_task.c` 全文；`arch/x86_64/entry/entry_64.S` 1–310；`arch/x86_64/sched/context/process_arch.c` 1–170；`process64_arch.c` 236–432；`switch_to_types_arch.h`、`switch_to_arch.h`、`switch_to_macro_arch.h` 全文；`arch/x86_64/include/asm/ptrace.h` 1–140；`include/generated/asm-offsets.h` 全文；`arch/x86_64/kbuild/asm-offsets.c`、`asm-offsets_64.c` 全文；`arch/x86_64/entry/common.c` 1–86；`entry/common.c` 全文；`fs/vfs/binfmt_elf.c` 1095–1140；`sched/forkexec/exec.c::kernel_execve` 开头；`include/linux/kernel/types.h` 的 reg_t；`include/linux/kernel/linkage.h` 的 SYM_CODE_START/END；`sched/forkexec/kthread.c` 的 kthread() 与 create_kthread，以及 kthread_exit 的 `__noreturn` 声明（kthread_exit 正文未读）；`sched/forkexec/exec.c` 中对 fs 的处理。
  - B03：`myinitramfs/myinit/initd.c` 全文；`myinitramfs/CMakeLists.txt`、`myinit/CMakeLists.txt` 全文；`make_install.sh` 全文；`drivers/char/tty_io.c` 全文；`char_dev.c` 1–189；`mem.c` 150–208；`myos_tty.c` 全文；`arch/x86_64/myos/keyboard.c` 1–200 与 `device.c`；`drivers/base/core.c` 60–160 与 225–260；`include/linux/device/device.h::dev_name`；`fs/vfs/inode.c::init_special_inode`；`fs/vfs/shmem.c` 100–440；`fs/vfs/open.c` 1–330；`fs/vfs/read_write.c` 60–407；`fs/vfs/file_table.c` 36–100；`kactive/completion/completion.h`（等待与完成）；`drivers/base/map.c::kobj_map`；`fs/syscall.c` 1–150；`arch/x86_64/entry/syscall_64.c`；`arch/x86_64/include/asm/syscalls_64.h` 全文；`unistd_64.h` 中相关编号；`include/linux/kernel/syscalls.h` 的 ALERT_DUMMY_SYSCALL；`include/linux/fs/fs.h` 中 FMODE_* 与 OPEN_FMODE。
- **没有读：** 仓库外的 musl（`/home/cheyh/projs/musl`）、FAT32/ATA 内部、shmem 超级块、kobj_map 中保存指针以外的部分、信号投递内部、中断入口、键盘扫描码表与帧缓冲映射。

## 3. 方法

- 源码一律用 `git -C <clone> show a039d9803ade:<path>` 与 `git -C <clone> grep ... a039d9803ade -- <paths>` 读取，不检出、不构建。
- **锚点引文**由会话内一次性辅助程序按“路径、起止行”从固定对象逐字取出（并用 03 包的 `analyse()` 计算 #if 条件），再由交付脚本 `scripts/ub_check.py` 独立复核；`facts-and-boundaries.yaml` 由会话内撰写，用一次性组装程序写成 YAML（只负责格式，不生成事实）。检索的期望值取自实际运行结果，并逐条看过命中内容后才写入。这些一次性辅助程序没有交付，复核不依赖它们。
- **汇编**不用 C 词法器判断函数边界：`ub_check.py` 以 `SYM_*_START(sym)` 到 `SYM_*_END(sym)` 为范围；没有 END 时以下一个符号或段切换为界并记为 `next_symbol`（`entry_SYSCALL_64` 与 `syscall_return_via_sysret` 都是这种情况）。宏 `PUSH_REGS`/`POP_REGS` 以 `.macro` 到 `.endm` 为范围。
- **否定结论**都带检索词、路径范围、返回码与未排除项（`records/queries.json`）。“未见”只表示所读快照与检索范围内未见。
- **不做的事：** 不为填表编译生成 asm-offsets；不展开 Intel/AMD 手册；不读仓库外 libc；不为通过计数器伪造 C 函数包装。

## 4. 核对结果

最终核对在 clone 根目录运行（完整命令、返回码、起止时间与输入摘要见 `records/run.json`）：

```text
PYTHONDONTWRITEBYTECODE=1 python3 -B -P <本目录>/scripts/ub_check.py <clone 根> check <本目录> <本目录>/records/ub_check.json
```

返回码 0，`all_ok: true`。逐项（数字取自 `records/ub_check.json`）：

| 项 | 结果 |
|---|---|
| 锚点 | 70/70：引文与 `time a039d9803ade` 对应行逐字一致（每条 1–5 行），落在所标函数/初始化器/宏/汇编范围内，记录的 #if 条件与重新计算的相同，且至少一行是活动代码。按类型：in_body 62、initializer 4、asm_range 3、macro 1 |
| 正文引文 | `user-entry-console.md` 的 22 处 `[VERIFIED 路径::符号]` 代码块 22/22 是源码中的连续原行，且落在所标符号范围内 |
| 检索 | 70 个检索全部执行，0 次工具失败；每个检索的返回码、总命中数、活动命中数都等于期望（61 个正向、6 个要求无活动命中、3 个要求只有注释命中）。能力、边、缺口、帧路径与链衔接中 42 处“检索 + 路径 + 行号”引用，都是对应检索的活动命中 |
| 结构检查 | 20/20（汇编指令序列 4 项、函数体正则计数 16 项） |
| 值表 | VT-PTREGS（22 个字段、生成偏移、压弹顺序与跳过 0x28）、VT-KFRAME（7 格切换帧与 56 字节）、VT-FLAGS（O_CREAT 与 LOOKUP_RCU 同值；OPEN_FMODE 产生不了 FMODE_CAN_WRITE）全部通过 |
| 记录结构 | 0 项错误：ID 不重复；证据、边端点、间接边三项与 05 外部 ID 都能解析；上限为能力 14/15、边 37/40、锚点 70/75、候选 4/4；counts 与重算一致 |
| 延期字段 | 0 项错误：14 条发现、2 项证据追加、1 项勘误的必填字段齐全；每条发现的 `future_verification.candidates` 与候选的 covers 一致；ER-01 引用的 2 处 05 原文在 `b12572f14eae` 中逐字存在（按空白归一后比较） |
| 40 位十六进制 | 包内 0 处 |

运行前后 clone 内都没有 `__pycache__`。这些核对只证明记录与源码文本之间机械一致，不证明语义正确。

## 5. 自查与更正

本批在交付前发现并改正了以下问题（都在本批写区内改正，05/06 原件未动）：

1. **锚点行数。** AB3-07 起初取了 6 行（101–106），超出 1–5 行的规定；改为 102–106。
2. **注释过滤。** 早期一次临时 grep 用 `grep -v '\s*\*'` 去掉注释行，把以 `*` 开头的真实代码行（如 `*this_cpu_ptr(&pcpu_hot.top_of_stack) = …`）也去掉了。该结果没有被采用；活动/注释的判定一律改用 03 包 scan_index 的注释掩码（`ub_check.py` 中 `active()`）。
3. **时序图。** §3.1 时序起初没有标出执行者；改为 start_kernel 由 0 号任务执行、挂载 "/"、chdir/chroot 与 mknod 由 kdevtmpfs 代为执行。第一次替换因原文行首写法不同而 0 处匹配，改为整块精确替换。
4. **两轮独立审查。** 草稿完成后启动了两个只读审查子代理，对草稿与源码做独立审查。第一轮 20 条、第二轮 25 条意见都逐条回到源码核对后处理，主要更正有：
   - 首个程序进入用户态时只有 rcx（入口）、r11（flags）、rsp（用户栈）来自 exec，其余 13 个通用寄存器为 0；
   - write(2) 只对以写方式打开的文件返回 -EINVAL，否则先返回 -EBADF；OPEN_FMODE 产生访问位与 `__FMODE_NONOTIFY`；
   - myglibc 中与 freopen 有关的行全部被注释（QB-31 改为整词检索）；
   - O_CREAT 结论补上前提：`do_filp_open` 原样传入查找标志（QB-52）；由此另立 UB-14（带 O_CREAT 的 open 不新建文件，新增 QB-69）；
   - 入口落入返回段的原因写准：`call do_syscall_64` 后没有 ret/jmp，`SYM_CODE_START` 只展开为对齐与标签；
   - UB-02 的影响扩大到提交方对已释放请求的写入（QB-55、QB-56）；UB-13 补上 kobj_map 原样保存指针与 chrdev_open 首次打开时的写入（QB-60、QB-61）；UB-06 补上 `kthread()` 以 `kthread_exit` 结束（QB-62）与零帧经信号处理的未知；
   - 新增勘误 ER-01：05 DRU-06 中“共享 fs 不会经用户态出现”不成立，pid 1 带着共享 init_fs 进入用户态（QB-57、QB-63、QB-64、QB-67、QB-68，initd.c 不调用 chdir 见 QB-70）；
   - 边的方向统一为 05 的约定；EB-05 写成 vfs_get_tree → legacy_get_tree → public_dev_mount 两跳间接边（AB1-28、QB-58）；EB-12 改为数据边，另立 EB-39（vfs_mknod 经 i_op->mknod 到 shmem_mknod，QB-59）；GAP-U03 的证据补上 path_lookupat、link_path_walk、do_new_mount、legacy_get_tree、do_add_mount、mnt_set_mountpoint 与 vfs_get_tree（AB1-22…AB1-28）；
   - 候选的 covers 与各条发现的 `future_verification.candidates` 对齐，并加进 `ub_check.py` 的机械核对；勘误原文逐字存在也加进核对；
   - UV-01 追加说明改正为 GAP-U03 静态闭合、GAP-U04 只收窄；读超级块失败只核到 alloc_super；
   - 旧编号在延期记录中一律写全名，facts 与正文给出编号约定。
5. **审查子代理的一条越界命令。** 其中一个审查子代理在 clone 中运行过 `git status`。这是只读查询（可能刷新 `.git/index` 的缓存信息），没有改动工作树或任何文件内容，但超出了本批给审查者限定的 `git show`/`git grep` 范围，在此如实记录。提交前我自己也用 `git status`、`git ls-files`、`git diff --cached` 核对写区（见 §6）。

## 6. 写区与回传

- **只新增：** 本批只在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boundaries-07/` 下新增 10 个文件（五个主件、`scripts/ub_check.py` 与 `records/` 下的四个记录）。
- **提交前边界核对：** 暂存后运行 `ub_check.py <clone 根> boundary records/boundary_staged.json --staged`。它把暂存区与 `b12572f14eae` 的树逐文件比较：基线 1346 个文件模式与对象逐个相同、没有缺失；相对基线的变更全部是本前缀下的新增；没有未暂存或未跟踪的文件；同时记下远端执行分支头，确认推送前没有被别人更新。这个记录先生成一次、加入暂存区后再生成一次，所以记录里的变更清单包含它自己。
- **提交与推送：** 在 `claude/dazzling-cori-q0dnyt` 上做一个普通提交，不改写历史、不 force-push；推送前再次确认远端头仍是 `b12572f14eae`。PR17 保持 Draft，不改 PR 描述，只追加一条评论，写明提交短号与远端读回结果。
- **不自引用：** 本文件与 MANIFEST 不写自己所在提交的标识，提交短号与读回结果在 PR17 评论与最终回复中给出。

## 7. 限度

- 这是静态源码说明与机械一致性核对，不是语义证明，也不是内核或用户程序的运行验证；所有运行层仍为 NOT_RUN。
- `TASK_threadsp`、`X86_top_of_stack`、`TSS_sp2` 只核到符号映射；数值需要编译生成。
- sysretq、swapgs、sti 的硬件语义与中断窗口没有一手资料，保留为外部语义待核。
- 镜像内容（是否有 dev 目录、/boot/init 是否存在）与 libc 行为不在仓库中。
