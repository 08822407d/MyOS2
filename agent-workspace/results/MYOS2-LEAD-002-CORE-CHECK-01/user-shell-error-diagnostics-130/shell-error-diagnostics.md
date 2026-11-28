---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SHELL-ERROR-DIAGNOSTICS-130
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
produced_by: GPT-6 (self-reported; backend/effort UNATTESTED)
model_per_owner: GPT-6＋Extra
date: 2026-11-23
base_snapshot: time
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---
# 第130批 · MyShell文件错误诊断及D00补正

**作者交付而非主线验收。** 所有结论来自固定time的源码与给定条件；不报告真实命令运行、链接、Graphviz或内核测试结果。

## D00 · 两项历史差量

R127-02以128固定facts/DOT三个C127簇为底，每簇精确6条旧边选择器替换和6条新增边：外层MEMCPY owner→prefix/page/word/tail是memcpy.c:40/45/51/57四次宏位置；owner→helpercall是memcopy.h:125的一次实际C直接helper调用；word→helpercall仅代表条件选择。prefix→page、page→word、callerupdate→tail、tail→result仅为SEQUENCE，非重复宏或返回数据。新增original_dstpp节点，只有original_dstpp→result是memcpy.c:59的原始入参RETURN_DATA；helper返回void。三簇逐一复核4个宏位/1条直接helper/1条原参返回。B127_02/08/16的iteration只记循环体写D0、D0..D6、D0..D14，循环外do0各一次写D1/D7/D15。B16首轮footer len8为真回do8，次轮len0为假落do0。先前43对a0/a1寄存来源（20/23）、case预取在goto之前、单字直接do0保持不动。选择器旧端点/旧类型/旧标签、新边、新B节点标签与复建投影都在facts/DOT；不覆盖历史冻结文件。

R129-01更正固定129共享27个parser步骤中iter25的误共享实参0，按完整case键重绑0/7/12345/999999，其余26步不动。原EMIT_byte_log是**按目的地址排序的最终字节表**而非时间顺序。四例先写输出偏移0..33的字面+19位日期+句点；再FL_ZERO按5/5/1/0写前导零；数字按逆序偏移39、39、39→35、39→34用直接赋值；LF写40，缓冲NUL写41而不送write。四例分别得到000000、000007、012345、999999，输出均41字节、vsnprintf rv41、printf请求write长度41。129错误簇main调用和忽略时间测试返回的位置实际为myshell.c:68，旧tests.c:110–113标注已更正。两项状态CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW，既有R127及R129的RETURN须待C09 Pro核收；R128-01继续CLOSED_DOCUMENTATION_ONLY。

## A · 五命令指定失败守卫

| 命令 | 给定守卫 | 真正打印前缀 | 第一个%s | 第二个%s | 命令返回 |
|---|---|---|---|---|---:|
| cd | chdir return -1 / i!=0 | cd:␠ | strerror(errno) | path=/lost | 1 |
| ls | opendir returns NULL | ls:␠ | strerror(errno) | path=. | -1 |
| cat | open returns -1; fd<0 | ls:␠ | strerror(errno) | filename=/note | -1 |
| rmdir | rmdir returns -1; err!=0 | rmdir:␠ | strerror(errno) | dirname=/tmp/d | -1 |
| rm | unlink returns -1; err!=0 | rm:␠ | strerror(errno) | filename=/log | -1 |

cat在current_dir为/、叶名note时形成/note（ASCII：2f 6e 6f 74 65），失败时源代码**意外打印ls:而非cat:**，本轮忠于源码不改写。run_cmd以void方式调用函数指针并丢弃命令返回。底层返回-1或NULL不能用作errno值；printf两实参表达式的C求值顺序不作断言。

## B · 条件错误表不等同实际strerror实现

有效活动宏ENOENT=0x02、EACCES=0x0d、ENOTEMPTY=0x42，对应下标2、13、66。条件表项分别是No such file or directory（25）、Permission denied（17）、Directory not empty（19）个ASCII；下标0是Success（7）。__get_errlist遇负入参取相反数，然后直接访问_sys_errlist_internal，因此仅在0、±2、±13、±66这七个指定有效槽上可推导同样文本；源中没有活动下标边界检查。机器件给出每个输入、索引、ASCII及十六进制。

shell真正调用的是strerror(errno)；独立存在__get_errlist不能证明strerror调用该辅助函数。myinitramfs顶层CMake中配置musl-gcc及musl头目录，myshell声明sh可执行目标，myglibc另声明mylibc静态库并收集stdio源，**但已读证据没有证明sh最终实际链接这张错误表**。因此下面给定错误文本是合同条件，不是实机实际报错；真实locale、链接与strerror实现UNKNOWN。

## C · 两个%s和逐字节write请求

每个%s从va_list按序取const char*、使用strlen，然后只复制strlen个非NUL字节；两个源字符串的终止NUL都不写到输出中。按前缀、错误文本、冒号空格、路径、LF组成完整行，另外在缓冲偏移N处终止NUL，不包含在write参数长度中。printf.c用4096字节缓冲调用vsnprintf，再请求一次write(stdout->_fileno,buf,rv)，忽略write返回并返回rv。

| 指定案例 | 实际条件性行（尾LF用转义显示） | write字节数 | NUL偏移 | printf返回 |
|---|---|---:|---:|---:|
| cd | cd: No such file or directory: /lost\n | 37 | 37 | 37 |
| ls | ls: Permission denied: .\n | 25 | 25 | 25 |
| cat | ls: No such file or directory: /note\n | 37 | 37 | 37 |
| rmdir | rmdir: Directory not empty: /tmp/d\n | 35 | 35 | 35 |
| rm | rm: Permission denied: /log\n | 28 | 28 | 28 |

按长度相加与逐字节hex独立核为37/25/37/35/28；五例的详细分段和完整ASCII hex见facts。若ls调用的write给定返回25或较短的2，printf返回都为格式化长度25，ls_command仍返回-1，run_cmd不消费该值。实际stdout fd、实际写入与显示均UNKNOWN。原始错误信息的最终运行来源是最早待证桥接，不得把本纸面模型说成操作系统实际结果。

## 交回

完整本地P9、编译/链接、Graphviz、QEMU、实机及真实文件操作一律NOT_RUN。作者READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，R127-02/R129-01保留CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW；交C09 Pro独立验收，不改变历史PR状态。
