---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SCHEDULE-TIMEOUT-50
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: "GPT-6（Owner所选，后台不可独立证明）"
model_per_owner: GPT-6
effort_per_owner: Extra
date: 2026-11-06
base_snapshot: "time（分支名），短号a039d9803ade"
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: schedule_timeout_static_report_and_R49_external_erratum
---
# 50批：外置R49-01文档勘误与schedule_timeout独立源码分析
## T00 第49批R49-01勘误（49仍RETURN）
原E12标为wait_for_common，但实际旧引文属于do_wait_for_common 59–63，旧标签的wrapper证据身份撤销；E13的旧摘录位于DEBUG extern声明区，只作CONTEXT；E01的struct completion前向声明与typedef分开。旧49六件、旧评论和36/36自报原样保留，本次只核新增C49-A01..A07七段，不追认其他34条。
| 原消费 | 错配 | 替代证据 | 具体消费与限制 |
|---|---|---|---|
| E01 | struct completion前向声明与typedef混记 | C49-A01 | W01; facts CW49-T01；前向声明仅CONTEXT |
| E12 | 旧片段实际位于do_wait_for_common 59–63，而非wait_for_common | C49-A02/C49-A03/C49-A05 | W02 wrapper; facts T05/T07; wait_matrix entry/public-return; DOT wrapper/lock/wunlock；旧E12不得证明wrapper锁、实参或返回 |
| E13 | 旧片段位于DEBUG extern区，不是do_wait定义 | C49-A04/C49-A05/C49-A06 | W02快慢路径; facts T06/T07; wait_matrix gate/slow/exit; DOT gate/fast/make；旧E13降为CONTEXT |
| W02与return | long wrapper和void公开接口分层 | C49-A02/C49-A03/C49-A06/C49-A07 | W02；facts T05/T07; wait_matrix public-return; DOT rvoid；不能当worker errno |
| 49 DOT节点 | 原图正确结构不弥补错误引文 | C49-A02..A07 | wrapper/lock/gate/fast/make/wunlock/rvoid；只提供外置证据修正，不重画49原图 |
| 49 MANIFEST 36/36 | 原标签数量自报成立但P2定义归属至少E12/E13失败 | C49新增7条 | 原49 MANIFEST及评论6681100222;本批独立计数；未重核其余34项不追认 |
纠正后的wrapper真实long类型/持锁实参/解锁/return见C49-A02/A03；do_wait实际初始!done与DECLARE_SWAITQUEUE见C49-A04；重锁/finish/计数消费见C49-A05/A06；public void及MAX参数见C49-A07。**仅申请R49-01_DOCUMENTATION_CLOSURE_REQUESTED，不自签CLOSED/PASS。**

## T01 schedule_timeout活动分支
固定time的真实入口是long schedule_timeout(long timeout)，局部proc_timer_s timer和ulong expire（ST50-A03）。源码注释并非执行证据；完整活动体759–806有以下分流：
| 输入 | 活动动作（先行callee正常返回前提） | return | 本函数直接schedule() |
|---|---|---|
| MAX_SCHEDULE_TIMEOUT=LONG_MAX | 实际schedule()，goto out；不进入局部timer路径 | LONG_MAX原值 | 一次 |
| 负数 | __set_current_state(TASK_RUNNING)，goto out | 0 | 无 |
| 0 | expire、timer.task、setup、mod、del、destroy | 0 | 无 |
| 1/一般有限正值 | 与0相同timer路径 | 原正值不变 | 无 |
| LONG_MAX-1 | 同有限路径；expire求和可有无符号回绕 | 原LONG_MAX-1 | 无 |
ST50-A04/A05是MAX活动schedule，ST50-A06/A07是负值恢复state，ST50-A08..A10是有限timer请求；其中`// schedule();`仅为注释，ST50-A11实际`return timeout < 0 ? 0 : timeout`，函数并无timeout按当前jiffies更新或剩余时间计算。MAX路径本体没有直接把任务状态改为TASK_RUNNING，不能把源码注释的完成保证套用所有分支。任何callee不正常返回则停在最早边界。
`MAX_SCHEDULE_TIMEOUT`来自宏LONG_MAX；`ulong`为unsigned long（ST50-A25/A26/A29），`jiffies`只有extern volatile ulong声明（evidence的CONTEXT）。若此声明绑定成立，C usual arithmetic conversions把signed long timeout转unsigned long相加，expire是其模2^W结果。W=64数例只是模型，不认证编译ABI、时钟快照、timer真正到期或系统效果；return仍取输入timeout。

## T02 timer对象、回调及清理
`proc_timer_s`是局部timer_list_s timer和task_s* task；有限分支先timer.task=current，再timer_setup_on_stack→simple_init_timer_key（初始化entry和function），再__mod_timer(&timer.timer,expire,MOD_TIMER_NOTPENDING)，没有接收int结果。随后del_timer_sync别名timer_delete_sync（返回int亦不接收），最后destroy_timer_on_stack在这个快照的inline函数体为空（ST50-A01/A09/A10/A12/A13/A14/A17/A18/A30/A31）。
`process_timeout`可能在定时器系统**另行派发回调**时经from_timer(container_of)找到包含timer的proc_timer_s并调用wake_up_process(task)；此处不证明定时器曾到期、回调发生、成功唤醒、任务真的调度或栈对象生命周期安全。__mod_timer/删除同步/真实timer wheel内部保持OPAQUE。

## T03 completion与msleep局部消费
纠正后的wait_for_common持锁将schedule_timeout函数指针传给do_wait（C49-A03），公开wait_for_completion只传MAX和TASK_UNINTERRUPTIBLE且返回void（C49-A07）。因此公开done0慢路若schedule()正常返回，真实action返回MAX非零；重锁时done仍0则`!done && timeout`仍真，不能从该action得到零超时退出。done曾增加但可能被其他消费者取走，只有指定waiter重锁实际见done>0才能消费。generic helper假设action返回0/负的49模型不能误当公开MAX的真实生产者；不证明实际死锁或调度必然返回。
`schedule_timeout_uninterruptible`先设置TASK_UNINTERRUPTIBLE，再返回schedule_timeout值；`schedule_timeout_interruptible`设置另一状态。`msleep(uint msecs)`以msecs_to_jiffies取得ulong timeout，while(timeout)每轮赋回schedule_timeout_uninterruptible的long返回（ST50-A19..A24）。转换路径含常量条件及运行时分支；本批不认证毫秒数值与真实时间。
转换结果若0，while不执行；若为可表示long的正有限值、且所有先行调用正常返回，则该值始终不被递减，源码级每个完成的循环迭代保持timeout>0；等于LONG_MAX则活动schedule可被调用，正常返回仍是MAX。更大无符号值转signed long的实现细节另保留unknown，不从这些条件推导实机永久忙等、所有用户程序受影响或真实sleep耗时。

## 有界有限模型与历史去重
本批仅用纯JS表达式构建22个有限条件案例：7个输入分支、5个W64算术例、10个completion/msleep/callback条件。模型和源码文字核对不构成构建、运行或实机验证。
同根因历史记录 `MYOS2-LEAD-002-CORE-CHECK-01/core/results.yaml::CA-03` 的V09已有有限timeout与msleep的有界宿主模型（非内核测试），本批独立固定源码核对后沿用其身份，不额外复制为ST50-F缺陷。`MYOS2-DR-002/completeness.yaml::time.timer.risks`仅历史S1不作权威裁定。
## 未证与STOP
完整调度器、timer wheel、try_to_wake_up、真实中断/计时器到期、jiffies绑定/ABI、对象生存期、内存序和端到端completion/msleep运行均未审。无内核修复/补丁，无预处理/编译/仓库脚本/QEMU/硬件/完整本地P9。49仍RETURN待C04闭合，50仅申请独立验收。
