---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXEC-ARGV-ENTRY-VIEW-140
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: phase2_executor_analysis
base_snapshot: "time（分支名）"
phase: 2
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第140批 · 程序入口指针表与参数来源

192号合同，固定time，阶段2。作者仅提交五项增量补正和三个独立静态模型，不修改138/139旧件或评论，不自行判闭。第25批已有vma前置门缺口；本批只在给定有效可读/写对象、已达到表写片段及正常辅助操作返回的前提下计算，绝非完整生产路径成功。

## D00 · 五项增量补正

### R138-03 历史余项：原字面值和定义身份

原作者及本轮增补者均为 CHAT-EXEC-21。对冻结138/139实物逐字重取如下，保留不同拼写，不把历史错误混为一条：
| 冻结位置 | 原字面值 | 修订后的可解析定位 |
|---|---|---|
| 138 report:41 | `D00.R137-02.old_window_crosswalk` | `D00["R137-02"].all_73_old_windows` |
| 138 evidence:17 | `facts.D00.R137_02` | `D00["R137-02"].all_73_old_windows` |
| 138 evidence:552 | `old_window_crosswalk` | `D00["R137-02"].all_73_old_windows` |
| 139 facts:443 | `D00.R137_02.old_window_crosswalk` | `retranscribe frozen 138 report literal D00.R137-02.old_window_crosswalk; actual key separately D00["R137-02"].all_73_old_windows` |

实际读取旧138 facts 的 `D00["R137-02"].all_73_old_windows` 为73项，同级 `new_47_window_index` 为47项。原138报告41、evidence17/552、139 facts443的**完整原行**存于本批 `D00["R138-03"]` 的四个 exact 字段。特别注意139误把原138报告的连字符改成下划线。47个旧ID对应46处物理区间、活动行拆分等C09已核收范围不重开。

E139-023旧symbol截断，修为完整跨行宏调用身份 `MYOS_SYSCALL_DEFINE3(execve, const char *,filename, const char *const __user *, argv, const char *const __user *, envp)`（定义头折叠空白用于定位，E140-001逐字保留原物理1460–1464及return）。E139-089修为 `MYOS_SYSCALL_DEFINE0(fork)`（E140-002定义头，E140-003活动exit_signal和return）。这两条用**源宏调用本身作为身份**，不借包装宏声称实际生成函数名；E140-004给固定CMake包装选项文本，未预处理或构建。机器索引和正式标签使用同一完整身份。

### R139-01：修好五个缺值、统一半开区间端点

旧139报告78–82的五个本次字节格均为 `undefined`，其文件名区间末端写成T0，地址表第94行与旧图第76行也有该表示。冻结原行、旧节点及旧DOT行分别留在 `D00["R139-01"]`。下面用T表示零偏移，逐段以源ASCII字符和NUL复算；时间序与地址序由同一份机器段生成。

| 来源 | 槽 | helper | 含NUL长 | 进入p → 离开p | 半开区间 | 全部本次字节（hex） |
|---|---|---|---:|---|---|---|
| bprm->filename alias filename->name | — | copy_string_kernel | 10 | T → T-10 | [T-10,T) | 2F 62 69 6E 2F 74 6F 6F 6C 00 |
| envp[1] | 1 | copy_strings (user reverse) + strnlen_user | 5 | T-10 → T-15 | [T-15,T-10) | 46 3D 78 79 00 |
| envp[0] | 0 | copy_strings (user reverse) + strnlen_user | 4 | T-15 → T-19 | [T-19,T-15) | 45 3D 31 00 |
| argv[1] | 1 | copy_strings (user reverse) + strnlen_user | 3 | T-19 → T-22 | [T-22,T-19) | 2D 71 00 |
| argv[0] | 0 | copy_strings (user reverse) + strnlen_user | 8 | T-22 → T-30 | [T-30,T-22) | 64 69 73 70 6C 61 79 00 |

地址递增：
| 区间 | 字符串（含NUL） | 字节 |
|---|---|---|
| [T-30,T-22) | display\0 | 64 69 73 70 6C 61 79 00 |
| [T-22,T-19) | -q\0 | 2D 71 00 |
| [T-19,T-15) | E=1\0 | 45 3D 31 00 |
| [T-15,T-10) | F=xy\0 | 46 3D 78 79 00 |
| [T-10,T) | /bin/tool\0 | 2F 62 69 6E 2F 74 6F 6F 6C 00 |

完整 [T-30,T) 30字节：`64 69 73 70 6C 61 79 00 2D 71 00 45 3D 31 00 46 3D 78 79 00 2F 62 69 6E 2F 74 6F 6F 6C 00`。与冻结139机器30字节逐项相等。先写文件名并在T-10保存bprm.exec，才调用envp复制；argv[0]位于T-30。源依据 E140-016–022、025–027；图 D140_B139 各节点含真实字节/地址/顺序。

### R139-02：计数的空向量外门及实际边选择器

原139 A0两条 `NULL_SLOT_TERMINATES_NOT_COUNTED` 边不符合源码：用户 `argv != NULL` 为假，从初始i=0返回；内核 `!argv` 为真直接返回0，均未取槽。A1–A3的向量才实际读取终止槽。
| 样本 | user count 取槽 | kernel count 直接读槽 | 真正终止槽 | 返回 | 后续普通复制重新取槽 |
|---|---|---|---|---:|---|
| A0 | 无 | 无 | 不存在 | 0 | 无 |
| A1 | 0 | 0 | 0 | 0 | 无 |
| A2 | 0,1 | 0,1 | 1 | 1 | 0 |
| A3 | 0,1,2 | 0,1,2 | 2 | 2 | 1,0 |

空串指针仍非空，计数不扫描字符串；后续复制才读字符。E140-005–013、018–020、035。原机器辅助表两个角色别名 `P,K,P_TO_K_WHOLE_STRUCT` 与 `P,J,P_TO_J_WHOLE_STRUCT` 以及getter来源，分别解析为冻结139实图四元组：
| from | to | kind | label |
|---|---|---|---|
| D139_P | D139_K | INDEPENDENT_P_TO_K_STRUCT_COPY | "" |
| D139_P | D139_J | INDEPENDENT_P_TO_J_STRUCT_COPY | "" |
| D139_K | D139_KGET | GETTER_DATA_SOURCE_K_ONLY | "" |
新图保留这三个实际选择器；别名映射在同名机器字段中。原139计量为120个节点ID、103条唯一from/to/kind/label四元组，边不按不存在的ID计数。

### R139-03：C案例地址、空串来源与释放边界

四个成功样本指针预算24、限额扣减后6,291,432；argmin地址是T-6,291,432，相对T的偏移才是-6,291,432。p在预算后仍T。下面逐例说明：
| 冻结139样本 | 实参字节来源 | 用户合成条件 | argmin地址 | 复制后p／释放前最后p | 下一边界 |
|---|---|---|---|---|---|
| C-U0 | kernel literal "" synthesized by user entry | 真 | T-6291432 | T-20（调用边界） | UNKNOWN at bprm_execve call |
| C-U1 | kernel literal "" synthesized by user entry | 真 | T-6291432 | T-20（调用边界） | UNKNOWN at bprm_execve call |
| C-U2 | caller user argv[0] pointing to NUL | 假 | T-6291432 | T-20（调用边界） | UNKNOWN at bprm_execve call |
| C-K0 | SKIPPED | 该函数无此分支 | SKIPPED | T（释放前） | -EINVAL only under given normal cleanup return |
| C-K1 | SKIPPED | 该函数无此分支 | SKIPPED | T（释放前） | -EINVAL only under given normal cleanup return |
| C-K2 | caller kernel argv[0] pointing to NUL | 该函数无此分支 | T-6291432 | T-20（调用边界） | UNKNOWN at bprm_execve call |

U0/U1文件名及环境先占19字节，调用copy_string_kernel("")合成一个NUL，再写argc=1；U2由调用者的用户argv[0]提供NUL，后续argc==0为假；K2由内核数组正常取空串、copy_string_kernel，不存在用户合成门。四个成功例p=T-20、bprm.exec=T-10、argc=1、envc=2，止于bprm_execve调用。K0/K1先置retval=-EINVAL，再goto out_free，清理之前仍可读p=T；free_bprm内最终kfree(bprm)，随后out_ret调用putname；**仅在通常清理正常返回条件下**才有函数return -EINVAL。释放后不读取bprm。不把调用边界当实际exec完成。E140-014/015、028–038、093–095。

### R139-04：每个窗口的归属与组合断言

原92个E139 ID（91个物理区间，E082/E090共窗）按原ID/路径/首尾行/原标题/实际归属/处置逐项登记在 `D00["R139-04"].old_windows`，已核收的物理窗沿用C09裁定而非重算为新P6。E139-057实际是create_elf_tables中的AT_EXECFN**使用点**（E140-039），auxvec.h定义另取E140-040。E139-043只证明循环后ret=0和公共出口标签（E140-023），实际return ret在E140-024，合读才能推出给定成功路径返回0。E139-067仅证明argc赋值（E140-030），正数条件由先行守卫E140-029提供，两窗联合推出到达条件。旧E023/E089标签归R138-03，不混入别项。

五项均为 CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW。原138的R138-01/02/04已文档关闭，R138-03余项及139原RETURN由C09独立裁定。

## A · argv/envp表写入及字段边界

T为各自139模型原始字符串游标，给定stack_shift=Δ，U=T−Δ，故字符串地址统一移到U相对位置。S是独立给定的表底，**不推定S与U的差**。每槽8字节。setup_arg_pages所见p平移并设置mm.arg_start、bprm.exec减Δ（E140-041/042）。在create_elf_tables所给有效写表片段中，sp=S，先写argc整数，sp自增；此时先前用于辅助布局的局部p具体值未给，argc写入不依赖它。随后第278行明确p=mm.arg_end=mm.arg_start。每个非空参数槽写指向字符串的地址，再strnlen_user得到包含NUL的长度并令p+=len；终止槽写0且sp自增，不扫描字符串也不改变p。环境同理。局部argc/envc的while后缀递减，最后失败测试后局部值为-1，bprm中的计数并非被这些局部变量写回。E140-043–056/098。

### A1 普通双参数（139 B布局）
| 槽 | 槽地址 | 含义 | 值 | 指向的字符串 | 扫描长度（含NUL） | p前→后 | sp前→后 |
|---:|---|---|---|---|---:|---|---|
| 0 | S | argc_integer | 2 | — | — | PRE_RESET_P_NOT_STIPULATED → PRE_RESET_P_NOT_STIPULATED | S → S+8 |
| 1 | S+8 | argv[0] | U-30 | display | 8 | U-30 → U-22 | S+8 → S+16 |
| 2 | S+16 | argv[1] | U-22 | -q | 3 | U-22 → U-19 | S+16 → S+24 |
| 3 | S+24 | argv_terminator | 0 | — | — | U-19 → U-19 | S+24 → S+32 |
| 4 | S+32 | envp[0] | U-19 | E=1 | 4 | U-19 → U-15 | S+32 → S+40 |
| 5 | S+40 | envp[1] | U-15 | F=xy | 5 | U-15 → U-10 | S+40 → S+48 |
| 6 | S+48 | envp_terminator | 0 | — | — | U-10 → U-10 | S+48 → S+56 |

字段：`arg_start=U-30`；`arg_end`初写U-30，终写U-19；`env_start=U-19`，`env_end`初写U-19，终写U-10。argv表头S+8，envp表头S+32，auxv起点S+56，`bprm.exec=U-10`。扫描末p=U-10，写表末sp=S+56；bprm.p为S。

### A2 单空参数（139 C成功布局）
| 槽 | 槽地址 | 含义 | 值 | 指向的字符串 | 扫描长度（含NUL） | p前→后 | sp前→后 |
|---:|---|---|---|---|---:|---|---|
| 0 | S | argc_integer | 1 | — | — | PRE_RESET_P_NOT_STIPULATED → PRE_RESET_P_NOT_STIPULATED | S → S+8 |
| 1 | S+8 | argv[0] | U-20 | 单字节 NUL | 1 | U-20 → U-19 | S+8 → S+16 |
| 2 | S+16 | argv_terminator | 0 | — | — | U-19 → U-19 | S+16 → S+24 |
| 3 | S+24 | envp[0] | U-19 | E=1 | 4 | U-19 → U-15 | S+24 → S+32 |
| 4 | S+32 | envp[1] | U-15 | F=xy | 5 | U-15 → U-10 | S+32 → S+40 |
| 5 | S+40 | envp_terminator | 0 | — | — | U-10 → U-10 | S+40 → S+48 |

字段：`arg_start=U-20`；`arg_end`初写U-20，终写U-19；`env_start=U-19`，`env_end`初写U-19，终写U-10。argv表头S+8，envp表头S+24，auxv起点S+48，`bprm.exec=U-10`。扫描末p=U-10，写表末sp=S+48；bprm.p为S。

A2的argv[0]指针值U-20在模型中非NULL，其目标第一个字节为00；S+16的argv终止槽与S+40的envp终止槽是**空指针值**，不同于空串对象。历史来源分别为U0/U1用户入口合成、U2显式用户空串、K2显式内核空串；只要给定相同有效布局，入口表一致。两例arg_end最终等于env_start，是因为题设相邻的参数/环境字符串区；env_end最终等于bprm.exec，是因为环境区后紧邻保存文件名。字段分别记录参数终点、环境起/终点和文件名地址；相等是本例布局关系，不是字段身份相同。auxv从当前sp开始，未重建全表。

## B · 候选C启动函数的环境指针

独立给定crt1_c.c::_start已经按C形参收到有效argc/argv。其 `while (argv[argc] != NULL);` 是固定槽位的空循环体，不递增argc；每个本例该槽都是NULL，条件一次求值为假。随后 `envs=argv+argc+1`，在8字节槽模型中跨过argv的终止槽。
| 样本 | argc | argv | 最后有效参数槽→值 | argv[argc]槽→值 | envs地址 | envs[0]槽→值 | envs[envc]槽→值 |
|---|---:|---|---|---|---|---|---|
| B1 | 2 | S+8 | S+16 → U-22 | S+24 → 0 | S+32 | S+32 → U-19 | S+48 → 0 |
| B2 | 1 | S+8 | S+8 → U-20 | S+16 → 0 | S+24 | S+24 → U-19 | S+40 → 0 |
| B3 | 1 | V | V → P | V+8 → 0 | V+16 | V+16 → 0 | V+16 → 0 |

B3三个连续槽V、V+8、V+16分别存P、0、0；P为非空有效指针，指向单字节00；envs=V+16，首环境槽本身为0、同时也是envs[envc=0]，不同于P所指的NUL。B1/B2环境数均2，首环境指针U-19指向E=1。若给定稳定非空终止槽，候选空循环体并不扫描到下一槽；本批三例均为有效终止槽，不分析异常运行。

给定三次fopen正常返回F0、NULL、F2，源码写入顺序为stdin=fopen("/dev/console","r")=F0，stdout=fopen("/dev/console","w")=NULL，stderr=fopen("/dev/console","w")=F2。所读语句间没有针对stdout空值的条件门，给定正常续行便到达 `exit(main(argc,argv))`；B1传main(2,S+8)，B2传main(1,S+8)，B3传main(1,V)。给定main返回7，exit实参7。只证可见调用及赋值，实际文件打开、寄存器绑定和退出行为未知。E140-057–060。

## C · /boot/sh 名称、参数、环境及shell自己的命令向量

initd.c main收到原argc/argv，重新打开控制台后调用normal_boot(argc,argv)。后者新建prog_name字符数组、args与envs两个独立指针数组，而不是将main的入口argv直接作exec参数。给定rv==0子分支，getpid之后到达execve(prog_name,args,envs)。
| exec实参/槽 | 源对象 | 内容与终止 |
|---|---|---|
| filename | prog_name数组首址 | /boot/sh\0 (9字节) |
| args[0] | 同一个prog_name首址 | /boot/sh\0 (9字节) |
| args[1] | 字符串字面量 | userarg_test_1\0 (15字节) |
| args[2] | 字符串字面量 | userarg_test_2\0 (15字节) |
| args[3] | NULL指针槽 | 不复制字符串 |
| envs[0] | 字符串字面量 | userenv_test_1\0 (15字节) |
| envs[1] | 字符串字面量 | userenv_test_2\0 (15字节) |
| envs[2] | 字符串字面量 | userenv_test_3\0 (15字节) |
| envs[3] | NULL指针槽 | 不复制字符串 |

在独立给定成功字符串复制模型中，argc=3/envc=3；filename源字符串先复制，保存bprm.exec，再逆序复制envp与argv。filename与args[0]虽同指prog_name，复制发生两次并占各自的新地址。E140-061–066、092、025–027。以下时间顺序使用平移前T：
| 来源 | 槽 | helper | 含NUL长 | p进入→离开 | 半开区间 | 本段完整hex |
|---|---|---|---:|---|---|---|
| bprm.filename = filename.name (original caller filename bytes from prog_name) | — | copy_string_kernel | 9 | T → T-9 | [T-9,T) | 2F 62 6F 6F 74 2F 73 68 00 |
| envs[2] -> literal | 2 | copy_strings -> get_user_arg_ptr/strnlen_user | 15 | T-9 → T-24 | [T-24,T-9) | 75 73 65 72 65 6E 76 5F 74 65 73 74 5F 33 00 |
| envs[1] -> literal | 1 | copy_strings -> get_user_arg_ptr/strnlen_user | 15 | T-24 → T-39 | [T-39,T-24) | 75 73 65 72 65 6E 76 5F 74 65 73 74 5F 32 00 |
| envs[0] -> literal | 0 | copy_strings -> get_user_arg_ptr/strnlen_user | 15 | T-39 → T-54 | [T-54,T-39) | 75 73 65 72 65 6E 76 5F 74 65 73 74 5F 31 00 |
| args[2] -> literal | 2 | copy_strings -> get_user_arg_ptr/strnlen_user | 15 | T-54 → T-69 | [T-69,T-54) | 75 73 65 72 61 72 67 5F 74 65 73 74 5F 32 00 |
| args[1] -> literal | 1 | copy_strings -> get_user_arg_ptr/strnlen_user | 15 | T-69 → T-84 | [T-84,T-69) | 75 73 65 72 61 72 67 5F 74 65 73 74 5F 31 00 |
| args[0] -> prog_name | 0 | copy_strings -> get_user_arg_ptr/strnlen_user | 9 | T-84 → T-93 | [T-93,T-84) | 2F 62 6F 6F 74 2F 73 68 00 |

低地址→高地址的平移后字符串区：
| 半开区间 | 字符串 | 字节 |
|---|---|---|
| [U-93,U-84) | /boot/sh + NUL | 2F 62 6F 6F 74 2F 73 68 00 |
| [U-84,U-69) | userarg_test_1 + NUL | 75 73 65 72 61 72 67 5F 74 65 73 74 5F 31 00 |
| [U-69,U-54) | userarg_test_2 + NUL | 75 73 65 72 61 72 67 5F 74 65 73 74 5F 32 00 |
| [U-54,U-39) | userenv_test_1 + NUL | 75 73 65 72 65 6E 76 5F 74 65 73 74 5F 31 00 |
| [U-39,U-24) | userenv_test_2 + NUL | 75 73 65 72 65 6E 76 5F 74 65 73 74 5F 32 00 |
| [U-24,U-9) | userenv_test_3 + NUL | 75 73 65 72 65 6E 76 5F 74 65 73 74 5F 33 00 |
| [U-9,U) | /boot/sh + NUL | 2F 62 6F 6F 74 2F 73 68 00 |

完整93字节可解码ASCII+NUL（反斜杠0每处代表一个00，不是两个字符）：`/boot/sh\0userarg_test_1\0userarg_test_2\0userenv_test_1\0userenv_test_2\0userenv_test_3\0/boot/sh\0`。两个/boot/sh副本分别位于U-93（argv[0]）和U-9（bprm.exec），各9字节，其余五串各15。末p在复制完为T-93，给定平移后U-93。指针表按A独立条件计算如下：
| 槽 | 槽地址 | 含义 | 值 | 指向的字符串 | 扫描长度（含NUL） | p前→后 | sp前→后 |
|---:|---|---|---|---|---:|---|---|
| 0 | S | argc_integer | 3 | — | — | PRE_RESET_P_NOT_STIPULATED → PRE_RESET_P_NOT_STIPULATED | S → S+8 |
| 1 | S+8 | argv[0] | U-93 | /boot/sh | 9 | U-93 → U-84 | S+8 → S+16 |
| 2 | S+16 | argv[1] | U-84 | userarg_test_1 | 15 | U-84 → U-69 | S+16 → S+24 |
| 3 | S+24 | argv[2] | U-69 | userarg_test_2 | 15 | U-69 → U-54 | S+24 → S+32 |
| 4 | S+32 | argv_terminator | 0 | — | — | U-54 → U-54 | S+32 → S+40 |
| 5 | S+40 | envp[0] | U-54 | userenv_test_1 | 15 | U-54 → U-39 | S+40 → S+48 |
| 6 | S+48 | envp[1] | U-39 | userenv_test_2 | 15 | U-39 → U-24 | S+48 → S+56 |
| 7 | S+56 | envp[2] | U-24 | userenv_test_3 | 15 | U-24 → U-9 | S+56 → S+64 |
| 8 | S+64 | envp_terminator | 0 | — | — | U-9 → U-9 | S+64 → S+72 |

字段：`arg_start=U-93`；`arg_end`初写U-93，终写U-54；`env_start=U-54`，`env_end`初写U-54，终写U-9。argv表头S+8，envp表头S+40，auxv起点S+72，`bprm.exec=U-9`。扫描末p=U-9，写表末sp=S+72；bprm.p为S。

### shell自己的命令行参数来源

| 对象 | 来源和使用 |
|---|---|
| main entry | loader-provided argc,argv (given entry model only)；main signature 66 |
| read line | local buf cleared then read_line(buf), return not checked by caller；line92 |
| fresh parser outputs | local __argc=0,__argv=NULL; parse_command(buf,&__argc,&__argv)；line96-98 |
| parser | counts tokens, if none returns -1; allocates (*argc+1) slots, clears them, assigns pointers into buf and writes NUL at separators; find_cmd(**argv)；parse_command 154-185; valid successful allocation given, no execution |
| dispatch | index<0 print error; otherwise run_cmd(index,__argc,__argv)；run_cmd calls shell_internal_cmd[index].function(argc,argv) |

源上shell main的argc/argv与每次循环局部__argc/__argv不是同一参数来源。read_line的返回值在该调用处未用作分派门；parse_command获得buf及两个输出对象地址；它计数、为空返回-1、在给定有效分配条件下分配并清零槽，槽指向buf并在分隔处写NUL，返回find_cmd。index<0打印错误，else才run_cmd，后者调用表中function(argc,argv)。没有执行解析器，不证明任意字符串的语法/安全边界。E140-067–076。

### 固定构建声明与实际入口的区别

顶层 myinitramfs/CMakeLists.txt 第40–41行设置MUSL_PATH与musl-gcc路径，第46–48行include并加入myinit、myshell；第50–51行install init/sh到boot。两子目录各声明add_executable(init...)、add_executable(sh...)。独立mylib/CMakeLists.txt声明myglibc、csu、util三个子目录；独立csu声明mycrt STATIC及C/ASM源列表。源窗E140-077–091。在所读固定顶层声明中未见加入mylib或直接链接mycrt的语句；这不认证全部构建配置、外部musl工具链及真实二进制的CRT选择。不能把候选crt1_c.c::_start的C形参模型接成真实ELF入口寄存器或首个main，保留第25/66/69的未证边界。

## 图和交付边界

机器件及DOT共193个唯一节点ID、165条唯一from/to/kind/label四元组，实际边对应，独立模型之间没有实际执行的因果边；历史修订和布局投影的关系另用明确类型。新正式源码98个ID/98个物理区间；旧139的92/91由C09核收记录及逐项索引承接，不混入本批计数。完整本地P9、预处理、编译链接、仓库程序/脚本、Graphviz、QEMU、实机及真实文件、进程、PID、调度、时钟操作均NOT_RUN。原执行分支仅本目录六件；主线独审PENDING，G04后STOP_WRITING。
