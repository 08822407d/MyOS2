---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PRINTF-INTEGER-FORMAT-92
record_type: bounded_integer_format_parser_generator_and_emission_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
executor_display_name: MYOS2-A-C06c 代码分析执行
produced_by: GPT-6 (visible model family; backend UNATTESTED)
model_per_owner: GPT-6 + Extra
date: 2026-11-16
base_snapshot: "time（分支名）"
read_channel: connector
status: AUTHOR_STATIC_PENDING_INDEPENDENT_C06_PRO
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [PF92-U01, PF92-U02, PF92-U03, PF92-U04, PF92-U05, PF92-U06, PF92-U07, PF92-U08]
---

# 第92批｜整数格式解析、数字分段与输出消费的限定源码分析

**范围与效力。** 本会话保持MYOS2-A-C06c / MYOS2-LEAD-002-CHAT-EXEC-12，依固定130号合同，在`time@a039d9803ade`只读内核源码上完成三个相关专题。第92批**没有D00**。第90批R90-01/02已独立关闭、组合PASS_PENDING_LOCAL；第91批PASS_PENDING_LOCAL须合读R91-S01/S02/S03，R90C-S01直接与间接宏消费者的映射仍有效。旧90/91六件、历史评论及主线审查冻结。G00通过后才有92专属目录六件及本人一次PR17顶层交付评论写权。

G00独立核实lead`6991d125d3d8`、exec`474a2c0363f0`、master`e8e60cd19540`、time`a039d9803ade`；只见开放的PR16/17，PR17仍Draft且指向原执行分支；92六件及全部92作者正式marker不存在。完整读取130、110/124、91审核、90文档闭合、92检查点、91评论6905875456与主线评论6907252863；本轮再次全文读取63、79和协议v2 P1–P13。其它九规则、128接收与Mnemosyne指导只在EXEC12本有效会话已真正读过、固定对象一致的范围复用，不另称本轮重做接收或指导刷新。主线评论勘误中两份workqueue头文件真实路径都含`mykernel/kactive/workqueue/`，未复制检查点错误的短路径。time的options_flags全文及time/master双分支金丝雀、公约`1(2)整句已重新核对；没有执行CMake里的任何命令。

## A｜格式解析状态与真实取参类型

### format_decode先读普通文本，再变成状态机

`format_decode`的`printf_spec`保留type、field_width（signed24位域）、flags、base、precision（signed16位域）。FORMAT_TYPE_NONE表示普通文字，WIDTH和PRECISION两个状态表示**解析遇到星号，需要外层再取一个int参数**，不是同一个状态。初始spec从零初始化；每次遇到新百分号转换时重新累积标志`- + 空格 # 0`。`d/i`置SIGN且base10，`u`使用base10但不置SIGN，`o`为base8，`x/X`为base16，`x`还置SMALL；`%%`分支直接输出一个百分号。所读`%p`另有路径但本任务不分析。

`"%*.*d"`的精确局部顺序是：

1. 首次解析到`*`，type设WIDTH并返回已读格式位置；`vsnprintf`通过`va_arg(args,int)`得到width并调用`set_field_width`，没有取数字实参。
2. 第二次同一个spec进入`format_decode`的WIDTH恢复点，处理负宽度：令字段取正并附LEFT，然后跳到precision；遇第二个`*`，type设PRECISION再次返回，外层又取一个int并调用`set_precision`。
3. 第三次从PRECISION恢复点，若负精度字段则置0，跳到qualifier再识别`d`和SIGN；外层此时才`va_arg(args,int)`取真实整数，再送`number`。

例如width=-6、precision=-2、数字42、所有输入及位域变换有效的模型，会在当前源码形成左对齐、精度恢复为0、最后补尾空格的六列结果“42后随四个空格”。**这三次解析与三次实参消费不能合为一次调用**。没有星号时，宽度与精度数字字面调用`skip_atoi`；它用`int i=i*10+digit`累积数字，代码局部未见超出有符号int的先行拒绝。过长宽度字符串在最早整数溢出处停证，不能按无限精度再推断位域和最终输出。（短锚PF92-E008..E043、E047..E054）

`set_field_width`先把int写signed24位域，然后用`WARN_ONCE(spec->field_width!=width)`判是否改变并调用clamp；`set_precision`相同模式写signed16，再用`clamp(prec,0,PRECISION_MAX)`。固定time所读`WARN_ONCE`在该配置相关活动定义是条件表达式，不可因名字就称它会实际输出警告；位域转换、比较、clamp前提要分别核。字面精度和动态精度的前置并不完全相同，不能把动态负精度自动解释成外部标准里的另一种语义。`format_decode`对本包不支持的转换字母会置INVALID，`vsnprintf`走`goto out`，不是继续从不匹配的va_list取值。

### 资格影响实际va_arg，不只是打印宽度

| 格式族 | 解析类型 | `vsnprintf`实际取参（再赋给unsigned long long num） |
|---|---|---|
| `d/i` 与 `u/o/x/X` | INT / UINT | `va_arg(args,int)` / 默认`va_arg(args,unsigned int)` |
| `hhd/hhu` | BYTE / UBYTE | 均从`va_arg(args,int)`开始，再转signed/unsigned char |
| `hd/hu` | SHORT / USHORT | 均从`va_arg(args,int)`开始，再转short/unsigned short |
| `ld/lu` | LONG / ULONG | 分别`va_arg(args,long)`和`va_arg(args,unsigned long)` |
| `lld/llu` | LONG_LONG | **均**`va_arg(args,long long)`，并未按SIGN改成unsigned long long |
| `zd/zu` | SIZE_T | 根据SIGN取`ssize_t`或`size_t` |
| `td/tu` | PTRDIFF | 均`va_arg(args,ptrdiff_t)` |

这是本快照的活动取参分派，不是对外部printf标准行为的声称。对`%llu`且真正传入超出signed long long范围的`unsigned long long`，取参表达式与实参类型存在匹配前提；在获得目标ABI与语言约束证明以前，不能将数学UINT64_MAX输出当作这个具体可变参数调用的定义良好结果。`hh/h`有窄化阶段，也必须分别保留整数提升和值域条件。`PF92-F01`只登记这个类型选择与契约问题，不重复第91批的kvasprintf长度候选。（PF92-E029..E039、E055..E062）

## B｜数字生成、临时空间与分段

### 小端数字对与实际物理写入

`decpair`是100个u16元素的数字对表；其`_(x)`单独定义把`x%10`置低字节、`x/10`置高字节，再加ASCII 0x3030并经`cpu_to_le16`。在本固定x86-64小端模型里，值9对应的两字节临时写入依次为“9、0”，99为“9、9”。已沿实际include定位`cpu_to_le16 -> __cpu_to_le16`和大写十六进制数字表，不把注释里的CPU端序当运行认证。（PF92-E065..E070）

`put_dec_trunc8`对较小余数以最多四次成对数字写构造**低位在前的临时字节串**。两个关键差别必须分开：

- 对`r=9`，虽然源代码`*((u16*)buf)=decpair[9]`物理写入两个字节“9、0”，`buf += r<10 ? 1 : 2`只增加1；返回指针距离为1，不表示只访问了1字节内存。
- 对`r=100`，固定乘法先取q=1，写低对“0、0”并推进2；最终写`decpair[1]`的物理两字节“1、0”，但末尾只推进1，因此有效倒序前三字符为“0、0、1”，经后续逆向消费变成“100”。对刚好只分配有效长度的任意临时目标，不能忽略额外物理写入的那一个字节。

`put_dec_full8`不截短末对：对0..1e8-1条件域依次写四个u16，共8个逻辑字符，也写8个物理字节。`0x28f5c29 >>32`与`0x147b >>19`的乘法近似分别出现在不同余数范围；单独有限算例中99999999的高商公式给999999、9999的小商公式给99，但原注释宣称的完整适用域没有在本会话穷举或形式证明。不能把数学等式和实际C乘法宽度混为一事。（PF92-E065..E078）

### do_div有“返回余数并修改商”的双向效果

`vsprintf.c`包含`math64.h`，实际引入`asm-generic/div64.h`的`do_div(n,base)`宏，活动宏先算`__rem = ((uint64_t)n)%base`，随后给`n`写回`((uint64_t)n)/base`，最后返回余数。它不是只返回商的纯函数，而且`n`出现多次，不可随意用有副作用的表达式替换当前`n`变量。（PF92-E079..E083）

`put_dec`当`n>=100000000`时先做一次`do_div(n,100000000)`，把余数送`put_dec_full8`，在商仍超过阈值时第二次分段，最后用`put_dec_trunc8`处理最高余数位。条件数学分解为：

| 输入 | 1e8余数及商 | 第二分段 | 残值与十进制逻辑长度 |
|---|---|---|---|
| `9` | 不分段 | 无 | 1位 |
| `99` | 不分段 | 无 | 2位 |
| `100` | 不分段 | 无 | 3位 |
| `100000000` | rem0，商1 | 无 | 1+8=9位 |
| `10000000000000000` | rem0，商1e8 | rem0，商1 | 1+8+8=17位 |
| `18446744073709551615` | rem9551615，商184467440737 | rem67440737，商1844 | 4+8+8=20位 |

上表是独立的**有限整数演算**，不是调用仓库`put_dec`、也不是对所有UINT64数值的动态认证。`number`和`num_to_str`分别使用`char tmp[3*sizeof(num)] __aligned(2)`；在8字节unsigned long long模型中为24字节。局部数据模型的最多20个逻辑十进制位，加上末对可能多触碰1字节，不超过24的数值预算；但从这个长度比较**不能自动得出**实际u16访问的别名/对齐/对象寿命、编译器ABI和物理设备行为已被验证。（PF92-E084/E101）

### 有符号幅度的单独止证

`number`首先把入参`unsigned long long num`按SIGN条件cast到`signed long long`并判断是否负；成立时设置符号`'-'`，随后实际执行`num = -(signed long long)num`。在明确的二补数目标/匹配变参条件下，`int32=-2147483648`的正幅度可由long long表示，能够继续推理；但`LLONG_MIN`的数学正幅度超出signed long long可表示范围，不能把“ulong long存得下”当作`signed long long`一元取负已定义良好的证明。对最低值在这一表达式前止证，原数值格式结果保持未证，登记`PF92-F02`而不是冒称已经实测错误。（PF92-E055/E061/E084..E087）

## C｜符号、前缀、补齐、容量与消费者

### 真实输出顺序不是数学排版的一步

`number`先确定原始符号、前缀是否需要、field_width扣减；数字在临时`tmp`中按低位在前生成并算出`i`，如果`precision<i`则抬高precision。随后从输出指针按以下顺序处理：

1. 当既非ZEROPAD也非LEFT时，写**前导空格**（每写一个字符之前检查`buf<end`），同时推进逻辑指针。
2. 有SIGN符号时，输出负号、PLUS或SPACE标志中优先确定的一个符号。
3. 发射前缀：十六进制SPECIAL需要时会给“0x/0X”预扣两列并写入前缀；八进制只有非零数值才额外写“0”。
4. 若非LEFT，依据剩余field_width写零或空格；再依据precision补额外的零。
5. 从`tmp`逆序逐字节输出实际数字；最后依据剩余field_width写LEFT场景的尾随空格。`number`返回逻辑推进后的buf指针。

这不是所有路径都会执行每一步。例：`%#08x`数值42，在已核的SMALL/SPECIAL/ZEROPAD组合下，源序为先`0x`再补四个零再发`2a`，数学字符串是`0x00002a`，需求8字符；`%-6.3d`的42可形成`042`后面三空格。`%08.3d`的42在**当前实现**仍保留ZEROPAD，补五个宽度零与一个精度零后输出42，得到`00000042`。`%#x`的零值在这个源码模型中得到`0x0`；这些结果仅描述固定实现，不调用未经查证的外部标准来断言兼容或不兼容。（PF92-E084..E100）

### 实际写字节、逻辑推进、NUL与返回并不是同一量

`number`内部`if(buf<end)`控制物理字符写入，却仍会在分支外`++buf`，所以逻辑指针的位置可能超过已分配输出缓冲的末尾；`vsnprintf`的`str=buf; end=buf+size`及最后`return str-buf`也要求有效C指针和范围条件。给定最终数学字符串不能证明这种指针加减、比较已经符合目标C语言规则或安全内存边界。这延续`KN91-U01`及`R91-S01`，本批新证据`PF92-U06`只追加限制，不将旧缺口重命名成新已证故障。

`vsnprintf`首先判断`size>INT_MAX`：所读`WARN_ON_ONCE`结果若使分支成立，会**直接返回0**，不执行输出末尾的NUL处理；不能拿“返回0”承诺目标已经空串。正常到达尾部时，`size>0`根据逻辑str是否小于end，将NUL放在`str`或`end[-1]`，返回`str-buf`（通常作为需求字符数，不含NUL）。`vscnprintf`再用`i<size`选择返回i或在非零size时返回size-1（零size返回0）。`snprintf`返回`vsnprintf`的i，`scnprintf`返回`vscnprintf`的i，两种包装不能混为一项。对需求“42”且有效size2目标，在源级字节写入条件成立时，物理会尝试写两个字符但结尾将第二个位置改为NUL，最终像为`'4',NUL`；`vsnprintf`返回数学需求2，`vscnprintf`返回最终写入字符数1。最终像不代表实际临时访存只有一个字节。（PF92-E045..E064/E106..E108）

`num_to_str`是另一个调用`put_dec`的**独立接口**，不是`number`的直接下游。它对num0..9有专门分支，检查`len>size || width>size`时返回0，成功时先写空格、从tmp倒序拷贝数字、返回`len+width`。在所读函数结尾**没有显式写NUL**。例如给定num12、width5、size5和有效缓冲，写满五个字节`'   12'`并返回5；不能未经其消费者合同就当作NUL终止的C字符串，也不能无上游需求证明便签为漏洞。（PF92-E101..E105）

`format_worker_id`仅作为一个真实`%d`使用者：它经`scnprintf`生成worker名字，`create_worker`未消费返回值并传递栈名给kthread包装。91已确认的42字节缓冲、name full/short、线程成功与调度效果限定继续有效；本批不重复名称所有权或线程创建判断。（PF92-E109/E110）

## 事实矩阵和有限模型

本批`facts-and-boundaries.yaml`实际记录**8组55行**：`parser_state_transition_matrix`8、`format_argument_type_matrix`8、`width_precision_input_matrix`6、`integer_sign_magnitude_matrix`6、`digit_chunk_and_temporary_storage_matrix`9、`prefix_padding_and_emission_matrix`7、`return_capacity_and_consumer_matrix`7、`ABC_join_matrix`4。每条有具体源操作、输入、条件结果、首个无法继续推导的门和独立短锚索引。

22个纯文本模型`PF92-M01..M22`均包含中间状态。A涵盖字面/%%、宽度精度多次恢复、负星号、invalid、字面溢出、hh窄化与llu变参类型止证；B涵盖小数字对9/99/100、近似商有限点、1e8/1e16/最大64位分段和LLONG_MIN止证；C涵盖十六进制前缀与补零、精度+ZEROPAD、LEFT、零值特殊前缀、num_to_str无NUL、size2截断及过大size早退。模型中“数学最终字符”“实际写过的临时字节”“逻辑返回值”分别记账；所有模型`NOT_RUN`，不是22项测试通过。

新分类`deferred-findings.yaml`有4个有条件源码候选`PF92-F01..F04`（llu等取参类型、LLONG_MIN幅度、skip_atoi溢出、显式精度与ZEROPAD组合）及8个缺口`PF92-U01..U08`（位域、va_arg/ABI、定点算法全域、临时对齐/小端、最低值转换、输出指针、num_to_str消费者、外部规范）。这不是四个动态复现bug；同根因与91的`KN91-U01/U02`和`KN91-F01/F02/F03/F04`逐一关联而不重登记。旧R90-01/02、R89及53已闭合文档问题不重开。R91-S02对“最终像不是全部读写足迹”、R91-S03对短引文/聚合图边/qualified ID缩写的限定继续应用。

## G04权限、质量与停写条件

本批严格只新增`user-printf-integer-format-92/`下`MANIFEST.md`、`printf-integer-format.md`、`facts-and-boundaries.yaml`、`printf-integer-format-paths.dot`、`deferred-findings.yaml`、`evidence.md`六件，及本人一次PR17顶层正式交付评论。`evidence.md`含110条新`PF92-E001..E110`短锚；每个宏独立定义内引用，不把数字对表里的独立宏与数组定义拼接。MANIFEST十四字段及规范`verified_claims/quotes_reconfirmed/downgraded_to_inferred`必须在全六件发布前重新统计和回源，再填写真实值。

发布前检查引文字面、真实定义内1–5行、符号支持范围、矩阵/模型/图的消费及所列前提；全六件40位十六进制串应零命中，分支金丝雀与公约原句按真实双分支核对。完整本地机械P9、仓库C/ASM和脚本、Graphviz、编译链接、QEMU、硬件及真实并发全为`NOT_RUN`。GitHub仅使用原执行分支、旧真实HEAD作expected_sha、force=false单父提交；更新前查六件/全部同批marker，后置compare只能是六件added，全部文件固定与活动blob一致，PR16/17及四direct refs不出现未知并发推进，评论唯一且全文回读。异常按63先确认远端实际效果后安全重试，不重复成功对象。

**执行者最终状态只可为**`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，完成真实GitHub提交、评论和全部回读后立刻`STOP_WRITING`。本作者不得自签92 PASS、不启动93、不改PR元数据、主线或内核；请原C06下一轮由Owner选择**Pro**做独立审核并同轮安排后继。

