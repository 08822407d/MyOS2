---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-UACCESS-COPY-63
record_type: bounded_static_uaccess_copy_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
source_pin_short12: a039d9803ade
execution_base_short12: 54101196eb11
fixed_contract_short12: 3044d74381f0
base_snapshot: time
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第63批：access_ok、批量复制和标量访问

本包只审固定time源码，不将源级数值谓词或回调请求当成真实用户映射、硬件错误恢复、已复制字节或最终用户返回。任何不合法对象、访问、类型计算或不返回路径在首次失去合法性前提时停住。evidence.md列64个独立真实定义内1–5行短锚；facts-and-boundaries.yaml将数值模型及历史消费分栏。原已闭合/接收材料不覆盖。

## G00及历史限定

固定93号、62审查回执、63与79号本轮全文回读。62为PASS_PENDING_LOCAL，无D00；R62-S01分别限定后置int自增与巨量循环、FAT零长度的先行出口、累计ret和局部position增量是两个输入；R62-S02要求图的调用边/累计/清理仅用于有条件的活动前缀。61三个补充、58 B25已确认正确并撤回主线指控、55、59、60、09和09-DOC-FIX的有限裁定保持不变。本包重读09原条目RS-06/07的status=deferred_owner_not_ready、59原DE59-F05/F06为needs_evidence、60原FC60-F03为needs_evidence、62原WV62-F07为needs_evidence、55原BI55-F01的disposition=source_only_conditional。只新增uaccess消费者及反例，不重编号或自签历史。

G00 direct time=a039d9803ade、master=e8e60cd19540、lead/PR16=3044d74381f0、exec/PR17=54101196eb11；两PR均Open/unmerged、17 Draft，开放集合只有16/17，新63目录写前404，time双金丝雀匹配。目标提交前仍须再核HEAD及写区。

## A / G01：定义选择和数值范围

linux/kernel/uaccess.h第11行包含asm/uaccess.h，后者先在第14行包含asm/uaccess_64.h、再在第16行包含generic access_ok.h。x86头的inline __access_ok之后定义同名对象宏 #define __access_ok __access_ok，generic头使用#ifndef __access_ok保护自己的另一个inline定义；generic随后在#ifndef access_ok下定义访问包装。故在所见include顺序和未被其它宏改变的额外前提下，generic备用阈值函数被跳过，实际access_ok包装调用x86版本。两个同名源定义不是运行时串行调用。include/guard位置是source_context；函数和单宏分别有A01–A08真实定义内短锚。实际宏展开/编译器绑定未运行。

x86的valid_user_address(x)定义为(long)x>=0，需明示目标有符号转换模型。__access_ok在编译器认为(size<=PAGE_SIZE)为常量且值真时，直接用起始地址符号；否则取ulong sum=size+(ulong)ptr，并返回valid_user_address(sum) && sum>=(ulong)ptr。PAGE_SHIFT=12，所读PAGE_SIZE为4096；关键是__builtin_constant_p是否认定布尔表达式为常量，不可从单次运行size数值直接倒推分支。

facts中的64位二补数模型覆盖NULL数值、size0/1/4096/4097、TASK_SIZE_MAX附近、符号边界附近和ulong加法回绕。比如ptr=2^63−16、size32：编译器确认常量小尺寸时快捷门给true，general计算出的端点进入负半区给false。generic备用式子为size<=limit && addr<=limit−size；独立所读limit=(1UL<<47)−4096。这是源级备用谓词，不能与x86判定在真实运行中相与。不管哪个给true，都不证明VMA映射、权限、页驻留、对象寿命及CPU可访问性；ptr数值0且size0出现true并非NULL可读。A01–A16；UC63-F01/F02。

## B / G02：残量、零填充与raw/asm

公开copy_from_user的活动体直接调用_inline_copy_from_user；后者res初值n，from方向access_ok、might_fault及instrument门被注释，直接令res=raw_copy_from_user(to,from,n)。res非零时才调用memset(to+(n−res),0,res)，并返回res。只有额外保证0<=res<=n、目的缓冲有效且raw遵守前缀拷贝合约，才能把n−res视为已复制前缀、后r字节视为被清零；例n4/r1为前三字节复制、末一字节填零，n4/r4为全目的清零，却不能说已读取用户正确数据。res>n时n−res无符号回绕，目标偏移及memset没有可靠安全前提；对象NULL/越界也会先阻断推论。B01–B03/B06/B07/B10/B23；UC63-F03。

公开copy_to_user直接使用_inline_copy_to_user。它在access_ok(to,n)为假时不发起raw调用而返回原n，判真时执行instrument与raw_copy_to_user，并返回raw残量。没有to方向的尾部memset；n0可以数值返回0而没有验证用户内存实际有效，不能从返回0无条件推出真实复制、硬件成功或数据完整。B04/B05/B08/B09/B11；UC63-F04。

两个raw方法分别把from/to传给copy_user_generic。所读活动源码顺序为stac()、内联asm的ALTERNATIVE("rep movsb","call rep_movs_alternative",ALT_NOT(X86_FEATURE_FSRM))、_ASM_EXTABLE_UA(1b,2b)、+c(len)/+D(to)/+S(from)输出约束、clac()及return len。stac/clac由smap.h的alternative封装；asm.h与alternative.h定义来源已读。这仅是源码文本和候选指令/错误表意图，不能承诺真实CPU FSRM选择、异常表安装、SMAP恢复或数据残量必正确。rep_movs_alternative在选定允许源中只定位到声明；未定位函数实现体，未将声明冒充VERIFIED定义锚，其实际实现与uint形式参数是否对应asm字符串调用宽度都标记缺口UC63-U03。n0仍只按当前C表达式及asm请求序列建模，不等于所有指针都合法。B12–B26。

## C / G03：标量宏及两个实际消费者

asm/uaccess.h的活动get_user宏是(x)=*(__typeof(&(x)))(ptr)，之后为-ENOERR；活动put_user是*(ptr)=(__typeof(*(ptr)))(x)，之后为-ENOERR；ENOERR=0。仅在直接加载/存储及相关类型转换正常完成时，宏表达式才有0结果。旧注释中-EFAULT、失败清零、异常汇编helper均非活动路径；无效ptr时不能推安全-EFAULT，也不能跨过坏访问而宣布正常返回0。get_user的目标x类型参与指针读取宽度，put_user的目的ptr指向类型决定存储转换；helper-only宽度反例不是实际调用者错误证据。C01–C03/C22/C23；UC63-F05。

rt_sigreturn由regs->sp−sizeof(long)构造frame，先用access_ok(frame,sizeof(*frame))检查数值范围，失败去badframe的活动return0。数值检查正常返回true后，两处get_user读取uc_sigmask中的u64与uc_flags；若每次加载正常，两次宏值都是0，if(error)不会被触发。若其中字段不能合法加载，则最早停在直接解引用处，不能凭该宏注释预测进入badframe。之后还有set_current_blocked及restore_sigcontext，后者copy_from_user可有非零残量并使restore返回false；所以即使两个get_user正常，也不能把整个badframe出口唯一归于地址检查或保证最终regs->ax用户可见。恢复侧和信号返回尾段沿09/SR08-02的历史限定，实际映射与硬件异常未执行。C04–C10；UC63-F06。

filldir64首先以直接store写prev->d_off及dirent的ino/reclen/type，才调用copy_to_user写名字，并把返回保存在不再判读的cplen；之后仍直接写终止NUL，更新prev_reclen/current_dir/count。若前序直接store对象无效，中途copy_to_user的access_ok门不可能补成对此前store的保护；若raw调用正常返回非零残量但前序store有效，本体也没有基于cplen的-EFAULT跳转。efault标签位于正常return之后，没有活动goto进入。getdents64收尾的put_user属于注释，不作为实际补写。既有DE59-F05/F06仍是原根因，本批只做底层复制/标量消费者精确解释。C11–C21。

## 验收门与NOT_RUN

A/B/C=completed_in_scope，仅指有界源码分析完成；新增UC63-F01–F06为6条需其它证据的条件候选，UC63-U01–U08为8项缺口。六件含64条固定time定义内短锚（A16、B25、C23）、15项source ledger、52例自建有限数学/状态模型及局部DOT；数量不等于独立漏洞数量。所有仓库C/ASM/旧脚本/夹具、预处理/编译/链接、完整本地P9、Graphviz、QEMU/实机、实际用户内存/异常表/CPU、用户态ABI、真实信号/目录调用均NOT_RUN。执行者不修改内核或旧成果、不自签63 PASS、也不启动64；当前状态只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，交MYOS2-A-C05独立验收。
