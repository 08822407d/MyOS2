# 第114批｜lseek的文件位置语义及两项追加纠正

执行者：MYOS2-A-C07c / MYOS2-LEAD-002-CHAT-EXEC-17；合同CORE-USER-FILE-SEEK-POSITION-114；源码固定time@a039d9803ade，主线lead@41b2c2f955c6、执行唯一父41697d76ae5e。阶段2静态分析，所有运行、编译和完整本地P9均NOT_RUN。

## D00｜R112-02、R113-01文档差量（OPEN，不自行关闭）

### R112-02｜五个非零模型的局部前提和两条旧图边

冻结113的 `D00.R112_02.roundtrip.models` 中C02/C04正输入误携“negative q needs signed conversion”的共用注记；原六例与12个回转整数已经C07核收。本批只逐行校正前提，冻结数值不变。

| 模型 | 输入ns | 分支 | q/rem | 秒、纳秒 | 经ktime回值 | 经to_ns回值 | 只在本行适用的前提 |
|---|---:|---|---|---|---:|---:|---|
| C02 | 1 | positive | 0/1 | (0,1) | 1 | 1 | 仅指针前提 |
| C03 | -1 | negative | 0/0 | (-1,999999999) | -1 | -1 | 指针＋u64商转signed秒 |
| C04 | 1000000000 | positive | 1/0 | (1,0) | 1000000000 | 1000000000 | 仅指针前提 |
| C05 | -1000000000 | negative | 0/999999999 | (-1,0) | -1000000000 | -1000000000 | 指针＋u64商转signed秒 |
| C06 | -1000000001 | negative | 1/0 | (-2,999999999) | -1000000001 | -1000000001 | 指针＋u64商转signed秒 |

所有非零路径都有 `s32 rem` 的地址作为实参、`u32*`作为`div_u64_rem`形参的类型不匹配（FS114-E048 / FS114-E052、FS114-P22）。正输入C02/C04仅声明辅助函数能够按本例写回非负余数；**不需要负商转换**。负输入C03/C05/C06须额外保证有符号内层`-nsec-1`可表示，且辅助函数的**u64商**q在外层`-q-1`以模2^64计算：q=0得到18446744073709551615，q=1得到18446744073709551614。该值存入有符号time64_t产生-1/-2，依赖目标实现定义的转换约定。最小s64的内层一元负号不安全，不推断运行结果（FS114-E050）。C01零输入直接返回初始化结构{0,0}，不调用除法（FS114-E048 / FS114-E051）。

旧113图114行将`ns_to_timespec64 -> negative`的条件证据错连到赋值行64–65；新图将 `ns_to_timespec64 -> nsec_negative_condition` 固定回`time_misc.c:58`（FS114-E047），再在条件为真时连到`negative_arithmetic`（FS114-E050）。旧图122行 `zero -> fields`混淆零返回和非零字段写入；新图`zero_initialized_pair -> timespec_return`引用52及68行，正/负分支各单独连到`nonzero_field_writes`。112历史八条图边修订保留，不改旧件。

### R113-01｜短路求值、C04–C06输出引文、CAND-07

113图137行把读取回调结果进入`!error`标作“CONTROL_ONLY_ON_SUCCESS”，不符合求值顺序：`kc->clock_get_timespec`正常返回后**总要先求值 `!error`**，仅当其为真时才对`put_timespec64`求值。修订边是`clock_result -> not_error_test [RETURN_VALUE_DATA_TO_TEST]`及`not_error_test -> put [SHORT_CIRCUIT_TRUE]`，旧图138行的直接有条件调用`clock_gettime -> put`仍成立（FS114-E053 / FS114-E054）。

113事实`ABC.C.bounded_examples`按数组里的**实际id**定位；C04和C05报告equal=false却只连秒数比较体，分别补相等函数的TS113-E039 / FS114-E055；C06的规范后compare=0应另连同秒纳秒差返回TS113-E042 / FS114-E058，原数值不变。

| 原模型id | 已核的输出 | 追加的独立引文 |
|---|---|---|
| C04 | equal=false, compare=-1 | TS113-E039 / FS114-E055 |
| C05 | equal=false, compare=+1 | TS113-E039 / FS114-E055 |
| C06 | 规范后compare=0 | TS113-E042 / FS114-E058 |

原CAND-07笼统写“overflow long/int”，应拆为**两个不同问题**：若纳秒同秒相减的数学差超出signed long范围（如LONG_MAX−(−1)），减法本身发生有符号溢出；若差能用long表示但超出返回int值域（如2147483648−0），则是long转int的实现定义结果／实现定义信号，不能称作long溢出。按x86-64 LP64且int32的前提，规范纳秒0..999999999之差在±999999999内，两者都安全。不同秒数直接返回±1，不执行纳秒减法（FS114-E056 / FS114-E057 / FS114-E058）。

**当前验收权属**：R112-02、R113-01仍OPEN；111组合PASS_PENDING_LOCAL，112与113仍RETURN，已闭合R111-02/R112-01不重开。所有差量仅在本114六件，旧112/113事实、图、评论保持不变。

## A｜系统调用入口接受哪些whence、谁返回什么

`__NR_lseek=8`（FS114-E002），映射`__SYSCALL(__NR_lseek,sys_lseek)`（FS114-E003）通过活动`x64_sys_call`中包含的映射展开成分派（FS114-E004 / FS114-E005）。`MYOS_SYSCALL_DEFINE3(lseek, unsigned int fd, off_t offset, unsigned int whence)`在`fs/syscall.c:75–91`；`fd`、`whence`无符号，`off_t=__kernel_off_t=long`，本地`ulong ret=0`（FS114-E009、FS114-P01/P03/P04）。

入口门`fd<0 || fd>=fd_count`的前半对unsigned fd恒假，后半含等号；失败直接`-EBADF`（FS114-E010）。`whence<0 || whence>=SEEK_MAX`前半也恒假，后半含等号（FS114-E011）。`SEEK_MAX`是`SEEK_HOLE=4`（FS114-E020），所以合法域仅0～3：

| whence | 数字 | 入口 | FAT32后端 |
|---|---:|---|---|
| SEEK_SET | 0 | PASS | PASS |
| SEEK_CUR | 1 | PASS | PASS |
| SEEK_END | 2 | PASS | PASS |
| SEEK_DATA | 3 | PASS | DEFAULT_-EINVAL |
| SEEK_HOLE | 4 | REJECT_-EINVAL | NOT_CALLED |

即DATA(3)可进入方法，但FAT32只处理SET/CUR/END而走default；HOLE(4)被**入口**拒绝，没有到FAT的机会。入口从`current->files->fd_array[fd]`取fp；以下以非空、有效file为前提，因为该函数没有独立对fp做空指针检查（FS114-E012）。只在`fp->f_op && fp->f_op->llseek`时调用方法，把loff_t结果存入ulong ret，否则返回未改的**0**且没有本体f_pos写入（FS114-E013 / FS114-E014）。

文件方法原型为`loff_t (*llseek)(file_s*,loff_t,int)`，源码中loff_t来自long long；系统调用包装宏则以long从`__do_sys`、`__se_sys`及x64 stub返回（FS114-E007 / FS114-E008、FS114-P05/P06/P09）。在LP64小整数模型中off_t与loff_t均为64位有符号，但**负errno跨loff_t→ulong→long时**，第一步按模2^64转换，第二步超出signed long表示域属于实现定义转换；只有明确二补码目标解释才能把负错误码视为原样返回。没有对实际编译的判断。方法缺席不能从返回0推导`f_pos=0`。

## B｜FAT32三个基准、后端拒绝与唯一写回

普通非目录inode分支给`i_size=de->size`，`i_fop=&FAT32_file_ops`（FS114-E021 / FS114-E022）；打开处理赋`file->f_op=inode->i_fop`（FS114-E023）；方法表 `.llseek=FAT32_lseek`（FS114-E024）。这仅证明源码绑定，不冒称已实际成功打开文件。

`FAT32_lseek`的局部候选是signed long `pos`（FS114-E025）；SET用offset、CUR用原f_pos+offset、END用inode.i_size+offset（FS114-E026 / FS114-E027 / FS114-E028）。未列origin走default`-EINVAL`（FS114-E029）。候选**先做运算**，之后才用`pos<0 || pos>i_size`判拒绝（FS114-E030）：通过才执行唯一`filp->f_pos=pos`并返回pos（FS114-E031 / FS114-E032）；`pos==i_size`恰好EOF合法，越过EOF不被接受。极端值的有符号加法溢出不能靠后面的范围检查补救。

以下11例**每一行独立初始化** `i_size=100`、`f_pos=40`；0/100边界和错误均在可表示的小整数域计算：

| 模型 | whence | offset | 入口 | 后端 | 结果 | f_pos | 最终位置 |
|---|---|---:|---|---|---|---|---:|
| B01 | SET | 0 | ACCEPT | SET | 0 | 写 | 0 |
| B02 | CUR | 5 | ACCEPT | CUR | 45 | 写 | 45 |
| B03 | CUR | -40 | ACCEPT | CUR | 0 | 写 | 0 |
| B04 | END | 0 | ACCEPT | END | 100 | 写 | 100 |
| B05 | END | 1 | ACCEPT | END | -EOVERFLOW | 不写 | 40 |
| B06 | SET | -1 | ACCEPT | SET | -EOVERFLOW | 不写 | 40 |
| B07 | CUR | -41 | ACCEPT | CUR | -EOVERFLOW | 不写 | 40 |
| B08 | DATA | 0 | ACCEPT | DEFAULT | -EINVAL | 不写 | 40 |
| B09 | HOLE | 0 | REJECT_-EINVAL | NOT_REACHED | -EINVAL | 不写 | 40 |
| B10 | SET | 100 | ACCEPT | SET | 100 | 写 | 100 |
| B11 | SET | 101 | ACCEPT | SET | -EOVERFLOW | 不写 | 40 |

B05/B06/B07/B11分别越界，未写回，最终仍40；B08 DATA(3)入口通过但后端default返回- EINVAL（即`-EINVAL`）；B09 HOLE(4)入口拒绝，后端根本未调用；B04/B10正好指向EOF，写为100。以上错误返回值是在入口/后端源码中的**有符号局部返回**，外部可见值需遵守A的转换前提。

## C｜null、zero、noop及没有方法不是同一种行为

`mem.c`在42及111行各有独立`zero_lseek→null_lseek`宏（FS114-E033 / FS114-E036）；`null_lseek`唯一函数体`return file->f_pos = 0`，**写入0并返回0**（FS114-E035）。`null_fops`和`zero_fops`分别有活动`.llseek`表项（FS114-E038 / FS114-E039）；devlist表明minor3 null、minor5 zero、minor7 full（FS114-E040 / FS114-E041 / FS114-E042）。`full_lseek`虽在43及112行也有别名（FS114-E034 / FS114-E037），但`full_fops.llseek`是**注释**，不能冒称它会清零（FS114-P16）。

`noop_llseek`完整体只`return file->f_pos;`，没有写位（FS114-E043，FS114-P18）。`def_chr_fops`确实把`.llseek=noop_llseek`（FS114-E044），不过`chrdev_open`可以把`filp->f_op`替换为设备`p->ops`（FS114-E045），所以不能据此声称实际设备打开后仍为noop。`chr_dev_init`源码调用注册和建设设备节点（FS114-E046），不是设备在真实系统已注册、打开成功的证据。

统一假定有效文件、入口放行`SEEK_CUR(1)`、offset=2、原始f_pos=9且无并发影响，按各自方法表得到：

| 模型 | 操作表 | 回调 | 返回 | 写位置 | 最终f_pos |
|---|---|---|---:|---|---:|
| C01 | null_fops | null_lseek | 0 | 写 | 0 |
| C02 | zero_fops | zero_lseek->null_lseek | 0 | 写 | 0 |
| C03 | def_chr_fops | noop_llseek | 9 | 不写 | 9 |
| C04 | full_fops | absent (.llseek commented) | 0 | 不写 | 9 |
| C05 | NULL | absent | 0 | 不写 | 9 |

**相同返回0能够对应不同位置。** null/zero归零回调写0且返回0；`full_fops`无有效llseek时和`f_op`缺席时，入口保留初始ret=0但f_pos依旧9；noop有方法但不改变位置，返回9。不能以一个`lseek`成功样式的数值断言执行了定位，也不能推断本项目已实际提供这些设备。

## 检查范围与交回

六件共用新证据索引：58个源码短引文，来自18份固定time源码，窗口1–5连续原行，逐字及单一定义所属58/58；28个类型、声明、整函数与注释范围只列POSITION_ONLY。facts记录D00两项、5个局部前提、2+1条旧图修订、A六例、B十一例、C五例及对应入参/返回/f_pos写回。Graphviz、完整本地P9、源码预处理/编译/链接、仓库程序/脚本、内核/QEMU、实机、设备运行与并发一概NOT_RUN。

作者状态：READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW。R112-02/R113-01继续OPEN待C07 Pro审查；本批完成不等于主线PASS。
