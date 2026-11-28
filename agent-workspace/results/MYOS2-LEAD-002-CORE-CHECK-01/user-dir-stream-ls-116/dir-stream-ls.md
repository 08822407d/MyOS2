# 第116批｜`ls` 目录流建立、缓存消费与收尾

执行身份：MYOS2-A-C07c / MYOS2-LEAD-002-CHAT-EXEC-17。任务CORE-USER-DIR-STREAM-LS-116；固定`time`源码短值a039d9803ade、165发布主线e96b1079ff0d、唯一执行父452444a1c4b4。阶段2，纯静态源码/纸面模型；没有编译或运行MyOS2。

## D00：第115批三项文档退回的追加补正

### R115-01：三张完整替代表，修复旧23个缺失单元格

旧115报告第83–89、95–97、111–116行的三表中，分别缺漏**七个`final_chunk_not_counted`、七个`sentinel_byte`、三个`loop_steps[].remaining_after_subtract`、六个`C.six_cells[].method`**，合计23格。本批从固定115 facts所对应真实键逐格重建，不按占位符猜值。旧A9／B7／C6有界返回及位置结果已由C08验收，保持不变（旧115模型、DS116-E013、DS116-E014）。

替代表一：每行独立长度n，哨兵来自原事实表且在n位置，初始f_pos=9：

| 旧B模型 | n | 实际清零 | 报告字节 | 最后未累计 | 轮数 | 哨兵 | 最终f_pos |
|---|---:|---:|---:|---:|---:|---|---:|
| B01 | 0 | 0 | 0 | 0 | 0 | unchanged | 9 |
| B02 | 1 | 1 | 0 | 1 | 1 | unchanged | 9 |
| B03 | 4095 | 4095 | 0 | 4095 | 1 | unchanged | 9 |
| B04 | 4096 | 4096 | 0 | 4096 | 1 | unchanged | 9 |
| B05 | 4097 | 4097 | 4096 | 1 | 2 | unchanged | 9 |
| B06 | 8192 | 8192 | 4096 | 4096 | 2 | unchanged | 9 |
| B07 | 8193 | 8193 | 8192 | 1 | 3 | unchanged | 9 |

替代表二：原`remaining_after_subtract`的三处空缺为**0、1、0**：

| 原请求n | 轮次 | memset起点 | chunk | 扣减后剩余 | 本轮break | 累计cleared |
|---:|---:|---:|---:|---:|---|---:|
| 4096 | 1 | 0 | 4096 | 0 | 是 | 0 |
| 4097 | 1 | 0 | 4096 | 1 | 否 | 4096 |
| 4097 | 2 | 4096 | 1 | 0 | 是 | 4096 |

替代表三：原`C.six_cells[].method`真实回调／方法缺席。模式单元格统一使用“WRITE＋CAN_WRITE”，不在单元内使用未经转义的竖线导致列错位：

| 模型 | 设备 | 操作 | 条件模式 | 回调／缺项 | 返回 | 缓冲效果 | 外层提交 | 最终位置 |
|---|---|---|---|---|---|---|---|---:|
| C_NULL_READ | null | read | READ（0x1） | read_null | 0 | unchanged | 是 | 9 |
| C_NULL_WRITE | null | write | WRITE＋CAN_WRITE（0x40002，显式条件） | write_null | 4097 | unchanged | 是 | 9 |
| C_ZERO_READ | zero | read | READ（0x1） | read_zero | 4096 | zeroed 4097 requested bytes; sentinel at 4097 untouched | 是 | 9 |
| C_ZERO_WRITE | zero | write | WRITE＋CAN_WRITE（0x40002，显式条件） | write_zero → write_null | 4097 | unchanged | 是 | 9 |
| C_FULL_READ | full | read | READ（0x1） | 未提供read/read_iter | -EINVAL | unchanged | 否 | 9 |
| C_FULL_WRITE | full | write | WRITE＋CAN_WRITE（0x40002，显式条件） | 未提供write/write_iter | -EINVAL | unchanged | 否 | 9 |

三表每行的单元格数量须分别严格是8、7、9（含首列），所有23个待补值已具实体来源；不修改冻结的第115批正文和facts。

### R115-02：旧证据归属及五处机器字段的分动作引文

按C08裁定保留旧115原账：**70个逐字短窗、69个旧符号标签的真实定义归属合格、六件全文含72次旧标签前缀命中**；超出的2次为MANIFEST:116和facts:32历史说明，不是另外两条源引文。原E001的字面CMake选项正确，错在把定义符号写成`CMake_C_FLAGS`；正确名称是`set(CMAKE_C_FLAGS)`，起始在第58行，包装配置在第65行。本批以独立引文DS116-E001和DS116-E002支撑，不改旧标签文本。

另将五个旧facts消费者由泛化`source`拆为每个动词的真实证据：

| 原键 | 追加的源级动作及正确证据ID |
|---|---|
| A.read_VFS.source | mode: MD115-E023；mode_full_body: MD115-P10；access: MD115-E024；verify: MD115-E033；limit: MD115-E025；dispatch: MD115-E026/MD115-E027 |
| A.write_VFS.source | mode: MD115-E028；mode_full_body: MD115-P11；access: MD115-E029；verify: MD115-E033；limit: MD115-E030；dispatch: MD115-E031/MD115-E032 |
| A.MAX_RW_COUNT | definition: MD115-E043；page_shift: MD115-E048；page_size: MD115-E049；page_mask: DS116-E004；int_max: DS116-E003 |
| C.operations.zero | loop: MD115-E051/MD115-E052；writes: MD115-E053；break: MD115-E054；accumulate: MD115-E055；return: MD115-E056；write_alias: MD115-E059/MD115-E060；write_callback: MD115-E058 |
| C.installation | copy_devlist: MD115-P27；cdev_init: MD115-E066；cdev_add: MD115-E067；method_from_inode: MD115-E042；ops_lookup: MD115-E068；ops_swap: MD115-E069 |

具体说，`A.read_VFS`的FMODE_READ门来自旧MD115-E023（及完整体P10），校验立即返回0来自E033，长度截断来自E025；`A.write_VFS`的双模式门来自旧E028（及P11），E030是验证／截断而非权限门。`MAX_RW_COUNT=INT_MAX&PAGE_MASK`还需固定`INT_MAX`第30行（DS116-E003）和`PAGE_MASK`第11行（DS116-E004）与旧E043/PAGE_SHIFT合读，32位int前提得2147479552。`C.operations.zero`须回到旧E051–E056完整循环而非仅E056返回。`C.installation`既需要devlist读取的旧普通位置P27，又要E066/E067的cdev_init/cdev_add，换表另连E042/E068/E069。所有追加只记本批独立ID；旧70/69/72保持冻结。

### R115-03：实际ksys调用主体、零次循环与同步迭代特殊结果

旧115图83、84–89、97、107、109–120和132的定点差异在新图和facts逐条登记：原`read → myos_fdget_pos`不能视作直接调用，正确是`read_syscall → ksys_read → myos_fdget_pos/file_ppos/vfs_read`；写侧是`write_syscall → ksys_write → fdget_pos/file_ppos/vfs_write`（DS116-E008、DS116-E009）。活动switch到桩须区分宏定义旧MD115-E002和映射调用的普通位置旧P01。VFS才负责检查后的`MAX_RW_COUNT`截断（DS116-E010、DS116-E011），阈值旧E043以及本批INT_MAX/PAGE_MASK相互独立。f_pos真实写回发生在ksys内，必须引E017/E021和本批DS116-E008／DS116-E009，不能只有局部pos采集的E016/E020。

旧图对read_zero只画非空循环，本批补出`n=0 → return cleared0`不取chunk不调用memset；`n>0`则每轮先memset再扣减，若剩余0直接break再return，否则累计后继续。归纳模型下n字节均清零，不意味着全n字节被计入返回。表绑定`zero_fops.read=read_zero`与该函数体为**同一个实现节点**，不添加虚假的自调用边（DS116-E012、DS116-E013、DS116-E014）。

同步迭代助手的前提也需纠正。`new_sync_read`在`read_iter`返回后，**先执行`BUG_ON(ret == -EIOCBQUEUED)`，正常越过此门后**只要ppos非空就将kiocb.ki_pos写入局部ppos；即使其它普通负返回也可能修改局部，但外层ret<0不提交file.f_pos。`new_sync_write`同样先执行BUG_ON，再要求ret>0且ppos非空才写局部（DS116-E006、DS116-E007）；排队特例由DS116-E005和固定`EIOCBQUEUED=529`限制，不能概括成普通负值并声称继续执行。本批有关目录流不走这两个同步迭代助手，原115 A9/B7/C6值保留。

以上R115-01/02/03均只**提交纠正，仍为OPEN**，D00=`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，第115批仍`RETURN`；旧114已在C08文档闭合、`PASS_PENDING_LOCAL`。

## A：shell `ls`如何建立目录流

`shell_internal_cmd`中`ls`条目绑定`ls_command`，调度函数`run_cmd`经函数指针调用（DS116-E015、DS116-E016）。`argc<=1`时路径设为`.`，否则取`argv[1]`（DS116-E017）。`opendir`使用实际标志`O_RDONLY|O_DIRECTORY|O_CLOEXEC`（DS116-E019）；先由`invalid_name`读取`name[0]`，空字符串即返回NULL，置ENOENT的语句在注释里（DS116-E020）；有效名字则`fd=open(name,opendir_oflags,O_RDONLY)`，**无检查fd是否负数，直接`__alloc_dir(fd,true,0)`**（DS116-E021、DS116-E022）。

`__alloc_dir`固定`allocation_size=32768`，还有`max_buffer_size=1048576`但实际没有据此调大/调小本次分配；请求内存`malloc(sizeof(DIR)+32768)`（DS116-E023、DS116-E024、DS116-E025）。成功依次令`DIR.fd=fd`、`allocation=32768`、`size=offset=filepos=errcode=0`（DS116-E027、DS116-E028），返回DIR指针（DS116-E029）。底部`data[0]`具long double对齐属性（DS116-E033），但`sizeof(DIR)`和成员偏移未经编译确定，不能凭名字推实际结构字节数。

四条分支彼此独立、仅观察`opendir/__alloc_dir`到返回，绝不拿负fd去执行后续内核系统调用：

| 模型 | 选定路径 | open返回 | malloc结果 | opendir返回 | 仍持有fd | close/free | 明确errno赋值 |
|---|---|---|---|---|---|---|---|
| A01 | . | 7 | nonNULL | DIR* | 7 | 否／否 | 无明确赋值 |
| A02 | 空字符串 | NOT_CALLED | NOT_CALLED | NULL | 无 | 否／否 | 无明确赋值 |
| A03 | /missing | -2 | nonNULL | DIR* even with fd=-2 | -2 | 否／否 | 无明确赋值 |
| A04 | /valid | 7 | NULL | NULL | fd7 remains not closed by examined opendir/__alloc_dir | 否／否 | 无明确赋值 |

空路径不会调用open/malloc；负fd但malloc成功时`DIR.fd`保存该负值，函数仍返回非NULL（不证明后续安全）；有效fd但malloc失败时函数返回NULL而未执行注释里的close，fd未由此函数释放。上述函数体都没有明确为此分支设置errno，不能把其错误原因替换成一条确定的`strerror(errno)`消息。`__alloc_dir`的`close_fd=true`仅是实参，该条件关闭路径处于注释，不是活动清理。

## B：一次getdents64的80字节，如何供多次readdir

本仓用户库公开符号名是`readdir`，其实现函数和声明的返回类型为`struct dirent64*`（readdir64.c:29–30，dirent.h:172，普通位置DS116-P04/P06）；shell却声明`struct dirent* buf`。两者是**不同C结构指针类型**，源码存在类型兼容与布局的编译前提，不在本批凭x86-64名字相似宣称编译安全。`bits/dirent.h`中`dirent64`又受`__USE_LARGEFILE64`条件控制（DS116-E053、DS116-P07），`getdents64`原型还受`__USE_GNU`门（DS116-P08）。

内联汇编的`SYSFUNC_DEF`宏生成用户态`getdents64`公开入口，调用实例在`syscall.c:66`（普通位置DS116-P09）；宏将该调用的`__NR_getdents64=217`装进%rax并跳到共享`syscall`指令（DS116-E042、DS116-E043、DS116-E044、DS116-E045、DS116-E047）。负返回转换errno的汇编语句已被注释（DS116-E046、DS116-P22）。内核映射`syscalls_64.h:67`是`__SYSCALL`调用实例（普通位置DS116-P10），真实`getdents64`设置filldir64回调，读写kernel file.f_pos并按是否生产记录返回字节数（DS116-E049、DS116-E050、DS116-E052）。内核file.f_pos、DIR.filepos、DIR.offset是三个独立来源，不能混成同一游标。

用户`readdir`先测`DIR.offset >= DIR.size`，若缓冲耗尽则用`maxread=DIR.allocation`，超过INT_MAX时缩至INT_MAX，再`getdents64(DIR.fd,DIR.data,maxread)`（DS116-E034、DS116-E035）。返回**正值**则设size=bytes、offset=0；若bytes<=0，无论0还是负值，直接返回NULL而**不更新**size/offset/filepos/errcode（DS116-E036、DS116-E037、DS116-E038）。缓存中有完整条目时不发系统调用，直接把`&DIR.data[offset]`转成`struct dirent64*`，按`d_reclen`累加offset、按`d_off`赋`DIR.filepos`，返回指向**同一缓冲区**的指针（DS116-E039、DS116-E040、DS116-E041）。refill可能覆盖此缓冲，记录指针不是新分配的独立所有权。

取第59批的限定布局：`d_name`前缀偏移19字节、记录向8字节对齐，在完整且长度正的前提下三项`.`、`..`、`alpha`分别需`ALIGN(19+1+1,8)=24`、`ALIGN(19+2+1,8)=24`、`ALIGN(19+5+1,8)=32`，总共**80字节**（DS116-E059、DS116-E048、DS116-E053）。假定首次getdents返回80个字节、三条记录和所有将被读取的字段都有效，`d_off`以**已初始化的符号D1/D2/D3**记录；不能因FAT actor的offset=0推断尾条D3为0：`filldir64`对前项的d_off赋值，但尾项系统调用修补代码被注释（DS116-E060、DS116-E062、DS116-P13）。

| 模型 | readdir次序 | 本轮新syscall（累计） | getdents返回 | 指向DIR缓冲 | 名字 | d_reclen | 用后offset/size | filepos | shell输出 |
|---|---:|---|---|---|---|---|---:|---|---|
| B01 | 1 | 1（累计1） | 80 | &DIR.data[0] | . | 24 | 24/80 | D1 | 跳过/停止 |
| B02 | 2 | 0（累计1） | NO_CALL | &DIR.data[24] | .. | 24 | 48/80 | D2 | 跳过/停止 |
| B03 | 3 | 0（累计1） | NO_CALL | &DIR.data[48] | alpha | 32 | 80/80 | D3 | alpha\t |
| B04 | 4 | 1（累计2） | 0 | NULL | — | — | 80/80 | D3 | \n after break; closedir called |

在首轮返80且三条有效记录的前提下，**前三次readdir只需要一次系统调用**；第二次、第三次直接消费缓冲，读完后offset=80=size。第四次才再次getdents，抽象返回0后立即返回NULL，shell随后打印换行并调用closedir。换成同样返回**普通负数-5**，readdir同样直接NULL，DIR的size=80、offset=80、filepos=D3保持；二者在此shell显示和返回分支上不区分（详见C）。另一独立情形：若size=80、offset=24且记录完整，下一次直接消费`..`，offset变48，syscall增量**0**。

所有数值均是给定正常批次的纸面模型，而不是实际盘面读取证书。内核FAT正常穷尽时可能把ctx.pos设置为-1并带动kernel file.f_pos，和用户DIR.filepos=D3不必相等；若无法证明尾D3已初始化，应停止给它数值，不从硬盘结构外推出结论。

## C：过滤、结束信号和closedir的资源事实

shell每轮调用`readdir(dir)`，返回NULL就退出循环；名字等于`.`或`..`时跳过，不执行打印。普通`alpha`打印`alpha`加一个制表符，没有对每个名字单独换行；循环结束才统一`printf("\n")`，然后调用`closedir(dir)`，最后**`ls_command`返回1**（DS116-E056、DS116-E057、DS116-E058）。

如果`opendir`本身返回NULL，shell会打印`ls: strerror(errno):path`样式的一行错误并**返回-1**，不进入循环，也不会走本成功分支的换行/closedir（DS116-E018）。但空名字、malloc失败等分支的errno值不是所审opendir代码明确设置的值，不能虚构错误文本内容。

当耗尽后的getdents返回0或负数时，`readdir`所读代码都只返回NULL，不更新size/offset/filepos；错误码转errno的相关汇编在注释，`DIR.errcode`在该函数无活动赋值。两条路径对shell都是“跳出循环→换行→调用closedir→返回1”，不能声称有准确的EOF/error区分。返回的记录指针指向可复用的DIR.data，下一次refill前后保存旧指针不能假定内容稳定（DS116-P04、DS116-P24）。

**当前`closedir`尤其不能按接口注释推断清理。** 它虽有`int closedir(DIR*)`签名和“成功返回0、失败返回-1”的接口注释，但固定`closedir.c:28–51`完整函数中，取fd、`free(dirp)`、关闭fd和`return`均被注释，**没有活动的fd关闭、内存释放或显式返回语句**（DS116-P01/DS116-P29）。shell确实调用它但不消费返回值，因此只确认“调用存在、未见释放动作”，不能给该函数捏造0/-1，或声称资源已经释放；非void函数落到末尾的返回值和真实编译行为未验证。

## 本包边界与独立验收

新增62个固定源码短窗，每个1–5物理连续行、按函数/宏/结构归属核验；27项普通定位分开。D00三项、A四例、B三记录四次调用＋一次负返回＋一次缓存情形、C失败/成功/EOF/error及关闭函数条件均在facts与DOT关联来源。引用第59批的19字节前缀、8字节对齐模型作为**有条件历史结果**，不当成目标已编译布局。完整本地P9、预处理/编译/链接、仓库程序、Graphviz、QEMU、实机、用户库运行及真实目录系统调用均NOT_RUN。

作者终点：`READY_FOR_MAINLINE_REVIEW`；`mainline_acceptance=PENDING`；`D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW`。R115-01、R115-02、R115-03继续OPEN，旧115仍RETURN，交C08 Pro独立核收后方可裁定。
