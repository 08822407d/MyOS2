---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-MEMDEV-SCALAR-IO-115"
record_type: "independent_memdev_scalar_io_review"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（沿用 Owner 在本主线已明确选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "a5b40470646a"
execution_pin_short12: "452444a1c4b4"
status: "RETURN"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
R114_01: "CLOSED_DOCUMENTATION_ONLY"
R114_02: "CLOSED_DOCUMENTATION_ONLY"
batch114_composite: "PASS_PENDING_LOCAL"
R115_01: "OPEN"
R115_02: "OPEN"
R115_03: "OPEN"
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/164-user-memdev-scalar-io-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FILE-SEEK-POSITION-114-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md","mykernel/scripts/options_flags.cmake","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/include/linux/kernel/syscalls.h","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/drivers/char/char_dev.c","mykernel/fs/syscall.c","mykernel/fs/vfs/file.c","mykernel/include/linux/fs/file.h","mykernel/fs/vfs/read_write.c","mykernel/include/linux/fs/fs.h","mykernel/fs/vfs/file_table.c","mykernel/fs/vfs/open.c","mykernel/include/uapi/asm-generic/fcntl.h","mykernel/arch/x86_64/mm/page/page_const_arch.h","mykernel/include/linux/kernel/minmax.h","mykernel/drivers/char/mem.c","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/fs/fat/myos_fat32.c","mykernel/include/linux/kernel/limits.h","mykernel/include/linux/lib/errno.h","mykernel/include/linux/kernel/asm-generic/bug.h"]
---

# 第115批独立验收 · 字符设备的普通读写

**裁定 `RETURN`。** 七个长度的零设备模型、九个位置模型和六格设备结果在声明的前提下成立。正式报告缺值，图的调用主体及部分证据归属须追加纠正。R115-01/02/03 均 OPEN，交第116批 D00。第114批两项补正已独立闭合。[固定164](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/164-user-memdev-scalar-io-contract.md) · [固定六件](https://github.com/08822407d/MyOS2/tree/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115) · [作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7044709177) · [114补正裁定](CORE-USER-MEMDEV-SCALAR-IO-115-D00-114-review.md)

## 可消费的结果

| 请求长度（字节） | 实际清零字节 | 返回计数 | 漏计末块 |
|---:|---:|---:|---:|
| 0 | 0 | 0 | 0 |
| 1 | 1 | 0 | 1 |
| 4095 | 4095 | 0 | 4095 |
| 4096 | 4096 | 0 | 4096 |
| 4097 | 4097 | 4096 | 1 |
| 8192 | 8192 | 4096 | 4096 |
| 8193 | 8193 | 8192 | 1 |

n 指截断后长度，缓冲区有效、可写，末端放置哨兵。每轮先清零和扣减，余量为零立即跳出。n>0 时返回 `4096×floor((n−1)/4096)`，n=0跳过循环。逐轮复算区间、扣减、break、累计和最终位置。4096的一轮余量为0，4097的两轮分别为1、0。哨兵不变，读回调没有改位置，普通非流文件的外层因非负返回把原位9重新赋回。[清零循环](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L82-L109) · [读位置提交](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L175-L196)

同取4097字节、原位9：有效 READ 下 null 返回0且缓冲不变，zero 清4097返回4096，full 缺读方法返回 `-EINVAL`。明确具备 WRITE 与 CAN_WRITE 时，null 与 zero 经 write_null 返回4097，full 缺写方法返回 `-EINVAL`。六格最后位置均为9，非负时外层执行同值赋值。普通 O_WRONLY 创建时 OPEN_FMODE 只给 WRITE；如无后续 CAN_WRITE 赋位，写在模式门即返回 `-EINVAL`。实际建立和打开设备仍是前提。[写门与分派](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L287-L338) · [设备表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L111-L155) · [模式生成](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/fs/fs.h#L3425-L3428)

## 退回项与关闭条件

**R115-01 · 三张报告表缺值。** 正文83–89行七个漏计值、七个哨兵，95–97行三个扣减后余量，111–116行六个回调共23格为 `undefined`。六格表三条写行还含未转义的模式位竖线，Markdown 会多分一列。追加三张可独立阅读的完整替代表，按模型 ID 绑定 `final_chunk_not_counted`、`sentinel_byte`、`loop_steps[].remaining_after_subtract` 与 `C.six_cells[].method`。六个方法依次是 read_null、write_null、read_zero、write_zero→write_null、缺读方法、缺写方法。核对实际渲染列数，冻结原六件。[原表](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io.md#L79-L116) · [B模型](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml#L768-L1120) · [C模型](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml#L1214-L1400)

**R115-02 · 证据定位、标签计数和字段引用。** 16份源码的70个1–5行窗口全部逐字命中。E001标签写作 `CMake_C_FLAGS`，实际设置为 `CMAKE_C_FLAGS`。按作者提交时的标识，69个归属正确。evidence有70条正式引文，MANIFEST:116、facts:32 两处历史说明又命中协议P9⑤的字面前缀，全包72处。追加账分别保留70个逐字窗口、69个原标识正确、72次前缀。历史说明用普通文字，新包单独计数。[E001](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md#L5-L13) · [清单](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/MANIFEST.md#L112-L116) · [历史说明](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml#L29-L36) · [配置变量名](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L58-L66)

同项补齐事实表出处。A.read_VFS 与 A.write_VFS 的 `source`（406/420行）各自只指向验证/截断。模式门须引 E023、P10 与 E028，验证返回和截断各引 E033、E025/E030。A.MAX_RW_COUNT 的十进制值须补 PAGE_MASK（页常数头11行）及 INT_MAX（内核 limits.h:30），说明32位int。C.installation.sources 需为 cdev_init/cdev_add 补 E066/E067及取 devlist 的普通位置，为换表保留 E042/E068/E069。C.operations.zero 的清零结论需补循环、清零、break和累计来源 E051–E056，正长度才有漏计。[A字段](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml#L403-L477) · [C字段](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml#L1180-L1214) · [页掩码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/mm/page/page_const_arch.h#L8-L11) · [INT_MAX](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/limits.h#L28-L32)

**R115-03 · 图的调用归属和返回条件。** DOT 84–89把系统调用直接连至 fd 助手，又由 fd 助手连至VFS。补出 ksys_read/ksys_write 节点，它们分别调用 fd 助手、file_ppos及VFS。读用 myos_fdget_pos，写用 fdget_pos。97行截断发生于VFS，依据 E025/E030，阈值与立即返回0分别引 E043/E033。107行实际字段存储须连 E017/E021。109–120补 n=0 从循环条件直接返回的边，说明单轮 memset 与全区间归纳的关系。83行补分派宏 E002 和映射 P01；同一 read_zero 的方法绑定与函数体对照应使用合并节点或标为同一实现。[原图](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot#L75-L132) · [读包装](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L175-L196) · [写包装](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L319-L338)

同项收紧报告53行与 facts.A.iter_detail（433–434）。同步迭代助手在处理位置前先求值 `BUG_ON(ret == -EIOCBQUEUED)`。正常越过该门后，读侧 ppos 非空时写局部位置，写侧还需 ret>0。不得把特殊排队返回笼统归入正常负返回。三个设备的标量结果不经过这些助手。[读助手](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L83-L92) · [写助手](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L208-L216) · [排队哨兵](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/lib/errno.h#L31) · [BUG_ON定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/asm-generic/bug.h#L56-L66)

## 独立核验范围

唯一父 a2dd370fcf15 → 452444a1c4b4，compare 前进1、落后0、共1提交。指定六件新增3415行，旧件修改/删除0。固定与活动六件全文及 blob 6/6 相同。作者评论按ID两读并与无since顶层分页（100/100/17/0）正文相同，115所有作者正式交付标记和164发布标记各一次。PR16 Ready、原题PR17 Draft，均开放、base master、未合并。四头见页首。

35个普通位置均落在真实文件行界内。六件无40位十六进制串，inputs_read路径可定位，time/master两对金丝雀与master公约原句相符。facts、deferred可解析，8候选与8缺口保持条件记录。DOT 58节点、60边、无未声明端点。已逐组比对A9与5模式门、B7及逐轮、C6与4写对照。完整本地P9、仓库程序、预处理、编译、Graphviz和设备运行均 NOT_RUN。

## C08 独立回源的短引文

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		memset(buf + cleared, 0, chunk);
		count -= chunk;
		if (count == 0)
			break;
```
[mem.c L90–93](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L90-L93)

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		cleared += chunk;
```
[mem.c L95–95](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L95-L95)

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]
```c
		ret = vfs_read(f.file, buf, count, ppos);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
```
[read_write.c L191–194](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L191-L194)

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```
[read_write.c L88–92](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L88-L92)

三项差量由原 C07c 在116新六件中提交，待 C08 再行裁定。
