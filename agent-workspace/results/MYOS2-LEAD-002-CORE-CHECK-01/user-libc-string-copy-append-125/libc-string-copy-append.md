---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-STRING-COPY-APPEND-125
record_type: source_pinned_copy_append_and_shell_path_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
conversation_display_name: "MYOS2-A-C08b 代码分析执行"
produced_by: "GPT-6（会话自报；实际后台/effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（176号任务书指定）"
date: 2026-11-22
date_timezone: Asia/Shanghai
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00: NO_OPEN_CORRECTIONS_ASSIGNED
open_questions: []
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/176-user-libc-string-copy-append-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-D00-123-review.md"
  - "myinitramfs/mylib/myglibc/string/strcpy.c"
  - "myinitramfs/mylib/myglibc/string/strcat.c"
  - "myinitramfs/mylib/myglibc/string/strlen.c"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "myinitramfs/myshell/myshell.c"
---

# 第125批｜`strcpy` / `strcat` / `cat_command` 定点源码模型

**分析前提。** 固定代码 `time@a039d9803ade`（短12），只选择仓内这些函数的默认别名和给定的普通合法对象作静态纸面核算。给定ASCII输入、源和目的互不重叠、所有字符串已正确NUL终止、目的容量足够；源侧每次提供至少16字节、8字节对齐的同一充分可读对象，使所选`strlen`的整字读取留在对象内。C的`filename`由给定成功的`malloc`获得8字节对齐、容量恰为请求值。真实sh链接到哪套libc、这些函数的有效预处理别名及实际文件系统效果为UNKNOWN。C的求值停在270行`open`请求，不重做117的read/printf路径。

第124批已由C08裁定`PASS_PENDING_LOCAL`；R123-01已`CLOSED_DOCUMENTATION_ONLY`，因此本批 **D00=NONE**，不重复返修。

## A · `strcpy` 复制首NUL与返回原目的指针

固定`strcpy.c`默认`STRCPY=strcpy`，本体为`return memcpy(dest,src,strlen(src)+1);`。由`STRCPY`函数本体直接调用`strlen(src)`，得到不含首NUL的长度`s`；将`s+1`作为**字节复制量**交给同一函数本体所调用的`memcpy`。多出的`1`专门覆盖源的第一个终止字节`0x00`；`memcpy.c:59`返回其原始目的实参`dstpp`，所以STRCPY也返回最初传入的`dest`，不是复制区间的末尾地址。字符串中的NUL后的字节不在复制范围。

目的起始数组全部是非零哨兵`0x23`（'#'），每一例重新初始化；A125-03把`dest`定义为一个10字节对象D内部的`D+2`，而非把整个对象D当作传入目的指针。下表完整显示目的对象的前后字节，区间为从**对象D起点**计的半开区间：

| case_id | 源字节（到首NUL） | strlen(src) | memcpy字节数 | 实际目的写区 | 目的前 → 后（HEX） | STRCPY返回 |
|---|---|---:|---:|---|---|---|
| A125-01 | `00` | 0 | 1 | [0,1) | `23 23 23 23 23 23 23 23` → `00 23 23 23 23 23 23 23` | D |
| A125-02 | `63 61 74 00` | 3 | 4 | [0,4) | `23 23 23 23 23 23 23 23` → `63 61 74 00 23 23 23 23` | D |
| A125-03 | `68 69 00` | 2 | 3 | [2,5) | `23 23 23 23 23 23 23 23 23 23` → `23 23 68 69 00 23 23 23 23 23` | D+2 |

A125-01空源仍复制1个终止字节；A125-02源`cat\0`复制4字节，末尾0在目标下标3；A125-03仅写D的下标[2,5)，两侧哨兵保持，返回`D+2`而不是`D`。对照已核的C124-03：同样给定`cat`，`strncpy(D,src,3)`只复制`63 61 74`，**不写**下标3的终止NUL。本批不把`strncpy`误当作可以替换`strcpy`的同一复制语义。

## B · `strcat` 的旧NUL、内外两层返回地址

固定`strcat.c`默认`STRCAT=strcat`；实现先由STRCAT实例调用`strlen(dest)`取得旧目的字符串长度d，再由**同一个STRCAT实例**调用`strcpy(dest+d,src)`。内层STRCPY实例又调用`strlen(src)`和`memcpy`，并返回`dest+d`。然而STRCAT的源码没有把这次内层返回作为自己的结果，而是在下一行`return dest;`，即**始终返回原外层起点D**。当d=0两指针数值碰巧相同；d>0则差d字节。

每组使用独立、已正确终止的目的8字节对象D，尾部哨兵0x23、源在单独的有效对象；源至少含首个NUL。写区覆盖旧终止零并把新的NUL写到下标`d+s`。复制数为`s+1`，区间`[d,d+s+1)`：

| case_id | 旧目的 / 追加源 | d / s | NUL旧→新下标 | 内层目标及写区 | 目的完整前→后（HEX） | 内层/外层返回 |
|---|---|---|---|---|---|---|
| B125-01 | `∅` / `xy` | 0 / 2 | 0 → 2 | D, [0,3) | `00 23 23 23 23 23 23 23` → `78 79 00 23 23 23 23 23` | D / D |
| B125-02 | `ab` / `∅` | 2 / 0 | 2 → 2 | D+2, [2,3) | `61 62 00 23 23 23 23 23` → `61 62 00 23 23 23 23 23` | D+2 / D |
| B125-03 | `ab` / `cd` | 2 / 2 | 2 → 4 | D+2, [2,5) | `61 62 00 23 23 23 23 23` → `61 62 63 64 00 23 23 23` | D+2 / D |

B125-01空目的+非空源，d=0，内外指针均D；B125-02`ab`+空源，d=2，虽内容不变仍在旧NUL位置执行1字节复制，内层返回D+2，外层返回D；B125-03`ab`+`cd`，内层从D+2复制`63 64 00`，得到`abcd\0`，内层返回D+2，外层D。每例源/目的不重叠且目标哨兵区没有写入。不能误标成`strlen→strcpy`直接调用或`strcpy→STRCAT`返回边；调用拥有者必须分层记录。

## C · `cat_command` 在容量内构造传给 `open` 的路径

固定`argc=2`、`argv[1]`有效、`current_dir`已终止，`len`与`i`均落在int可表示范围内，给定`malloc(i+2)`成功且所分配区域F有整整请求的容量。shell在260/261行直接发出两次`strlen`，`len=strlen(current_dir)`，`i=len+strlen(argv[1])`，请求`i+2`字节；263行`memset(F,0,i+2)`后才开始使用目的字节。源字符串各独立、8字节对齐且至少16字节可读；**源存储区不是malloc目的容量**：

| case_id | cwd源字节（含NUL） | 参数源字节（含NUL） | 源存储容量 cwd/argv | filename分配字节 |
|---|---|---|---:|---:|
| C125-01 | `2f 00` | `72 65 61 64 6d 65 00` | 16/16 | 9 |
| C125-02 | `2f 62 69 6e 00` | `68 69 00` | 16/16 | 8 |
| C125-03 | `2f 62 69 6e 00` | `2f 68 69 00` | 16/16 | 9 |

| case_id | current_dir + argv[1] | 两次显式strlen结果 | len / i / i+2 | len>1分支 | 最终路径 |
|---|---|---|---|---|---|
| C125-01 | `/` + `readme` | 1, 6 | 1, 7, 9 | 跳过 | `/readme` |
| C125-02 | `/bin` + `hi` | 4, 2 | 4, 6, 8 | 写F[4]=2f | `/bin/hi` |
| C125-03 | `/bin` + `/hi` | 4, 3 | 4, 7, 9 | 写F[4]=2f | `/bin//hi` |

以下各阶段列出F容量**内全部字节**；每步以前一步结果为输入，不拿`malloc`尚未初始化时的内容当零。首次复制`strcpy(F,current_dir)`包括cwd的终止零，故第一复制后的终止零在`F[len]`。若`len>1`，shell在266行将该旧NUL覆写为`'/'`；此前263行已清零的下一个字节`F[len+1]`此时提供新的终止零，然后`strcat`才能从该处追加。根目录`len=1`跳过斜杠分支，原`F[1]=0`继续作为旧尾端。

| case_id | 操作阶段 | F的完整HEX | 该时刻首NUL下标 | 本步目的写区 |
|---|---|---|---:|---|
| C125-01 | 清零后 | `00 00 00 00 00 00 00 00 00` | 0 | [0,9) |
| C125-01 | 首次strcpy后 | `2f 00 00 00 00 00 00 00 00` | 1 | [0,2) |
| C125-01 | 条件斜杠后 | `2f 00 00 00 00 00 00 00 00` | 1 | 无 |
| C125-01 | strcat后 | `2f 72 65 61 64 6d 65 00 00` | 7 | [1,8) |
| C125-02 | 清零后 | `00 00 00 00 00 00 00 00` | 0 | [0,8) |
| C125-02 | 首次strcpy后 | `2f 62 69 6e 00 00 00 00` | 4 | [0,5) |
| C125-02 | 条件斜杠后 | `2f 62 69 6e 2f 00 00 00` | 5 | [4,5) |
| C125-02 | strcat后 | `2f 62 69 6e 2f 68 69 00` | 7 | [5,8) |
| C125-03 | 清零后 | `00 00 00 00 00 00 00 00 00` | 0 | [0,9) |
| C125-03 | 首次strcpy后 | `2f 62 69 6e 00 00 00 00 00` | 4 | [0,5) |
| C125-03 | 条件斜杠后 | `2f 62 69 6e 2f 00 00 00 00` | 5 | [4,5) |
| C125-03 | strcat后 | `2f 62 69 6e 2f 2f 68 69 00` | 8 | [5,9) |

**分层地址与返回（每例均到open请求为止）：**

| case_id | F原地址 | 第一次strcpy复制字节 | strcat求得原目的长d | 内层strcpy的dest | 内层复制字节 | 内层strcpy返回 | 外层strcat返回 | open实参 |
|---|---|---:|---:|---|---:|---|---|---|
| C125-01 | F | 2 | 1 | F+1 | 7 | F+1 | F | `open(F,O_RDONLY,0)` |
| C125-02 | F | 5 | 5 | F+5 | 3 | F+5 | F | `open(F,O_RDONLY,0)` |
| C125-03 | F | 5 | 5 | F+5 | 4 | F+5 | F | `open(F,O_RDONLY,0)` |

给定`/bin`与`hi`（C125-02），shell显式两次strlen返回4和2；shell直接发出`strcpy(F,current_dir)`，其内部strlen(current_dir)=4、memcpy长度5、返回F；shell的`F[4]='/'`写入使之前清零的`F[5]`成为当前NUL；shell直接发出`strcat(F,argv[1])`，STRCAT内部strlen(F)=5，再调用内层strcpy(F+5,argv[1])，该内层strlen(argv[1])=2，memcpy长度3、返回F+5；STRCAT自身返回F。之后shell在270行请求`open(F,O_RDONLY,0)`，此时F内容为`/bin/hi\0`。**总strlen五次**：shell直接两次，第一次STRCPY内部一次，STRCAT内部一次、后一个STRCPY内部一次。**总strcpy两次**：shell直接一次、STRCAT内部一次；两个memcpy都属于各自的STRCPY，而非shell的直接调用。shell另直接调用malloc、memset、strcat和open各一次，条件分隔符是赋值而不是函数调用。

C125-01根目录加`readme`变成`/readme`，没有多写分隔符，最后一个NUL下标7、容量9仍留1字节零；C125-03给定`argv[1]="/hi"`，追加源中**自带的斜杠**仍被作为首字节复制，与shell写入的斜杠相邻，形成`/bin//hi`，不会自动规范化。C125-02与C125-03的NUL分别在下标7和8，恰好位于分配区最后一个字节。上表将每条`open`的实参值止于**请求阶段**；实际解析、打开结果、打开的目标及错误处理为UNKNOWN。

## 调用图、构建与停止边界

`libc-string-copy-append-paths.dot`为九个case各自的实例图：每条`DIRECT_CALL_INSTANCE`只由真实函数实例发出；长度/指针返回用`RETURN_DATA`，调用实参与算术传递用`DATA`，局部先后用`SEQUENCE`，斜杠判断走`CONTROL`，目的缓冲区改变走`WRITE`，末尾停在`BOUNDARY`。对根目录例，图保留潜在斜杠写节点但从其案例入口不可达，不用不存在的虚构端点替代否证。Graphviz与实际程序执行均未运行。

仓内`mylib/myglibc/CMakeLists.txt`建立STATIC mylibc并加入string源码，`myinitramfs/CMakeLists.txt`文本设置musl-gcc路径且添加myshell；`myshell/CMakeLists.txt`创建sh目标。三者不能直接推出最终sh所链接的STRCPY、STRCAT与strlen版本；源宏重命名、符号表和完整链接回执均为UNKNOWN。编译、预处理、Graphviz、QEMU、实机、真实文件访问与完整本地P9均`NOT_RUN`；任何对操作系统实际运行效果的判定留给以后经授权的验证，不由本批假定。作者只提交`READY_FOR_MAINLINE_REVIEW`，由C08 Pro独立验收。
