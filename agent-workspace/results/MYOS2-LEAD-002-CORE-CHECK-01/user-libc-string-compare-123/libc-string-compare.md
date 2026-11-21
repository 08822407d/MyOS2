---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-STRING-COMPARE-123
record_type: phase2_libc_string_compare_and_caller_consumption
produced_by: "GPT-6（会话自报；后台与effort不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
owner_decisions_requested_now: []
open_questions: []
inputs_read: [agent-workspace/lead/MYOS2-LEAD-002/173-user-libc-string-compare-contract.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-D00-120-121-review.md, myinitramfs/mylib/myglibc/string/strcmp.c, myinitramfs/mylib/myglibc/string/strncmp.c, myinitramfs/myshell/myshell.c]
---

# 第123批：字符串比较的真实循环与shell实际消费点

源码仅固定 `time@a039d9803ade`。本批分析仓内实现与选择了这一实现的**条件性逐字节纸面模型**，不证明实际交付`sh`链接仓内myglibc。证据中新的SC123-E001～E059为连续定义体短窗，SC123-P01～P15为普通定位。所有调用、返回与输出均未通过实际程序执行；使用合法已知NUL终止字节数组和给定控制分支作有限手工等价模型。

## D00 · R122-01：同一段调用顺序不能误写成直接调用

C08在第122批裁定唯一新开放项为R122-01；R120-02、R121-01、R121-02已正式`CLOSED_DOCUMENTATION_ONLY`，120/121原件与勘误组合`PASS_PENDING_LOCAL`。本轮追加的`libc-string-compare-paths.dot`有独立D00组，用**真实拥有调用点的函数**作为DIRECT_CALL和INDIRECT_CALL边的起点。下面这张表是源码调用现场，不是按图中节点相邻就推导的调用栈。

| 实际调用者 | 被调用者 | 类型 | 固定源码行 | 门或限定 |
|---|---|---|---|---|
| do_sys_open | build_open_how | DIRECT_CALL | open.c:268 | 源中有此调用点，条件另按现场 |
| do_sys_open | do_sys_openat2 | DIRECT_CALL | open.c:269 | 源中有此调用点，条件另按现场 |
| do_creatat | dir->i_op->create | INDIRECT_CALL | namei.c:921 | valid dir i_op and method present |
| do_creatat | done_path_create | DIRECT_CALL | namei.c:922 | method present and callback returns to caller |
| do_creatat | putname | DIRECT_CALL | namei.c:928 | normal/out_putname path, not method-missing early return |
| do_mkdirat | dir->i_op->mkdir | INDIRECT_CALL | namei.c:988 | method present |
| do_mkdirat | done_path_create | DIRECT_CALL | namei.c:989 | method present and callback returns |
| do_mkdirat | putname | DIRECT_CALL | namei.c:995 | normal/out_putname, not method-missing early return |
| rm_command | unlink | DIRECT_CALL | myshell.c:370 | argc==2 |
| rm_command | strerror | DIRECT_CALL | myshell.c:372 | err!=0 |
| rm_command | printf | DIRECT_CALL | myshell.c:372 | err!=0 |
| rmdir_command | rmdir | DIRECT_CALL | myshell.c:356 | argc==2 |
| rmdir_command | strerror | DIRECT_CALL | myshell.c:358 | err!=0 |
| rmdir_command | printf | DIRECT_CALL | myshell.c:358 | err!=0 |
| _IO_new_fopen | _IO_new_file_fopen | DIRECT_CALL | iofopen.c:87 | valid FILE P |
| _IO_new_file_fopen | _IO_file_open | DIRECT_CALL | fileops.c:267 | initial fd -1; accepted first char and suffix loop complete |
| _IO_file_open | open | DIRECT_CALL | fileops.c:178 | entered helper |

必须特别区分：`do_sys_open`在268行调`build_open_how(flags,mode)`，接收返回的`how`；同一个`do_sys_open`在269行调`do_sys_openat2(dfd,filename,&how)`。`build_open_how`本体只**返回how**，并没有调用`do_sys_openat2`。正确模型有`do_sys_open→build_open_how`和`do_sys_open→do_sys_openat2`两条DIRECT_CALL；`build_open_how返回值→how→&how实参`为DATA和SEQUENCE，**没有callee之间的DIRECT_CALL**。旧122的`k_build→k_next` DIRECT_CALL由本表和新图覆盖，旧件不改。

`do_creatat`与`do_mkdirat`各自在i_op方法存在前提下调用相应函数指针，类型INDIRECT_CALL。回到本函数后，依次到`done_path_create`及`putname`，后两者的DIRECT_CALL调用者仍是各自`do_creatat/do_mkdirat`；回调→done→putname只为SEQUENCE，不把回调或done当成下一函数的调用者。缺失方法时直接−EPERM，给定有效dir/i_op但缺create或mkdir方法的D121-A01/B01仍能**在选定用户库正常传回的条件下**到shell本地err和内建0，但不进入回调和任一清理。存在方法的A02/B02可经回调、done及putname，当前仍不是文件落盘证明。

对于`rm_command`和`rmdir_command`，它们分别在合法argc请求`unlink`或`rmdir`，在负值条件中**各自**直接调用`strerror(errno)`与`printf`。`strerror`的字符串返回成为`printf`的第一格式实参，`argv[1]`的文件/目录名成为第二实参，这两条是**DATA**而非`strerror→printf` DIRECT_CALL。两个命令的图分离，不能从rm的案例走到rmdir的输出。实际errno及文本内容未证。

| 错误图边 | 正确替代 |
|---|---|
| `build_open_how` | `do_sys_openat2` | 不能标为DIRECT_CALL；检查拥有调用点的调用者、顺序或数值数据 |
| `dir->i_op->create` | `done_path_create` | 不能标为DIRECT_CALL；检查拥有调用点的调用者、顺序或数值数据 |
| `done_path_create` | `putname` | 不能标为DIRECT_CALL；检查拥有调用点的调用者、顺序或数值数据 |
| `dir->i_op->mkdir` | `done_path_create` | 不能标为DIRECT_CALL；检查拥有调用点的调用者、顺序或数值数据 |
| `strerror` | `printf` | 不能标为DIRECT_CALL；检查拥有调用点的调用者、顺序或数值数据 |
| `case_id` | `_IO_new_fopen` | 不能标为DIRECT_CALL；检查拥有调用点的调用者、顺序或数值数据 |
| `_IO_file_open` | `_IO_new_fopen` | 不能标为DIRECT_CALL；检查拥有调用点的调用者、顺序或数值数据 |

R121相关返回层次：case_id→给定函数现场是MODEL边，不是来自case节点的源级直接函数调用。C121-01的`open`给定−1，`_IO_file_open`返回NULL，`_IO_new_file_fopen`第339行返回其`result`即NULL，`_IO_new_fopen`第87行转交NULL——三个层级的返回不能折叠为“open直接返回给包装器”。C121-04的`q`首字母在**解析函数**返回NULL，五参helper及open都未参与；C121-05和A121-03直接调用已有`_fileno=7`的解析函数，外层包装、helper及open均未参与。R122-01仅`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，主线C08负责判闭。本轮不重做122批四种模式和八组how数值；C08的低两位解码0/1/2、原始用户0x8不被内核有效位接受和0x800为NONBLOCK/NDELAY同位别名沿用。

## A · strcmp：读取、停止、返回的确切整数

`myglibc/string/strcmp.c`先`#undef strcmp`，若未外部定义STRCMP则`#define STRCMP strcmp`，真实函数头是`int STRCMP(const char *p1,const char *p2)`。它将两个`const char*`指针分别转为`const unsigned char*`，每轮**先读c1再读c2**，两指针各后置递增一字节。若c1为NUL，立即返回`c1-c2`；否则若c1==c2继续循环，出现差异才在循环后返回`c1-c2`。这里的返回是实际字节差值，不只是抽象−1/0/+1，虽上层常只需要等零、正负符号。

| case_id | 类型 | p1字节（含00终止） | p2字节（含00终止） | 读取对：索引 HEX/HEX | 对数 | 决定下标 | 停止原因 | 仓内返回 |
|---|---|---|---|---|---:|---:|---|---:|
| A123-01 | same | `70 77 64 00` | `70 77 64 00` | 0:0x70/0x70 · 1:0x77/0x77 · 2:0x64/0x64 · 3:0x00/0x00 | 4 | 3 | FIRST_STRING_NUL | 0 |
| A123-02 | first_byte | `63 61 74 00` | `62 61 74 00` | 0:0x63/0x62 | 1 | 0 | BYTE_DIFFERENCE | 1 |
| A123-03 | prefix_s1 | `72 6d 00` | `72 6d 64 69 72 00` | 0:0x72/0x72 · 1:0x6D/0x6D · 2:0x00/0x64 | 3 | 2 | FIRST_STRING_NUL | -100 |
| A123-04 | prefix_s2 | `72 6d 64 69 72 00` | `72 6d 00` | 0:0x72/0x72 · 1:0x6D/0x6D · 2:0x64/0x00 | 3 | 2 | BYTE_DIFFERENCE | 100 |
| A123-05 | high_unsigned_char | `80 00` | `7f 00` | 0:0x80/0x7F | 1 | 0 | BYTE_DIFFERENCE | 1 |
| A123-06 | same_short | `63 64 00` | `63 64 00` | 0:0x63/0x63 · 1:0x64/0x64 · 2:0x00/0x00 | 3 | 2 | FIRST_STRING_NUL | 0 |

例如输入`rm`与`rmdir`，先读r/r、m/m，第三对读0/d(0x64)，返回−100：它**不认为短前缀等于完整名称**。对相反方向返回+100。高位字节例是`0x80`对`0x7F`，因为unsigned char转换返回+1；不从中推导Unicode或语言排序，也不把宿主`strcmp`实现的具体差值强加到实际sh。

仓内`string/string.h:155–161`只有函数声明；`include/string.h:62`接入它，`string/CMakeLists.txt`把*.c列为mylibc目标源码。然而已读顶层initramfs CMake在project后设置仓外musl-gcc路径，只加入myinit、myshell，不足以证明交付sh已链接仓内`STRCMP`。因此下表的**实际shell条件分支**可依源码比较是否为零，**具体整数和字节对数**仅在选择仓内实现且输入与模型一致时成立。

## B · strncmp：四次依次检查，预算耗尽与遇NUL分别停止

`strncmp.c`同样`#undef strncmp`并以`STRNCMP`默认别名。入口定义`unsigned char c1='\\0',c2='\\0'`，所以`n=0`在合法指针但未解引用时最终`c1-c2=0`。当`n>=4`，先令`n4=n>>2`，每组**按顺序**读四对；每一对后均检查`c1==NUL || c1!=c2`，满足立即返回差值。只在完成整个组后执行`--n4`，四字节组全部执行完才把`n &=3`留给尾部`while(n>0)`。尾部每次读取一对、检查停止，然后`n--`；用完预算时返回最后一次`c1-c2`。

| case_id | 两个合法字符串 | n | 组进入/完整组数 | 尾部实际读数 | 合计读对 | 实际字节对 HEX/HEX | 决定/最后下标 | 出口 | 返回值 |
|---|---|---:|---|---:|---:|---|---:|---|---:|
| B123-01 | `abcdX` / `abcdY` | 0 | 0 / 0 | 0 | 0 |  | 无 | N_EXHAUSTED | 0 |
| B123-02 | `abcdX` / `abcdY` | 3 | 0 / 0 | 3 | 3 | 0:0x61/0x61 · 1:0x62/0x62 · 2:0x63/0x63 | 2 | N_EXHAUSTED | 0 |
| B123-03 | `abcdX` / `abcdY` | 4 | 1 / 1 | 0 | 4 | 0:0x61/0x61 · 1:0x62/0x62 · 2:0x63/0x63 · 3:0x64/0x64 | 3 | N_EXHAUSTED | 0 |
| B123-04 | `abcdX` / `abcdY` | 5 | 1 / 1 | 1 | 5 | 0:0x61/0x61 · 1:0x62/0x62 · 2:0x63/0x63 · 3:0x64/0x64 · 4:0x58/0x59 | 4 | BYTE_DIFFERENCE | -1 |
| B123-05 | `abXd` / `abYd` | 4 | 1 / 0 | 0 | 3 | 0:0x61/0x61 · 1:0x62/0x62 · 2:0x58/0x59 | 2 | BYTE_DIFFERENCE | -1 |
| B123-06 | `abc` / `abcd` | 4 | 1 / 0 | 0 | 4 | 0:0x61/0x61 · 1:0x62/0x62 · 2:0x63/0x63 · 3:0x00/0x64 | 3 | FIRST_STRING_NUL | -100 |
| B123-07 | `abc` / `abc` | 5 | 1 / 0 | 0 | 4 | 0:0x61/0x61 · 1:0x62/0x62 · 2:0x63/0x63 · 3:0x00/0x00 | 3 | FIRST_STRING_NUL | 0 |
| B123-08 | `abcdefgh` / `abcdefghi` | 8 | 2 / 2 | 0 | 8 | 0:0x61/0x61 · 1:0x62/0x62 · 2:0x63/0x63 · 3:0x64/0x64 · 4:0x65/0x65 · 5:0x66/0x66 · 6:0x67/0x67 · 7:0x68/0x68 | 7 | N_EXHAUSTED | 0 |
| B123-09 | `a` / `b` | 1 | 0 / 0 | 1 | 1 | 0:0x61/0x62 | 0 | BYTE_DIFFERENCE | -1 |

这解释两种“返回0”不可混淆：`strcmp("abcdX","abcdY")`会读到第5个不同字节并返回−1，但`strncmp`给定n=4时只读前4对、预算用尽返回0。另一种是遇两串的共同NUL提前返回0，例如B123-07在n=5、第四对遇NUL时停止。B123-05则在n=4的一组中**第三次检查**就退出，绝不补读第4对；B123-06在第四次检查遇首串NUL即返回负差值。没有用仓内代码进行编译运行，这些是逐语句复算。

## C · shell的三个真实strcmp调用现场

### C1 find_cmd：线性十项全名查找

实际十项静态顺序是`cd, ls, pwd, cat, touch, rm, mkdir, rmdir, exec, reboot`。`find_cmd(cmd)`以`i=0`顺序调用`strcmp(cmd,shell_internal_cmd[i].name)`，结果**等0**立刻返回该索引，否则继续，十项都没等零才返回−1。它用的是`strcmp`，**没有调用`strncmp`**；故短前缀`pw`不会被误认为`pwd`，`rm`与`rmdir`共享前两字母也不影响全名匹配。下面每个括号是`(比较器返回整数,本次读取的字节对数)`，顺序覆盖从索引0起到命中或耗尽。

| case_id | 输入命令 | 实际依次比较的索引:表项(返回,读对) | strcmp实际调用数 | 字节对总数 | find_cmd返回 |
|---|---|---|---:|---:|---:|
| C123-F01 | `pwd` | 0:cd(13,1)；1:ls(4,1)；2:pwd(0,4) | 3 | 6 | 2 |
| C123-F02 | `pw` | 0:cd(13,1)；1:ls(4,1)；2:pwd(-100,3)；3:cat(13,1)；4:touch(-4,1)；5:rm(-2,1)；6:mkdir(3,1)；7:rmdir(-2,1)；8:exec(11,1)；9:reboot(-2,1) | 10 | 12 | -1 |
| C123-F03 | `rm` | 0:cd(15,1)；1:ls(6,1)；2:pwd(2,1)；3:cat(15,1)；4:touch(-2,1)；5:rm(0,3) | 6 | 8 | 5 |
| C123-F04 | `rmdir` | 0:cd(15,1)；1:ls(6,1)；2:pwd(2,1)；3:cat(15,1)；4:touch(-2,1)；5:rm(100,3)；6:mkdir(5,1)；7:rmdir(0,6) | 8 | 15 | 7 |
| C123-F05 | `rmd` | 0:cd(15,1)；1:ls(6,1)；2:pwd(2,1)；3:cat(15,1)；4:touch(-2,1)；5:rm(100,3)；6:mkdir(5,1)；7:rmdir(-105,4)；8:exec(13,1)；9:reboot(8,2) | 10 | 16 | -1 |
| C123-F06 | `cd` | 0:cd(0,3) | 1 | 3 | 0 |

**调用次数不等于读字节次数**：例如`pwd`是3次strcmp、总共6个字节对；`rmdir`是8次strcmp、总共15个字节对；`rm`命中索引5，不必比较后面的`rmdir`。负返回−1在main走“Input Error, No Command Found!”的printf请求，不调用run_cmd；非负索引走run_cmd和命令表函数指针，运行后状态不在本模型授权范围。

### C2 cd_command：只有完全等于单独点号才跳过chdir

给定argc=2且路径来自有效`argv[1]`，`cd_command`用**实参顺序**`strcmp(".",path)`，并在返回等零时直接`return 1`。不是按字面是否“以点开头”判断。

| case_id | path | strcmp实际实参顺序 | 实际读取 HEX/HEX | 返回整数 | 读对 | 直接return 1 | 到达chdir请求 |
|---|---|---|---|---:|---:|---|---|
| C123-D01 | `.` | `(".", ".")` | 0:0x2E/0x2E · 1:0x00/0x00 | 0 | 2 | 是 | 否 |
| C123-D02 | `./` | `(".", "./")` | 0:0x2E/0x2E · 1:0x00/0x2F | -47 | 2 | 否 | 是，仅请求 |

`path="."`完全相等，直接返回1，不执行chdir。`path="./"`第二对为NUL/斜杠0x2F，返回−47，故到`chdir(path)`请求点；**不能**据此说已经改变工作目录。后续chdir值、缓存更新和错误行需要额外给定结果，本轮不赋予实际值。

### C3 ls_command：先比较点号，&&短路控制第二个比较

假设`opendir`已成功且`readdir`返回一个`d_name`可读、NUL终止的有效记录，然后下一次返回NULL从循环退出。源码仅当第一次`strcmp(buf->d_name, ".") !=0`为真，才评估第二次`strcmp(buf->d_name, "..") !=0`；只有两次都非零才请求`printf("%s\\t", buf->d_name)`。循环结束的`printf("\\n")`单独发生一次，不应计作名字输出。

| case_id | d_name | 第一次返回 / 读对 | 第二次调用？ | 第二次返回 / 读对 | 名称printf请求 | 循环结束LF请求 |
|---|---|---|---|---|---:|---:|
| C123-L01 | `.` | 0 / 2 | 否 | 未调用 / NULL | 0 | 1 |
| C123-L02 | `..` | 46 / 2 | 是 | 0 / 3 | 0 | 1 |
| C123-L03 | `.config` | 99 / 2 | 是 | 53 / 2 | 1 | 1 |

点号记录第一次比较为0，第二次根本不调用——它的机器字段必须为NULL，不能把`strcmp(".","..")`的推测值填成“执行结果”。双点号第一次返回46，第二次返回0；`.config`第一次返回99，第二次返回53，所以只有第三例请求一次名称输出。这里的LF是给定目录流结束后的一次独立请求，不证明实际设备全部显示或目录记录真实存在。

## 可用性与交回边界

正文、facts、替代DOT共享`A123-*`、`B123-*`、`C123-F/D/L*`与旧D00 case_id。DOT小型替代图只在D00修正真实源函数直接调用归属与SEQUENCE/DATA，新增A/B/C的字节与短路模型，不重画旧122的217节点/八个how。新P2短窗与普通位置分开；旧C08历史文档闭合一律沿用且不自签R122-01。完整本地P9、编译/链接、仓库程序、Graphviz、QEMU/实机及真实文件操作均NOT_RUN。作者只向C08提交`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。
