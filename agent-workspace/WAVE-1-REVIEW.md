# 第一波回收评审报告（WAVE-1-REVIEW）

```yaml
record_type: wave_review
wave: 1（MYOS2-DR-001~009，2026-08-31 夜发出，2026-09-01 回收）
reviewed_by: claude-fable-5 主导会话＋17 个并行子代理（8 精读＋8 对抗核查＋1 综合），全部核查在本地 time 检出上实做
review_date: 2026-09-01
kernel_baseline: time @ a039d9803ade2a1613d620bda375e028530d5242
workspace_baseline: master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719
verdict: 8/9 有交付；材料价值高但事实可靠性参差，全部结论须按本报告的降权清单使用
```

## 1. 交付与合规总表

| 任务 | 交付方式 | 合规裁决 | 抽查可靠性 | 抽查结果 | 伪造 |
|---|---|---|---|---|---|
| 001 外围工程审计 | PR #2 | PARTIAL | **low** | 9 实 / 14 伪 / 1 无法验 | **是**（SHA＋成段虚构） |
| 002 完成度盘点 | PR #3 | PARTIAL | medium | 20 / 5 / 1 | 否 |
| 003 依赖图 | PR #1 | PASS | **low** | 15 / 4 / 1 | 是（行号系统性偏移） |
| 004 调试桩 | **零交付** | — | — | — | 否（诚实报告故障） |
| 005 x86 资料包 | PR #5 | PASS | **low** | 20 / 8 / 0 | 是（SDM 位域错值） |
| 006 测试与 CI | 对话降级 | PARTIAL | **low** | 10 / 12 / 1 | **是**（SHA×14 处） |
| 007 重要度与路线 | 对话降级 | PASS | medium | 11 / 2 / 1 | 否 |
| 008 技术债台账 | PR #4 | PASS | medium | 21 / 3 / 0 | 否 |
| 009 教学蓝本预研 | 对话降级 | PARTIAL | medium | 15 / 3 / 0 | 否 |

字符编码：九份回复全部为干净 UTF-8，无乱码、无替换字符。

## 2. 004 失败诊断（Owner 优先关注项）

**故障表现**：该会话报告"GitHub 连接器把私有仓库的文件内容读取请求错误路由为仓库元数据请求"，未取得任务书、公约、地图与任何源码；建分支/写文件/建 PR 动作同样未能解析。它建出了空的 `agent/MYOS2-DR-004` 分支（指向当时 master），未写入任何内容。

**诊断**：
1. 判定为**该次会话的偶发连接器故障**，非系统性问题——同期八个任务全部成功读取 time 分支；
2. 其对仓库性质的判断即已出错（MyOS2 是 public 非 private），说明连元数据都未正确取得；
3. 它**未走对话降级路径**，理由是"任务书正文与源码快照均未读到，兜底会要求猜测交付物清单与源码事实"——**这个判断正确**，零交付优于伪交付。对照本波次两个编造 commit SHA 的任务，004 的诚实是本波次的正面样本。

**处置**：新任务书 `MYOS2-DR-004R`，加三道保险（开工自检逐字引用公约、public raw URL 降级读法、明示零交付优于假交付），并把 008 已备好的调试桩建议表作为强制输入。

## 3. 已确证的伪造与失实（必须降权使用）

### 3.1 溯源字段伪造（最危险，因其最像真的）

- **001**：全篇文件头与 proposed/ 每个文件头声明 `time@a039d9803ade94c67918930525530d2a1b46e9f0`——该对象不存在（`git cat-file -e` 失败）。真值 `a039d9803ade2a1613d620bda375e028530d5242`，前 12 位相同、其后 28 位为补全捏造。
- **006**：`a039d9803893e0ce54164413aa8a0c29eacf5c78`，14 处复用（含 ci.yml 的 BASE_SNAPSHOT env 与 harness 头注释）。其 tree 哈希 `fa8d26ff…` 却正确，说明确实读到真实树，只是把短 SHA 幻觉补全成 40 位。下游若按此 checkout 必然失败。
- 其余六个任务的基线申报全部正确。**结论：无一任务真的分析错分支，问题是溯源字段被编造这一行为本身。**

### 3.2 001 的成段虚构（不是笔误）

- H-03 整条不存在：所指控的 `/dev/nbd0`、`qemu-nbd`、`losetup -D` 全仓零命中；真实实现是 `losetup --show -f` ＋ `kpartx`；
- H-05 整条不存在：`install_boostloader()` 与 `|| true` 均不存在，真实函数名 `make_install_bootloader`，且用的是 `cmake && make && make install`；
- H-04 一半不成立：`util_cmds.sh` 全文只有一行 `cloc ...`，无 eval 无函数（但 part_vdisk.sh 的 eval+sudo 属实）；
- M-02 判反且危险：称 make_install.sh "所有动作均为注释行、默认无操作"，实际无参运行即 `rm -rf ./build/*` ＋挂盘安装三件套；
- M-11 判反：GDB 符号路径其实一致（`cp kernel $CMAKE_GEN_DIR/../kernel` 就是根 build/kernel）；
- M-12 判反：`user-guide/` 在 time 分支上存在且 README 四条链接全部有效；
- MANIFEST 声称 `bash proposed/make_install.sh help` 可直接运行——实测 exit=1（`MYOS2 ERROR: set MYOS2_ROOT`）。

### 3.3 005 的架构事实错值（四处，须以 SDM 为准）

- ICR destination shorthand 写 `17:16`，实为 **19:18**（仓库自身 apicdef.h:67-69 即可证伪）；
- CR4 bit19 写 Reserved，实为 **Key Locker (CR4.KL)**（且与自己的 cpuid-atlas leaf 0x19 条目自相矛盾）；
- PTE 物理地址字段写自 bit13，实为自 **bit12**；
- 能力字计数写 11，实测 `get_cpu_cap()` 填 **15** 个槽；
- inputs_read 列了两个不存在的文件：`kernel/apic/lapic.c`、`insns/special_insns.h`（真名 `special_insns_arch.h`）；
- "KVM clock 为 stub、未见调用者"三处 [VERIFIED] 断言失实——`kvm.c:147 kvmclock_init()` 有活动调用链。

**但 005 的技术含量整体是真的**：LVT timer 向量 0xEE（穿三层宏推出）、IOAPIC 24 项 RTE、SFMASK 14 个 RFLAGS 位、页表软件位别名等大量非平凡推导均被核实通过（28 条抽查 20 条 CONFIRMED）。

### 3.4 行号锚点不可靠（影响自动化改码）

- **003**：`init/main.c` 行号系统性偏移 5~26 行（如称 setup_arch 在 112-120，实为 138）；8 处引证越过文件末尾（引 `mm_init.c:570-650`，该文件仅 430 行）；
- **008**：69 条证据中 23 条行号越界；热点图静默截断（只列 52/60）；
- **002**：5 条证据路径不存在（被引用 16 次，其中 4 处打了假 [VERIFIED]）。

**使用纪律：重锚完成前，一律把这些证据当"函数级事实断言"而非"行级定位索引"。**

### 3.5 其他

- 003 判 `drivers.rtc` 为"可裁剪叶、主链未见 RTC"——实际主链读 RTC（`main.c:165 timekeeping_init` → `read_persistent_clock64` → `rtc.c:39-48`）；
- 006 未覆盖任务书点名的四类对象中的三类（xv6/SerenityOS/Redox/syzkaller/KernelCI/osdev 全文零命中），且任务书点名的 `isa-debug-exit` 退出码约定零覆盖；其 ci.yml 无工具链/QEMU/OVMF 安装步骤，实测跑不起来；
- 007 MANIFEST 的 tier 计数有误（T1 实为 23、T2 实为 11）；
- 009 因设计上零源码接触，把 entry（74 行）、device（0 源码）、debug（1 文件）都排成了教学单元；
- 三份深度研究（006/007/009）引用为内部检索句柄（175 个）而非可解析 URL/DOI——**外部结论无法独立复核**。

## 4. 评审中独立发现的新事实（九份产出无一涉及）

1. **配置面从未被任何任务读过**：`mykernel/scripts/options_flags.cmake:58-72` 才是真正的内核配置面，实际生效宏仅 12 个（CONFIG_FLATMEM、CONFIG_NR_CPUS=256、CONFIG_64BIT、CONFIG_PHYS_ADDR_T_64BIT、CONFIG_ZONE_DMA、CONFIG_ZONE_DMA32、CONFIG_SLUB、CONFIG_ARCH_HAS_SYSCALL_WRAPPER、CONFIG_BUG、GRUB2_BOOTUP_SUPPORT、CONFIG_HYPERVISOR_GUEST、CONFIG_KVM_GUEST）；第 71 行 `-DROOTBLK_NVME` 被注释。**没有任何一份产出把该文件列入 inputs_read。**
2. **由此闭合 003 的头号悬案**：ROOTBLK_NVME 未定义 → `obsolete/device.h:53-55` 的 #else 生效 → `ROOTBLK_TRANSFER → ATA_master_ops.transfer`。**当前根块设备是 ATA 而非 NVMe**，drivers.ata 属启动核集合。
3. **两个 P0 缺陷（本地读码确认，合计改动 <20 行）**：
   - `arch/x86_64/lock_IPC/atomic/atomic_arch.h:270-280`：`arch_atomic_add_test_negative` 函数名与注释都是加法，生效汇编却是 `LOCK_PREFIX "subl %2, %0"`（addl 版被注释在上一行）；别名 `arch_atomic_add_negative` 同样中招。
   - `spinlock/spinlock_smp_arch.h:50-57`：`arch_spin_trylock` 只读 val 比较 head/tail 就返回 true，全程无 cmpxchg、无 tail 自增——多 CPU 可同时"获锁成功"。
4. **调度唤醒链是断的**：`scheduler_core.c:331` 以 `success = 0` 开头、`:474 ttwu_queue` 被注释、`:482 return success`——`try_to_wake_up` 恒返回 0 且永不入队；`enqueue_task` 体全注释；`wake_up_new_task` 固定取 `per_cpu(runqueues, 0)`。
5. **定时层确定性失效**：`timer.c:798 schedule()` 被注释、`:805` 原样返回入参；`msleep` 为 `while (timeout) timeout = schedule_timeout_uninterruptible(timeout)`——任何非零 msecs 都是**确定死循环**（非产出所写的"可能忙等"）。全树无 `__run_timers`/`do_softirq` 实现。
6. **永久自旋全树 32 处 / 23 个文件**（008 只覆盖 5 个文件）：`futex.c:9 do_futex` 首条语句即 `while(1)`、未知系统调用、fault.c 四处、mmap.c 三处等。后果：用户态一条 syscall 即可挂死 CPU，且自动化测试全部退化为超时而非失败。
7. **SMP 未上线**：AP trampoline 长模式入口 `jmp .` 原地自旋，全树无 INIT/SIPI 唤醒闭环；printk logbuf 锁三处被注释。**所有"基本路径可跑"的判断只对单 BSP 成立，并发结论不可外推。**
8. **TSC 频率是确定性错误**：`cpu/common.c:390` 无条件把 x86_vendor 设为 UNKNOWN → `tsc.c:320` 厂商判断恒不成立 → 频率必然落到 `DUMMY_TSC_KHZ=3GHz`。
9. **分支关系澄清**：`time` 与 `origin/master` 的内核源码**零分叉**（`git diff time...origin/master -- mykernel/ myloader/ myinitramfs/` 为空）；time 落后的 3 个提交全是 agent-workspace 工作区提交。故 time 是内核分析的完整且唯一正确基线。
10. **规模基线（可直接引用免重扫）**：mykernel 837 个 .c/.h 约 150,823 行；CMakeLists 共 106 份（内核 85/initramfs 20/loader 1），其中 **70 份为 0 字节空文件**，真正分发靠 `target_kernel.cmake` 的 15 个 add_subdirectory；`scheduler_core.c` 1643 行中约 41% 为注释；全仓被注释的 return 形态 2244 处；引用 obsolete/ 的活动文件 50 个。
11. **词汇表缺口**：共享词汇表精确为 53 个 ID，`mykernel/time/misc/`（time_misc.c 313 行实码）未获分配 ID，而同构的 mm.misc、sched.misc 都有。002 与 009 的"53/53 全覆盖"成立，缺口在词汇表本身。

## 5. 交叉综合：主攻靶心

**三重交集**（007 的 T0 必修 × 002 的 critical × 003 的启动核集合）共同命中 7 个：`mm.page_alloc`、`mm.kmalloc`、`mm.vm_map`、`mm.fault`、`sched.forkexec`、`sched.scheduler`、`fs.vfs`。

反向信息同样有用：`fs.fat`、`drivers.ata`、`drivers.char` 在 002 是 critical 但在 007 只是 T2——**危险但不值得作为学习主线，应按风险治理而非教学深挖处理**。

**一条元结论**：002 判定的 5 个 maturity-3 子系统全是库与启动期登记（mm.early、lock.atomic、time.ktime、lib.digit、lib.string），没有任何核心机制达到 3；而 lock.atomic 恰恰被查出确定语义 bug。即 **002 的成熟度轴衡量的是"代码完整度"而非"正确性"**，两个口径在第二波必须分开。

## 6. 主攻清单（重要 × 未完成 × 依赖就绪度）

1. **P0 快赢**：修 atomic add/sub 语义反转 ＋ spin_trylock 不做 CAS（<20 行，零前置，005 已备好三组测试向量）
2. **重建验证 substrate**：重跑 004（启动检查点桩）＋ 拆用 006 的冒烟 harness
3. **永久自旋改受控错误返回**（32 处；既是安全修复也是 CI 前置——不改则 CI 只能报"挂了"）
4. **修唤醒链与调度状态机**（三重交集正中；007 已给出四条不变量作为现成验收标准）
5. **接通定时到期 → 唤醒 → 调度链路**（与第 4 项是同一病灶两端，建议并做）
6. **lockdep-lite**（纯绿地 2 行、教学证据最强、有现成真 bug 可抓、SMP 化前置）
7. **构建 feature 边界第一刀**：开 `-Wreturn-type`/`-Wmaybe-uninitialized`/`-Wmisleading-indentation` ＋ GLOB 换显式清单（仅开诊断一项即可暴露 008 的五条 S1）
8. **ATA 根盘路径下的存储栈与生命周期治理**（需第 2、3 项先就位）

## 7. 向前推进的风险

1. **伪造溯源字段的先例已出现**——第二波必须建机械闸门：产出入库前自动跑 `git cat-file -e <sha>`，不通过即拒收。不能靠人工阅读发现。
2. **删码风险**：全部"死代码/未启用"判断都缺配置面依据，已实证被推翻一条。任何删除动作必须先给出"该符号在当前 CONFIG 组合下不可达"的证明。
3. **验证链断裂可能被复制**：第一波九个任务全部未编译、未启动、零运行时验证。第二波须把 004 重跑与冒烟 harness 设为其他实施类任务的硬前置。
4. **真实的数据毁坏风险**：`scripts/phys_nvme_install.sh` 挂载调用者传入设备后无条件 `rm -rf /mnt1/*`；`scripts/part_vdisk.sh` 文件末尾无条件调用 `do_part` 重建 GPT 并 mkfs、目标硬编码指向 Owner 真实 VMDK；`make_install.sh` 无参运行即 `rm -rf ./build/*` 并挂盘。此项被 001 的误报掩盖了。
5. **三套分级口径打架**（002 risk_level / 007 tier / 008 severity），且 002 能力节点粒度过粗、003 只覆盖 37/53——三份 YAML 现状无法安全叉乘。
6. **Owner 身份冲突**：既是作者又是学习者，对自有代码的判断会系统性偏离陌生人视角；叠加本波已有多条失实结论，若被当作"关于我自己内核的事实"吸收，纠正成本极高。
7. **学习目标可能被工程治理挤占**：第一波暴露的问题大量是工程卫生。缓解：主攻清单第 1、4、6 项由 Owner 亲手做并带教学产出，治理类交本地 Claude 并行。
8. **额度与人力错配**：深度研究模式不接触源码，把仓库现成件（`myshell/tests.c`、`dbg-qemu.sh.bak`）当成未知，反推高成本。严格分流：外部文献走深度研究并强制可解析出处，一切需 grep/编译/git 的走本地。
