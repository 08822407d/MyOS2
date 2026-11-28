# 第160批：有界字符串预算、对象范围与复制顺序

CORE-USER-LIBC-BOUNDED-STRING-OBJECT-GATES-160；C11c／EXEC-27。固定time@a039d9803ade，224@6f860774cf10。三场景十一行仅为给定地址语义的静态模型。停止是本模型第一个对象范围门失败之前，不声称通用C或真实运行的唯一首个问题。

## D00｜R159-01 证据分类的追加补正

固定159提交ff7c39dea3dd，主线159正式裁定RETURN_DOCUMENTATION，技术十行一致。85原窗中E055–059、E061–062、E064、E067、E068–083共25转普通（5 extern、4类型声明、16用户CMake）。余60正式，含2个P4内核配置窗；原23普通加25得48。保留原ID、原代码窗、源path/symbol/行尾；不是降为INFERRED。旧MANIFEST 85/85/0为历史；组合阅读60/60/0，六件字面分布0/0/0/0/0/60。事实件D00_correction含全部85映射、精确旧行/字段替换、13个source_index调整及引用消费核验；实际临时文本重建，85原代码窗逐字不变，旧技术rows/source_graph不改。原159六件和评论不改，本批41新窗另账。旧E062/E057只支持声明定位，printf请求与返回由旧E037–041及结构成员E060支撑；用户CMake不证实际链接。补正状态CORRECTION_SUBMITTED_PENDING_C12_REVIEW，不代签159整体PASS。

## A｜端点形成与前导读取

BS160-E002先形成end_ptr，再BS160-E003判断零预算。BS160-E004只有形成后的端点比较才可能进入，不能补救更早的对象门。BS160-E005以地址模8为前导条件，没有maxlen读取预算守卫；BS160-E006在遇NUL时才判断char_ptr>end并返回偏移。四行S基址mod8=1。

| case | S物理容量/maxlen | 已发生的源读、端点与前导 | 结果 |
|---|---|---|---|
| A160_01 | 1/0 | end=S+0，零预算，无读 | return0 |
| A160_02 | 1/1 | end=S+1，读S0=41；合法形成one-past S+1，mod8=2 | 下一L49读之前STOP，无返回 |
| A160_03 | 2/1 | end=S+1；读S0=41、S1=00，后者预算外但对象内；char_ptr=end，>为假 | return1 |
| A160_04 | 1/2 | 候选S+2超one-past S+1；end尚未形成，无读 | L35形成前STOP，无返回 |

A02的1次源读不等于return1。A04不越过端点门进入后续判断。

## B｜起点比较不限制完整整字窗口

BS160-E010用longword_ptr与end_ptr的给定地址次序比较；BS160-E011完整读取并后增、随后使用实际B掩码(word-lomagic)&himagic，无strlen的取反项。BS160-E012至BS160-E019分别确认候选字节；BS160-E020在未找到零的整字路径末将char_ptr设为end_ptr；word_ptr独立；BS160-E021最后夹取后返回。

| case | S容量/maxlen | 已完成整字/确认 | 结论 |
|---|---|---|---|
| B160_01 | 7/1 | 起点0<1，候选[0,8)超过对象，整字未读/未后增 | L116操作前STOP |
| B160_02 | 8/1 | 0047464544434241，mask 8000000000000000；确认0–7，最后零 | char_ptr7夹为1，return1 |
| B160_03 | 9/9 | 首词4141414141414141，mask0；char_ptr=end9、word_ptr8 | 下一起点8<9，但[8,16)不满足对象，第二次L116前STOP |

B02整字读8字节、另逐字节确认8次，返回1。B03首词读8字节、无确认；不是返回9。未达整字不提供word、mask或后增值。

## C｜先补零后复制，目标首停与部分写入

BS160-E023/BS160-E024/BS160-E025：STRNCPY(D,S,5)先调用__strnlen，整字7e7e7e7e00636261、实际B-mask 0000000080000000，确认0–3，size3。size!=n，先形成D+3调用memset(D+3,0,2)，才有MEMCPY(D,S,3)。BS160-E026长度2不进>=8；BS160-E027每轮先存字节，再增加整数dstp，再减len；BS160-E028返回原dstpp，不是末游标。BS160-E031/BS160-E039长度3不进>=16，BS160-E032/BS160-E036/BS160-E037/BS160-E038实际宏次序是读__x、src_bp+=1、__nbytes-=1、写__x、dst_bp+=1；BS160-E033返回原dstpp。byte typedef为BS160-L18普通定位，不计正式。

| case | D物理容量 | 终点、真实写序 | 停止前快照或正常最终hex |
|---|---:|---|---|
| C160_01 | 2 | D+3形成前STOP；未调用memset | 7e 7e |
| C160_02 | 3 | D+3 one-past合法形成、进入memset；第一次L78写前STOP | 7e 7e 7e |
| C160_03 | 4 | 写D3=0、整数游标4、len1；第二次L78写前STOP，无MEMCPY | 7e 7e 7e 00 |
| C160_04 | 5 | 补零3、4，memset返回D+3；复制0、1、2，MEMCPY和STRNCPY返回D+0 | 61 62 63 00 00 |

C前三行均无正常外层返回；不把停止前快照当正常末态。C04最后写者D0–2为MEMCPY字节宏，D3–4为memset。图以STRNCPY为三个真实调用拥有者；相邻seq边全部LOCAL_CONTROL，事件种类独立，返回来源另列RETURN_DATA；STOP没有正常出边。

## 来源与限界

BS160-L01/BS160-L04/BS160-L02是声明/包含定位；BS160-E001/BS160-E022/BS160-E029为真实条件宏，实际选中与链接未认证。BS160-L09/BS160-L10/BS160-L13/BS160-L17只是用户构建定位；内核P4配置不认证用户ABI。旧124/126充分对象模型不重做；仅对124的SC124-READ-BEYOND-BUDGET追加本有限模型证据，实际ABI、链接、STRNLEN符号及helper身份等保持未知。实际R159-01待主线；R152/R155/R158闭合及旧RETURN保留。完整本地P9、项目执行、预处理、编译、Graphviz、QEMU及真实I/O NOT_RUN。
