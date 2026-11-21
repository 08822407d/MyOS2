---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-STDIO-OPEN-FLAG-ABI-122
record_type: bounded_static_open_flag_abi_and_D00
produced_by: "GPT-6（当前会话自报；后端/effort不认证）"
model_per_owner: "GPT-6＋Extra（Owner选择）"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
date: 2026-11-22
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
owner_decisions_requested_now: []
open_questions: []
inputs_read: [agent-workspace/lead/MYOS2-LEAD-002/172-user-stdio-open-flag-abi-contract.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-review.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-D00-117-120-review.md, myinitramfs/mylib/myglibc/bits/fcntl.h, mykernel/include/uapi/asm-generic/fcntl.h, mykernel/fs/vfs/open.c]
---

# 第122批：四种fopen模式，两种标志编码，build_open_how纸面计算

固定源码是`time@a039d9803ade`，本批只在172授权目录提交分析文档。**本报告不认证实际sh使用仓内myglibc，亦不把静态整数视为已执行的open、截断、追加、创建、权限改变或文件描述符安装。** 新P2短引文见`evidence.md`中ABI122-E001～E085；普通位置和引用边界另列。旧原件保持冻结。

## D00：三个OPEN事项的逐项纠正

### R120-02：13行来源值和条件返回完全对齐

旧121图让`D121-A01/B01`先指向可同时转向“方法存在/缺失”的公共检查节点，使这两个**给定缺失方法**的case_id仍误达回调/清理。新`stdio-open-flag-abi-paths.dot`将每个案例作为独立入口并以专有的缺失/存在节点承载条件；负例从各自case_id进入`-EPERM`→有条件库返回→局部err→内建0；不允许由相同case_id到callback、done_path_create或putname。对照存在方法的A02/B02经回调0→done→putname；B03给定有效name且filename_create返回ERR_PTR(-2)走putname但不走done。argc错不调系统接口。rm/rmdir拥有各自独立的argc/返回/错误printf/终点，互不可达。

下表**每列都有单独来源**：“内核返回”仅适用于达到内核路径的情况，“用户接口返回”在跨层时加条件；输出是本命令自己的请求。这里不把错误的内核值直接填作用户值或内建值。

| case_id | 命令 | argc | 给定条件 | 内核返回 | 用户接口给定/有条件返回 | 局部err（适用时） | 内建返回 | done | putname | 自身报错请求 |
|---|---|---:|---|---|---|---|---:|---|---|---|
| D121-A01 | touch | 2 | METHOD_MISSING | -EPERM | -EPERM_IF_RAW_USER_RESULT | -EPERM_IF_RAW_USER_RESULT | 0 | 否 | 否 | 无自身错误行 |
| D121-A02 | touch | 2 | CALLBACK_RETURNS_0 | 0 | 0 | 0 | 0 | 是 | 是 | 无自身错误行 |
| D121-A03 | touch | 1 | INVALID_ARGC | — | — | — | -1 | 否 | 否 | 无自身错误行 |
| D121-B01 | mkdir | 2 | METHOD_MISSING | -EPERM | -EPERM_IF_RAW_USER_RESULT | -EPERM_IF_RAW_USER_RESULT | 0 | 否 | 否 | 无自身错误行 |
| D121-B02 | mkdir | 2 | CALLBACK_RETURNS_0 | 0 | 0 | 0 | 0 | 是 | 是 | 无自身错误行 |
| D121-B03 | mkdir | 2 | FILENAME_CREATE_ERRPTR_MINUS2 | -2 | -2_IF_RAW_USER_RESULT | -2_IF_RAW_USER_RESULT | 0 | 否 | 是 | 无自身错误行 |
| D121-B04 | mkdir | 3 | INVALID_ARGC | — | — | — | -1 | 否 | 否 | 无自身错误行 |
| D121-R01 | rm | 1 | INVALID_ARGC | — | — | — | -1 | 不适用 | 不适用 | 无自身错误行 |
| D121-R02 | rm | 2 | USER_INTERFACE_RESULT_ZERO | — | 0 | 0 | 0 | 不适用 | 不适用 | 无自身错误行 |
| D121-R03 | rm | 2 | USER_INTERFACE_RESULT_MINUS5 | — | -5 | -5 | -5 | 不适用 | 不适用 | rm: %s: %s\\n |
| D121-D01 | rmdir | 1 | INVALID_ARGC | — | — | — | -1 | 不适用 | 不适用 | 无自身错误行 |
| D121-D02 | rmdir | 2 | USER_INTERFACE_RESULT_ZERO | — | 0 | 0 | 0 | 不适用 | 不适用 | 无自身错误行 |
| D121-D03 | rmdir | 2 | USER_INTERFACE_RESULT_MINUS5 | — | -5 | -5 | -5 | 不适用 | 不适用 | rmdir: %s: %s\\n |

其中A01/B01跳过两项清理，是**函数控制流观察**；不声称已复现内存泄漏。run_cmd是void，丢弃所有内建返回。对于rm/rmdir负值，`strerror(errno)`的输入未从内核负返回形成已证映射，仍不能推断确定文字。B120-06原始输入以旧facts的`mkdir docs extra`为准。本项`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，仍OPEN。

### R121-01：五形参helper与三实参open以及FILE指针身份

这三个函数/调用必须各自标明，不可把下层的参数字典混入最终open：

1. `_IO_new_fopen(filename, mode)`，入口先`malloc(sizeof(FILE))`，有效分配P时只显式设置`P->_fileno=-1`，调用`_IO_new_file_fopen(P,filename,mode)`，返回下层结果；分配NULL缺安全检查，另留未证前提。
2. `_IO_new_file_fopen(fp,filename,mode)`解析模式，**恰五个有序实参**传到`_IO_file_open(fp,filename,omode | oflags,oprot,read_write)`。121旧E023标题的“四参”和旧图同义说明作废；其源码第267行实际一直是五参。对模式r、路径note的给定模型，五参数为`[P,"note",0,438,8]`。
3. `_IO_file_open(fp,filename,posix_mode,prot,read_write)`在实际调用open时只使用**三个有序实参**`open(filename,posix_mode | O_LARGEFILE,prot)`，上述r模型若选择用户O_LARGEFILE=0则是`["note",0,438]`；read_write不是open第四参，亦未被活动`_IO_mask_flags`写入FILE，因为该代码是注释。

| case_id | 调用链实际参与 | mode | 进入内层_fileno | 已调用helper的五实参 | 最终open三实参 | open次数 | 给定open返回 | 最终_fileno | 内层返回 | 外包装返回 |
|---|---|---|---:|---|---|---:|---|---:|---|---|
| C121-01 | 包装→模式→helper | r | -1 | ["P","note",0,438,8] | ["note",0,438] | 1 | -1 | -1 | NULL | NULL |
| C121-02 | 包装→模式→helper | r | -1 | ["P","note",0,438,8] | ["note",0,438] | 1 | 0 | 0 | P | P |
| C121-03 | 包装→模式→helper | r | -1 | ["P","note",0,438,8] | ["note",0,438] | 1 | 5 | 5 | P | P |
| C121-04 | 包装→模式→helper | q | -1 | 未调用 | 未调用 | 0 | — | -1 | NULL | NULL |
| C121-05 | 直接内层 | r | 7 | 未调用 | 未调用 | 0 | — | 7 | NULL | NOT_CALLED |

新图不再允许`_fileno=-1 → open`直跳绕过模式解析；每个实例都先过首字符、后缀解析出口，再到五参helper和三参open。C121-01给定−1：一次open、P字段仍−1、内层和外层各返NULL；C121-02/03给定0或5：一次open、写入相应fd、两层都返回**同一P**。C121-04的q首字母非法：经过外层与内层入口，但没有helper调用和open，沿内层NULL回传外层NULL。C121-05以及A121-03是**另一独立直接调用内层的现场**，P原本的_fileno=7，不经过外层malloc/赋−1，内层在已有fd门返NULL且open次数0，不能捏造外层返回值。

121旧图`b_b→b_oflags`也被撤销：`b`仅更新`last_recognized`游标，并继续扫描；不是O_*位，wbx的x才增加O_EXCL。用户侧编译/头文件/实际sh二进制是否连到了这三个函数保持UNKNOWN。R121-01仍待C08验收。

### R121-02：time活动C配置正式短窗

121将`mykernel/scripts/options_flags.cmake:39–70`仅列普通位置，而合同要求正式短窗。本122 `evidence.md`分别摘选同一实际`set(CMAKE_C_FLAGS)`命令内39–43及64–66，包含`-mcmodel=kernel -fno-pie -fno-pic`和`-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER`。它证明**固定time源码设置了这些内核C配置文本**，与myinitramfs/myglibc另一个CMake文件的`-D_GNU_SOURCE`或仓外musl构建意图不能混为实际编译。122的正式引文、前缀数均独立按最终六件重新计算，不能沿用121的69个。R121-02保持OPEN待C08签闭。

## A：两套头文件来源和活动调用桥

仓内用户侧可追的**包含文本路径**为：`libio/fileops.c:29`包含`<fcntl.h>`；若解析到`myglibc/include/fcntl.h`，它接入`myglibc/io/fcntl.h`，后者`#include <features.h>`、`#include <bits/fcntl.h>`，并声明三参的`open(const char*,int,__mode_t)`。相应bits头定义用户位。myglibc/CMakeLists.txt文本创建`mylibc STATIC`、追加`-D__USE_GNU -D_GNU_SOURCE`及目标包含目录；`features.h`的`_GNU_SOURCE`分支设`_XOPEN_SOURCE=700`和`_LARGEFILE64_SOURCE=1`，随后可启用`__USE_XOPEN2K8`和`__USE_LARGEFILE64`，使O_CLOEXEC条件定义及用户O_LARGEFILE=0可见。**这是该目标若按此头与标志预处理的推导，不是交付sh真实编译结果。** 顶层initramfs CMake在project之后设置仓外musl-gcc路径，并未在已读top/myshell路径把mylibc实际链接到sh。

内核侧的独立包含文本路径为：`mykernel/fs/vfs/open.c:15`→`linux/kernel/fcntl.h:5–6`→`uapi/linux/fcntl.h:5–6`→`uapi/asm-generic/fcntl.h`以及`uapi/linux/openat2.h`。内核通用头用不同数值定义O_*，有效位集合由`VALID_OPEN_FLAGS`确定，mode掩码由`linux/kernel/stat.h::S_IALLUGO`确定；具体宏不按用户头改写。

若实际选择仓内用户桩，`syscall.c:30`的`SYSFUNC_DEF(open)`将`__NR_open=2`装入rax；内核`syscalls_64.h:6`映射到sys_open，经`fs/syscall.c:38–42`入口**调用`do_sys_open(0,filename,flags,mode)`**，dfd参数为源码给定0（不是凭想象改成AT_FDCWD）。`do_sys_open`调用`build_open_how(flags,mode)`后交`do_sys_openat2(dfd,filename,&how)`。这些都是各自可读函数或宏实例；“同一整数无转换穿过实际用户库、ABI和内核返回”必须显式视为**条件桥**，当前没有实际链接或运行的认证。本批C只算进入`build_open_how`后形成的对象，不展开后续真正文件打开、fd和磁盘。

## B：r、w、a+、wbx的两个整数，逐位不同名的地方

两个头的关键数值（左是仓内myglibc bits，右是内核asm-generic fcntl）：

| 源码符号 | 用户值（十六进制；十进制） | 内核同名值（十六进制；十进制） | 把用户数字原样读成内核数时的意义 |
|---|---|---|---|
| O_RDONLY | 0x0；0 | 0x0；0 | 都是只读，值0不产生置位但确为只读访问 |
| O_WRONLY | 0x1；1 | 0x1；1 | 同名同值 |
| O_RDWR | 0x2；2 | 0x2；2 | 同名同值 |
| O_CREAT | 0x200；512 | 0x40；64 | **原始0x200在内核是O_TRUNC** |
| O_TRUNC | 0x400；1024 | 0x200；512 | **原始0x400在内核是O_APPEND** |
| O_APPEND | 0x8；8 | 0x400；1024 | 原始0x8不在此内核VALID_OPEN_FLAGS中 |
| O_EXCL | 0x800；2048 | 0x80；128 | **原始0x800在内核是O_NONBLOCK** |
| O_LARGEFILE | 0（仅用户相应条件宏可见） | 0x8000；32768 | 此组用户模型不增位；不能等于内核同名位 |
| O_CLOEXEC | 0x400000（用户条件宏） | 0x80000；524288 | 本批四组不使用e后缀；不推断任何CLOEXEC行为 |

四行按用户头的宏进行解析，omode|oflags形成如下原始数字；**右侧重编码只是对照，不是实际发生的转换**。

| case_id | 原模式 | 参与的源符号 | 用户整数 HEX（十进制；八进制） | 内核按相同符号重新编码 HEX（十进制；八进制） | last_recognized最后下标 |
|---|---|---|---|---|---:|
| ABI122-01 | `r` | O_RDONLY | 0x0 (0; 00) | 0x0 (0; 00) | 0 |
| ABI122-02 | `w` | O_WRONLY \| O_CREAT \| O_TRUNC | 0x601 (1537; 03001) | 0x241 (577; 01101) | 0 |
| ABI122-03 | `a+` | O_RDWR \| O_CREAT \| O_APPEND | 0x20A (522; 01012) | 0x442 (1090; 02102) | 1 |
| ABI122-04 | `wbx` | O_WRONLY \| O_CREAT \| O_TRUNC \| O_EXCL | 0xE01 (3585; 07001) | 0x2C1 (705; 01301) | 2 |

wbx的b在下标1只写`last_recognized`而不添加O_*位，下标2的x加入用户O_EXCL(0x800)。a+下标1的+把访问模式改O_RDWR，并保留内置`_IO_IS_APPENDING`计算位，不是额外open参数。四种模式在给定首字符、可用指针、NUL正常终止及选定用户头的限定下都进入五参helper；默认oprot为0666。此处没有读入外部musl的实际头，不使用宿主Linux数字替代两份固定源码。

## C：同一个build_open_how，用原值和重编码值各算一次

纯静态helper传入`flags`和`mode=0666`（438），按`mykernel/fs/vfs/open.c:27–41`：先`how.flags=flags & VALID_OPEN_FLAGS`；`how.mode=mode & S_IALLUGO`；若有O_PATH则再按O_PATH_FLAGS剪裁（本八个输入都没有）；若`!WILL_CREATE(how.flags)`则将how.mode清零；返回结构体。源结构中`resolve`未显式写，但C指定初始化其余成员为0，故本给定模型为0。内核有效标志位OR后的数值为`0x7FFFC3`，S_IALLUGO为八进制07777，`0666 & 07777=0666`。WILL_CREATE只测试**内核**的`O_CREAT(0x40)|__O_TMPFILE(0x400000)`，不是用户的0x200。

| case_id / 模式 | 传入种类 | flags原值 | 保留bits | 丢弃bits | WILL_CREATE | how.flags（HEX / DEC） | how.mode（OCT / DEC） | how.resolve |
|---|---|---|---|---|---|---|---|---:|
| ABI122-01 `r` | 原始用户整数 | 0x0 | 0x0 | 0x0 | 否 | 0x0 / 0 | 00 / 0 | 0 |
| ABI122-01 `r` | 内核同名重编（未观察到转换） | 0x0 | 0x0 | 0x0 | 否 | 0x0 / 0 | 00 / 0 | 0 |
| ABI122-02 `w` | 原始用户整数 | 0x601 | 0x601 | 0x0 | 否 | 0x601 / 1537 | 00 / 0 | 0 |
| ABI122-02 `w` | 内核同名重编（未观察到转换） | 0x241 | 0x241 | 0x0 | 是 | 0x241 / 577 | 0666 / 438 | 0 |
| ABI122-03 `a+` | 原始用户整数 | 0x20A | 0x202 | 0x8 | 否 | 0x202 / 514 | 00 / 0 | 0 |
| ABI122-03 `a+` | 内核同名重编（未观察到转换） | 0x442 | 0x442 | 0x0 | 是 | 0x442 / 1090 | 0666 / 438 | 0 |
| ABI122-04 `wbx` | 原始用户整数 | 0xE01 | 0xE01 | 0x0 | 否 | 0xE01 / 3585 | 00 / 0 | 0 |
| ABI122-04 `wbx` | 内核同名重编（未观察到转换） | 0x2C1 | 0x2C1 | 0x0 | 是 | 0x2C1 / 705 | 0666 / 438 | 0 |

**ABI122-01（`r`）：** 原始整数 0x0 在内核宏表中保留0x0，内核所认O_RDONLY(0)，WILL_CREATE=false，mode→00；若按同名内核宏重新编码 0x0，WILL_CREATE=false，mode→00。

**ABI122-02（`w`）：** 原始整数 0x601 在内核宏表中保留0x601，内核所认O_WRONLY / O_TRUNC / O_APPEND，WILL_CREATE=false，mode→00；若按同名内核宏重新编码 0x241，WILL_CREATE=true，mode→0666。

**ABI122-03（`a+`）：** 原始整数 0x20A 在内核宏表中丢弃0x8，保留0x202，内核所认O_RDWR / O_TRUNC，WILL_CREATE=false，mode→00；若按同名内核宏重新编码 0x442，WILL_CREATE=true，mode→0666。

**ABI122-04（`wbx`）：** 原始整数 0xE01 在内核宏表中保留0xE01，内核所认O_WRONLY / O_TRUNC / O_APPEND / O_NONBLOCK / O_NDELAY，WILL_CREATE=false，mode→00；若按同名内核宏重新编码 0x2C1，WILL_CREATE=true，mode→0666。

正反校验：`ABI122-01/r`两种解读都是0且how=(flags0,mode0,resolve0)，属于**相同解读**；`ABI122-02/w`用户原值0x601按内核命名成为O_WRONLY|O_TRUNC|O_APPEND，how.mode被清成0，**并非**同名重编码0x241（O_WRONLY|O_CREAT|O_TRUNC），其mode保留0666。a+的原始0x20A丢掉0x8，保留0x202，转成O_RDWR|O_TRUNC但不创建；同名0x442在内核仍包含O_CREAT，保留0666。wbx原始0xE01逐位成为O_WRONLY|O_TRUNC|O_APPEND|O_NONBLOCK，mode0；同名0x2C1成为O_WRONLY|O_CREAT|O_EXCL|O_TRUNC，mode0666。两组差别来源是**具体数值重合/缺位**，不是宏名自动翻译。

以上止于`build_open_how`产生的`flags/mode/resolve`和其传给`do_sys_openat2`的位置；30号原验收及纠正回执关于下游`build_open_flags`、错误门和部分初始化的限定必须保留。即使WILL_CREATE为真，也只能说明mode在此helper保留，不能据此说真实文件成功创建、附加标志已生效、访问权限获准、fd安装完成或磁盘持久化。

## 证据范围与停写资格

本批新`evidence.md`收集85条固定time真实定义体内短窗，普通位置18项；第121批的69段不算入本批P6。机器件的ABI四案例与C同八行，以及D00十三例、历史C121五例与直接内层A121-03统一case_id。DOT将真实include声明/宏/函数调用、条件桥、整数相等、同名重编码和case特定正反路径分别连线，机械检查端点和禁达关系。time活动C配置为正式短窗；完整本地P9、预处理/编译链接、Graphviz、仓库程序、QEMU、实机、真实文件操作均为NOT_RUN。R120-02、R121-01、R121-02仅报告`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`；C08独立裁定，作者不越权关闭或自行发布123。
