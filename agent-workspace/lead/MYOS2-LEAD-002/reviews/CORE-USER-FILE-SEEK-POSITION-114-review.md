---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILE-SEEK-POSITION-114
record_type: independent_file_seek_position_review
produced_by: GPT-6 Astra Pro
model_per_owner: Pro
conversation_display_name: MYOS2-A-C07 内核分析主线续建
date: 2026-11-20
phase: 2
read_channel: connector
source_pin_short12: a039d9803ade
master_direct_short12: e8e60cd19540
lead_before_publication_short12: 41b2c2f955c6
execution_pin_short12: a2dd370fcf15
full_local_P9: NOT_RUN
build_and_runtime: NOT_RUN
status: RETURN
R114_01: OPEN
R114_02: OPEN
R112_02: CLOSED_DOCUMENTATION_ONLY
R113_01: CLOSED_DOCUMENTATION_ONLY
inputs_read: [contract_162, prior_decisions, fixed_six, active_six, direct_refs, pull_requests, author_comment_and_all_pages, fixed_source_and_canaries]
self_check: {literal_windows: 58, single_definition_eligible: 57, ordinary_positions_as_delivered: 28, mainline_review_quotes: 4, retained_models_A_B_C: [6,11,5]}
---

# 第114批主线独立验收 · 文件定位

**裁定 `RETURN`，限两项文档余项 R114-01/02。** 有效对象下的入口分派、FAT32位置边界和字符设备状态对照可按下列前提消费。D00 历史两项独立关闭。[D00独立裁定](CORE-USER-FILE-SEEK-POSITION-114-D00-112-113-review.md) · [114正式作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7034736478) · [已执行的162合同](https://github.com/08822407d/MyOS2/blob/41b2c2f955c6/agent-workspace/lead/MYOS2-LEAD-002/162-user-file-seek-position-contract.md)

## 交付与独立核验

| 项目 | C07核验结果 |
|---|---|
| 父链、路径 | 41697d76ae5e → a2dd370fcf15，单父；compare ahead 1、behind 0、commits 1。六件均新增，共2,581行，旧件修改0、删除0。 |
| 固定与活动 | 原执行分支与固定六件全文、树中 blob 逐件相同。MANIFEST、报告、facts、DOT、deferred、evidence 全部回读。 |
| 发布归属 | 正式评论 #7034736478 按ID两次及全分页正文相同。审核时213条顶层评论，分页50/50/50/50/13/0。全批作者正式标记恰一次，归 EXEC-17；162发布标记一次。 |
| 现场 | 主线发布前 41b2c2f955c6，执行 a2dd370fcf15，time a039d9803ade，direct master e8e60cd19540。仅PR16/17开放，原head/base，16 Ready，17原题Draft，均未合并。 |
| 源码与图 | 逐一取固定版本原行核58个字面窗口，均命中；按单一定义重新分类后57个合格。原列28个普通位置路径及行界成立。图59个声明节点、58条带标签边，端点均能解析。8候选及8缺口按静态条件保留。 |
| 约束及验证 | direct master公约§1第2条，time/master 的 CMake 模型与 PIE 行、panic 的 CPU 读取行及 time 的有效 C 选项已核。仅做文本检查与有界整数复算。完整本地P9、预处理、编译、链接、仓库程序、Graphviz、内核和设备 I/O 均 NOT_RUN。 |

[执行提交](https://github.com/08822407d/MyOS2/commit/a2dd370fcf15) · [固定六件](https://github.com/08822407d/MyOS2/tree/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114) · [evidence.md](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/evidence.md) · [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/MANIFEST.md)

## 可保留的源码结论

| 专题 | 有界结果与前提 |
|---|---|
| A · 入口的6个模型 | x86-64 lseek 编号8。fd、whence 均为 unsigned int；fd 达上界先返回 −EBADF，whence 达 SEEK_MAX=4 返回 −EINVAL。有效 file、whence 为0–3时再查看方法表。缺表或方法则返回初始0，入口体无 f_pos 写入。回调返回的负 loff_t 先转 ulong，外层返回 long 时超出其有符号正值域，需目标转换约定。入口提前错误直接返回。 |
| B · FAT32的11例 | 普通 inode 的 i_fop → 打开 file 的 f_op → FAT32_file_ops.llseek。在候选计算可表示时，SET取offset，CUR与END分别加旧位置、文件长度，再检查闭区间 [0,i_size]。默认项 −EINVAL，越界 −EOVERFLOW，均保留旧位置。i_size=100、旧位40时，B01–B11独立复算均符；EOF 100可写，101被拒。DATA=3经入口、在后端默认项被拒；HOLE=4在入口即被拒。 |
| C · 字符设备的5例 | 同取已放行的 CUR、offset=2、旧位9。null / zero 各写0并返回0。缺回调或缺表时返回0且仍在9。保留 def_chr_fops 时 noop_llseek 返回9且无写入。full 表的 llseek 为注释；chrdev_open 可用 p->ops 替换 file 的表。这里没有运行注册或打开设备。 |

[syscall.c L75–91](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/syscall.c#L75-L91) · [myos_fat32.c L278–309](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L278-L309) · [mem.c L42–56](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L42-L56) · [mem.c L111–155](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L111-L155) · [char_dev.c L70–78](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/char_dev.c#L70-L78) · [read_write.c L42–45](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L42-L45)

## 两项 OPEN 的依据与闭合条件

**R114-01 · 映射实例和包装宏的证据归属。** FS114-E003 在 syscalls_64.h 第11行，是 `__SYSCALL(__NR_lseek, sys_lseek)` 列表调用。其字面和编号映射成立，应归普通位置；E004 才是该处展开所用的宏定义。与107中将 P01/P02 作为映射实例的裁定相符。E044 属于一个完整的 def_chr_fops 聚合定义，具备定义归属。冻结作者58/58原账，差量须列58次逐字、57次合格、原列28普通位置与E003重分类的对应，并同步消费它的 facts/图/报告。[syscalls_64.h L11–11](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L11-L11) · [syscall_64.c L26–31](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L26-L31) · [char_dev.c L95–98](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/char_dev.c#L95-L98)

E007 能证明生成宏调用 `__SYS_STUBx(x64, sys##name)`；实际 x64 桩返回 long 和返回 `__se_##name(regs)` 须连到104–109行；E008再连接 `__se` 到 `__do`。有效 C 的 wrapper 开关在65行，包含门在syscalls.h 的93–102行，E001的66行只给 CONFIG_BUG。差量须将定义、映射实例、配置选择和实际调用分别绑定到相关消费处，保留源码层的证据等级。[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66) · [syscalls.h L93–102](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L93-L102) · [syscalls.h L1383–1389](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L1383-L1389) · [syscall_wrapper.h L104–109](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L104-L109) · [syscall_wrapper.h L241–250](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241-L250)

**R114-02 · 两个门的机器可读语义。** facts 第389行 `A.fd_gate.no_pointer_dereference_before_return=true` 范围过宽。即使 fd 达上界，条件仍经 `curr->files->fd_count` 取值；随后不会取 fd_array[fd]、读 file 的方法表、调用 llseek 或写位置。第402、408、414行把 SET/CUR/END 三个 FAT 后端结果笼统记为 PASS，只能表示识别了 origin。最终是否写位置仍取决于候选是否在闭区间。B05/B06/B07/B11 均为被识别却越界；B04/B10 在等号处成功。差量应给出这两处准确的字段语义，连接现有有界行并对齐报告和事实表。原件保持冻结。[facts-and-boundaries.yaml L382–415](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/facts-and-boundaries.yaml#L382-L415) · [syscall.c L78–90](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/syscall.c#L78-L90) · [myos_fat32.c L283–308](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L283-L308)

消费边界：在声明的 LP64 范围内 long 与 long long 的有符号值域相同；越界加法可能在范围检查之前发生，不能用之后的检查证明算术安全。C 中对表替换的数据流可从 `fops = p->ops`、空值门、`filp->f_op = fops` 三处回查。以上有界模型不证明设备状态、并发稳定性或运行结果。[myos_fat32.c L280–305](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L280-L305) · [char_dev.c L70–78](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/char_dev.c#L70-L78)

## 独立引文

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```

[syscall_wrapper.h L105–108](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L105-L108)

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;
```

[syscall.c L82–85](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/syscall.c#L82-L85)

[VERIFIED mykernel/fs/syscall.c::lseek]
```c
	fp = current->files->fd_array[fd];
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
	return ret;
```

[syscall.c L87–90](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/syscall.c#L87-L90)

本回执记录第114批裁定和验收条件。执行者 C07c / EXEC-17 已申报 STOP_WRITING；第115批暂停。[主线检查点](../checkpoints/2026-11-20-seek114-reviewed-c07-to-c08-handoff-prepared.md)
