---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SHELL-DATETIME-FORMAT-129
record_type: phase2_shell_datetime_format_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
conversation_display_name: "MYOS2-A-C08b 代码分析执行"
produced_by: "GPT-6（会话可知；后台具体型号与effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（180号任务书指定）"
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
execution_parent_short12: 4a8952c6c956
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
open_returns: [R127-02, R128-01]
owner_decisions_requested_now: []
open_questions: []
supersedes:
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml"
---

# 第129批 · shell启动日期、六位微秒及printf提交的字节

**本批只完成固定源码和有界输入的静态取证，未验证真实运行。** 入口为`time@a039d9803ade`中的`myshell.c:66-76`及`tests.c:104-127`；候选用户libc为仓内`stdio/printf.c`和CRLF源文件`stdio/vsnprintf.c`。给定`gettimeofday`成功、`localtime`返回有效指针、`strftime`成功把`2026-11-22 09:08:07`这19个ASCII字符和NUL写入64字节buffer。该日期是合同定义的**模型输入**，不是本批读取到的实际系统时间，时区和实际sh链接库不做推断。

## D00 · R127-02 与 R128-01 两项历史文档补正

**更正位置与保留边界。** 第127批两个旧返回和第128批原文件、原评论均冻结；本批仅在新目录给定点替代映射，机器字段和图件可从原件重建。R127-01已经C08裁定`CLOSED_DOCUMENTATION_ONLY`，不再修改。两项当前都为`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，只有原C08 Pro可以裁定关闭。

### D129-R127-02：43个写节点、43条数据来源边的寄存变量

旧第128批D00补正图`D00.R127_02.graph`共有224节点、254条边，43个`DATA_REGISTER`的源读→目标写端点和访存顺序已被C08认可，但**43个写节点标签及43条边的说明文本缺少真实暂存变量名**。逐例以第127批固定facts`A_entry.cases[].ordered_trace`中的`WRITE.from_reg`、配对READ变量及`wordcopy.c`赋值表达式复核，生成精确选择器、旧值、替代值和原始寄存变量。下表数据来源于机器差量（例名完整，不跨案例重用序号）。

| 第127批入口案例 | 写节点与来源边数 | 真实变量逐目标顺序 |
|---|---:|---|
| `A127-00` | 0 | 零字无写 |
| `A127-01` | 1 | D0←a1 |
| `A127-02` | 2 | D0←a0、D1←a1 |
| `A127-07` | 7 | D0←a1、D1←a0、D2←a1、D3←a0、D4←a1、D5←a0、D6←a1 |
| `A127-08` | 8 | D0←a0、D1←a1、D2←a0、D3←a1、D4←a0、D5←a1、D6←a0、D7←a1 |
| `A127-09` | 9 | D0←a1、D1←a0、D2←a1、D3←a0、D4←a1、D5←a0、D6←a1、D7←a0、D8←a1 |
| `A127-16` | 16 | D0←a0、D1←a1、D2←a0、D3←a1、D4←a0、D5←a1、D6←a0、D7←a1、D8←a0、D9←a1、D10←a0、D11←a1、D12←a0、D13←a1、D14←a0、D15←a1 |

共**43对**，其中`a0`为20、`a1`为23。旧`from undefined`改为对应的`from a0/a1`，旧`stored in undefined`改为`stored in a0/a1`，86个真实输出字段逐一消去；原本通过的43个端点、数据边类型、入口先预读再goto、16字case的do/while回边保留。

额外补正12个已有的B/C节点标签：B127-02/08/16的switch必须明确`len%8`依次为2/0/0且先选择`case 2/case 0/case 0`，后续预读、局部游标与len调整完才显式跳至`do1/do7/do7`；B循环体摘要不包含外部`do0`最终写回，len=0的do/while假分支**顺序落入**do0。C127-M16-U3的一字helper在case1先读a1并调整后，`goto do0`立即最终写D[136,144)，**没有do8..do1循环迭代**。原三个C例各一条MEMCPY拥有的helper调用、四个宏位置及`dstpp`返回来源保留。

消费端从`facts-and-boundaries.yaml::D00.R127_02`的`register_pairs`和`additional_node_labels`按完整节点ID执行替换；图`shell-datetime-format-paths.dot`的R127部分为43条读写数据来源的**精确差量视图**，不是复制整份历史127图。新视图总历史节点/边不变224/254，标签缺失降至零。

### D129-R128-01：每次外层只一次helper，写账不重复

**旧固定父为`4a8952c6c956`。** 精确修改目标是`user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml::C_outer_memcpy.cases[i].byte_copy_trace`及旧DOT的三个`C128_*`簇。旧账把同一次整段helper按目的整字数重复登记；新的单层事件表保留原前导与尾部逐字节事件，只保留一条helper调用事件，并在其`helper_word_writes`子表记录各实际整字存储。

| 原case_id | 原整段helper事件数 | 原账宽度和 | 更正后的调用数 | helper实际整字写 | 更正后账宽 | 新事件总行 |
|---|---:|---:|---:|---:|---:|---:|
| `C128-M16-S129-D128` | 2 | 32 | 1 | 2 | 16 | 1 |
| `C128-M16-S130-D131` | 1 | 16 | 1 | 1 | 16 | 9 |
| `C128-M41-S131-D128` | 5 | 201 | 1 | 5 | 41 | 2 |

正确三次写入总宽为**16、16、41**，每个请求目标写索引集合大小恰等于长度，区间不重叠、不遗漏。第二例的五个前导字节、一次8字节helper和三个尾部字节仍在总宽16内，完整保留。

对原三个`C128`图簇只调整**18条既有边的类型/说明**、新增3个原始`dstpp`来源节点及12条边；每例从MEMCPY出发的**四条宏位置边和一条DIRECT_CALL_INSTANCE helper调用边完全保留**。原阶段相邻的`MACRO_EXPANSION`类型改为`SEQUENCE`，`helperreturn→callerupdate`改为返回后顺序，更新数值由**外层MEMCPY保存的游标与WORD字节数**提供；`tail→result`仅为顺序边，`return dstpp`的真实来源新增为入口原始参数。图中3例的详细源字读取和MERGE数据边继续保留。新机器补正对原端点、边类型及旧/新字段都提供精确替换选择器，不能在未独立复核时声明旧128已PASS。

## A · 启动调用中时间数据与两个格式的边界

`myshell.c::main`在第68行直接调用`test_PrintDateTime()`，**没有承接返回值**；之后的两个问候printf等调用另属后续语句。`test_PrintDateTime`栈内建立`struct timeval tv`、`struct tm *tm_info`和`char buffer[64]`。第110行`gettimeofday(&tv,NULL)`若返回-1，则请求`perror("gettimeofday failed")`并立即`return 1`，不进入正常localtime/strftime/printf路径；main仍不读取该返回值。本包不推断perror最终写何处或真实失败率。

在给定正常返回时：`tv.tv_sec`通过`localtime(&tv.tv_sec)`成为tm_info输入，`strftime(buffer,64,"%Y-%m-%d %H:%M:%S",tm_info)`用它组装19字节日期文本和`buffer[19]=NUL`；该`strftime`调用返回值**在tests.c未被检查**，本包按给定成功的19字节输出继续。第三个时间字段`tv.tv_usec`不参与本段日期字符串，作为`printf`的第二个格式化实参进入`%06ld`；`buffer`则对应`%s`。这是分别来自不同函数的格式：
- `strftime`格式负责年-月-日、小时-分钟-秒，**不是**printf直接解析；
- `printf("DateTime Now: %s.%06ld\n",buffer,tv.tv_usec)`负责字面前缀、字符串、句点、六位微秒及换行。

`test_PrintDateTime`忽略printf的返回，正常`return 0`。这条静态链不能证明真实localtime时区、真实gettimeofday读数或sh实际链接此stdio实现。

## B · vsnprintf如何处理%s及%06ld

`printf`使用局部`buf[4096]`和变参`ap`，第18行`rv=vsnprintf(buf,4096,fmt,ap)`。`vsnprintf`维护格式游标p、缓冲游标q、已生成字符数o、`flags/rank/width/prec`和状态`st_normal/st_flags/st_width/st_prec/st_modifiers`。本格式有23个字符（含最后一个换行、**不含格式自身NUL**）；状态切换与p的回退可按机器记录的27个解析轮次复核。

**两个转换位置及重读。** 在第14个字符偏移看到`%`，重置flags=0、rank=rank_int、width=0、prec=-1，转`st_flags`。下个`s`并不是flag或width，故`p--`两次、同一个`s`分别由`st_flags`→`st_width`→`st_modifiers`重复读取；最后消费`va_arg(ap,const char *)`，`strlen(sarg)=19`，循环`EMIT`19个字符，**不复制日期源的NUL**。句点是普通态的一个字面EMIT。

第二个`%`再一次**重置**状态；`0`在`st_flags`设置`FL_ZERO`，`6`第一次由flags判非标志而`p--`，重读为`st_width`中的十进制宽度6；`l`先由width遇到非数字而`p--`，再由`st_modifiers`将rank从int加至long；`d`进入带符号十进制`base=10`且`FL_SIGNED`，按rank_long消费**`va_arg(ap,signed long)`**，然后`goto is_integer`。给定`tv.tv_usec`与这个取参类型兼容且值非负，不分析真实不匹配变参类型的情况。

| 源格式偏移 | 当前字符 | 状态转移 | 此轮p回退 | 结果width/rank/flags | q变化 |
|---:|---|---|---|---|---|
| 14 | `%` | `st_normal` → `st_flags` | 否 | 0 / 0 / 0 | 14→14 |
| 15 | `s` | `st_flags` → `st_width` | 是 | 0 / 0 / 0 | 14→14 |
| 15 | `s` | `st_width` → `st_modifiers` | 是 | 0 / 0 / 0 | 14→14 |
| 15 | `s` | `st_modifiers` → `st_normal` | 否 | 0 / 0 / 0 | 14→33 |
| 16 | `.` | `st_normal` → `st_normal` | 否 | 0 / 0 / 0 | 33→34 |
| 17 | `%` | `st_normal` → `st_flags` | 否 | 0 / 0 / 0 | 34→34 |
| 18 | `0` | `st_flags` → `st_flags` | 否 | 0 / 0 / 1 | 34→34 |
| 19 | `6` | `st_flags` → `st_width` | 是 | 0 / 0 / 1 | 34→34 |
| 19 | `6` | `st_width` → `st_width` | 否 | 6 / 0 / 1 | 34→34 |
| 20 | `l` | `st_width` → `st_modifiers` | 是 | 6 / 0 / 1 | 34→34 |
| 20 | `l` | `st_modifiers` → `st_modifiers` | 否 | 6 / 1 / 1 | 34→34 |
| 21 | `d` | `st_modifiers` → `st_normal` | 否 | 6 / 1 / 65 | 34→40 |
| 22 | LF | `st_normal` → `st_normal` | 否 | 6 / 1 / 65 | 40→41 |

格式转换时`is_integer: sz=format_int(q,n-o,val,flags,10,width,prec)`返回本地字符长度，本包四例均`sz=6`，外层`q+=6/o+=6`。这里的`format_int`不是printf之外又执行的系统调用；`EMIT`是格式化器局部字符写入宏，而`write`才是printf里的用户写请求。

**十进制六位：** `format_int`先循环用`tmpval /= 10`数实际十进制位数；`val==0`时初始位数0，随后源码把`ndigits`强制改为1，保留“数字0本身”所占的一位。`FL_ZERO`在宽度6大于`nchars`时，先用`EMIT('0')`补齐`width-nchars`个前导零；再把局部q前进实际数字位数，使用qq从右向左依次`*qq=digits[val%10]`，每次`val/=10`，最终返回局部o=6。

| tv_usec | 初始数字位数 | 处理val0后的位数 | 宽度补零数 | 数字逆向写入的printf buf偏移顺序 | 六字符结果 |
|---:|---:|---:|---:|---|---|
| 0 | 0 | 1 | 5 | 39 | `000000` |
| 7 | 1 | 1 | 5 | 39 | `000007` |
| 12345 | 5 | 5 | 1 | 39→38→37→36→35 | `012345` |
| 999999 | 6 | 6 | 0 | 39→38→37→36→35→34 | `999999` |

例如tv_usec=0：先填**5个宽度零**于结果的前5个位置，再由数字生成循环写**一个真正的数值0**于结果第6位置。tv_usec=7也补5个前导零，末位是数字7；12345补1个零再从右到左写5位数字；999999不需要宽度零，但仍右到左写6次。这些都是有效、容量充足情况下的源码级写动作，不能只凭输出相同把填充与倒写的动作合并。

## C · 41字节整行、NUL与stdout一次写请求

对于每个`tv_usec`，生成的整行分为**五段固定的半开区间**：

| 段 | 偏移 | 字节数 | 数据来源 |
|---|---|---:|---|
| 前缀 `DateTime Now: ` | [0,14) | 14 | vsnprintf普通字符EMIT |
| 日期文本 `2026-11-22 09:08:07` | [14,33) | 19 | strftime先生成，再由%s循环EMIT |
| 句点 `.` | [33,34) | 1 | vsnprintf普通字符EMIT |
| 六位微秒 | [34,40) | 6 | format_int补零+逆向数字写 |
| 换行 `LF` | [40,41) | 1 | vsnprintf普通字符EMIT |

| case_id | tv_usec | 完整行（末尾另有LF） | vsnprintf o | printf中rv | write长度参数 | buf中NUL偏移 |
|---|---:|---|---:|---:|---:|---:|
| `DT129-000000` | 0 | `DateTime Now: 2026-11-22 09:08:07.000000` + LF | 41 | 41 | 41 | 41 |
| `DT129-000007` | 7 | `DateTime Now: 2026-11-22 09:08:07.000007` + LF | 41 | 41 | 41 | 41 |
| `DT129-012345` | 12345 | `DateTime Now: 2026-11-22 09:08:07.012345` + LF | 41 | 41 | 41 | 41 |
| `DT129-999999` | 999999 | `DateTime Now: 2026-11-22 09:08:07.999999` + LF | 41 | 41 | 41 | 41 |

`vsnprintf`在处理格式自身的终止符后以`o=41`收尾，因为`o<4096`，在`buf[41]`写NUL（0x00），**NUL是第42个缓冲区字节，但不是41字节输出请求的成员**；返回值41**不包含**NUL。printf从本地`buf[4096]`取这些字节，第22行请求唯一一次`write(stdout->_fileno,buf,rv)`，传入长度`rv=41`，文件描述符来自`stdout->_fileno`成员，**不能凭常规习惯假定数值为1**。该函数没有保存或检查`write`的返回值，第25行`return rv`，所以printf返回的是vsnprintf的41，`test_PrintDateTime`忽略这个值后正常返回0。

以微秒7为有界对照，给`write`**假定返回41**与**假定返回12**两种单次结果：两者请求的指针、fd表达式、长度及全部41字节内容不变，printf返回始终41、test_PrintDateTime正常返回始终0，main未使用它。只有假定的写调用返回计数分别是41或12；既未运行程序，也不能据此断言真实终端实际可见字节、错误处理或重试。这和B阶段`format_int`写目标字符的源码动作是不同层次。

## 自检、未完成项和主线消费

- 历史两项：`R127-02`43条寄存来源边/43个WRITE节点差量及12个B/C节点标签补正；`R128-01`三账的宽度复原（16、16、41）及C128边类型/返回源补正，均仅提交**作者候选**，C08负责独立验收。
- `facts-and-boundaries.yaml`完整保留旧ID选择器/旧值/新值及四例ASCII和格式状态；DOT仅包括**171节点/153边的两项历史有界替代视图**和**154节点/218边的新日期模型**，合计**325唯一节点、371条类型边**，全端点及声明独立核验。历史第128批完整图449节点/549边只作冻结来源，不能把本批局部替代图说成完整旧图的版本更新。
- 本批`evidence.md`新增固定time源码正式窗口独立计数、保留`vsnprintf.c`原CRLF及tab字节，所有新窗口为同一函数、宏或活动配置连续1–5物理行。用户程序真实链接至所选libc、strftime具体时区、gettimeofday实际返回、真实stdout绑定以及最终屏显仍未知。
- 完整本地P9、C预处理、编译链接、Graphviz、QEMU、实机、真实文件操作全部`NOT_RUN`；仅做连接器读取、有限JS静态模型和Git元数据回读。**作者状态`READY_FOR_MAINLINE_REVIEW`；主线验收`PENDING`；两项历史补正`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`。** 作者本轮正式交付后不得自行核收或继续启动130批。
