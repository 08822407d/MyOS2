---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PRINTF-INTEGER-FORMAT-92-REVIEW-001
record_type: independent_bounded_static_review_with_mainline_supplements
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner正式验收安排）
date: 2026-11-16
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract130_short12: 6991d125d3d8
reviewed_base_short12: 474a2c0363f0
reviewed_head_short12: dbd0ee1684d4
reviewed_delivery_comment: 6909800484
executor: MYOS2-A-C06c / MYOS2-LEAD-002-CHAT-EXEC-12
status: REVIEW_COMPLETE_IN_BOUNDED_STATIC_SCOPE
acceptance_verdict: PASS_PENDING_LOCAL
returned_items: []
mandatory_supplements: [R92-S01, R92-S02, R92-S03, R91-S01, R91-S02, R91-S03]
original_artifacts_modified: false
original_author_self_check: 110/110/0
reviewer_readable_raw_tag_count: 110
self_check: {verified_claims: 8, quotes_reconfirmed: 8, downgraded_to_inferred: 0, scope: 本回执八段固定源码短锚；与原110项分账}
full_local_P9: NOT_RUN
original_six_file_automatic_regex_JSON_DOT_checks: NOT_RUN
original110_automatic_byte_and_definition_scan: NOT_RUN
reviewer_math: INDEPENDENT_INTEGER_BOUNDS_AND_FINITE_ARITHMETIC_NOT_REPOSITORY_EXECUTION
runtime_evidence: NOT_RUN
next_followup: CORE-USER-PRINTF-STRING-POINTER-93
next_contract: ../131-user-printf-string-pointer-contract.md
phase: 2
inputs_read: [130合同及91限定的同一C06固定原文, 92六件及正式评论全文, 本轮固定源码与规则和独立数学核对，详见同轮检查点]
owner_decisions_requested_now: []
open_questions: []
---

# 第92批按补充限定接收，无新增返工

第92批为PASS_PENDING_LOCAL，须合读R92-S01/S02/S03。主线直接纠正局部字节描述、调用帧和图示边类型，并补充整数区间论证；不要求重做整包，不另派文档纠正批。90文档闭合、91限定接收及其他历史闭合保持；没有修复内核，没有运行认证。

## 1. 固定交付与可读验收

正式评论6909800484全文已读，创建/更新时间为2026-11-16T07:04:41Z。初核direct lead=6991d125d3d8、exec=dbd0ee1684d4、master=e8e60cd19540、time=a039d9803ade；PR16/17均Open/unmerged，17为Draft且保持原head/base。PR返回的缓存base_sha不代替direct master。作者和Owner所报STOP_WRITING作为交回状态，不据此认证另一聊天后台进程。

compare 474a2c0363f0→dbd0ee1684d4：ahead1/behind0、单提交、严格六件added3798行；MANIFEST104、正文128、facts1980、DOT103、deferred331、evidence1152。旧文件零净修改删除。六件全部读取至末尾，长响应在facts的920–975及1240起重叠补齐；不是只读作者摘要。

| P9项 | 本轮实际范围与判定 |
|---|---|
| 1 | 可读全文未见40位十六进制串；原六件自动正则检查未运行，不签本地机械PASS。 |
| 2/13 | 110短锚逐组阅读；强制主体及超过30%的非强制旁证独立回源。固定vsprintf相关完整定义本轮重读，外部helper新读或同一C06同pin原文复用。未发现需退回的跨定义引文；短片段支持范围按S03限制。全110自动字节/符号边界解析未运行。 |
| 3/4 | 交付与源路径实际可读，短锚不依赖行号；全树路径/符号机械检查未运行。 |
| 5 | 可读枚举E001–E110，其余五件未见额外原始源码标签；MANIFEST有规范110/110/0且等式成立。作者检查过程不转为主线后台认证。 |
| 6 | 不签外部printf标准、实际ABI、编译器或硬件行为认证；相应前提仍单列。 |
| 7/8 | connector、逐字公约、time/master金丝雀按本有效C06同pin原文核对；time配置本轮重新全文读取，未声称其他三份canary也本轮新下载。 |
| 9/10/12 | 无代码件、补丁或聊天降级，相应N/A。 |
| 11 | 仓库程序、脚本、编译、Graphviz、QEMU、实机与真实并发均NOT_RUN。 |

8组55行矩阵、22条件模型、4候选和8个本批缺口记录不是运行测试数，也不是12个互不重复的故障根因。facts的38节点/46边按可读图核对，未运行DOT解析或Graphviz。原110/110/0与所有原件和旧评论冻结。

## 2. 主干结论

A的星号两次恢复与外层取参、不同资格的实际va_arg类型、动态设置与字面解析分离有固定源码支持。B的数字对、两次1e8分段、余数与商的双重效果、临时物理写宽和逻辑长度分离成立。C的前缀/补齐次序、包装返回值、拒绝分支无NUL保证及num_to_str独立接口边界成立。限定的最终字符示例不等于实际C轨迹或线程运行已获证明。

PF92-F01/F02/F03继续作有条件源码候选；F04先作为本版本组合行为，尚无外部标准或消费者需求证明其违反约定。名称截断和不传播返回值等旧KN91性质不因本次整数分析自动变成漏洞或获得关闭。

## 3. R92-S01：解析算术必须保留真实求值阶段

原A-PS08、A-WP05、M05和F03把skip_atoi简称为i*10+digit，不能据此确定最早溢出点。真实表达式为：

[VERIFIED mykernel/lib/printf/vsprintf.c::skip_atoi]
```c
	do {
		i = i*10 + *((*s)++) - '0';
	} while (isdigit(**s));
```

在明确int32、ASCII数码编码的模型中，先完成乘法，再加数码字符编码，再减48。前缀i=214748364、下一个字符为'7'时，乘法为2147483640仍可表示，但随后加55得到2147483695，已经超过2147483647；不能因最终数学差值恰好等于INT_MAX，就继续宣称整个表达式可表示。更早的乘法也可能成为别的输入的第一停止点。原“足够长字面有风险”保留；补充说明即使最终数字值本来可表示，也不保证各中间运算可表示。沿用PF92-F03，不另造根因，不认证这种格式在实际调用者中可达。

动态宽度先存位域再按是否改变决定clamp：

[VERIFIED mykernel/lib/printf/vsprintf.c::set_field_width]
```c
	spec->field_width = width;
	if (WARN_ONCE(spec->field_width != width, "field width %d too large", width)) {
		spec->field_width = clamp(width, -FIELD_WIDTH_MAX, FIELD_WIDTH_MAX);
	}
```

恢复点又执行：

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]
```c
		if (spec->field_width < 0) {
			spec->field_width = -spec->field_width;
			spec->flags |= LEFT;
		}
```

有符号24位范围的最低值-8388608可以在第一次赋值中保持不变，故不能假定必进入clamp；其数学正幅度8388608却超出该位域正上限8388607。必须在再次写回位域前保留目标转换前提，不能从M03的-6推广成所有负宽度均正常归一化。字面宽度也不经过set_field_width。该边界补入PF92-U01，不以未知的目标截断结果补造最终字符串。

%llu和%tu的取参表达式与非有符号实参之间仍有类型和值域条件；不把“类型名字不同”泛化成所有输入必坏，也不把相同机器宽度当全部C契约已经满足。LLONG_MIN的源级一元取负与int32最小值提升后的幅度分别消费。上述均不替代外部规范或实际编译证据。

## 4. R92-S02：字节顺序、真实调用帧与新增数学证明

B-DIG03把decpair[1]的两字节写成'01'，应为低位在前的'10'；主文和M10已经给出正确方向。r=100的临时物理写序为'0','0','1','0'，有效逻辑前三字节为'0','0','1'，倒序输出为100。M10中方括号0/0/1指字符，不是二进制数值0/0/1。

最后一次成对存储和返回推进不同：

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]
```c
	*((u16 *)buf) = decpair[r];
	buf += r < 10 ? 1 : 2;
	return buf;
```

B-DIG04/M11的9999与0x147b公式是一个受限内部余数的数学检查，不能写成put_dec_trunc8(9999)实际经过那条语句；该初始输入第一步得q=99后跳out_q。初始99999999经两次除100后才可使后段内部r为9999。模型必须保留“函数输入”和“某级局部变量”的区别。

本主线对原PF92-U03增加以下独立论证，不冒充执行者原来完成了全域证明：

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]
```c
	q = (r * (u64)0x28f5c29) >> 32;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;
```

[VERIFIED mykernel/lib/printf/vsprintf.c::put_dec_trunc8]
```c
	q = (r * 0x147b) >> 19;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;
```

令x=100q+r，0<=r<=99，B=2^k，100C=B+delta，则C*x/B=q+(delta*q+C*r)/B。只要后项分子在[0,B)内，右移结果即为floor(x/100)。

| C、k与输入域 | delta与最大q | 残差分子统一上界 | 乘法最大值 |
|---|---|---|---|
| C=42949673，k=32，0<=x<100000000 | delta=4，q<=999999 | 4256017623 < 4294967296 | 4294967257050327 < 2^64 |
| C=5243，k=19，0<=x<10000 | delta=12，q<=99 | 520245 < 524288 | 52424757 < 2^32 |

在明确u64为64位、unsigned为32位及相应无符号运算/移位前提下，这证明两条公式在本算法实际需要的域内给出正确整数商，相应余数索引为0..99。第二级大公式输入已经小于1000000，小公式输入小于10000；不需要穷举全部输入，也不采信注释作为证明。该论证只收窄PF92-U03的整数公式/表下标子项，不关闭类型、别名、对齐、指针、构建或运行缺口。

对0<=n<=2^64-1，两次1e8分段后的商至多1844；最大值的两段余数为9551615、67440737。每个低块按8位补齐，高块截短，在该数学域最多20个十进制逻辑字符，成对存储预算也最多20字节；八进制最多22、十六进制最多16。tmp的24字节预算在sizeof(num)=8前提下足够容纳这些数字存储，但不认证实际C别名、对象生命周期或编译产物。put_dec注释中的第一次商上限1.6e11不是全uint64域的真上界；最大商184467440737大于它，活动第二次分段仍须按源码分析。

## 5. R92-S03：输出足迹、证据支持和图示消费

C-RET03的“条件写1字符及NUL”只可当最终保留像，不能当完整操作序列。对值42、size2，数字循环会先写'4'和'2'，随后末尾NUL覆盖第二字节；该具体例的逻辑末尾正好等于end，不是已经越过end。更长需求的指针前提仍按R91-S01和PF92-U06保留。

[VERIFIED mykernel/lib/printf/vsprintf.c::number]
```c
	while (--i >= 0) {
		if (buf < end)
			*buf = tmp[i];
		++buf;
	}
```

C-RET01/M22引用的E042是WARN_ONCE，不是WARN_ON_ONCE；其判定必须补用实际的另一个宏：

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::WARN_ON_ONCE]
```c
	#		define WARN_ON_ONCE(condition) (condition)
```

该定义取自CONFIG_BUG的C分支；固定options_flags有对应配置。本轮只核所读include与定义，不认证全工程预处理/链接选择。size拒绝分支的return0及普通NUL处理须合读完整vsnprintf；E045只到if，E063不是拒绝分支后缀。E065数组起始、E033分支前缀、E092宽度扣减等短片段也不单独代表整个表、base赋值或precision抬升；主线已取得相应完整定义，不把索引存在当作足够支持。

DOT中fmt为数据节点，fmt→parse的蓝边按“外层vsnprintf调用解析器”消费；dec→div是do_div宏展开，不是普通函数调用；end→vsc是返回结果流，真实调用方向为vscnprintf→vsnprintf。WIDTH/PRECISION的再入跨越外层分派，不是解析函数内部一次连续执行，也非原子事件。图仍为有条件摘要而不是完整CFG，旧图不改。

PF92-U06是对CORE-USER-KTHREAD-NAME-STORAGE-91::KN91-U01的本批索引和追加证据，不另计一个独立根因。PF92-U03只按本回执收窄，不误称全部缺口已关闭。旧53文档误判的撤回不等于CO53-F01源码候选关闭；所有历史qualified_id与规范状态继续分开。

## 6. 方法、限制和后继

主线完整读取六件和固定合同，独立取得解析器、数字生成、输出完整定义及关键helper。纯Python仅核上述整数上界、最大64位分段、四组数字对字节、signed24/int32边界和矩阵行数，不执行仓库实现，不声称原22模型已实跑。原六件自动正则、JSON/DOT解析、110条自动字节扫描、全套P9及所有内核运行均NOT_RUN；可读比对与本地机械门分账。

第93批合包字符串读取/错误占位、字符与字段补齐、指针格式分派与返回值消费三个增量。pointer入口仅为后继预读，不在92越界签新结论；不补造被注释的格式实现，不展开全部%p扩展或完整printk。93无D00，继续现C06c/12次一档执行，交本C06用Pro同轮验收和安排后继。无额外模型、子代理、付费API、阶段3或跨项目维护；Owner无需逐项审查或修复。
