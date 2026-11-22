---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-ALIGNED-WORD-COPY-127
record_type: bounded_aligned_wordcopy_analysis_and_R126_01_errata
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
conversation_display_name: "MYOS2-A-C08b 代码分析执行"
produced_by: "GPT-6（会话自报；后台与effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（178号指定；未认证实际后台档）"
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
execution_parent_short12: 345d0f32374c
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
open_returns: [R126-01]
supersedes: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot"
owner_decisions_requested_now: []
open_questions: []
---

# 第127批 · 对齐整字 helper 的入口、读写流水及外层 memcpy

固定源码为`time@a039d9803ade`（短12），本批**仅做只读源码取证和有限字节/整字算术**。使用仓内`wordcopy.c::WORDCOPY_FWD_ALIGNED`函数、通用`memcopy.h`和`memcpy.c`，假定8字节`op_t`、`OP_T_THRES=16`、选中空页面复制宏、独立且足够大的正常可读源/目的数组。未证明实际sh的链接库、有效头文件路径或汇编指令。本批的机器事实在`facts-and-boundaries.yaml`，新引文在`evidence.md`，修正的历史126图与新A/B/C图合置`libc-aligned-word-copy-paths.dot`。

## D00 · R126-01（只补126图件，不动原件）

**固定目标：** `345d0f32374c`下的
`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot`。原图原样保留，126的13个数值案例、旧源码证据及评论均不改。更正算法在机器件`D00.R126_01`提供原节点/边选择器、删增和替代标签，可从旧Blob重建。DOT中的`cluster_D00_R126_01`是完整的**修正阅读图**；它不是126 Git原件，也不承担新127专题事实的计数。

**原始与组合视图计数严格分开：**

| 图对象 | 唯一节点 | 节点声明次数 | 类型边 | 直接函数调用实例边 | 所选复制宏位置边 |
|---|---:|---:|---:|---:|---:|
| 126历史原图 | 184 | 194 | 211 | 40 | 12 |
| D00修正后完整126阅读视图 | 192 | 192 | 218 | 40 | 13 |

修正涉及以下四组，且只在127新文件中记录：

1. **A126-M16 前导和尾部分开。** 原`A126_M16_decision → A126_M16_page_macro`的控制边替换为`decision → pre_byte_macro → page_macro`，并由真实MEMCPY函数实例新增一条第40行`BYTE_COPY_FWD`宏展开边。原`byte_macro`明确重标为第57行尾部。两次字节宏迭代数均为0，中间空PAGE是第45行、WORD是第51行且helper参数2字。选定的复制宏**位置**以四个A126 `memcpy`案例加三个C125案例各两次内部MEMCPY为分母，分别计1+1+1+4+2+2+2=**13/13**；不把类型、常量宏或helper的C直接调用并入此分母。
2. **A126-M00 空循环不是真实内存访问。** 删去`byte_macro→read→write→cursor`三条把空区间当作访问的旧边；改为`byte_macro→zero_loop→cursor`，`zero_loop`明确为`__nbytes=0`、无读取、无写入、无源/目的游标递增。旧`read/write 0 bytes`节点可保留为不可达的注释状态，不能叫实际访问。原始`dstpp=D+2`返回不变。
3. **B组六例的原始指针来源。** 六例旧`final dstp→return`的RETURN_DATA统一改为SEQUENCE，并为每例新增`original_dstpp`数据节点；`owner→original_dstpp→return`承载真实返回数据。即使某些零起点例最后游标与返回值偶然相同，也不能把末游标当作返回来源。尤其`B126-S09-U3`末局部游标D+12而返回原D+3；`B126-S00`零长无存储操作但返回D+0。
4. **归簇、写区与源求长。** 原`B126_S09_U3`的10处重复声明从错误的`B126_S09`簇中消去，专属完整`B126_S09_U3`簇只声明一次。三条cat案例的零填充、首次复制和末次复制共9个节点标签，把“闭右端”写法改为**半开区间**；第二次复制长度的`DATA`边将误写的`argc`改为`argv[1]`源字符串求长加1。更正只影响标签/图语义，不改变原facts的写宽或路径。

| 旧节点选择器（九项） | 原标签区间 | D00替代区间 |
|---|---|---|
| `C125_01_zero` | `[0,9]` | `[0,9)` |
| `C125_01_first` | `[0,2]` | `[0,2)` |
| `C125_01_second` | `[1,8]` | `[1,8)` |
| `C125_02_zero` | `[0,8]` | `[0,8)` |
| `C125_02_first` | `[0,5]` | `[0,5)` |
| `C125_02_second` | `[5,8]` | `[5,8)` |
| `C125_03_zero` | `[0,9]` | `[0,9)` |
| `C125_03_first` | `[0,5]` | `[0,5)` |
| `C125_03_second` | `[5,9]` | `[5,9)` |

三例依次是`[0,9)、[0,2)、[1,8)`，`[0,8)、[0,5)、[5,8)`，`[0,9)、[0,5)、[5,9)`。每个区间`右端-左端=原facts写宽`。已有R125-01/R125-02由C08裁定`CLOSED_DOCUMENTATION_ONLY`；本轮**只有R126-01维持`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`**，必须由C08独立裁定是否关闭。

## 模型预设及S/D数据

为避免因前置指针算术产生对象外地址，A/B/C均使用**两个互不重叠的384字节数组**S和D，示意基址都8字节对齐。源数组字节有可重建规律：对下标`i=0..383`，`S[i]=0x20+(floor(i/8)%32)+16*(i%8)`；D初始384字节均为`0xCC`哨兵。helper直接输入S/D+128（均对齐），不是把S[0]视作第一个源字。W0=S[128..136)=`30 40 50 60 70 80 90 a0`，W1=S[136..144)=`31 41 51 61 71 81 91 a1`，所以可按字节区分每个被加载的`op_t`。每个案例独立重置数组，副作用仅发生在目的合法复制区内，源与目的区间外哨兵逐例检查。上层三请求另指定起点偏移128或131。

读/写下标、局部游标、helper形参、外层MEMCPY实参及返回指针是不同概念。下文所有计数是**源码表达式纸面模型**，不是运行测量。

## A · 入口分派：长度单位、预取与进入的do标签

仓内函数首行`void WORDCOPY_FWD_ALIGNED(long int dstp,long int srcp,size_t len)`，`len`以**op_t整字数**计。入口`switch(len%8)`有不同的预读a0/a1及局部整数游标调整，随后用`goto`进入相应标签。游标可能先被减到源字符串起点前的某个**同一384字节对象内部**地址，但直到具体`((op_t*)srcp)[k]`取值时才是实际内存读取；单独的加减与跳转不是读写访问。所有给定有效下标位于S/D对象及请求内。

| case_id | helper字数 | 入口余数 | 首进入标签 | 首预读源字/暂存寄存变量 | 实际整字读/写 | 末局部srcp/dstp（对象内偏移） | do0末写 |
|---|---:|---:|---|---|---:|---|---|
| `A127-00` | 0 | undefined | — | — / — | undefined/0 | 128 / 128 | 未发生 |
| `A127-01` | 1 | undefined | do0 | — / — | undefined/1 | 136 / 128 | D0 ← S0 |
| `A127-02` | 2 | undefined | do1 | — / — | undefined/2 | 144 / 136 | D1 ← S1 |
| `A127-07` | 7 | undefined | do6 | — / — | undefined/7 | 184 / 176 | D6 ← S6 |
| `A127-08` | 8 | undefined | do7 | — / — | undefined/8 | 192 / 184 | D7 ← S7 |
| `A127-09` | 9 | undefined | do8 | — / — | undefined/9 | 200 / 192 | D8 ← S8 |
| `A127-16` | 16 | undefined | do7 | — / — | undefined/16 | 256 / 248 | D15 ← S15 |

- **A127-00**：余数0且len0时，源码`OP_T_THRES<=3*OPSIZ`在本选型中成立（16<=24），直接`return;`，没有任何解引用/存储；这与外层`memcpy(...,0)`会跳过长分支且不调用helper是两层不同入口。
- **A127-01**：预取S0到a1，srcp局部先变为S+136、len减为0，直接`goto do0`，由循环外`D0=a1`单次写回；`dstp`仍是D+128。没有运行do8..do1循环。
- **A127-02**：预取S0到a0，局部srcp减48字节到S+80、dstp减56字节到D+72，len由2变8，跳`do1`；有效地从`srcp[7]=S1`读取a1，同时写`dstp[7]=D0←a0`；随后底部游标都加64、len变0，循环外`do0`写`D1←a1`。结果是**两字读、两字写，不是访问了8个有效目标字**。
- **A127-07**入口预取S0到a1，转到do6；**A127-08**入口预取S0到a0，转do7；**A127-09**入口预取S0到a1，先增srcp且len减1后转do8。三例均按具体标签推进，最后仍由do0写回尾字。所有独立模型的源词、目的词和寄存变量对应关系在机器表逐操作列出。
- **A127-16**：`case0`预取S0，先从do7运行七个交替读写位置；底部len由16变8，跳回do8完成第二轮八个位置；len变0后再执行循环外do0。**第16个目的字D15直到do0才由暂存a1写入**。

## B · a0/a1交替与真正的最后一字

源循环的八个标签按`do8, do7, ..., do1`次序，每个标签先读取一个新的源`op_t`到a0或a1，再把**另一个寄存变量之前保存的值**写入相应目的词。即“一词读入—上一词写出”流水。末尾`srcp/dstp += 8*OPSIZ`，`len -=8`，只在还剩字数时回到do8。循环外的`do0: dstp[0]=a1`写最后暂存字；do0不在do8..do1的循环体内。

| case_id | 字数 | 入口标签 | 各轮底部len值 | do0写的目的字 | 寄存变量 | 实际整字读/写 |
|---|---:|---|---|---:|---|---|
| `B127-02` | 2 | do1 | 0 | undefined | a1 | 2/2 |
| `B127-08` | 8 | do7 | 0 | undefined | a1 | 8/8 |
| `B127-16` | 16 | do7 | 8→0 | undefined | a1 | 16/16 |

**2字完整顺序：** S0→a0（入口）、S1→a1（do1）、D0←a0（同一do1）、循环底部调整、D1←a1（do0）。**8字：** 入口预读S0；do7读S1并写D0，之后交替直至do1读S7写D6；do0写D7。**16字：** 第一轮do7..do1将D0..D6写妥，循环底部len=8；第二轮do8先读S8并把上一轮的S7写入D7，然后继续写到D14；do0再写D15。

因此2、8、16字恰分别读2/8/16个源词，写2/8/16个目的词，每个目标词恰写一次，不包含额外的隐式末字写或跨对象读写。goto和label都是`WORDCOPY_FWD_ALIGNED`同一函数内的**控制转移**，不是被调用函数；helper本身返回类型是void，其内部最终游标不是外层memcpy的返回指针。

## C · 外层三个memcpy请求接入helper

只使用上述同一组384字节源/目的对象，MEMCPY直接接收`srcpp/dstpp`及字节长度。长分支判断`len>=OP_T_THRES`即`len>=16`；目标指针若模8非零先按`(-dstp)%OPSIZ`复制前导字节，随后给定空PAGE宏；`WORD_COPY_FWD`在**源已对齐**时于通用宏内调用`_wordcopy_fwd_aligned(...,(nbytes)/OPSIZ)`，第三实参为**整字数**。helper参数按值传入，仅内部改变自身局部dstp/srcp；调用结束后`WORD_COPY_FWD`宏独立修改**外层MEMCPY的两个游标**并将长度变成余数，最后`BYTE_COPY_FWD`处理尾部，MEMCPY`return dstpp`仍返回原传入地址。

| case_id | 请求字节 | S/D初始偏移 | 前导BYTE字节 | helper起点（两端） | helper整字实参 | 外层WORD后游标（两端） | 尾BYTE字节 | memcpy返回 |
|---|---:|---|---:|---|---:|---|---:|---|
| `C127-M16-A` | 16 | S/D+128 | 0 | S/D+128 | 2 | S/D+144 | 0 | D+128 |
| `C127-M16-U3` | 16 | S/D+131 | 5 | S/D+136 | 1 | S/D+144 | 3 | D+131 |
| `C127-M129-A` | 129 | S/D+128 | 0 | S/D+128 | 16 | S/D+256 | 1 | D+128 |

- **C127-M16-A：** 两端起点S/D+128，16字节；前导零，helper收2字并按A127-02的入口/最后写回完成D[128,144)，宏随后把两个外层游标推进到+144，尾部零字节；返回D+128。
- **C127-M16-U3：** 两端从+131开始，模8均为3。先由第40行字节宏复制`[131,136)`共5字节（此时局部srcp/dstp均到+136），剩余11字节；helper收1字，`case1`预读后直达do0，将`[136,144)`的8字节写好；外层WORD宏独立推进到+144、长度余3，尾BYTE复制`[144,147)`三字节；最终16字节恰覆盖`[131,147)`，返回原D+131。
- **C127-M129-A：** S/D+128开始129字节；前导零，helper收16字，按A127-16两轮/末次do0完成`[128,256)`共128字节，外层游标到+256、余1；尾BYTE再复制`[256,257)`，返回D+128。末尾的1字节不属于helper写入。

C的每例都分别核对前导、helper字节范围、尾部三段不重不漏：字数`(0+16+0)=16`、`(5+8+3)=16`、`(0+128+1)=129`。每个有效目标字节与对应源字节相同，目的对象外部0xCC哨兵、源对象保持原状；本模型与真实运行/CPU指令没有等价认证。

## 图、证据与停止边界

本批DOT包括经补正的历史126完整阅读图（192唯一节点、218边、13个所选复制宏位置）和127新增A/B/C案例图（174节点、223边），全图共**366个唯一节点、441条有类型边**，节点声明均唯一、端点齐全。历史126的40条直接调用保持不变；127新增三个外层MEMCPY实例各有一条真实C helper调用，宏展开单独以`MACRO_EXPANSION`标注，a0/a1数据来源与goto控制不合并成直接调用。图仅作纯文本构建与核验，**Graphviz未运行**。

固定time源码下的`wordcopy.c`名称可能受外部编译宏改变；对真实`sh`的有效libc、实际包含的是哪个memcopy头、PAGE_COPY_THRESHOLD值、编译优化和机器指令均未独立核实。本批不审逆向、dest-only对齐helper或跨目标源对象、不预处理/编译/链接、不使用QEMU/实机和真实文件操作，完整本地P9仍`NOT_RUN`。

**本执行者完成的是`R126-01`补正候选和第127批条件模型；状态`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`。是否关闭R126-01只能由原C08主线使用Pro独立裁定。**
