---
task_id: "MYOS2-LEAD-002"
track_id: "MYOS2-LEAD-002"
produced_by: "GPT-6 Astra Pro（当前会话标识；不认证后台路由）"
model_per_owner: "Pro（主线）"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "7564e79a9b02"
exec_pin_short12: "4d3183678a64"
master_direct_short12: "e8e60cd19540"
phase: 2
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
record_type: "independent_timekeeping_static_delivery_review"
followup_id: "CORE-USER-TIMEKEEPING-READOUT-103"
status: "RETURN"
R103_01: "OPEN"
R103_02: "OPEN"
batch102: "RETURN"
R102_01: "CLOSED_DOCUMENTATION_ONLY"
R102_02: "OPEN"
R102_03: "OPEN"
inputs_read: ["147合同、102审核/101闭合/检查点、执行评论","103固定/活动六件；137项与固定time三十个引用来源逐字复核","旧102六件、D00表41行、timekeeping/clocksource/types/整数辅助和源码直接消费点"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":6,"quotes_reconfirmed":6,"downgraded_to_inferred":0,"scope":"本主线文件新增的短锚；不是作者包的自检或本地P9"}
---

# 第103批独立审核：交付完整，定点 RETURN

**作者完成了发布；主线已完成本轮独立审查。第103批裁定 RETURN，R103-01、R103-02 OPEN。** 需要补的是证据资格和两处影响消费的条件结论，不是缺文件，不重做整个 timekeeping 专题。第102批 D00 另见[部分闭合回执](CORE-USER-HPET-ENABLE-CLOCKSOURCE-102-DOC-PARTIAL-CLOSURE-review.md)。

## 1. 交付与核验边界

[147合同](https://github.com/08822407d/MyOS2/blob/7564e79a9b02/agent-workspace/lead/MYOS2-LEAD-002/147-user-timekeeping-readout-contract.md)；[第103批固定六件](https://github.com/08822407d/MyOS2/tree/4d3183678a64/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103)；[作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6983342388)。[单步比较](https://github.com/08822407d/MyOS2/compare/1a92f9839e53...4d3183678a64) 的唯一父为1a92f9839e53，只新增六件6052行，旧文件净修改/删除=0/0。固定及活动全文、blob均相同。作者评论按ID与PR17顶层无since分页两路读回一致；审查时50+50+50+39=189条，作者正式标记只在6983342388出现。

| 文件 | 新增行 | blob短SHA |
|---|---:|---|
| MANIFEST.md | 112 | 5f0603f07ec2 |
| deferred-findings.yaml | 293 | 022ffefceb6d |
| evidence.md | 1057 | 75fe2b692f57 |
| facts-and-boundaries.yaml | 4327 | 14b62bbb8990 |
| timekeeping-readout-paths.dot | 166 | 5337d2df1ea2 |
| timekeeping-readout.md | 97 | c90ba2fab6db |

审查前 direct lead=7564e79a9b02、exec=4d3183678a64、time=a039d9803ade、master=e8e60cd19540；PR16 Open/Ready、PR17 Open/Draft，均未合并。PR缓存base_sha不代替直接master。后续主线发布将改变 lead，不改变103固定输入。

源码短锚依原顺序核137条，在30份固定time源文中逐一对照：原文、1–5行、辅助行范围、facts源码索引与引文均相符；原始六件标签137/137/0，说明文字无额外词头，40hex为0。七组事实表的行数分别10、7、8、9、6、9、6，共55行；逐行索引与行中ID列表一致。有限模型23、候选8、缺口7，均为静态记录，不按数量签通过。定义单元及其是否证明命题另审如下。

## 2. 目前可以消费的主干

| 主题 | 静态支持 | 首个不能越过的门 |
|---|---|---|
| A 初始化 | timekeeping_init 先索取墙钟和 boot_offset，再取默认源；有 enable 时通过字段请求，随后 setup；mono/raw 的 clock/mask/cycle_last、mult/shift 与 mono墙钟分数分别建立。tk_set_wall_to_mono 的保存语句被注释 | 默认函数为弱符号，实际ELF选择未核；回调正常返回、对象寿命、多字段一致和旧clock条件不能从初始赋值推出 |
| B 计数换算 | delta 的无符号差及mask；与 max_cycles 比较后分普通乘加、返回旧分数、辅助乘加三支。普通u64结果可能与无限精度不同。辅助高字非零才消费 32-shift | 不能把人工helper输入当成选中jiffies的真实样本。低32位mask时 delta 的高32位为0；本节默认shift=8。位移域不成立即停止正常值推导 |
| C 读接口 | ktime_get 使用有符号base与u64纳秒；real接口取xtime_sec后加纳秒。商u32与余数u64不同；余数规范不保证秒商精确。READ_ONCE取一个clock指针不是整组字段快照 | 实际时钟、真实日期、链接、同步与输出指针状态未证。负值秒的完整类型链及初始化原值要按§4修正 |

在 HZ=1000、JIFFIES_SHIFT=8、TICK_NSEC 定义选入、连续低32位mask及表内 fraction=0 的条件下，mult=256000000；delta=1/10分别得1000000/10000000ns，delta=11选辅助分支并在合法域得11000000ns。last=0xfffffffe、now=1时低32位差为3。默认值不是所有输入的保证；TK103-M11/M12等高字样例是构造的通用helper场景。公式有独立定义；作者E089本身不能证明该公式。

## 3. R103-01 OPEN：原文存在，但定位符、定义或读账不足

| 原项 | 直接检查结果 | 最小追加 |
|---|---|---|
| TK103-E001 | 逐字连续的 CMake 段存在；set_CMAKE_C_FLAGS 不是实际命令名 | 用可定位的单条 set(CMAKE_C_FLAGS …) 及 ${UNUSED_MACRO} 起始内容消歧，不跨相邻命令 |
| TK103-E007–011；E012–013；E030 | 分别属于 __clocksource_update_freq_scale、__clocksource_register_scale、myos_hpet_clkevt_set_state_periodic；旧标签仅缩写 | 新ID+真实定义名+旧ID→新ID→确切消费者；相同源码可复用，不把这九条（含E001）列成九个新发现 |
| TK103-E089 | 只引了宏前的说明注释；实际 #define 在下一行。147§7明确要求定义 | 以真实 TICK_NSEC 宏体补锚；连接默认源字段、数值样例和直接依赖。保留有条件的数值，撤回旧引文的定义资格 |
| 读账/P1 | 30条 source_paths 不是全部读入清单；原回执点名的路径须有范围；原MANIFEST把固定SHA写进base_snapshot值 | 明确分开引文源、完整读、窗口读和历史复用；补 R102-02 指定文件，clocksource_types.h 用于 clock 自身类型，timekeeping_types.h 用于缓存类型。快照字段只写分支名，短SHA另列 |

两批共同消费处只做一次新增。这是可操作的最小修订清单，不把“十条有问题”反推为“已认证其余127个任意断言”。引文、结构索引和命题资格分别处置；原103历史137/137/0保持原样。对E089及别名建立真实 supersedes；发现同类误指则连受影响的行一起修正。

## 4. R103-02 OPEN：字段写入与负数表示的适用条件

**A-F09 / TK103-F04 / 正文。** 作者在源码叙述中正确提到旧时钟分支会移位，却在之后的概括及结构化候选中把 raw.xtime_nsec 列入“这些函数中未写”的字段。E065/E066已给出反例。old_clock 为空时跳过这两次分支写；非空且到达合法赋值时，mono 和 raw 的分数均被读后写回。位移0时值可不变，但仍有赋值。mono.base 的局部无写与 raw.xtime_nsec 的条件写应分行。静态初态、没有其它写者和后续对mono分数的 tk_set_xtime 也要分开。D00只需补直接受影响的 prose、A-F09、F04、相关引用，不重审整个初始化。

**M15 / A-I05 / F02。** 指向 clocksource.shift 自身的 u32 声明（可复用固定 HC102-E089），不要把 tk_read_base.shift 代作同一结构的证据。先在声明的整数宽度/提升域算差，再到 int，再分支；若选到负值，还要考察取负能否表示，之后才是位移计数。移位合法不等于时钟确被更换；没有方言、编译开关及ABI依据，不签真实机器的实现选择。

**M20 / C-N07 / F06 与入口。** ns_to_timespec64 的负数表达式里有两个不同的负号。内层针对 s64 nsec，最小值须先检查取负；外层针对返回 u64 的 div_u64_rem，然后再减1，所以外部算式先是无符号结果，随后才写入有符号 tv_sec。对 -1：内层被除数为0、余数为0；外层无符号秒为全1，得到有符号 -1 还需要相应转换语义。s32 rem 传给 u32* 的接口约束另行记账。local_clock() 返回 u64、形参为 s64，是调用前另一转换门；不要求展开 native_sched_clock。将数值样例写成清楚的分阶段条件，区别数学结果、源算式及未编译的实际行为。

**B和图的消费限定。** 分数为0才可直接复用上述三个示例值；位移33且高字非零示例不能归因于低32位mask默认链。新图所保留的间接调用、数据流与未证连接可消费。若用图讲旧时钟或负数路径，要反映上述两组真实门；不要求重画所有正确的103边。不能把 8项候选 当作八个实测故障。

## 5. 主线独立短锚

### RV103-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

单条 set 命令的连续片段；不能证明实际编译或链接。 固定 time 短 SHA a039d9803ade；辅助位置 58–61。

### RV103-Q2

[VERIFIED mykernel/time/systick/systick_const.h::TICK_NSEC]
```c
	#define TICK_NSEC		((NSEC_PER_SEC+HZ/2)/HZ)
```

实际宏定义，不是前一行注释。 固定 time 短 SHA a039d9803ade；辅助位置 7–7。

### RV103-Q3

[VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
	if (old_clock) {
		int shift_change = clock->shift - old_clock->shift;
		if (shift_change < 0) {
			tk->tkr_mono.xtime_nsec >>= -shift_change;
			tk->tkr_raw.xtime_nsec >>= -shift_change;
```

旧clock存在且差为负的条件写。 固定 time 短 SHA a039d9803ade；辅助位置 161–165。

### RV103-Q4

[VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
		} else {
			tk->tkr_mono.xtime_nsec <<= shift_change;
			tk->tkr_raw.xtime_nsec <<= shift_change;
		}
```

另一分支也有raw复合赋值。 固定 time 短 SHA a039d9803ade；辅助位置 166–169。

### RV103-Q5

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```

应分别跟踪内外取负、减1与输出存储。 固定 time 短 SHA a039d9803ade；辅助位置 64–65。

### RV103-Q6

[VERIFIED mykernel/include/linux/kernel/math64.h::div_u64_rem]
```c
		*remainder = dividend % divisor;
		return dividend / divisor;
```

被调体写余数并返回商；u64返回/u32*参数另外核对该定义的真实签名。 固定 time 短 SHA a039d9803ade；辅助位置 43–44。


## 6. 可复用边界

本轮领域结论限定于固定源码及写明的整数域，不认证预处理、编译、ELF、设备采样、实际并发或运行。完整本地P9 NOT_RUN。固定旧件与评论冻结；原作者继续STOP_WRITING（103）。依据28/79/110/124，把四项尚开问题合并成104的一个D00；RTC三个专题可独立继续。下次只有C07独立核收才可关闭。
