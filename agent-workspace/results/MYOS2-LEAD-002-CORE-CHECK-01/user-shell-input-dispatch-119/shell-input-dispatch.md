---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SHELL-INPUT-DISPATCH-119
record_type: phase2_shell_input_dispatch_and_D00_report
produced_by: "GPT-6（后端身份/effort无法独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
conversation_display_name: MYOS2-A-C08a 代码分析执行
date: 2026-11-21
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
open_questions: []
inputs_read: [myinitramfs/myshell/myshell.c, myinitramfs/mylib/myglibc/string/strcmp.c, mykernel/fs/vfs/open.c, mykernel/fs/vfs/d_path.c, mykernel/scripts/options_flags.cmake, agent-workspace/lead/MYOS2-LEAD-002/169-user-shell-input-dispatch-contract.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md]
---

# 第119批｜shell从一行输入到内建函数派发（阶段2，静态分析）

**执行交付 READY_FOR_MAINLINE_REVIEW；C08独立验收 PENDING。** 原有的117、118项仍待C08裁定，旧件与旧评论不修改。本批三项文档纠正和A/B/C纸面模型均不等于用户态实测。全部固定内核/用户程序源码来自time短12=`a039d9803ade`；主线合同发布短12=`009d3ffdaa7d`；唯一执行父短12=`cb96288abb8b`。索引：`evidence.md`中IN119-E001～E046定义体短窗，IN119-P01～P10普通位置；同目录facts与DOT可供独审复算。

## D00｜旧件不覆盖：三项独立补正

### D00-1 / R117-02｜把“源码短窗数”与“字面前缀次数”分开

以下按每件原始文本中的正式标签前缀逐件搜索，而非仅数引文标题；统计顺序为 MANIFEST、正文、facts、DOT、deferred、evidence：

| 批次与固定源 | 六件逐项字面次数 | 真正的定义体短引文 | 六件字面合计 | 额外位置及解释 |
|---|---|---:|---:|---|
| 117 / `52c51a3752d3` | 1 / 0 / 0 / 0 / 0 / 88 | 88 | 89 | 117 MANIFEST第113行说明自身包含前缀，不能升格为引文 |
| 118 / `cb96288abb8b` | 0 / 0 / 0 / 0 / 0 / 92 | 91 | 92 | 118 evidence第3行解释也写了前缀，非引文窗 |
| **119本次六件** | **0 / 0 / 0 / 0 / 0 / 46** | **46** | **46** | 本批说明文字不用标签的完整字面前缀，所有46处均在evidence短窗 |

历史117首次核查中的88窗/89次和118的91窗/92次是**两套旧账**，119不得据此宣称旧件已有一致的P6。新46窗逐条按真实固定源、定义体和1～5行边界回读；MANIFEST自检的46/46/0只适用于**本批六件**。纠正状态仍为 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW`，原编号R117-02继续OPEN直到主线验收。[历史裁定](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md)。

### D00-2 / R118-01｜证据分类、真正消费者及第60批归属

CW118-E051与CW118-E052分别来自 `myshell.c:59` 文件作用域 `homedir` 指针对象初始化、`:60` 文件作用域 `current_dir` 数组定义，虽是源码事实，但**不是函数定义体内部**。在119降为普通位置 IN119-P01/P02，不再作为P2锚。CW118-E050的 `PATH_MAX` 宏可继续单独使用，本批IN119-E046在宏定义内重读。其余命题由真实消费者自证：

| 旧定位/误读 | 新证据与真正活动消费者 | 修正后的限界 |
|---|---|---|
| CW118-E051充当函数体锚 | IN119-P01（myshell.c59）＋IN119-E026（cd_command194–197） | `argc<=1`时cd_command取homedir指针，值是字符串字面`/`；不能把文件对象声明当cd调用 |
| CW118-E052充当函数体锚 | IN119-P02（myshell.c60）＋E001/E002/E003（main） | `current_dir` 是文件作用域4096字节数组；main先清零、getcwd并用于提示符 |
| 旧证据用静态对象代替cd/pwd消费 | IN119-E028（cd清缓存）、E030（pwd打印）、E031/E032（cat拼路径） | 各消费者函数自己有调用/格式串；`if(current_dir)`检数组地址不检字符串内容 |
| chdir的lookup_flags被泛归其他调用 | IN119-E033（open.c273–277）、E034（279–283） | `LOOKUP_FOLLOW | LOOKUP_DIRECTORY`来源于kernel chdir**局部初始化**；查找失败不执行本函数`pwd`赋值 |
| kernel getcwd的pwd/root、分配/拼接/复制混归 | IN119-E036/E037/E038/E039/E040/E041（d_path.c） | `pwd/root`供路径回溯；`PATH_MAX`决定内部b.len和分配，copy_to_user结果与返回int需分开 |
| 原报告、facts把指针→long→int及复制效果归R60-S02 | [第60批正式裁定§3 R60-S01第3项](https://github.com/08822407d/MyOS2/blob/009d3ffdaa7d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md) | **实际归R60-S01第3项**：成功表达式不是len；转换值依ABI，成功复制不自动证明完整字符串 |
| 原报告、facts对R60-S02的错误占用 | 同裁定§4 R60-S02 | R60-S02实际约束DOT图边及证据范围，不负责该返回/复制语义 |

固定time C配置再核：`mykernel/scripts/options_flags.cmake:39–43` C编译选项含 `-mcmodel=kernel -fno-pie -fno-pic`，`:64–66` 列 `CONFIG_SLUB`、`CONFIG_ARCH_HAS_SYSCALL_WRAPPER` 和 `CONFIG_BUG`（IN119-E044/E045）；相对直接master的large/PIE金丝雀区分了快照。**不能凭没有执行编译而确认这些选项在某次实际构建生效**。R118-01为作者提交纠正，仍OPEN，交C08复核。

### D00-3 / R118-02｜调用、次序、数据来源各用不同边

原118 A/B/C DOT存在把调用者或数据源错认成相邻操作的错误。119 `shell-input-dispatch-paths.dot` 的 `cluster_D00A`、`cluster_D00B`、`cluster_D00C`为替代子图，**不修改**旧DOT。旧节点到新语义的逐项映射：

| 旧边或节点关系 | 新直接调用边（真正拥有调用的函数） | 新非调用边/模型路径 |
|---|---|---|
| `a_loop → a_lf`标DIRECT_PRINTF_FIRST | `a_main → a_lf`，由main直接调用printf（E002） | `a_loop → a_lf`只是循环体控制顺序；`a_lf → a_prompt`也是先后顺序；main另外直接调用提示printf及fflush |
| `a_mzero → a_mgetcwd`旧边若误认被调用者 | `a_main → a_mzero`和`a_main → a_mgetcwd`分别直接调用（E001） | `a_mzero → a_mgetcwd`只能表示清零后顺序、同一缓存的数据准备；复制/未写两纸面分支 |
| `b_cachezero → b_getcwd`混写SEQUENCE_THEN_DIRECT_CALL | `b_cd → b_cachezero`、`b_cd → b_getcwd`分别直接调用（E028） | `b_cachezero → b_getcwd`严格SEQUENCE；成功才刷新，失败和点命令早退都不刷新 |
| `c_pwd → c_alloc`标DATA_PWD_ROOT_FOR_PREPEND | `c_kernelgetcwd → c_alloc`代表内核kzalloc调用（E037） | **pwd/root不提供分配大小**；`c_pathmax → c_alloc`是长度数据；`c_pwd → c_prepend`才是逆向拼接数据 |
| `c_alloc → c_prepend`标CALL_PREPEND_FUNCTIONS | `c_kernelgetcwd → c_prepend`为调用myos_prepend及myos_prepend_path（E038） | `c_alloc → c_prepend`仅为内存准备/序列，myos_prepend_path继续调用__prepend_path（E042） |
| `c_succret → c_intret`易被消费成已复制字符串证明 | kernel getcwd自己执行copy_to_user/kfree/return（E040/E041） | length门成功→尝试复制；成功表达式`error=(long)buf`赋给int；复制效果与int值是独立命题 |

替代图保留启动时“已正确复制/未写入”的两条纸面路径；cd的无参成功、点命令、普通成功与错误路径；内核getcwd长度失败、复制失败、成功返回，以及缓存供pwd/提示符/cat三条路径。新图按 DIRECT_CALL、SEQUENCE、CONTROL、DATA、MODEL、RETURN 等类型明确标签。R118-02同样 `CORRECTION_SUBMITTED_PENDING_C08_REVIEW` / OPEN。

## A｜从console读行，区分光标请求和缓冲状态

[固定源 myshell.c L83–137](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L83-L137)，新P2为E002–E010。**模型前提**：console_fd有效，主循环已把4096字节buf全清零，每次`read(fd,&key,1)`按表给定成功返回1，所用正常输入长度≤32；每个write只记录调用请求字节，不推断终端实际显示。BS表示0x08，LF表示0x0A，NUL表示0x00，EOT表示0x04。

| 输入key/模型 | read返回 | count前→后 | 对buf写/清的下标 | 对console_fd按顺序提出的write字节 | 停止或继续 | 当前C串 |
|---|---:|---|---|---|---|---|
| `p` | 1 | 0→1 | `buf[0]='p'` | BS、`p`、`_` | 继续 | p |
| `x` | 1 | 1→2 | `buf[1]='x'` | BS、`x`、`_` | 继续 | px |
| `BS` | 1 | 2→1 | `buf[1]=0`（前减count） | BS、BS、`_` | 继续 | p |
| `w` | 1 | 1→2 | `buf[1]='w'` | BS、`w`、`_` | 继续 | pw |
| `d` | 1 | 2→3 | `buf[2]='d'` | BS、`d`、`_` | 继续 | pwd |
| `LF` | 1 | 3→3 | 无 | **仅BS** | `return 3`，不写尾部下划线 | pwd |
| `LF`（空行） | 1 | 0→0 | 无 | **仅BS** | `return 0` | 空串 |
| `BS`（count=0） | 1 | 0→0 | 无 | BS、`_`（没有第二次BS） | 继续 | 空串 |
| `NUL` | 1 | k→k | 无 | **仅BS** | `continue`跳过尾下划线 | 原串不变 |
| `EOT`（已录pwd后） | 1 | 3→3 | 无 | 无（循环体不执行） | 循环条件结束、函数体后无return | pwd（条件模型） |
| `read=0`（已录pwd后） | 0 | 3→3 | 无 | 无（循环体不执行） | 同上，key不再参与判断 | pwd（条件模型） |

每个实际处理的非EOT、成功读取字节，**进入switch前**总先请求一次BS；退格且count>0，分支中**再次**请求BS；switch尾请求下划线，但LF在switch内部直接返回、NUL在case0直接continue，均不会执行尾部语句。循环因EOT或read非1结束后，非void函数尾没有显式return；**返回值在C语言层未定义/不可用**，不能将其默认为3，主循环也不消费返回值（main只使用被改写的buf）。main还在读行之后单独请求一次LF（E004），那不是read_line对console_fd的write。下一轮main再次清空buf，不是累积输入。

## B｜parse_command：仅空格，指针借用buf

[固定源 myshell.c L154–186](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L154-L186)，新P2 E011–E016。普通成功路径只按已清零4096字节缓冲、字符串在256范围内、malloc成功、8字节指针宽度及零位表示NULL的纸面前提复算；**表达式实际写的是** `sizeof(char **) * (*argc + 1)`，不是字符串字节数，也不是用户态新复制的词。原`buf`的词内容零拷贝复用，只有argv指针向量另行malloc。

| 原buf输入（引号外） | 跳前导空格后j | 词尾扫描及argc | malloc表达式与容量（8字节模型） | argv词首下标 | 被重写为NUL的buf位置 | 仅跳过、未改的空格 | 末槽 |
|---|---:|---|---|---|---|---|---|
| `"  cd  docs  "` | 2 | 词尾3和9；2 | `sizeof(char **)*3=24`字节 | `argv[0]=&buf[2]`，`argv[1]=&buf[6]` | 4、10 | 0、1、5、11 | `argv[2]=NULL`（已清零） |
| `"pwd"` | 0 | 词尾2；1 | `sizeof(char **)*2=16` | `&buf[0]` | 3原本就NUL，写回0 | 无 | `argv[1]=NULL` |
| `"pwd   "` | 0 | 词尾2；1 | 16 | `&buf[0]` | 3（原空格） | 4、5 | `argv[1]=NULL` |
| `"   "` | 3 | 无；0 | **不调用malloc** | 无，main初始为NULL | 无 | 0、1、2 | 不分配 |
| `"nope"` | 0 | 词尾3；1 | 16 | `&buf[0]` | 4原本NUL | 无 | `argv[1]=NULL` |

词数是由扫描区每个**非空格**且其下一个字节为`' '`或NUL的词尾决定；遍历不把所有空格改写。逐词存下`&buf[j]`后走到空格/NUL，用`buf[j++]=0`终结此词，随后跳过后续连续空格。即使末尾已经是NUL也再次写0，j自增。本模型末槽NULL依赖所给表示假定，不等于所有C对象表示的普遍定理。内建函数同步调用期间argv词指针指向本轮main栈上的buf；下一轮main清零同一buf，不能把旧指针当持久数据。所读main、parse_command与run_cmd内有malloc和调用，没有显式free(argv)；**只限定这几个可见函数内**，不声称全仓库均无释放或实际泄漏。

## C｜十项内建命令线性匹配与派发

[表 L22–56](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L22-L56) 与 [find_cmd/run_cmd/主循环](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L94-L185)，新P2 E017–E025。实际活动表按索引恰有10项，逐项大小写敏感的名字→函数指针为：`0 cd→cd_command`、`1 ls→ls_command`、`2 pwd→pwd_command`、`3 cat→cat_command`、`4 touch→touch_command`、`5 rm→rm_command`、`6 mkdir→mkdir_command`、`7 rmdir→rmdir_command`、`8 exec→exec_command`、`9 reboot→reboot_command`。这里仅列静态表绑定，不展开未授权的其他命令实现。

| 完整输入 | parse后argc、argv[0] | strcmp比较次数 | find_cmd索引 | 实际调用者→绑定目标/输出请求 |
|---|---|---:|---:|---|
| `cd docs` | 2，`"cd"` | 1（第一项） | 0 | main→run_cmd；run_cmd通过表项函数指针调用`cd_command(2, argv)`；后续chdir是否成功未给定，输出依cd真实结果 |
| `pwd` | 1，`"pwd"` | 3（cd,ls,pwd） | 2 | main→run_cmd→函数指针绑定`pwd_command`，它从当前`current_dir`请求`printf("%s\n",...)` |
| `nope` | 1，`"nope"` | 10（逐项不等） | −1 | main不调用run_cmd，而请求`"Input Error, No Command Found!\n"` |
| 空行 `""` | 0，argv仍NULL | 0（parse提前返−1） | −1 | 无malloc、无查表、无run_cmd；main仍请求同一错误消息 |
| 全空格 `"   "` | 0，argv仍NULL | 0（先跳空格/词数0） | −1 | 同空行 |

find_cmd的实际`strcmp`定义以`unsigned char`逐字节比较，不经大小写折叠；`strcmp("Pwd","pwd")`不相等。main先在read_line之后单独`printf("\n")`，**然后**根据负索引另请求带LF的错误行，因此两个换行来源绝不合并。run_cmd返回类型为void，表达式调用函数指针却不保存其内建函数返回；pwd_command的返回1被忽略。对pwd的输出内容仅连接118已核`current_dir`缓存模型；缓存为`/`时请求`"/\n"`，缓存空串时仅请求LF，不能从静态源码认证终端实际显示。

## 机器复算入口与边界

- `facts-and-boundaries.yaml`独立记录历史计数、原证据替代、A的逐字节事件、B逐例分词下标和内存容量、C逐例比较数及前提。
- `shell-input-dispatch-paths.dot`保留A/B/C三项D00替代图及输入、分词、查表三簇；边标必须明确直接调用与顺序/数据，模型分支非运行轨迹。
- `deferred-findings.yaml`保留三项待审、未做的完整P9/编译/运行、C返回/内存生命周期/显示等假设。
- 当前源只可推出本批给定纸面输入下的请求、条件和数据变化。没有执行真正C程序、预处理、编译、链接、仓库测试脚本、Graphviz、QEMU或实机。C08继续掌握三项关闭与批次验收权；不得给119自行PASS。
