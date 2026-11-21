---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-STDIO-FILE-OPEN-121
record_type: phase2_stdio_fopen_analysis_and_D00_three_corrections
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
produced_by: "GPT-6（后端与effort未独立认证）"
model_per_owner: "GPT-6＋Extra（Owner选择）"
date: 2026-11-22
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
open_questions: []
inputs_read: [agent-workspace/lead/MYOS2-LEAD-002/171-user-stdio-file-open-contract.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-FILE-COMMANDS-120-review.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-FILE-COMMANDS-120-D00-117-119-review.md, myinitramfs/CMakeLists.txt, myinitramfs/mylib/myglibc/libio/iofopen.c, myinitramfs/mylib/myglibc/libio/fileops.c]
---

# 第121批：仓内fopen、模式解析与FILE内文件描述符

**状态只为作者提交就绪，C08独立验收PENDING。** 所有技术模型读取固定 `time@a039d9803ade`，并以指定C08主线正式裁定为先。本件不修改原117–120六件；不执行CMake、C/ASM、用户程序、文件操作或硬件。新定义内源码窗口编号在`evidence.md`的FO121-E001～E069；其它固定文本和符号调用位置为FO121-P01～P20。

## D00：三个OPEN项，追加修正并保留C08关闭权

### R117-02：历史字面计数和单符号短窗资格

| 批次 | 原窗口数 | 真实字面标签出现 | 当前可用P2资格 | 准确调整 |
|---|---:|---:|---:|---|
| 117 | 88 | 89 | 旧88仅按历史原窗资格待后续确认 | 多一次在旧MANIFEST第113行的描述文字 |
| 118 | 91 | 92 | 89个候选 | E051/E052文件作用域对象转普通位置；额外字面来自evidence第3行 |
| 119 | 46 | 46 | 43个候选 | E020/E021/E022为全局内建命令数组初值；E019仍为结构体体内短窗 |
| 120 | 81 | 81 | 80个按原署名合格 | E072原引文跨两个汇编符号标签 |
| **121新件** | **69** | **69** | **69个新单符号合格窗口** | 仅新件范围；全部锚到各自函数/宏/结构体/汇编标签内 |

旧FC120-E072写`LABEL_SYSCALL`并取71–75行，71–73才是该入口，74的`sysexit_ret_addr`已进入另一符号。新FO121-E065逐字锚71–73，E066锚74–76，E067锚79–81的第三个`LABEL_SYSCALL_RET`。77–78的errno保存与返回归一化语句在注释中，不是活动汇编。原窗口计数不覆盖、不调整旧作者评论；本批新P6按最终六件的文字而非历史报告计算：仅evidence包含69处正式短窗，分布0/0/0/0/0/69，自检69/69/0。原R117-02仍OPEN。

### R120-01：用户库来源、系统调用编号、错误文字各自立证

| 层 | 本轮固定来源 | 能证明的事 | 第一个未证条件 |
|---|---|---|---|
| shell四个内建调用 | `myshell.c:331–334,343–346,355–360,369–374` | C源码中直接调用creat/mkdir/rmdir/unlink、给定argc分支与返回/printf | 具体sh实际链接哪个libc |
| 仓内myglibc用户桩 | `myglibc/syscall/syscall.c:53–57`及宏19–27 | 仓内有四个用户SYSFUNC_DEF调用实例，可从宏理解寄存器编号/共享syscall跳转 | 交付sh使用此源实现/选此头 |
| 内核系统调用号 | `asm/unistd_64.h:103,104,106,108` | mkdir=83、rmdir=84、creat=85、unlink=87。**源行号不是调用编号** | 对应用户发起的实际系统调用尚未执行 |
| 内核映射与活动宏 | `asm/syscalls_64.h:44,45,46,48`及`syscall_64.c::x64_sys_call` | 这四个入口在固定time的内核switch映射实例与宏定义有据可查 | 未证明用户二进制实际发送何号，亦未跑系统调用 |
| initramfs构建文本 | `myinitramfs/CMakeLists.txt:3,40–51`及`myshell/CMakeLists.txt:1–10` | project在设置MUSL_PATH的编译器变量之前；sh目标由本目录源建立，顶层只add_subdirectory(myinit/myshell) | 真正编译器、所选外部musl版本及实际二进制链接未核 |
| errno、字符串、输出 | `syscall.c:71–81`、`errlist.c::__get_errlist`、`printf.c::printf` | 仓内汇编errno赋值和强制−1均为注释；实际errlist会对负errnum取反后索引`_sys_errlist_internal[errnum]`；若选仓内printf，它会请求write | 负返回是否/怎样被实际libc映射至errno，以及最后用户看到的文字均未知 |

修正170的P03：应为源码行103/104/106/108，而不是83/84/85/87；数值仍分别为83/84/85/87。P10应为`namei.c::filename_create` **770–823**，825已开始`kern_path_create`。补E079：原只含`if(errnum<0)`入口，新FO121-E062包含原实际负号转换和数组索引返回。它只能推出传入某个确定错误编号时的内部索引，不认证当前shell或实际链接库如何设置errno。`printf`请求与实际屏幕显示同样分开。实际交付程序、外部musl和内核返回尾段还受第08批SR08-02关于信号/重启改写用户返回的限制。R120-01只申请文档补正，仍OPEN。

### R120-02：两个创建方法缺失早返与两条删除命令必须分隔

`stdio-file-open-paths.dot`含D00_CRE/D00_MKD/D00_RM/D00_RMD四个互相独立的纠正子图，不能用共享`r_argc`或`r_error`把rm错误跳入rmdir路径。方法缺席时，在内核`do_creatat`和`do_mkdirat`的各自`i_op`检查节点立即返回−EPERM，**均跳过done_path_create/putname**。只有在用户库正常完成且把该错误作为内建调用结果交回的条件下，`touch_command`与`mkdir_command`才把它存到局部err并最终仍返回0；run_cmd不消费这个返回。源码的shell函数确实无条件在合法argc路径return0，跨层关联必须保留库实现和无信号改写等前提。

| case_id | 命令 | argc | 内核/用户给定条件 | 内核/接口值（模型） | 内建值 | 本路径清理情况 | 此命令自己的输出 |
|---|---|---:|---|---|---:|---|---|
| D121-A01 | touch | 2 | 无方法 | -EPERM | -EPERM_if_library_exposes_unmodified_result | 0 | 两者均绕过 | 无新增打印 |
| D121-A02 | touch | 2 | 可用方法 | 0 | 0 | 0 | done+putname | 无新增打印 |
| D121-A03 | touch | 1 | 给定接口返回 | — | — | -1 | 两者均绕过 | 无新增打印 |
| D121-B01 | mkdir | 2 | 无方法 | -EPERM | -EPERM_if_library_exposes_unmodified_result | 0 | 两者均绕过 | 无新增打印 |
| D121-B02 | mkdir | 2 | 可用方法 | 0 | 0 | 0 | done+putname | 无新增打印 |
| D121-B03 | mkdir | 2 | ERR_PTR(-2) | -2 | -2_if_library_exposes_unmodified_result | 0 | 仅putname | 无新增打印 |
| D121-B04 | mkdir | 3 | 给定接口返回 | — | — | -1 | 两者均绕过 | 无新增打印 |
| D121-R01 | rm | 1 | 给定接口返回 | — | — | -1 | 无此创建清理 | 无本命令printf |
| D121-R02 | rm | 2 | 给定接口返回 | 0 | 0 | 0 | 无此创建清理 | 无本命令printf |
| D121-R03 | rm | 2 | 给定接口返回 | -5 | -5 | -5 | 无此创建清理 | 本命令错误printf |
| D121-D01 | rmdir | 1 | 给定接口返回 | — | — | -1 | 无此创建清理 | 无本命令printf |
| D121-D02 | rmdir | 2 | 给定接口返回 | 0 | 0 | 0 | 无此创建清理 | 无本命令printf |
| D121-D03 | rmdir | 2 | 给定接口返回 | -5 | -5 | -5 | 无此创建清理 | 本命令错误printf |

上表`rm`、`rmdir`各自的负值例，其错误格式必须分别为`rm: %s: %s\n`、`rmdir: %s: %s\n`，且两个格式串绝不能通过另一个命令的路径到达。给定−5不直接确定`strerror(errno)`结果；只在已给定错误字符串输入时才能算格式请求。第120批B120-06原始输入以facts的`mkdir docs extra`为准。此前C08已闭合R118-02、R119-01；可复用其实际DOT路径及非负索引0的正确解释，但不重审。R120-02仍OPEN。

## A：构建来源与仓内fopen入口

从给定文件可查的两条路径是**相互分离的静态构建图**：顶层`myinitramfs/CMakeLists.txt`定义project、条件LINK_TYPE、MUSL_PATH及外部`musl-gcc`和include路径，仅把myinit和myshell加入子目录；`myshell/CMakeLists.txt`建立目标`sh`。另一条是`mylib/CMakeLists.txt`→myglibc→`add_library(mylibc STATIC)`→`add_subdirectory(libio)`→`target_sources(mylibc ...)`，其中包含iofopen.c和fileops.c。**在已读顶层没有把第二条接入sh，也没有实际链接命令的结果**。CMake先调用project后设CMAKE_C_COMPILER，故这份文本本身不足以证明实际编译器后来采用了musl-gcc。任何“sh采用仓内glibc”或“已证sh采用某版musl”都过界。

仓内`libio/stdio.h:246`是普通`fopen`声明；`include/stdio.h:193`才是调用式`fopen(fname,mode)`改写成`_IO_new_fopen(fname,mode)`的宏；`libio/iofopen.c:83–88`是真正函数定义；90–97的strong_alias/versioned_symbol/weak_alias全部是注释，不能冒充已经导出相同符号。以下模型的前提是**确实选用仓内这份头和这些定义**。

| case_id | 进入现场 | 分配结果 | 包装器对FILE唯一可确认写入 | 进入下层 | 局部结果边界 |
|---|---|---|---|---|---|
| A121-01 | inrepo_macro_and_new_fopen_conditional | P_VALID_WRITABLE_FILE | {"_fileno":-1} | 是 | 转下层结果 |
| A121-02 | inrepo_new_fopen | NULL | {} | 未能安全推导 | NOT_MODELED_UNDER_INVALID_ACCESS |
| A121-03 | direct_inner_file_fopen_not_same_as_new_fopen | 已有fp | {"_fileno":7} | 未能安全推导 | NULL |

对A121-01有效可写FILE，`_IO_new_fopen`请求`malloc(sizeof(FILE))`，随后只显式写入`new_f->_fileno=-1`，再把同一指针与filename/mode传`_IO_new_file_fopen`；_flags、_flags2、_offset、缓冲指针等的值**不能**由普通malloc推定为0或已初始化。A121-02给定malloc返回NULL，函数未检查就解引用；从这里停止，没有虚构的安全NULL返回。A121-03是**直接调用下层函数时FILE已有_fileno=7**的另一个现场，`_IO_new_file_fopen`按入口门直接返0/NULL且不走open，不能把它拼到刚写−1的普通包装器路径。

## B：模式串逐字解析为open参数

源码实际次序是首字符`r/w/a`设omode、oflags、read_write；随后`i=1;i<7;++i`最多读取后缀索引1–6，借`*++mode`先移动再读取；遇NUL退出，其余符号按分支更新。`oprot`在下层始终取0666（八进制，十进制438）。下表采用**假设选择本仓bits/fcntl.h且O_CLOEXEC可见、O_LARGEFILE已定义为0**的数值模型；`_flags2`的旧值也每例明确指定。没有预处理与真实二进制证据时，数字不能直接当sh的实际open标志。

| case_id | 原模式串 | 首字母被接受 | 实际访问的后缀下标 | omode / oflags / posix_mode（十六进制） | read_write（八进制） | 给定旧值后_flags2 | 循环停止原因 |
|---|---|---|---|---|---|---:|---|
| B121-01 | `r` | 是 | 1 | 0x0 / 0x0 / 0x0 | 010 | 0 | NUL |
| B121-02 | `w` | 是 | 1 | 0x1 / 0x600 / 0x601 | 04 | 0 | NUL |
| B121-03 | `a+` | 是 | 1,2 | 0x2 / 0x208 / 0x20a | 010000 | 0 | NUL |
| B121-04 | `wbx` | 是 | 1,2,3 | 0x1 / 0xe00 / 0xe01 | 04 | 0 | NUL |
| B121-05 | `re` | 是 | 1,2 | 0x0 / 0x400000 / 0x400000 | 010 | 64 | NUL |
| B121-06 | `q` | 否 | 无 | — | — | 未访问 | 首字母拒绝 |
| B121-07 | `rmmmmmmZ` | 是 | 1,2,3,4,5,6 | 0x0 / 0x0 / 0x0 | 010 | 1 | I_BECOMES_7_SIX_SUFFIXES_MAX |
| B121-08 | `rmce` | 是 | 1,2,3,4 | 0x0 / 0x400000 / 0x400000 | 010 | 67 | NUL |
| B121-09 | `rmce` | 是 | 1,2,3,4 | 0x0 / 0x400000 / 0x400000 | 010 | 99 | NUL |
| B121-10 | `r?b` | 是 | 1,2,3 | 0x0 / 0x0 / 0x0 | 010 | 0 | NUL |

- `r` 得O_RDONLY(0)、oflags0、内部`_IO_NO_WRITES=8`；`w` 得O_WRONLY(1)与O_CREAT(0x200)|O_TRUNC(0x400)，内部`_IO_NO_READS=4`。
- `a+` 起初O_WRONLY、O_CREAT|O_APPEND(0x208)、内部`_IO_NO_READS|_IO_IS_APPENDING=0x1004`；`+`把omode改O_RDWR(2)，并将read_write按位与`_IO_IS_APPENDING`，最终为0x1000。这里`O_APPEND=0x0008`与`_IO_IS_APPENDING=0x1000`属于两个不同层的值。
- `wbx`扫描b后再扫描x，保留w的创建和截断并新增O_EXCL(0x800)，posix_mode为0xE01。b只更新last_recognized指针，不创建额外的POSIX binary flag；x和b的顺序均属于同一短串正常路径。
- `re`在条件定义可见时给open端增加O_CLOEXEC(0x00400000)，同时把FILE的`_flags2`与64按位或；这**不证明内核的close-on-exec已经生效**。m与c分别或1、2；B121-08从旧值0得到67，B121-09从旧值32得到99，既有位不会被清除。
- `rmmmmmmZ`只读取六个m（下标1–6），**不读取**第七个后缀Z（下标7）；这里退出因为i到7，不是读到了NUL。`r?b`的问号属于default忽略，但继续读取b。非法首字符`q`在后缀循环和open前直接返回NULL，设置EINVAL的语句是注释。
- last_recognized虽然在入口和+、x、b更新，但此后搜`,ccs=`的程序段全部注释，不能声称有运行中的字符集转换。read_write传给`_IO_file_open`，其中`_IO_mask_flags`又是注释，所以**不能据传参宣称FILE的_flags实际得到NO_READS/NO_WRITES/APPENDING状态**。

注意：`O_LARGEFILE`在所选本仓bits头只于`__USE_LARGEFILE64`条件下定义为0；`O_CLOEXEC`只于`__USE_XOPEN2K8`定义。未实际执行宏展开时，不保证这些条件启用或实际头选择；若缺定义，不可照数值模型直接认定程序已成功编译。且`_IO_new_fopen`只初始化_fileno，未初始化_flags2：对于含m/c/e的模式，`|=`会读取既有值；本批只在独立给定初值0或32的helper模型里报告67/99，**不为真实新分配FILE虚构初始_flags2=0**。

## C：open返回为负、0、正数时FILE会怎样

真实下层`_IO_file_open(fp,filename,posix_mode,prot,read_write)`执行的open调用只有三个实参：`open(filename,posix_mode | O_LARGEFILE,prot)`。read_write虽然被传给此函数，却没有作为open第四参数；后续`_IO_mask_flags`、seek、close都在注释中。给定正常可用FILE、模式`r`与固定路径`note`，不运行文件系统：

| case_id | 模式 | 旧_fileno | 给定open返回 | open请求次数 | 新_fileno | 底层返回 | 最外层包装返回 |
|---|---|---:|---:|---:|---:|---|---|
| C121-01 | `r` | -1 | -1 | 1 | -1 | NULL | NULL |
| C121-02 | `r` | -1 | 0 | 1 | 0 | SAME_P | SAME_P |
| C121-03 | `r` | -1 | 5 | 1 | 5 | SAME_P | SAME_P |
| C121-04 | `q` | -1 | — | 0 | -1 | NULL | NULL |
| C121-05 | `r` | 7 | — | 0 | 7 | NULL | NOT_APPLICABLE |

C121-01给定open=-1：底层立即NULL，不写_fileno，仍为−1；包装器转发NULL，当前两函数没有free(new_f)语句，不能据此称失败已正确释放流对象。C121-02给定open=0同样**是有效成功fd**，写`_fileno=0`并返回同一FILE指针；C121-03给定5时对应写5并返回同一指针。**这些都是用户库函数体根据给定返回值的模型**，不是系统调用/文件偏移、缓冲、流同步、磁盘是否创建截断追加的认证。即使open成功，也不能凭局部代码宣称FILE其它字段、vtable、初始文件位置和流内部标志已初始化。

C121-04非法首字符与C121-05已有fd的两个早返均**不调用open**；C121-05只适用于直接调用`_IO_new_file_fopen`的独立现场，原FILE的_fileno=7保留。分配失败A121-02不能补造安全返回。跨层fd值还需确认实际链接哪套libc、实际syscall返回尾部、用户头文件/编译ABI及完整对象前提。

## 质量与验收边界

`facts-and-boundaries.yaml`的D00十三例、A三例、B十例和C五例均带case_id；DOT分别按命令身份、malloc现场、模式解析、open结果三组建模并用可机械检查的有向边与不可达负例证明非混淆。新evidence共69个独立定义体短窗，其余二十个普通定位按原源行界阅读。所有实际CMake结果、外部musl、链接产物、真正C运行/编译/Graphviz/QEMU/硬件和文件操作一律`NOT_RUN`。旧R117-02、R120-01、R120-02仍`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`；作者只交`READY_FOR_MAINLINE_REVIEW`，不越权关闭、不发第122批。
