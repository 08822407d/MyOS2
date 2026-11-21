# MyOS2 第115批｜字符设备标量读写、文件位置与零设备计数

固定源码：`time`@a039d9803ade；C08主线合同164：lead@a5b40470646a；唯一执行父：exec@a2dd370fcf15。身份：MYOS2-A-C07c / MYOS2-LEAD-002-CHAT-EXEC-17。阶段2只读GitHub固定代码并以有限整数推导，**没有运行内核、设备、编译器或Graphviz**。

## D00：第114批两项OPEN的追加纠正

### R114-01 · 映射调用、实际宏定义、x64包装链

冻结第114批作者的数字原样保留：**58段逐字命中、57段符合单一定义P2条件、原列28个普通位置**。唯一重新归类的`FS114-E003`来自`mykernel/arch/x86_64/include/asm/syscalls_64.h:11`，原字面`__SYSCALL(__NR_lseek, sys_lseek)`是**映射宏调用实例**，不是宏定义，因此在本包归`MD115-P01`普通位置。`FS114-E004`才是活动`__SYSCALL`的定义；`FS114-E044`覆盖的是完整`def_chr_fops`聚合初始器，仍满足原定义资格（本包新锚MD115-E002、MD115-E008）。本批新引文自建索引，不改114的旧引文、清单或评论。

沿物理行号把原报告41/55行、facts的`A.dispatch_chain`和DOT第89–91行全部补成以下有类型的链：

| 阶段 | 证据边类型 | 固定出处 | 新ID | 可验证的动作 |
|---:|---|---|---|---|
| 1 | BUILD_DEFINE | mykernel/scripts/options_flags.cmake#L65 | MD115-E001 | -DCONFIG_ARCH_HAS_SYSCALL_WRAPPER, distinct from line66 CONFIG_BUG (FS114-E001) |
| 2 | CONDITIONAL_INCLUDE_POSITION_ONLY | mykernel/include/linux/kernel/syscalls.h#L93–102 | MD115-P02 | #ifdef branch includes asm/syscall_wrapper.h at line101 |
| 3 | MACRO_USE | mykernel/include/linux/kernel/syscalls.h#L1386 | MD115-E004 | MYOS_SYSCALL_DEFINE3 invokes __MYOS_SYSCALL_DEFINEx |
| 4 | MACRO_INVOCATION_POSITION_ONLY | mykernel/arch/x86_64/include/asm/syscalls_64.h#L11 | MD115-P01 | mapping instance of __NR_lseek to sys_lseek |
| 5 | MACRO_DEFINITION | mykernel/arch/x86_64/entry/syscall_64.c#L26 | MD115-E002 | active __SYSCALL expansion into case nr: return __x64_##sym(regs) |
| 6 | ACTIVE_SWITCH_MACRO_USE | mykernel/arch/x86_64/entry/syscall_64.c#L29–31 | MD115-E003 | active switch includes mapping header |
| 7 | GENERATOR_MACRO_USE | mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241–244 | MD115-E005 | FS114-E007 invokes __SYS_STUBx(x64,sys##name) |
| 8 | STUB_DIRECT_CALL_AND_RETURN | mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L105–109 | MD115-E006 | __x64_sys_lseek long stub returns __se_sys_lseek(regs) |
| 9 | SE_TO_DO_DIRECT_CALL_AND_RETURN | mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L245–249 | MD115-E007 | FS114-E008 __se_sys_lseek calls __do_sys_lseek then returns long |

尤其，固定C配置中的`-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER`来自**options_flags.cmake:65**（MD115-E001），`syscalls.h:93–102`按配置包含asm/syscall_wrapper.h（MD115-P02）；原`FS114-E001`只取**第66行CONFIG_BUG**，不能代替包装器开关。`MYOS_SYSCALL_DEFINE3`调用`__MYOS_SYSCALL_DEFINEx`（MD115-E004），生成宏调用`__SYS_STUBx`（MD115-E005），实际x64桩在105–109行`return __se_##name(regs);`（MD115-E006），`__se`在245–249行转调`__do`并返回long（MD115-E007）。这证明固定配置下的源码宏链；没有预处理/编译结果。原图映射边只能标`MACRO_INVOCATION_POSITION_ONLY`，后续活动switch/生成器/桩分别标宏展开、直接调用及返回，不能混成一个直接调用。

### R114-02 · fd计数读取与FAT识别case的界限

原114 `A.fd_gate.no_pointer_dereference_before_return=true`范围过宽。`lseek`体先取得`curr=current`，为了判断`fd>=curr->files->fd_count`必须**读取`curr->files->fd_count`**。当上界命中，立即返回`-EBADF`；但**尚未**访问`fd_array[fd]`、文件的`f_op`、`llseek`方法或任何`f_pos`写入。此修订只针对原事实第389行和报告43行，完整体在MD115-P07、旧FS114-E010，不能写成“返回前完全无指针解引用”。

原114 `A.whence_gate.values`中的三个FAT`PASS`仅表示`FAT32_lseek`的switch识别`SEEK_SET`、`SEEK_CUR`、`SEEK_END`。随后仍须计算候选pos，在可表示前提下检查`0≤pos≤i_size`，只有通过才写`f_pos`和返回pos；不符合范围则`-EOVERFLOW`且不写位（固定myos_fat32.c:283–308，MD115-P08）。因此同一case类型不能说明所有输入最终成功：

| 来源模式 | FAT分支 | 修订后的含义 | 已核114具体例子（返回／写回） |
|---|---|---|---|
| SEEK_SET (0) | FAT识别case | CASE_ONLY_NOT_FINAL_PASS | B01:0/写；B06:-EOVERFLOW/不写；B10:100/写；B11:-EOVERFLOW/不写 |
| SEEK_CUR (1) | FAT识别case | CASE_ONLY_NOT_FINAL_PASS | B02:45/写；B03:0/写；B07:-EOVERFLOW/不写 |
| SEEK_END (2) | FAT识别case | CASE_ONLY_NOT_FINAL_PASS | B04:100/写；B05:-EOVERFLOW/不写 |
| SEEK_DATA (3) | 未作为FAT case接受 | ENTRY_ALLOWED_FAT_DEFAULT_-EINVAL | B08:-EINVAL/不写 |
| SEEK_HOLE (4) | 未作为FAT case接受 | ENTRY_REJECTED_FAT_NOT_INVOKED | B09:-EINVAL/不写 |

以冻结的`i_size=100、旧f_pos=40`为前提：B04(END+0)及B10(SET=100)在**等号**处成功并写100；B05/B06/B07/B11虽然SET/CUR/END已经识别，仍因候选超界而保留40并返回`-EOVERFLOW`。B08的DATA=3经入口，在FAT后端default被拒；B09的HOLE=4在入口先被拒。旧114 A6/B11/C5结果及两种拒绝层次不变。

上述两项仅属**纠正已提交、待C08独立核收**。R114-01/R114-02继续`OPEN`，第114批仍`RETURN`；第112、113批已由C07签文档闭合、组合`PASS_PENDING_LOCAL`，不重开。

## A · 普通标量read/write的模式、方法和文件位置

固定`MYOS_SYSCALL_DEFINE3(read)`真实转`ksys_read`（MD115-E009），由**`myos_fdget_pos(fd)`**从当前fd_array取file（MD115-E011）；固定`MYOS_SYSCALL_DEFINE3(write)`真实转`ksys_write`（MD115-E010），此路径用**`fdget_pos(fd)`**，下层`__to_fd`同样取当前fd_array（MD115-E012、MD115-E013）。这两种拼写和fd对象flags不相同，不在本包擅自等价实现。以下只分析有效file、不碰未经验证的fd安全或真实调度。

`file_ppos`根据`FMODE_STREAM`选择`NULL`或`&file->f_pos`（MD115-E014）；非流时`ksys_read/ksys_write`先将原位置复制到本地`loff_t pos`，将`&pos`传给VFS。**外层只要`ret>=0`且ppos非空，就执行`file->f_pos=pos`**（MD115-E016、MD115-E017、MD115-E020、MD115-E021）。这既包括返回零时的同值写回，也包括回调改变局部pos后返回零的真正改位。负值则外层不提交局部变化；流文件传NULL则没有本次外层写回。不能把“执行了赋值”混为“字段数值发生变化”。

读入口`vfs_read`首先要求`FMODE_READ`，否则`-EBADF`，`FMODE_CAN_READ`的代码**处于注释**；然后核`access_ok`，失败`-EFAULT`，调用`rw_verify_area`（当前函数第一条可执行语句直接返回0），再对`MAX_RW_COUNT`截断；方法优先序是**`.read_iter`通过new_sync_read，然后`.read`，都缺失则`-EINVAL`**（MD115-E023、MD115-E024、MD115-E033、MD115-E025、MD115-E026、MD115-E027）。

写入口`vfs_write`先要求`FMODE_WRITE`，否则`-EBADF`，还必须有`FMODE_CAN_WRITE`，否则`-EINVAL`；之后同样`access_ok`、`rw_verify_area`和MAX_RW_COUNT截断。写方法却是**先`.write`、后`.write_iter`、两者都没有则`-EINVAL`**（MD115-E028、MD115-E029、MD115-E030、MD115-E031、MD115-E032）。迭代读助手即使返回非正也会在ppos非空时读回`kiocb.ki_pos`（最终负值外层不提交）；迭代写助手只有ret>0才把该位置读回本地ppos（MD115-E034、MD115-E035）。当前read_zero/read_null/write_null均是标量方法，没有这两个迭代方法。

在固定x86-64的PAGE_SHIFT12下，`PAGE_SIZE=4096`，`MAX_RW_COUNT=INT_MAX&PAGE_MASK=0x7ffff000=2147479552`（MD115-E048、MD115-E049、MD115-E043）。当前`rw_verify_area`是进入函数后立即return0，不能因其名称而声称完整校验已经实施。若请求超过截断上界，实际回调获取的是VFS缩减后的长度n；本批七例远低于上界。

常规打开建file时，`__alloc_file`实际`f_mode=OPEN_FMODE(flags)`（MD115-E041），该宏按`(flags+1)&O_ACCMODE`形成权限位（MD115-E040）；源中O_RDONLY=0、O_WRONLY=1、O_RDWR=2分别形成`FMODE_READ=0x1`、`FMODE_WRITE=0x2`、两者合计0x3。**这一表达式没有给普通O_WRONLY自动添加`FMODE_CAN_WRITE=0x40000`**；后续其他位置是否设置该位属于未证事项。因此写侧必须分三个状态：READ-only时`-EBADF`，普通O_WRONLY产生WRITE但未证CAN_WRITE时`-EINVAL`，显式假定文件同时具备WRITE与CAN_WRITE才进入设备方法（MD115-E045、MD115-E046、MD115-E047、MD115-E038）。

以下是在普通有效对象、初始`f_pos=9`、没有并发和没有回调直接改file字段的**有限条件模型**。`回调后的局部pos`指传入`*ppos`可见的值，不是直接写回file：

| 例 | 操作 | 回调返回 | 回调后局部pos | ksys赋值f_pos | 最终f_pos | 数值改变 |
|---|---|---:|---:|---|---:|---|
| A01 | read | 5 | 14 | 是 | 14 | 是 |
| A02 | read | 0 | 9 | 是 | 9 | 否 |
| A03 | read | -5 | 14 | 否 | 9 | 否 |
| A04 | read | 0 | 14 | 是 | 14 | 是 |
| A05 | write | 4 | 13 | 是 | 13 | 是 |
| A06 | write | 0 | 9 | 是 | 9 | 否 |
| A07 | write | -22 | 17 | 否 | 9 | 否 |
| A08 | read | 5 | NULL | 否 | 9 | 否 |
| A09 | write | 5 | NULL | 否 | 9 | 否 |

对于FMODE_STREAM，表内ppos是NULL，因此没有局部pos模型或外层提交；实际某设备回调若自行改file->f_pos，本包没有证据把它等同于这些外层赋值。

## B · /dev/zero的最后一块：实际清零字节与返回字节分离

限定一个对所有写入都可访问、初始前n字节非零、在`buf[n]`设哨兵的普通缓冲区。`n`表示VFS截断之后传给`read_zero`的长度；没有尝试任何真实用户指针。源码先初始化`cleared=0`，然后每轮取`chunk=min_t(size_t,count,PAGE_SIZE)`，从`buf+cleared`清零chunk字节，`count-=chunk`，如果剩余为0立即`break`；**只有没有break时才执行`cleared+=chunk`**，函数最后返回`cleared`（MD115-E051、MD115-E052、MD115-E053、MD115-E054、MD115-E055、MD115-E056）。

由循环顺序在`P=4096`下推出：**n=0时零次循环，返回0且不写缓冲；n>0时清零全部n字节`[0,n)`，但返回`P×floor((n−1)/P)`，遗漏最后一块（长度1～P）的计数。** 这不是只遗漏“不满一页”的残块，恰好整页的最后一块同样不累计。因此n=P返回0、n=2P返回P。`buf[n]`哨兵始终不在`memset`覆盖范围，前提下保持原值。

| 模型 | 输入n | 实际清零 | 返回计数 | 漏计末块 | 循环轮数 | 哨兵 | 最终f_pos |
|---|---:|---:|---:|---:|---:|---|---:|
| B01 | 0 | 0 | 0 | undefined | 0 | undefined | 9 |
| B02 | 1 | 1 | 0 | undefined | 1 | undefined | 9 |
| B03 | 4095 | 4095 | 0 | undefined | 1 | undefined | 9 |
| B04 | 4096 | 4096 | 0 | undefined | 1 | undefined | 9 |
| B05 | 4097 | 4097 | 4096 | undefined | 2 | undefined | 9 |
| B06 | 8192 | 8192 | 4096 | undefined | 2 | undefined | 9 |
| B07 | 8193 | 8193 | 8192 | undefined | 3 | undefined | 9 |

指定的P及P+1逐轮如下；注意每一轮的memset发生**在**判断break之前：

| 初始n | 第几轮 | memset起点 | chunk | 剩余count前→后 | break与累计动作 | 最后cleared |
|---:|---:|---:|---:|---|---|---:|
| 4096 | 1 | 0 | 4096 | 4096 → undefined | 是，跳过累计 | 0 |
| 4097 | 1 | 0 | 4096 | 4097 → undefined | 否，继续累计 | 4096 |
| 4097 | 2 | 4096 | 1 | 1 → undefined | 是，跳过累计 | 4096 |

额外用n=2P=8192检验最后块恰一整页：第1轮清[0,4096)、累计4096；第2轮清[4096,8192)、count变0并提前break，最终只报4096。正长度请求返回0（n在1～4096）时，调用者**从返回值只能看到0字节**，尽管条件性内存模型显示缓冲已清零n字节，形成EOF样式计数与缓冲副作用不一致。`read_zero`没有更新ppos，普通非stream调用的`ksys_read`因ret>=0仍会把未变的本地位置9重新赋给`f_pos`；最终数值为9。源码不支持凭此声称实机实际访问已成功。

## C · 三设备×标量读写：同一长度、不同结果

`mem.c`的devlist把minor3的`null`、minor5的`zero`、minor7的`full`分别关联`null_fops`、`zero_fops`、`full_fops`；三者mode都是**0666**，但devlist.fmode依次是`FMODE_NOWAIT`、`FMODE_NOWAIT`、0。**设备的0666权限、devlist.fmode及普通已打开file.f_mode并不是一个字段**，不能拿0666或NOWAIT推断`FMODE_CAN_WRITE`。初始化中按数组取memdev、用`memdev.fops`调用`cdev_init`、调用`cdev_add`，成功分支请求`myos_device_create`（MD115-E063、MD115-E064、MD115-E065、MD115-E066、MD115-E067）。这证明代码怎样尝试注册，不证明设备已注册、路径可访问或打开成功。

打开时file的`f_op`从inode.i_fop取，`chrdev_open`又能把它设为`p->ops`（MD115-E042、MD115-E068、MD115-E069）。活动`null_fops`含`read_null`和`write_null`；它们分别直接返回0与count而不改变缓冲或ppos（MD115-E061、MD115-E057、MD115-E058）。活动`zero_fops`含`read_zero`和`write_zero`，`write_zero`在源码44及113行重复定义为`write_null`的别名，两处一致（MD115-E062、MD115-E059、MD115-E060）。`full_fops`中的Linux风格读写方法名都只在注释中，当前**无活动read/read_iter/write/write_iter**（MD115-P34），不能臆造write_full=ENOSPC。

六格使用共同请求**4097字节**、初始f_pos=9、有效file/缓冲和相应准入位；读使用FMODE_READ，写格**明确假设额外具有FMODE_WRITE|FMODE_CAN_WRITE**，不冒称来自普通O_WRONLY：

| 设备 | 操作 | f_mode前提 | 回调 | 返回值 | 缓冲区变化 | 位置提交 | 最终f_pos |
|---|---|---|---|---|---|---|---:|
| null | read | FMODE_READ (0x1) | undefined | 0 | unchanged | 同值写回9 | 9 |
| null | write | FMODE_WRITE|FMODE_CAN_WRITE (0x40002), explicit hypothetical mode | undefined | 4097 | unchanged | 同值写回9 | 9 |
| zero | read | FMODE_READ (0x1) | undefined | 4096 | zeroed 4097 requested bytes; sentinel at 4097 untouched | 同值写回9 | 9 |
| zero | write | FMODE_WRITE|FMODE_CAN_WRITE (0x40002), explicit hypothetical mode | undefined | 4097 | unchanged | 同值写回9 | 9 |
| full | read | FMODE_READ (0x1) | undefined | -EINVAL | unchanged | 不写回 | 9 |
| full | write | FMODE_WRITE|FMODE_CAN_WRITE (0x40002), explicit hypothetical mode | undefined | -EINVAL | unchanged | 不写回 | 9 |

在同一写请求下再单独比较准入门：只读file先由FMODE_WRITE门以`-EBADF`拒绝，普通O_WRONLY建file所得WRITE但未证CAN_WRITE，则由第二门以`-EINVAL`拒绝，**即使设备存在write_null也到不了它**；显式WRITE|CAN_WRITE条件下，null/zero走write_null各报告4097且丢弃数据，full因方法缺席返回`-EINVAL`。full前层CAN_WRITE失败与后层方法缺席虽然可能都出现同一个`-EINVAL`，但位置和调用路径的证据不同。

六格中read_null返回0、read_zero返回4096、full读取返回- EINVAL（即`-EINVAL`）；三格在普通非流文件前提下，前两种非负返回导致ksys_read对f_pos执行**同值写回9**，full负返回不提交。write_null和write_zero成功返回4097且不修改调用者源缓冲，ksys_write同值写回9；full后端缺方法为负值，无写回。实际设备可能未被注册，故不推断真实用户程序已经观察到这些行为。

## 限定与交回

本批70个独立新源码窗口来自16份固定time文件；均为1–5行连续原文，70/70逐字与单一定义所属预检，35个普通类型/宏调用/完整函数位置单独存账。旧114的58次文字匹配、57次定义合格与28个普通位置保持冻结。事实表含A九个位置模型、五个模式门反例、B七个长度及逐轮轨迹、C六格与四个写准入对照，所有数值均为纸面计算。完整本地P9、预处理/编译/链接、仓库脚本、Graphviz、QEMU/内核、实机注册/读写、并发均**NOT_RUN**。

交付作者终态`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，`D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW`；`R114-01`和`R114-02`仍`OPEN`、第114批仍`RETURN`，仅C08 Pro可独立核收。
