---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-EXEC-ARGUMENT-LIMITS-143"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-22"
conversation_display_name: "MYOS2-A-C10a 代码分析执行"
produced_by: "GPT-6.2（系统模型族；界面/实际后台型号未独立核实）"
model_per_owner: "次一档GPT-6.2 Sol，Extra／高推理强度"
actual_effort: "UNATTESTED"
date: "2026-11-24"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
read_channel: "connector"
status: "AUTHOR_CONTENT_PREPARED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
full_local_P9: "NOT_RUN"
inputs_read: [{"path":"mykernel/scripts/options_flags.cmake","ref":"a039d9803ade","blob":"6019436417ad"},{"path":"mykernel/sched/forkexec/exec.c","ref":"a039d9803ade","blob":"0861ebae6fc2"},{"path":"mykernel/lib/string/strnlen_user.c","ref":"a039d9803ade","blob":"dae5c6c14aa4"},{"path":"mykernel/lib/string/string.c","ref":"a039d9803ade","blob":"cbb2752bc8b2"},{"path":"mykernel/include/uapi/linux/binfmts.h","ref":"a039d9803ade","blob":"a60f4e7c53b0"},{"path":"mykernel/include/uapi/linux/resource.h","ref":"a039d9803ade","blob":"6a683439847c"},{"path":"mykernel/include/uapi/linux/limits.h","ref":"a039d9803ade","blob":"bddfd09e4be2"},{"path":"mykernel/include/linux/fs/binfmts.h","ref":"a039d9803ade","blob":"002c2aec4aa8"},{"path":"mykernel/include/linux/kernel/minmax.h","ref":"a039d9803ade","blob":"d22095ba1acc"},{"path":"mykernel/include/linux/kernel/types.h","ref":"a039d9803ade","blob":"ef531baa8317"},{"path":"mykernel/mm/misc/mm_misc_const.h","ref":"a039d9803ade","blob":"ec983b79166f"},{"path":"mykernel/arch/x86_64/mm/page/page_const_arch.h","ref":"a039d9803ade","blob":"b10e280f26f0"},{"path":"mykernel/include/uapi/asm-generic/posix_types.h","ref":"a039d9803ade","blob":"0246766d9a75"},{"path":"mykernel/include/uapi/linux/const.h","ref":"a039d9803ade","blob":"d8bcaf7e3151"}]
open_questions: ["实际预处理、C对象/指针有效性、GUP返回/页、真实用户访问与运行仍未证"]
record_type: "phase2_exec_argument_limits_analysis"
---

# 143 · exec参数预算、单串长度及拒绝时点

## 输入、证据及限制

199合同限定19组（8+4+7）；142 PASS_PENDING_LOCAL，R141-01 CLOSED_DOCUMENTATION_ONLY，141合并消费PASS_PENDING_LOCAL，140及五项历史文档关闭保持，D00空。198完整读取并持续继承。以下SOURCE_FACT来自evidence/source_index；给定值及整数/条件模型均为[INFERRED]，不代表实测。采用int有符号32位、ulong/size_t/指针8字节，PAGE_SIZE=4096；配置先读，实际预处理未做。ulong及size_t链、PAGE_SHIFT/PAGE_SIZE/_AC的C分支分别见E143-008,E143-009,E143-010,E143-011,E143-005,E143-006,E143-007,E143-089,E143-090,E143-091。必要补读page_const_arch.h、const.h、asm-generic/posix_types.h以落实页/类型定义；mm_misc_const.h所读为auxv常量，不把它当页尺寸定义。

139 A/B/C及R139-03和140判闭用于旧小向量、T基准与清理前字段消费；冻结140 A1的字符串顺序由当前重读机器件得出。23/24的GUP缺return与页引用、70/71清理/正常返回约束保留：get_arg_page局部int ret接收GUP返回、<=0门及page输出是不同契约（E143-057,E143-058,E143-059）。本批不补造真实有效返回/页、跨越未定义行为的成功路径或真实用户地址。C给定短预算独立于A，不与142 p0混用。

## A · 总预算、指针预算及p阈值（8组）

_STK_LIM=8388608，ARG_MAX=131072，MAX_ARG_STRLEN=PAGE_SIZE*32=131072，MAX_ARG_STRINGS=0x7fffffff（2147483647），源E143-001,E143-002,E143-003,E143-004。L0=max_t(ulong,8388608/4*3,131072)=6291456=0x600000。rlim_cur参与min仅注释，不是活动限额。E143-021,E143-022,E143-023,E143-024,E143-025,E143-026,E143-027。

bprm argc/envc是int，p/argmin为ulong；当前活动max(x,y)展开__cmp(x,y,>)的条件表达式，两个候选均int。max_t则显式转换操作数。故max(argc,1)+envc先作为int求和，之后才同size_t的sizeof(void*)相乘并赋ulong。E143-015,E143-016,E143-017,E143-013,E143-012,E143-026。A_INT_SUM的数学值2147483648超出所给有符号32位模型，停止推导，不把它解释为定义良好的负值、正常-E2BIG或64位先行运算。A_NEG在负计数门即返回。

| ID | argc/envc | L0 | 计数门 | max(argc,1) | 数学和 | int和 | ptr_size字节 | 指针门 | 减预算实际执行 | 剩余limit | set_limit | 返回 | argmin |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| A_NEG | -1/0 | 6291456 | REJECT | NOT_REACHED | NOT_REACHED | NOT_REACHED | NOT_REACHED | NOT_REACHED | false | NOT_REACHED | NOT_REACHED | -E2BIG | A_old |
| A_ZERO | 0/0 | 6291456 | PASS | 1 | 1 | 1 | 8 | PASS | true | 6291448 | REACHED | 0 | 0x200008 |
| A_SMALL | 2/2 | 6291456 | PASS | 2 | 4 | 4 | 32 | PASS | true | 6291424 | REACHED | 0 | 0x200020 |
| A_PTR_BELOW | 1/786430 | 6291456 | PASS | 1 | 786431 | 786431 | 6291448 | PASS | true | 8 | REACHED | 0 | 0x7ffff8 |
| A_PTR_EQUAL | 1/786431 | 6291456 | PASS | 1 | 786432 | 786432 | 6291456 | REJECT | false | NOT_REACHED | NOT_REACHED | -E2BIG | A_old |
| A_INT_SUM | 2147483647/1 | 6291456 | PASS | 2147483647 | 2147483648 | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED |

六例入口p=0x800000，旧argmin=A_old（未指定）。A_PTR_EQUAL数学剩额0但源码没有执行减预算/后续set_limit，argmin保留A_old；A_INT_SUM仅记录表达式前最后定义良好的p/argmin，表达式后NOT_DERIVED。A_ZERO仅直接helper模型，不证明kernel_execve零argc到达。A_PTR_BELOW余8；无实际字符串容量/页映射结论。预留只是max(argc,1)+envc个指针，不含argc槽、两终止NULL、auxv、平台串及字符串需求。

set_limit的直接模型（E143-019,E143-012）：

| ID | limit | 入口p | 旧argmin | 严格p<limit门 | 返回 | argmin | p末 |
|---|---|---|---|---|---|---|---|
| A_SET_EQ | 8 | 0x8 | A_old | NONE_IN_HELPER | 0 | 0x0 | 0x8 |
| A_SET_LT | 8 | 0x7 | A_old | p<limit | -E2BIG | A_old | 0x7 |

等值合法算术结果0不是实际访问地址0；不足则-E2BIG且不赋argmin。

## B · 单串长度上限（4组，两helper同输入）

显式选用string.c的#ifndef __HAVE_ARCH_STRNLEN内generic strnlen（E143-028,E143-029,E143-030），条件定义与构建面分开，不认证实际预处理。M=131072。for(sc=s; count-- && *sc!='\\0'; ++sc)最多读取索引0..M−1；短路时不读索引M。返回sc−s为size_t；strnlen_user活动体return strnlen(str,count)+1，返回long再赋int len；kernel直接strnlen+1赋int，均在本题可表示。E143-031,E143-034,E143-047,E143-037,E143-018,E143-038,E143-048。

| ID | 给定首NUL | 实际读取索引 | r | 两helper len | 最终测试count旧值 | 测试后count | 回绕 | 长度门 | user返回/p | kernel返回/p |
|---|---|---|---|---|---|---|---|---|---|---|
| B_SHORT | 131070 | [0,131070] | 131070 | 131071 | 2 | 1 | false | PASS | NOT_DERIVED/NOT_DERIVED_AFTER_LENGTH_GATE | NOT_DERIVED/NOT_DERIVED_AFTER_LENGTH_GATE |
| B_EXACT | 131071 | [0,131071] | 131071 | 131072 | 1 | 0 | false | PASS | NOT_DERIVED/NOT_DERIVED_AFTER_LENGTH_GATE | NOT_DERIVED/NOT_DERIVED_AFTER_LENGTH_GATE |
| B_NEXT | 131072 | [0,131071] | 131072 | 131073 | 0 | 2^64-1 | DEFINED_UNSIGNED_WRAP_ON_FAILED_CONDITION | REJECT | -E2BIG/UNCHANGED | -E2BIG/UNCHANGED |
| B_UNTERM | UNKNOWN_AFTER_PREFIX | [0,131071] | 131072 | 131073 | 0 | 2^64-1 | DEFINED_UNSIGNED_WRAP_ON_FAILED_CONDITION | REJECT | -E2BIG/UNCHANGED | -E2BIG/UNCHANGED |

B_SHORT和B_EXACT仅通过长度门，不签后续预算/取页/复制。B_NEXT与B_UNTERM前M字节均非零，r=M、len=M+1，两者均在valid_arg_len拒绝，p未扣、页/复制SKIPPED。返回值不说明索引M或其后是否存在NUL，不是完整真实串长。耗尽M次读后仍有count旧0的终止测试，unsigned size_t减1回绕到2^64−1，旧0使&&短路；与A_INT_SUM的有符号不可表示不同。短串NUL测试也递减，但不回绕。正常给定len不为0；user !len门用先前-EFAULT，kernel len==0返回-EFAULT，均非本四例实际选中。注释中的do_strnlen_user、user_read_access_begin和return0未激活（E143-032,E143-033），不能证明用户故障恢复、权限或并发安全。

## C · 独立短预算、预扣p和后继调用（7组）

源码两helper均先测长/长度门，再整体减p，直接比较p<argmin；bprm_hit_stack_limit定义虽相同但这些位置没有调用它。取页发生在空间门之后的循环，局部pos/str或arg/len按chunk先推进；copy/put需要独立有效页和正常操作前提（E143-039,E143-049,E143-020,E143-042,E143-051,E143-043,E143-052,E143-044,E143-053,E143-046,E143-054）。空间失败无该局部p回滚；最终对外errno另需caller清理正常返回。

| ID | 入口p | argmin | len | 预扣后p | 余预算 | 空间门 | get_arg_page | memcpy_to_page | 局部返回 |
|---|---|---|---|---|---|---|---|---|---|
| C_EQ | 0x70000000 | 0x6ffffff0 | 16 | 0x6ffffff0 | 0 | PASS | CONDITIONALLY_REACHED_IF_NEXT_LOOP_STEP_REACHED | CONDITIONAL_ON_VALID_PAGE_AND_NORMAL_REACH | NOT_DERIVED |
| C_OVER | 0x70000000 | 0x6ffffff0 | 17 | 0x6fffffef | -1 | REJECT | SKIPPED | SKIPPED | -E2BIG |

C_EQ、C_OVER分别同一输入适用两helper；C_EQ仅允许进入后段，不写return0或复制成功。C_OVER在取页前拒绝，p已变；B_NEXT/B_UNTERM同为-E2BIG却在长度门拒绝，尚未扣减p。两者实际复制均NOT_ESTABLISHED。

### C30/C29、C20/C19 条件流水

冻结140 A1：filename /bin/tool 10，envp[1] F=xy 5，envp[0] E=1 4，argv[1] -q 3，argv[0] display 8（含NUL），按源码filename、逆序env、逆序argv顺序。用户及内核非零argc路线分别由copy_strings与copy_strings_kernel消费，filename均copy_string_kernel；C20/C19仅用户零argc路线，argv helper计数0不进入字符串循环，补空串由kernel helper。count和入口拒绝、限额、copy、保存exec、补串与后继调用证据：E143-060,E143-061,E143-062,E143-063,E143-064,E143-065,E143-066,E143-068,E143-069,E143-070,E143-071,E143-072,E143-073,E143-074,E143-075,E143-076,E143-079,E143-080,E143-081,E143-082,E143-083,E143-084,E143-085,E143-086,E143-055,E143-056。

下表每行是独立条件观察点，前缀正常返回及条件写入效果分别给定，而非从p变化或ret0证明真实字节写入。actual_copy_status每行均NOT_ESTABLISHED。剩余需求按总需求减条件写入前缀（失败串仍未满足），与p预扣后剩余预算分列。array wrapper仅在该数组所有元素成功后才返回0，某一元素条件正常效果不是整个wrapper返回。字段记录到清理前，不读取已释放bprm。

#### C30（do_execveat_common / kernel_execve；需求10+5+4+3+8）

| 阶段ID/动作 | 本次需求/预扣 | p | argmin | bprm.exec | argc/envc | 累计预扣 | 条件写入 | 剩余需求 | 余预算 | 取页 | 复制 | 结果/门 | 后继 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| C30_T01 entry to copy stage | 0/0 | 0x70000000 | 0x6fffffe2 | NOT_STIPULATED | 2/2 | 0 | 0 | 30 | 30 | NOT_REACHED | NOT_REACHED | NOT_REACHED | filename copy |
| C30_T02 filename | 10/10 | 0x6ffffff6 | 0x6fffffe2 | 0x6ffffff6 | 2/2 | 10 | 10 | 20 | 20 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C30_T03 envp[1] | 5/5 | 0x6ffffff1 | 0x6fffffe2 | 0x6ffffff6 | 2/2 | 15 | 15 | 15 | 15 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C30_T04 envp[0] | 4/4 | 0x6fffffed | 0x6fffffe2 | 0x6ffffff6 | 2/2 | 19 | 19 | 11 | 11 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C30_T05 argv[1] | 3/3 | 0x6fffffea | 0x6fffffe2 | 0x6ffffff6 | 2/2 | 22 | 22 | 8 | 8 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C30_T06 argv[0] | 8/8 | 0x6fffffe2 | 0x6fffffe2 | 0x6ffffff6 | 2/2 | 30 | 30 | 0 | 0 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C30_T07 conditional bprm_execve call boundary | 0/0 | 0x6fffffe2 | 0x6fffffe2 | 0x6ffffff6 | 2/2 | 30 | 30 | 0 | 0 | SKIPPED | SKIPPED | BPRM_EXECVE_RESULT_NOT_DERIVED | bprm_execve conditionally reached; result and later cleanup not derived |

C30条件调用前最后观测p=0x6fffffe2、exec=0x6ffffff6、条件前缀=30字节、actual_copy_status=NOT_ESTABLISHED，bprm_execve=CONDITIONAL_REACHED。末串等值通过；条件正常复制后才能到达exec调用，结果未知。

#### C29（do_execveat_common / kernel_execve；需求10+5+4+3+8）

| 阶段ID/动作 | 本次需求/预扣 | p | argmin | bprm.exec | argc/envc | 累计预扣 | 条件写入 | 剩余需求 | 余预算 | 取页 | 复制 | 结果/门 | 后继 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| C29_T01 entry to copy stage | 0/0 | 0x70000000 | 0x6fffffe3 | NOT_STIPULATED | 2/2 | 0 | 0 | 30 | 29 | NOT_REACHED | NOT_REACHED | NOT_REACHED | filename copy |
| C29_T02 filename | 10/10 | 0x6ffffff6 | 0x6fffffe3 | 0x6ffffff6 | 2/2 | 10 | 10 | 20 | 19 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C29_T03 envp[1] | 5/5 | 0x6ffffff1 | 0x6fffffe3 | 0x6ffffff6 | 2/2 | 15 | 15 | 15 | 14 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C29_T04 envp[0] | 4/4 | 0x6fffffed | 0x6fffffe3 | 0x6ffffff6 | 2/2 | 19 | 19 | 11 | 10 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C29_T05 argv[1] | 3/3 | 0x6fffffea | 0x6fffffe3 | 0x6ffffff6 | 2/2 | 22 | 22 | 8 | 7 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C29_T06 argv[0] | 8/8 | 0x6fffffe2 | 0x6fffffe3 | 0x6ffffff6 | 2/2 | 30 | 22 | 8 | -1 | SKIPPED | SKIPPED | -E2BIG | goto out_free (bprm_execve SKIPPED) |
| C29_T07 pre-cleanup error snapshot | 0/0 | 0x6fffffe2 | 0x6fffffe3 | 0x6ffffff6 | 2/2 | 30 | 22 | 8 | -1 | SKIPPED | SKIPPED | -E2BIG | free_bprm then putname; return -E2BIG only if cleanup returns normally |

C29清理前失败现场p=0x6fffffe2、exec=0x6ffffff6、条件前缀=22字节、actual_copy_status=NOT_ESTABLISHED，bprm_execve=SKIPPED。末8字节扣减越界，最后取页/复制SKIPPED；exec字段在已给定filename正常返回后赋T−10，所读局部不自动撤销。

#### C20（do_execveat_common；需求10+5+4+1）

| 阶段ID/动作 | 本次需求/预扣 | p | argmin | bprm.exec | argc/envc | 累计预扣 | 条件写入 | 剩余需求 | 余预算 | 取页 | 复制 | 结果/门 | 后继 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| C20_T01 entry to copy stage | 0/0 | 0x70000000 | 0x6fffffec | NOT_STIPULATED | 0/2 | 0 | 0 | 20 | 20 | NOT_REACHED | NOT_REACHED | NOT_REACHED | filename copy |
| C20_T02 filename | 10/10 | 0x6ffffff6 | 0x6fffffec | 0x6ffffff6 | 0/2 | 10 | 10 | 10 | 10 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C20_T03 envp[1] | 5/5 | 0x6ffffff1 | 0x6fffffec | 0x6ffffff6 | 0/2 | 15 | 15 | 5 | 5 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C20_T04 envp[0] | 4/4 | 0x6fffffed | 0x6fffffec | 0x6ffffff6 | 0/2 | 19 | 19 | 1 | 1 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C20_T05 argv copy call with count 0 | 0/0 | 0x6fffffed | 0x6fffffec | 0x6ffffff6 | 0/2 | 19 | 19 | 1 | 1 | SKIPPED | SKIPPED | 0 | zero argc branch |
| C20_T06 synthetic empty argv[0] | 1/1 | 0x6fffffec | 0x6fffffec | 0x6ffffff6 | 1/2 | 20 | 20 | 0 | 0 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | argc assignment=1 then conditional exec call |
| C20_T07 conditional bprm_execve call boundary | 0/0 | 0x6fffffec | 0x6fffffec | 0x6ffffff6 | 1/2 | 20 | 20 | 0 | 0 | SKIPPED | SKIPPED | BPRM_EXECVE_RESULT_NOT_DERIVED | bprm_execve conditionally reached; result and later cleanup not derived |

C20条件调用前最后观测p=0x6fffffec、exec=0x6ffffff6、条件前缀=20字节、actual_copy_status=NOT_ESTABLISHED，bprm_execve=CONDITIONAL_REACHED。补串正常返回0后才argc=1；exec调用条件可达，调用结果未知。

#### C19（do_execveat_common；需求10+5+4+1）

| 阶段ID/动作 | 本次需求/预扣 | p | argmin | bprm.exec | argc/envc | 累计预扣 | 条件写入 | 剩余需求 | 余预算 | 取页 | 复制 | 结果/门 | 后继 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| C19_T01 entry to copy stage | 0/0 | 0x70000000 | 0x6fffffed | NOT_STIPULATED | 0/2 | 0 | 0 | 20 | 19 | NOT_REACHED | NOT_REACHED | NOT_REACHED | filename copy |
| C19_T02 filename | 10/10 | 0x6ffffff6 | 0x6fffffed | 0x6ffffff6 | 0/2 | 10 | 10 | 10 | 9 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C19_T03 envp[1] | 5/5 | 0x6ffffff1 | 0x6fffffed | 0x6ffffff6 | 0/2 | 15 | 15 | 5 | 4 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C19_T04 envp[0] | 4/4 | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0/2 | 19 | 19 | 1 | 0 | CONDITIONALLY_REACHED_IN_INDEPENDENT_PAGE_FRAGMENT | GIVEN_CONDITIONAL_COPY_EFFECT_ONLY | GIVEN_NORMAL_RETURN_0_FOR_LATER_CONSUMPTION | continue only with given normal prefix effects/returns |
| C19_T05 argv copy call with count 0 | 0/0 | 0x6fffffed | 0x6fffffed | 0x6ffffff6 | 0/2 | 19 | 19 | 1 | 0 | SKIPPED | SKIPPED | 0 | zero argc branch |
| C19_T06 synthetic empty argv[0] | 1/1 | 0x6fffffec | 0x6fffffed | 0x6ffffff6 | 0/2 | 20 | 19 | 1 | -1 | SKIPPED | SKIPPED | -E2BIG | goto out_free (bprm_execve SKIPPED) |
| C19_T07 pre-cleanup error snapshot | 0/0 | 0x6fffffec | 0x6fffffed | 0x6ffffff6 | 0/2 | 20 | 19 | 1 | -1 | SKIPPED | SKIPPED | -E2BIG | free_bprm then putname; return -E2BIG only if cleanup returns normally |

C19清理前失败现场p=0x6fffffec、exec=0x6ffffff6、条件前缀=19字节、actual_copy_status=NOT_ESTABLISHED，bprm_execve=SKIPPED。补串1字节先预扣后拒绝，argc仍0，赋1和exec调用SKIPPED。

### K0

独立给定kernel_execve的count_strings_kernel返回0，先把retval改-EINVAL，retval<0跳out_free，bprm.argc赋值尚未到达。初值NOT_STIPULATED，不写已赋0；env计数/赋值、限额、filename/exec赋值、env/argv复制与bprm_execve全SKIPPED。清理前p/argmin/exec均未给定。E143-079,E143-080,E143-081,E143-082,E143-083,E143-084,E143-085,E143-086,E143-087。

C29/C19/K0的最终外返相同errno分别仅在free_bprm及putname正常返回后成立；free_bprm末有kfree(bprm)，不读释放后字段（E143-077,E143-087,E143-088）。不由清理函数名补写回滚或真实资源归还。23/24、70/71的限制仍在。C30/29/20/19各最多8个观察点。

## 消费与未证范围

共19组，A8/B4/C7；新91窗；图68节点/81边，源顺序、给定模型、独立片段及条件调用分开。没有真实CPU/内存/用户访问、编译测量、预处理选型、Graphviz、MyOS2运行或完整本地P9。本轮D00空，不重计旧129/73；交独立主线审查，不启动144。
