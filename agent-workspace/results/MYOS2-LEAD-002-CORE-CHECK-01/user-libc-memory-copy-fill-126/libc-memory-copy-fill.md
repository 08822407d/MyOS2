---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-MEMORY-COPY-FILL-126
record_type: D00_memory_copy_fill_and_cat_interval_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
conversation_display_name: "MYOS2-A-C08b 代码分析执行"
produced_by: "GPT-6（会话可知；后台型号及effort UNATTESTED）"
model_per_owner: "GPT-6＋Extra（177号指定）"
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
execution_parent_short12: bd1372961eaa
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
open_returns: [R125-01, R125-02]
open_questions: []
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COPY-APPEND-125-review.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "myinitramfs/mylib/myglibc/string/memset.c"
  - "myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h"
  - "myinitramfs/myshell/myshell.c"
---

# 第126批 · 两项125勘误与`memcpy`、`memset`、cat缓冲区

所有新分析都以固定`time@a039d9803ade`（短12）的仓内源文本为输入，采用**容量充分、合法、不重叠、普通对象**进行纸面推导。用户态使用的真实libc、实际预处理宏、二进制指令和文件打开结果均未确认。本批**不是内核修复、补丁或运行试验**。新增独立定义体短窗在`evidence.md`，机器案例及重建补正详见`facts-and-boundaries.yaml`，调用关系类型在`libc-memory-copy-fill-paths.dot`。

## D00 · 第125批两项退回的冻结原件与更正视图

125原件仍固定在执行提交`bd1372961eaa`，本批只新增有界勘误。`R125-01`和`R125-02`在本作者交付阶段均为`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，C08尚需独立裁定，不能据本报告自动宣称旧125整批PASS。

### D126-R125-01 · P6字面计数和构建定位分类

原六件逐字统计的核证前缀分布按文件顺序`MANIFEST/正文/facts/DOT/deferred/evidence`为`1/0/0/0/0/34`，合计**35**。其中具有符号、原文及代码块结构的证据共有**34**个，全在旧`evidence.md`。第35次字面命中来自原`MANIFEST.md:105`对该前缀的中文描述，不能算成第三十五个引文，也不能再把MANIFEST计为零而声称全部六件字面命中只有34。

177要求将原`SC125-E022–E029`、`SC125-E032–E034`共11条**用户侧构建命令**保留原代码文本、源文件位置及ID，但改列普通定位；只留下`SC125-E001–E021`的21个C函数窗和`SC125-E030–E031`的2个time内核C配置窗，形成**23个正式引文**的替代阅读版本。用户侧构建不能证明C调用者及运行时返回，最关键的是`E025`仅设置`MUSL_PATH`工具根目录，`E034`才将其用于`CMAKE_C_COMPILER`变量；两条也都不是最终sh有效链接的证明。

旧清单仅在**组合阅读视图**中重建以下5处整行（并不改原Git Blob）。唯一定位均是固定父`bd1372961eaa`下`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/`的文件及物理行。机器件为每处保留完整的新行文本，报告表仅示摘要。

| 固定原件行 | 虚拟更正动作 | 重建后的字段/解释 |
|---|---|---|
| `MANIFEST.md:35` | 整行覆盖（仅阅读视图） | `self_check: {verified_claims: 23, quotes_reconfirmed: 23, downgraded_to_inferred: 0}` |
| `MANIFEST.md:36` | 整行覆盖（仅阅读视图） | `per_file_literal_prefix: {MANIFEST.md: 0, libc-string-copy-append.md: 0, facts-and-boundaries.yaml: 0, libc-string-…` |
| `MANIFEST.md:37` | 整行覆盖（仅阅读视图） | `new_source_definition_windows: 23` |
| `MANIFEST.md:100` | 整行覆盖（仅阅读视图） | `\| `evidence.md` \| 23条正式短窗（21个C函数体、2个time内核活动配置）；另11条用户侧CMake窗口降为普通定位，原源码行和ID保留 \|` |
| `MANIFEST.md:105` | 整行覆盖（仅阅读视图） | `- 正式P2短窗：SC125-E001–E021、SC125-E030–E031，共23处；SC125-E022–E029及E032–E034为保留原文的构建普通定位；其余五件不含正式短窗标签。P6三数 `23/23/0`。` |

旧证据文件的11处分类更正以ID为唯一选择器，保留其原单行CMake命令，不把它的注释变成C函数的核证：

| 固定证据ID | 源码位置 | 虚拟分类及正确用途 |
|---|---|---|
| `SC125-E022` | `myinitramfs/mylib/myglibc/string/CMakeLists.txt:3-3` | 用户侧构建文本普通定位 |
| `SC125-E023` | `myinitramfs/mylib/myglibc/CMakeLists.txt:1-1` | 用户侧构建文本普通定位 |
| `SC125-E024` | `myinitramfs/mylib/myglibc/CMakeLists.txt:20-20` | 用户侧构建文本普通定位 |
| `SC125-E025` | `myinitramfs/CMakeLists.txt:40-40` | 仅设置MUSL_PATH工具根目录 |
| `SC125-E026` | `myinitramfs/CMakeLists.txt:46-46` | 用户侧构建文本普通定位 |
| `SC125-E027` | `myinitramfs/CMakeLists.txt:47-47` | 用户侧构建文本普通定位 |
| `SC125-E028` | `myinitramfs/CMakeLists.txt:48-48` | 用户侧构建文本普通定位 |
| `SC125-E029` | `myinitramfs/myshell/CMakeLists.txt:4-4` | 用户侧构建文本普通定位 |
| `SC125-E032` | `myinitramfs/mylib/myglibc/string/CMakeLists.txt:1-1` | 用户侧构建文本普通定位 |
| `SC125-E033` | `myinitramfs/mylib/myglibc/string/CMakeLists.txt:2-2` | 用户侧构建文本普通定位 |
| `SC125-E034` | `myinitramfs/CMakeLists.txt:41-41` | 仅设置CMAKE_C_COMPILER文本值 |

程序化应用上述更正到临时文本、其余四件不变后，替代视图的字面分布为`0/0/0/0/0/23`，合计23，正式引文与复核三数为`23/23/0`。**三个数字必须分别保留**：旧原件字面35、旧结构化34、更正阅读视图正式23。第126批自己另按最终六件统计，**不得把旧23项合并进126的P6**。旧MANIFEST的35/36/37/100/105行、旧evidence的11个构建章节、旧作者正式评论中的34引文自报，均由此补正说明约束阅读；Git历史文件及评论原封不动。原有九个125函数案例可按C08认可前提继续引用。

### D126-R125-02 · 根目录F[1]的最后写者

对照旧`C_cat_command.cases[i].stages[2]`（`after_optional_separator`）。当前操作前，所有F字节先经`memset(F,0,cap)`写零；首次`strcpy(F,current_dir)`**又通过其内层memcpy写入包括终止零在内的全部cwd字节**。因此根目录`/`的F[1]虽然仍为0，却在清零之后被第一次复制再次写入了0。紧接的`len>1`判断为假，本步写集为空，F[1]既非刚写成的新零，也非最后由memset写零。另两个普通目录在F[4]覆写斜杠后，尚未复制追加源之前，F[5]的最后写者确实仍为memset。

| 原125案例 | 首次复制后首NUL | 条件步骤执行写入 | 判断后首NUL | 判断后该字节最近写者 | 本步新成为首NUL的下标 | 原机器字段的替代值 |
|---|---:|---|---:|---|---|---|
| `C125-01` | 1 | 无 | 1 | `STRCPY#1->MEMCPY(F,current_dir,len+1)` | null | `/C_cat_command/cases/0/stages/2/new_NUL_from_prior_memset_offset` 1→null |
| `C125-02` | 4 | F[4]写入2f | 5 | `memset(F,0,cap)` | 5 | `/C_cat_command/cases/1/stages/2/new_NUL_from_prior_memset_offset` 5→5 |
| `C125-03` | 4 | F[4]写入2f | 5 | `memset(F,0,cap)` | 5 | `/C_cat_command/cases/2/stages/2/new_NUL_from_prior_memset_offset` 5→5 |

精确字段路径、更正值、先前零初始化者、最近写者、守卫真假、条件步骤改变的字节及最终值均在机器件D00的`R125_02.cases`。注意“新成为首NUL”不等于“这一步写了新的0”：普通目录的新首NUL F[5]是旧有清零暴露出来的，不是斜杠赋值写出的0。三个最终路径`/readme`、`/bin/hi`、`/bin//hi`、STRCPY/STRCAT返回指针及第270行open请求均保留。旧125`facts`中只有根目录那一个`new_NUL_from_prior_memset_offset`字段改成null；其余两例仍为5。

## A · `MEMCPY`两个游标的前进与返回

选择仓内`myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY`与通用`sysdeps/generic/memcopy.h`作为条件模型：`op_t=unsigned long int`、`OPSIZ=8`、`OP_T_THRES=16`；假设选中的`PAGE_COPY_FWD_MAYBE`为空，且整字helper正常完成约定复制。函数先将原始参数`dstpp/srcpp`转换到本地整数游标`dstp/srcp`。对于0、3、15字节，因`len<16`直接跳过长分支，执行`BYTE_COPY_FWD`的零次或逐字节循环：每轮从当前srcp读一个byte到临时`__x`，递增srcp，写入当前dstp，再递增dstp。传入的`dstpp`未被这些局部变量变化覆盖，最终`return dstpp`。

本组S为40字节独立源对象，S[k]=0x41+k；D为40字节目的对象，初始全0x23作为哨兵，双方基址给定模8为0且**互不重叠**。A126-M03从D+3开始，故目的对象内指针偏移不是整字对齐；短分支不要求该对齐。下表读区与写区分别相对S、D基址：

| case_id | len | 源→目的实参 | 读区 / 写区（半开） | 实际复制字节 | 分支与helper边界 | 最终srcp/dstp | 返回 |
|---|---:|---|---|---|---|---|---|
| `A126-M00` | 0 | `S+1` → `D+2` | [1,1) / [2,2) | 无 | 字节宏0次 | S+1 / D+2 | D+2 |
| `A126-M03` | 3 | `S+4` → `D+3` | [4,7) / [3,6) | `45 46 47` | 字节宏3次 | S+7 / D+6 | D+3 |
| `A126-M15` | 15 | `S+2` → `D+5` | [2,17) / [5,20) | `43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 50 51` | 字节宏15次 | S+17 / D+20 | D+5 |
| `A126-M16` | 16 | `S+8` → `D+8` | [8,24) / [8,24) | `49 4a 4b 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58` | 阈值16；0前导，0页复制，两字helper，尾0 | S+24 / D+24 | D+8 |

A126-M00的字节宏执行零轮、不读不写，返回D+2。A126-M03三轮依次读S[4..6]并写D[3..5]，末游标S+7/D+6；A126-M15的15轮可依机器表合并成连续区间，但每个字节的前进均已记录，尾哨兵不动。

A126-M16给定S+8和D+8两端均8字节对齐，长度恰好达到16阈值；进入长分支，前导对齐字节为0，空PAGE宏没有复制。宏`WORD_COPY_FWD`选`_wordcopy_fwd_aligned(dstp,srcp,(nbytes)/OPSIZ)`，这里helper第三个实参为**2个字**，不是16个字或16字节的`nbytes`原值。只在**helper按约正常完成两字复制并返回**的假设下，宏将源和目的游标各推进16，并把外层`len`更新为0。随后`BYTE_COPY_FWD(...,0)`宏仍被展开/执行但没有字节循环，返回原始D+8。这个C helper调用与`WORD_COPY_FWD`/BYTE宏的宏展开不是同一边类型；真实预处理选择、指令与优化仍是UNKNOWN。

## B · `memset`按字节/整字/整组写入

选择固定`memset.c`、8字节`op_t`、足够大的独立88字节D对象（原有字节全0xAA）。给定`int c=0x123`（291），函数保存本地目的游标`dstp`，写入单字节`(unsigned char)c = 0x23`；当原始`len>=8`时才构造重复字`cccc=0x2323232323232323`。先逐字节填到8字节对齐，再按每组8个op_t（即64字节）、剩余单个op_t（8字节）、最后的1字节尾循环覆盖目标。短长度0、7未进入重复字构造分支。0x123不代表每个字节写0x0123，也不代表字节数乘以两个字节。

| case_id | 目的起点与len | 对齐前导字节 | 八词组迭代次数（整字数） | 单词迭代次数 | 尾字节数 | 目的写区 | 最后局部dstp | 返回 |
|---|---|---:|---|---:|---:|---|---|---|
| `B126-S00` | D+0，0 | 0 | 0（0个op_t） | 0 | 0 | [0,0) | D+0 | D+0 |
| `B126-S07` | D+0，7 | 0 | 0（0个op_t） | 0 | 7 | [0,7) | D+7 | D+0 |
| `B126-S08` | D+0，8 | 0 | 0（0个op_t） | 1 | 0 | [0,8) | D+8 | D+0 |
| `B126-S09` | D+0，9 | 0 | 0（0个op_t） | 1 | 1 | [0,9) | D+9 | D+0 |
| `B126-S64` | D+0，64 | 0 | 1（8个op_t） | 0 | 0 | [0,64) | D+64 | D+0 |
| `B126-S09-U3` | D+3，9 | 5 | 0（0个op_t） | 0 | 4 | [3,12) | D+12 | D+3 |

- 长度8：仅一个8字节op_t写；长度9、起点对齐：一个8字节op_t加一字节尾；长度64：一组八个8字节op_t，单词与尾段均零。
- 特别是`B126-S09-U3`，起点D+3，原始len=9进入大于等于8的分支，但先写D[3,8)的**5个前导字节**后剩余4字节，既无八词组也无单个整字写，尾部写D[8,12)的4个字节。两段合计9，返回仍为原实参D+3，不是D+12。
- 对所有六例，前导/整组/单词/尾字节之和严格等于请求长度，且写区之外0xAA哨兵保持。源码`return dstpp`与局部`dstp`最终位置分别记录，不把它们混同。

## C · 三个cat路径的真实helper写宽及最后写者

复用125的同一组输入、F对齐、分配成功、容量9/8/9及充分可读不重叠源；保留旧路径与函数指针结论。各例shell依次直接请求`memset(F,0,cap)`、第一次`strcpy(F,cwd)`（内层MEMCPY）、条件单字节`F[len]='/'`、`strcat(F,argv[1])`（再进内层STRCPY与MEMCPY），到`open(F,O_RDONLY,0)`实参为止。

- 三例F均按**原始容量**而非当前字符串长度清零。F对齐且cap分别为9/8/9，选定的`memset`均进入大于等于8的分支，执行一个8字节整字零写；容量9的例子再写尾部1字节。
- 第一次strcpy的MEMCPY按cwd长度+1请求根目录2字节、两例/bin均5字节；第二次strcpy内的MEMCPY按参数长+1请求7、3、4字节。每次都未达到16阈值，采用`BYTE_COPY_FWD`逐字节写，不调用整字helper。
- 单字节斜杠赋值只在`len>1`的两例发生；它既非memset的一部分，也不是额外函数调用。
- 即使`memcpy`写入一个原本经memset已经为0的终止字节，**这也是一次真实的纸面写入**。例如根目录F[1]、普通目录F[4]第一次复制的0，以及最终MEMCPY写入尾NUL；只按前后值统计会漏掉写入次数与最后写者。

| case_id | 输入 | F容量 | memset写宽 | 第一次memcpy写宽 | 斜杠赋值 | 第二次memcpy写宽 | 所有写入字节次数之和 | 不同F位置数 | 最终open路径 |
|---|---|---:|---|---|---|---|---:|---:|---|
| `C125-01` | `/` + `readme` | 9 | 9（8整字+1尾字节） | 2（字节宏） | 跳过0 | 7（字节宏） | 18 | 9 | `/readme` |
| `C125-02` | `/bin` + `hi` | 8 | 8（8整字+0尾字节） | 5（字节宏） | 写1 | 3（字节宏） | 17 | 8 | `/bin/hi` |
| `C125-03` | `/bin` + `/hi` | 9 | 9（8整字+1尾字节） | 5（字节宏） | 写1 | 4（字节宏） | 19 | 9 | `/bin//hi` |

**总写宽**是每次存储所覆盖字节数相加，不因为写入值相同而抵消：根目录为`9+2+0+7=18`；`/bin/hi`为`8+5+1+3=17`；`/bin//hi`为`9+5+1+4=19`。**不同目的字节数**为F被写过的索引集合大小，因每例一开始就清零全部分配容量，所以分别为9、8、9。这两个值不是程序运行时间、CPU指令数、内存带宽或实际运行次数，严格是给定源码路径下的写入宽度算术。

| case_id | 追加前首NUL | 当时最新写者 | 最终关心的下标及最新写者 | 相同值的覆盖写次数 |
|---|---:|---|---|---|
| `C125-01` | 1 | first_strcpy_memcpy | F[1]最终由`second_strcpy_memcpy`写入 | 重写同一值2次 |
| `C125-02` | 5 | shell_memset | F[5]最终由`second_strcpy_memcpy`写入 | 重写同一值2次 |
| `C125-03` | 5 | shell_memset | F[5]最终由`second_strcpy_memcpy`写入 | 重写同一值2次 |

C125-01在守卫为假时F[1]最新写者仍为第一次MEMCPY；追加后F[1]由第二次MEMCPY改写成字符'r'。C125-02/03的F[4]被shell斜杠赋值覆盖，F[5]在追加之前仍由memset保留为0；追加之后F[5]分别是字母'h'和参数自带的斜杠。机器件按步骤列F全容量字节、每个F索引的写入次数、最后写者和“写入相同值”记录，可由第125批旧十二张高层快照逐步对照。第126批图以真实函数实例作为直接调用起点，将宏展开、DATA、CONTROL、SEQUENCE、WRITE和返回指针分开；不能将字节宏误写为一次C函数调用。

## 范围与交回

- `memcpy`函数及宏对象的模型依赖选定通用头、8字节unsigned long、空页面复制宏；实际选择/宏开关与字helper是否存在于真实sh的二进制均未证明。16字节整字案例需要明确的正常helper返回前提。
- `memset`所有案例的源对象/目的容量已给定，不覆盖无效指针、对象越界、重叠、硬件原子性或具体汇编。memcpy明确排除重叠，不能据此推论memmove。
- 三个cat最终字符串只说明270行`open`接到的原始参数字节；路径归一化、是否成功打开、errno与显示结果仍未知，不能从内部库源码自动证明真实sh采用它。
- `R125-01`和`R125-02`作者已提交补正候选，**是否关闭仅由C08独立裁定**；本轮不重新打开R123-01或125之外的旧项。完整本地P9、预处理、编译链接、Graphviz、QEMU、实机、真实文件操作均`NOT_RUN`。作者终态`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，待远端唯一交付及回读后`STOP_WRITING`。
