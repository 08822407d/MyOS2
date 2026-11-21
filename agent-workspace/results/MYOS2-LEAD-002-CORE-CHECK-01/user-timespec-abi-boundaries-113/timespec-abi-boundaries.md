# 第113批｜用户态时间结构复制、校验、比较与规范化

执行身份：MYOS2-A-C07c / MYOS2-LEAD-002-CHAT-EXEC-17。任务 CORE-USER-TIMESPEC-ABI-BOUNDARIES-113，固定源码 time@a039d9803ade，161任务书主线发布头c58d1ad8f222，执行唯一父f909700f17ab。此为阶段2静态源码取证与有限整数核对；没有编译或运行MyOS2。

## D00：第111/112批三项OPEN的附加纠正

### R111-02：七个原表实际键、三模型签名与M09写回

112冻结事实表中带`AT111-`前缀的B08/C07/C08/C10/X06/X07/X08并非111原矩阵的真实行键；111的矩阵`id`是简短的B08等，模型的`id`才带`AT111-`前缀。这不是丢失数据，而是跨矩阵取行方式错误。现依111原件逐条复核并给出真正的两个维度：

| 冻结112错误键 | 冻结111实际矩阵／真实id | 111原记录中的source_ids |
|---|---|---|
| AT111-B08 | current_cpu_wrappers / **B08** | AT111-E019, AT111-E020, AT111-E021, AT111-E023, AT111-E024, AT111-E026 |
| AT111-C07 | explicit_cpu_add_timer_on / **C07** | AT111-E045, AT111-E046, AT111-E047 |
| AT111-C08 | explicit_cpu_add_timer_on / **C08** | AT111-E039, AT111-E040, AT111-E041, AT111-E045, AT111-E046 |
| AT111-C10 | explicit_cpu_add_timer_on / **C10** | AT111-E039, AT111-E032, AT111-E034, AT111-E033, AT111-E051 |
| AT111-X06 | cross_path_boundaries / **X06** | AT111-E037, AT111-E038, AT111-E039, AT111-E044 |
| AT111-X07 | cross_path_boundaries / **X07** | AT111-E037, AT111-E038, AT111-E041 |
| AT111-X08 | cross_path_boundaries / **X08** | AT111-E039, AT111-E040, AT111-E045, AT111-E046 |

三个模型返回类型独立补充：`AT111-M08`说三个公开包装返回void，需同时引AT111-P02/P03/P04的三个签名；`AT111-M10`（等基准）和`AT111-M11`（空回调）说`add_timer_on`返回void，各需引AT111-P05。将B08/C07矩阵补正不能当作已为三个模型补证。完整体辅助范围另见TS113-P17/P18。

`AT111-M09`冻结原模型本身不含E052，问题是112追加消费者记录误把`AT111-E052`加入其写位来源。E052的实际内容是`timer.c::schedule_timeout`栈定时器注册，与本例CPU/桶号写入无关，现从M09的补正出处撤出。M09先在`add_timer_on`的CPU位改写处（AT111-E044、KD112-E016、本批TS113-E018）得到**0x00100007**；在非pending、回调有效、旧CPU5→显式CPU7、两基准指针不同且已到达入桶、forward后clk=1000、expires=1064的前提下，delta=64→层1，先计算有效桶期限1072、idx=70，再由`enqueue_timer→timer_set_idx`（KD112-E022/E023、本批TS113-E019/TS113-E017）写入位域，结果**0x11900007**。入口CPU改写与下游桶号写入是两个不同时间点。没有真实CPU/锁/运行观测。

### R112-01：三个复合标签拆成十个单一宏

历史112总计53个文本匹配，**只有50个属于单一定义**：E026合并4宏，E027合并2宏，E034合并4宏。新引文逐宏独立归属（行号和定义由固定time源码直接重读）：

| 原复合标签 | 真实单宏 | 本批独立引文 | 原文件及行 | 用途 |
|---|---|---|---|---|
| KD112-E026 | `LVL_BITS` | `TS113-E002` | timer_const.h:108 | six bits per level |
| KD112-E026 | `LVL_SIZE` | `TS113-E003` | timer_const.h:109 | level width=1<<6 |
| KD112-E026 | `LVL_MASK` | `TS113-E004` | timer_const.h:110 | level index mask=64-1 |
| KD112-E026 | `LVL_OFFS` | `TS113-E005` | timer_const.h:111 | n*64 offset, requires n |
| KD112-E027 | `TIMER_ARRAYSHIFT` | `TS113-E006` | timer_const.h:195 | bucket-index bit shift |
| KD112-E027 | `TIMER_ARRAYMASK` | `TS113-E007` | timer_const.h:196 | high index bit mask |
| KD112-E034 | `KTIME_MAX` | `TS113-E008` | time_misc_const.h:44 | s64 positive cap |
| KD112-E034 | `KTIME_MIN` | `TS113-E009` | time_misc_const.h:45 | s64 negative cap |
| KD112-E034 | `KTIME_SEC_MAX` | `TS113-E010` | time_misc_const.h:46 | truncated signed sec boundary |
| KD112-E034 | `KTIME_SEC_MIN` | `TS113-E011` | time_misc_const.h:47 | negative sec boundary |

选层阈值与步长不能混写：`calc_wheel_index`按delta<LVL_START(1)=63及delta<LVL_START(2)=504选出层1（TS113-E015/TS113-E014），然后`LVL_CLK_SHIFT=3`和`LVL_SHIFT(1)=3`决定8节拍粒度（TS113-E012/TS113-E013）；`LVL_OFFS(1)=64`、`LVL_MASK=63`与calc_index共同给出idx70（TS113-E005/TS113-E004/TS113-E016）。最后将桶号用SHIFT=22、MASK=0xFFC00000写回（TS113-E006/TS113-E007/TS113-E017）。结构秒字段类型`time64_t`在`time_type_declaration.h:10`显式为`__s64`，本批TS113-P23普通位置补证，类型声明不伪充函数锚。

冻结112的`MANIFEST.md`缺三个YAML头字段：`inputs_read`、`open_questions`、`branch_canary_quotes`。本批在`facts-and-boundaries.yaml`的`D00.R112_01.frozen112_manifest_metadata_append`下**明确归属112而非113**追加机器可解析的完整路径清单（49个实际112已读取MyOS2路径）、`open_questions: []`（Owner未被要求决策，技术待证另在deferred）和time/master两对真实金丝雀原句。不修改112固定文件，不把113新输入冒称112已读。

### R112-02：六例双向转换与负数表达式/图的补正

112的C01–C06原表只从纳秒标量回到秒/纳秒，本批在同样输入域和**声明的编译/类型前提**下，为每项分别复算`timespec64_to_ktime`与`timespec64_to_ns`两列：

| 冻结112模型 | 原标量 ns | 转换所得 (秒,纳秒) | 除法商/余数 | 回经ktime_set | 回经timespec64_to_ns |
|---|---:|---|---|---:|---:|
| C01 | 0 | (0,0) | — / — | 0 | 0 |
| C02 | 1 | (0,1) | 0 / 1 | 1 | 1 |
| C03 | -1 | (-1,999999999) | 0 / 0 | -1 | -1 |
| C04 | 1000000000 | (1,0) | 1 / 0 | 1000000000 | 1000000000 |
| C05 | -1000000000 | (-1,0) | 0 / 999999999 | -1000000000 | -1000000000 |
| C06 | -1000000001 | (-2,999999999) | 1 / 0 | -1000000001 | -1000000001 |

这里第C01行nsec=0时，仅有初始化`ts={0,0}`并由函数尾`return ts`返回（TS113-E049/TS113-E052）；**没有**`div_u64_rem`调用，不应为零分支挂除法锚。其余非零模型中，函数局部`s32 rem`的地址是`s32*`，但`div_u64_rem`形参为`u32*`（TS113-P14），复制行为、编译诊断与严格类型规则下的实际运行没有验证。

负数路径要严格区分两次取负：内层在有符号`s64 nsec`上先计算`-nsec`再减一；最小s64的内层一元负号会出现有符号溢出风险。把该结果传给返回`u64`商的`div_u64_rem`后，外层`-q-1`则按**无符号64位模运算**，q=0得到18446744073709551615，q=1得到18446744073709551614；写入有符号`time64_t`秒字段时再按目标实现定义的unsigned→signed转换约定取得-1/-2，不是ISO C保证的跨平台结果（TS113-E051/TS113-E053、TS113-P08/P14）。负秒虽具有规范纳秒分量，仍被外部`timespec64_valid`拒绝。112 A包装按值接收结构，传入`ktime_set`的则是秒、纳秒**两个标量**（TS113-E054，TS113-P15）；`timespec64_to_ns`另收结构指针。

冻结112图需修正的全部指定行（旧边仅作历史定位，新图用确有源码动作的端点）：

| 旧图行 | 原端点及类型 | 正确端点及类型 | 源码依据 |
|---:|---|---|---|
| 82 | `on -> explicit (DIRECT_CALL_TARGET_CPU; KD112-E014)` | add_timer_on -> get_timer_cpu_base (DIRECT_CALL)； get_timer_cpu_base -> per_cpu_ptr (MACRO_USE) | TS113-E020, AT111-E039, KD112-E014 |
| 89 | `pprev -> unhashed (DATAFLOW_INIT_FIELD; KD112-E002)` | simple_init_timer_key -> entry.pprev=NULL (WRITE_FIELD)； entry.pprev -> hlist_unhashed_lockless (READ_ONCE_CONSUMER) | TS113-E021, AT111-E012, KD112-E002, TS113-E060 |
| 102 | `valid -> tskt (NO_DIRECT_CALL_IN_BODY)` | timespec64_to_ktime -> no_validate_inside (ANNOTATION_ONLY_NO_CALL; scope complete function) | KD112-P03, KD112-P04, TS113-P15 |
| 103 | `valid -> to_ns (NO_DIRECT_CALL_IN_BODY)` | timespec64_to_ns -> no_validate_inside (ANNOTATION_ONLY_NO_CALL; scope complete function) | KD112-P04, KD112-P05, TS113-P16 |
| 107 | `test -> cap (BRANCH_TRUE_DIRECT_CALL)` | safe_add -> guard_true (CONTROL_BRANCH)； ktime_add_safe -> ktime_set (DIRECT_CALL) | TS113-E058, KD112-E047 |
| 112 | `pos -> div (DIRECT_CALL)` | ns_to_timespec64 -> div_u64_rem (DIRECT_CALL at positive branch)； ns_to_timespec64 -> positive (CONTROL_BRANCH) | TS113-E050, TS113-E053 |
| 113 | `neg -> div (DIRECT_CALL)` | ns_to_timespec64 -> div_u64_rem (DIRECT_CALL at negative branch)； ns_to_timespec64 -> negative (CONTROL_BRANCH) | TS113-E051, TS113-E053 |
| 115 | `div -> out (RETURN_QUOTIENT_POINTER_REMAINDER)` | div_u64_rem -> quotient (RETURN_VALUE)； div_u64_rem -> rem (POINTER_WRITE)； ns_to_timespec64 -> tv_sec (FIELD_WRITE)； ns_to_timespec64 -> tv_nsec (FIELD_WRITE) | TS113-E053, TS113-E050, TS113-E051, TS113-P14 |

上述三项**全部保持OPEN**，仅属`CORRECTION_SUBMITTED_PENDING_C07_REVIEW`。R110-01/02及R111-01已由C07签为CLOSED_DOCUMENTATION_ONLY，第110批组合PASS_PENDING_LOCAL，不能因本轮重开。111及112仍RETURN，必须由C07独立验收本次补正。

## A：用户态时间结构的入、出边界

按固定源码，用户ABI的`__kernel_timespec_s`包含`__kernel_time64_t tv_sec`（别名long long）和`long long tv_nsec`；内核`timespec64_s`的`tv_sec`为`time64_t=__s64`、`tv_nsec`为`long`（普通类型位置TS113-P05–P08/P23）。本批分析在x86-64的LP64、字段值可表示和有效对象前提下讨论字段传送；**结构确切字节偏移、对齐和大小未经本地编译证明**。复制请求的实参是**`sizeof(kts)`**，不自行固定为16或断言内存布局。

`get_timespec64`（TS113-P01）先声明局部`__kernel_timespec_s kts`和`int ret`，随后一次`ret=copy_from_user(&kts,uts,sizeof(kts))`。若ret非零，立即返回`-EFAULT`（TS113-E022），本函数的`ts->tv_sec`和`ts->tv_nsec`两次赋值**均尚未发生**；若ret为0，先写`ts->tv_sec=kts.tv_sec`（TS113-E023），再写`ts->tv_nsec=kts.tv_nsec`，返回0（TS113-E024）。第285–287行的compat掩码代码全部是注释，不能当作执行路径；第289行的32位模式注释也不足以证明当前确切字节处理。不能从复制函数单个返回值倒推已复制字节数、源缓冲内容、填零、错误原因或页面故障行为。

对照`put_timespec64`，它先在本地一次初始化两个字段`kts.tv_sec=ts->tv_sec`、`kts.tv_nsec=ts->tv_nsec`（TS113-E025），然后仅调用一次`copy_to_user(uts,&kts,sizeof(kts))`，其返回为0则本层返回0，非零则返回`-EFAULT`（TS113-E026）；用户缓冲的部分写入或填充状态仍不可推断。固定真实调用者`clock_gettime`先将clockid映射为kc，空kc返回`-EINVAL`；非空才执行`kc->clock_get_timespec`，**仅该调用返回0**才求值`put_timespec64(&kernel_tp,tp)`，其报告非零导致最终`-EFAULT`（TS113-E027/TS113-E028/TS113-E029）。既有107墙钟资料只在此实际相交调用点作为历史背景，不推断另有完整ABI验证。

## B：基本有效性、strict与settimeofday时间界

三者均仅检查结构字段，不复制用户内存：`timespec64_valid`首先要求秒数非负，再将纳秒强制转换成`ulong`并以`>=NSEC_PER_SEC`拒绝非法纳秒；负的`long tv_nsec`在LP64下转换为巨大无符号数也被拒绝（TS113-E030/TS113-E031）。`timespec64_valid_strict`先调用basic，成功后才额外要求`(ulonglong)tv_sec < KTIME_SEC_MAX`；`timespec64_valid_settod`也先调用basic，成功后要求`(ulonglong)tv_sec < TIME_SETTOD_SEC_MAX`（TS113-E032/TS113-E033/TS113-E034/TS113-E035）。两个附加上界的`>=`都**包含等号**。基础判断失败时，两个上界门均未到达。

由单独宏计算：NSEC_PER_SEC=1,000,000,000；KTIME_SEC_MAX=KTIME_MAX/NSEC_PER_SEC=**9223372036秒**；TIME_UPTIME_SEC_MAX=30×365×24×3600=**946080000秒**；TIME_SETTOD_SEC_MAX=9223372036−946080000=**8277292036秒**（TS113-E036/TS113-E010/TS113-E037/TS113-E038）。这是固定宏的整数表达式推算，不代表真实系统允许设置该时间或该调用链已经发生。

| 模型 | tv_sec | tv_nsec | valid | strict | settod | strict上界门 | settod上界门 |
|---|---:|---:|---|---|---|---|---|
| V01 | 0 | 0 | 真 | 真 | 真 | 到达 | 到达 |
| V02 | 0 | -1 | 假 | 假 | 假 | 未到达 | 未到达 |
| V03 | 0 | 999999999 | 真 | 真 | 真 | 到达 | 到达 |
| V04 | 0 | 1000000000 | 假 | 假 | 假 | 未到达 | 未到达 |
| V05 | -1 | 999999999 | 假 | 假 | 假 | 未到达 | 未到达 |
| V06 | 8277292035 | 0 | 真 | 真 | 真 | 到达 | 到达 |
| V07 | 8277292036 | 0 | 真 | 真 | 假 | 到达 | 到达 |
| V08 | 8277292037 | 0 | 真 | 真 | 假 | 到达 | 到达 |
| V09 | 9223372035 | 0 | 真 | 真 | 假 | 到达 | 到达 |
| V10 | 9223372036 | 0 | 真 | 假 | 假 | 到达 | 到达 |
| V11 | 9223372037 | 0 | 真 | 假 | 假 | 到达 | 到达 |
| V12 | 1 | 7 | 真 | 真 | 真 | 到达 | 到达 |

表中“到达”仅表示先行basic结果为真后才进行该函数的第二门；不是整个调用者是否真的调用这些函数。A的`get_timespec64`/`put_timespec64`完整体未见调用这些验证器；B的`strict→valid`与`settod→valid`才是实际直接调用。

## C：字段比较与进位借位规范化

`timespec64_equal`按`tv_sec`相等再短路检查`tv_nsec`相等（TS113-E039）。`timespec64_compare`先比较秒数：左秒更小直接返回**-1**，更大直接返回**+1**；秒数相同才计算`lhs->tv_nsec-rhs->tv_nsec`并以`int`返回（TS113-E040/TS113-E041/TS113-E042）。若双方纳秒都在0..999999999，差在±999999999范围内，可表示为int；例如(2,7)比(2,0)返回**+7**，反过来是**-7**，不同秒可直接±1。若输入未规范且差值超出long或int范围，不能套用表中的有界结论。

`set_normalized_timespec64`先重复处理nsec≥1e9的进位（减1e9并++sec），再重复处理nsec<0的借位（加1e9并--sec），最后依次写`tv_sec`、`tv_nsec`（TS113-E043/TS113-E044/TS113-E045/TS113-E046）。以(0,1000000000)与(1,0)为例，原始字段相等为假、原始compare按秒门返回-1；将前者规范化到(1,0)后，equal为真、compare变0。另(0,-1)借位成为(-1,999999999)，虽纳秒字段规范，但秒为负，因此B的valid在规范化前后均拒绝。

真实两个调用者：`timespec64_add`按两字段求和，再直接调用`set_normalized_timespec64`，例如(0,999999999)+(0,1)→(1,0)；`timespec64_sub`按两字段做差再调用该函数，例如(1,0)-(0,1)→(0,999999999)，均须限定中间整数可表示（TS113-E047/TS113-E048）。归一化不是用户复制函数隐含的操作，也不能推断C会自动调用B的valid；关系只有源码中真实的直接调用和分别定义的字段条件。

## 证据与交付边界

本批新引文**60个**，全部从固定time逐字取1–5连续原行，每条仅绑定一个真实函数体/宏/CMake定义；26处普通定位与旧批锚分账。事实表保存三项D00、十单宏、七行键、六例两列往返、A抽象复制分支、B完整边界真值表、C有限比较与规范化模型。DOT边分开直接调用、宏展开、数据/写入、控制条件、静态范围注记，Graphviz未运行。所有操作仅作源级/算术核对，完整本地P9、预处理、编译/链接、QEMU、内核、实机、并发及外部代理均**NOT_RUN**。

作者交付状态：READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW；R111-02、R112-01、R112-02均OPEN。交回后由C07 Pro独立核收，不能自行出PASS或关闭三项。
