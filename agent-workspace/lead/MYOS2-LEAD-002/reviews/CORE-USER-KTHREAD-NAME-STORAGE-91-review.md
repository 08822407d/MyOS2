---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-KTHREAD-NAME-STORAGE-91-REVIEW-001
record_type: independent_bounded_static_review_with_mainline_supplements
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner正式验收安排；本轮未重新声明档位）
date: 2026-11-16
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract129_short12: cc8bd9c768fd
reviewed_base_short12: 034cd9c52dbf
reviewed_head_short12: 474a2c0363f0
reviewed_delivery_comment: 6905875456
executor: MYOS2-A-C06c / MYOS2-LEAD-002-CHAT-EXEC-12
status: REVIEW_COMPLETE_IN_BOUNDED_STATIC_SCOPE
acceptance_verdict: PASS_PENDING_LOCAL
returned_items: []
mandatory_supplements: [R91-S01, R91-S02, R91-S03, R90C-S01]
R90_01_and_02: CLOSED_BY_SEPARATE_SAME_ROUND_RECEIPT
batch90_composite: PASS_PENDING_LOCAL
original_artifacts_modified: false
original_author_self_check: 81/81/0
reviewer_readable_raw_tag_count: 81
self_check: {verified_claims: 6, quotes_reconfirmed: 6, downgraded_to_inferred: 0, scope: 本回执六段固定源短锚的可读核对，与作者81条分账}
full_local_P9: NOT_RUN
original_six_file_automatic_regex_JSON_DOT_checks: NOT_RUN
reviewer_math_cases: 17
reviewer_additional_arithmetic_checks: 2
runtime_evidence: NOT_RUN
next_followup: CORE-USER-PRINTF-INTEGER-FORMAT-92
next_contract: ../130-user-printf-integer-format-contract.md
phase: 2
inputs_read: [91六件及正式评论全文, 129与历史回执的同一C06固定对象, 固定源码和本轮独立数学核对，详见检查点]
owner_decisions_requested_now: []
open_questions: []
---

# 第91批按限定接收，无新增返工

91裁定PASS_PENDING_LOCAL，必须合读R91-S01/S02/S03；90两项文档补正另件关闭。主线直接追加局部模型、索引及消费限定，不要求重做91，也不安排纯文档返工回合。完整本地P9和真实内核运行仍未完成。

## 1. 固定交付与可读闸门

评论6905875456创建/更新于2026-11-16T02:46:39Z，全文已读。02:03:10Z以来评论集合仅见这一条91正式交付，和Owner所报STOP_WRITING一致；不据此认证另一聊天的后台进程。
compare 034cd9c52dbf→474a2c0363f0为ahead1/behind0、一个提交，精确六件added：MANIFEST97、正文118、facts1631、DOT130、deferred314、evidence719，共3009行，旧件零净改删。初核direct lead=cc8bd9c768fd、exec=474a2c0363f0、master=e8e60cd19540、time=a039d9803ade。PR16/17均Open/unmerged，17保持Draft；缓存base_sha不替代master直接引用。

本轮六件全文读取，长响应重叠补齐：正文1至末尾并85至EOF补尾；evidence1–250、250–500、501至EOF；facts1–300、301–620、621–940、915–1220、1221–1500、1501至EOF；deferred全文及285至EOF补尾；MANIFEST/DOT全文。没有以作者摘要代替正文。

| P9项 | 本次范围与结果 |
|---|---|
| 1 | 可读全文未见40位十六进制串；原六件自动正则扫描NOT_RUN，不冒称完整机械门。 |
| 2/13 | 逐组读81短锚，强制主题定义及主要旁证以独立固定源码核对；符号/引文位于所指定义内，短片段的语义范围须按S03补足。未发现须另退回的跨定义锚；全81自动字节/定义解析NOT_RUN。 |
| 3/4 | 六件及源码路径有实际固定原文；短锚不依赖行号定位，全树符号/路径机械验证NOT_RUN。 |
| 5 | 可读枚举E001–E081，其他五件未见额外原始证据标签；MANIFEST直接提供81/81/0，等式成立。独立自动全包grep NOT_RUN。 |
| 6 | 不作外部C99全兼容、真实ABI或工具运行认证；外部尚未证前提不作为已核事实。 |
| 7/8 | 公约逐字句、规范通道connector与两分支金丝雀按本有效C06同pin既有原文核对，本轮未重新下载四份金丝雀。 |
| 9/10/12 | 无代码件、补丁或聊天降级交付，相应N/A。 |
| 11 | 所有仓库程序、构建、Graphviz、QEMU、实机、真实并发均NOT_RUN。 |

9组49行、22条件模型、4候选和9缺口是不同计数域；不等于22项测试通过或13个已证故障。规范自检字段已齐备，不把发布前状态文字误认为提交尚未存在；后置交付以真实commit/comment为准。

## 2. 主干技术结论

A：三种worker格式、42字节自动数组、调用方未消费scnprintf返回、再以%s传入创建包装，均有固定定义支持。int32且十进制转换正确的条件上界为普通31、unbound32字符，加NUL仍可放入42字节；不能据任意小缓冲案例断言实际worker截断。
B：kvasprintf两遍格式化、独立申请、NULL返回及注释掉的不一致检查成立；va_copy没有复制所指字符串字节。名字分配失败由创建包装转换为ERR_PTR(-ENOMEM)，不能当作NULL任务返回证据。第一遍NULL零容量的表达式、异常返回来源、供应器与并发稳定性仍为未证前提。
C：请求、堆全名、私有kthread、task.comm、栈id_buf及读回目标不是同一个对象。全名指针转移不等于深拷贝；释放create不自动释放full_name；get_kthread_comm的void返回与__get_task_comm返回buf不同。释放函数存在与实际退出调用成立不同，所读free_task处注释不能外推为全仓没有其它调用。

## 3. R91-S01：格式化返回和指针模型边界

需求长度与已写字符的区分，只在受支持格式、对象有效、先行调用正常返回、结果为非负可表示整数且未走拒绝分支时成立。下面分支在正常输出及NUL处理之前：

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
	if (WARN_ON_ONCE(size > INT_MAX))
		return 0;
```

若该条件表达式取真并正常返回，目的缓冲不由此分支写成空串。vscnprintf随后得到0，也不能把“返回0”当作NUL已经写入的证据。原A-FMT03/05和相关模型须排除这一分支；它们不是任意size输入的完整契约。

小尺寸、两遍增长与NULL零尺寸模型中的逻辑str推进，不等于已证明所有指针始终位于实际分配对象及允许边界内。即便物理写入受end控制，仍须独立保留buf+size、反复递增、比较及str-buf的目标语言/编译实现前提。原KN91-U01扩展到一般截断时的逻辑指针，不只限NULL；给定数学输出不认证实际C轨迹定义良好。

first为unsigned int不能直接证明真实计数已溢出。以明确int32/uint32且首遍正常返回非负int为前提，first至多INT_MAX，first+1不会发生uint32回绕。若另假设first=INT_MAX并且大分配成功，则第二遍size=INT_MAX+1会遇到上述拒绝门；这只是有条件接续，不证明这种长度真实可达。原M13的-1到UINT_MAX到0仅保留为额外假设，KN91-F02按needs_evidence消费，不认定已发生的源码故障。

两遍字符串变化还须保证每遍合法可读、参数类型相符和变化安排本身成立；不能把未同步真实数据竞争当作普通定义良好的序列。长度相等也不能证明字节相同；即使将来恢复first!=second检查，也只比较长度。当前没有采用或实施修复。

## 4. R91-S02：最终字符串不代表完整访问轨迹

strscpy在遇到容量耗尽之前，先读取并写入每个当前字节：

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
			c = src[res];
			dest[res] = c;
			if (!c)
				return res;
			res++;
```

容量内未读到NUL后，再覆盖最后一字节：

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		if (res)
			dest[res-1] = '\0';
```

因此C-READ03、M16/M17的“拷贝前4/31字节”只能作为最终保留内容：count5、源ABCDEFGH时，源E也被读取且暂写dest[4]，最后才覆盖为NUL；count32同理。不能据最终4字节反推源只需可读4字节。count0及过大count的早退不保证目的区有新NUL；正常补零和截断模型不能越过这些门。当前名称例为ASCII字节，不推广为任意Unicode字符数量。

task.comm容量的实际字段为：

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		char			comm[TASK_COMM_LEN];
```

KN91-F03/F04描述状态不传播和短名容量差异，但尚无调用方必须拒绝截断或必须返回完整全名的需求证据；保留为接口性质/条件候选，不自动计为设计错误或真实漏洞。32字节容量属于所读源码，运行布局和完整字符串转换仍不是本轮认证对象。

C-REL02的失败清理需补明done非NULL时才通知等待者由其清理create。另一分支为：

[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
		kfree(create->full_name);
		if (!done) {
			kfree(create);
			return;
		}
```

原M20已经明确done有效；不能将矩阵省略的条件推广成所有负PID结果都由等待者释放请求。旧R53/R68对象、通知、首入口和退出限定继续有效。

## 5. R91-S03：索引、短锚与图的消费方式

facts.fact_to_evidence_ids.short_comm_capacity列E006/E068，其中E068是set_kthread_struct调用而非comm数组。有效映射为E006常量与E070字段；E068仍用于私有对象初始化，不能仅因编号存在就认为支持容量。

E046只展示申请和if前缀；E063只展示result写入及相邻注释；E065只展示失败分支前缀。不得分别仅凭这些短片段证明完整ERR_PTR返回、complete发生和done非空通知。本C06已取得其完整固定函数；通知调用另以本回执补足：

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	complete(done);
```

这条只证明局部调用语句，不认证wait已返回或业务函数已运行。序列、分支和其他字段仍按完整定义与R53/R68消费，不把本回执补证写成作者原短锚已经覆盖全部事实。

facts/deferred中用斜线合写的CO53-F01/F02/F03、KT68-F01/F02和R89/R90编号是人读缩写，不是可直接查找的单一qualified_id。后继实际引用须分别展开完整原ID，不新编号或错误合并根因。

DOT中idbuf、create等聚合节点的蓝边表示其所在调用方执行动作，不是数据对象自行调用函数；notify包含不同上下文，不能当一个原子事件。宏/所有权/返回信息边只按标注消费，成功、失败及done门不可因图省略而消失。Graphviz、完整CFG和真实并发未认证。

## 6. 方法、保留范围及后继

本轮新读定义和同pin原文复用范围见检查点。强制主体独立回源，不以执行者摘录代替源码。自写纯Python独立核6个ASCII长度、4个需求/容量数学例、3个双遍长度例、4个字符串最终像例，并核49行求和与uint32模运算；17例和2项算术检查不执行仓库实现，也不复现C内存访问、ABI、调度或22个作者模型的实际运行。

作者4候选/9缺口在上述限定下保留；普通风险进入记录，无Owner当前修复决策。R90-01/02另件关闭，89、88、86及53既有闭合不重开。原91六件、81/81/0及正式评论保持冻结。

下一包92选择格式解析状态、整数数字生成、宽度/精度与输出组合三个关联增量，补KN91-U02而不重做名称所有权或整个printf。92无D00，继续C06c/12次一档；交回C06用Pro同轮验收及安排后继。角色独立复核仍是GPT会话对GPT会话产出审查，不构成异族、人工逐字或运行认证。
