---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-COMPLETION-WAIT-49
track_id: MYOS2-LEAD-002
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: "GPT-6（自报；实际后台未独立认证）"
model_per_owner: GPT-6
effort_per_owner: Extra
date: 2026-11-06
base_snapshot: "time（分支名）@a039d9803ade"
record_type: source_quotation_ledger
read_channel: GitHub_connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# evidence · 固定 time 逐字引文

本页所有锚点均在本对话重新读取 `time@a039d9803ade` 的真实文件后取连续 1–5 行；一个标签指向一个真实定义/对象/宏，示例只支持标题所限定的局部事实。默认分支搜索结果没有作为固定源码证据。

## E01 · completion typedef
[VERIFIED mykernel/kactive/kactive_type_declaration.h::completion_s]
```c
	struct completion;
	typedef struct completion completion_s;
```
起始行（辅助定位）：14。

## E02 · done 与 wait 成员
[VERIFIED mykernel/kactive/completion/completion_types.h::struct completion]
```c
	struct completion {
		uint			done;
		swqueue_hdr_s	wait;
	};
```
起始行（辅助定位）：19。

## E03 · 静态初始化为零和 swait 头
[VERIFIED mykernel/kactive/completion/completion_macro.h::COMPLETION_INITIALIZER]
```c
	#define COMPLETION_INITIALIZER(work)	\
			{ 0, __SWAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
```
起始行（辅助定位）：4。

## E04 · 静态声明宏
[VERIFIED mykernel/kactive/completion/completion_macro.h::DECLARE_COMPLETION]
```c
	#define DECLARE_COMPLETION(work)	\
				completion_s work = COMPLETION_INITIALIZER(work)
```
起始行（辅助定位）：15。

## E05 · ONSTACK 源码别名
[VERIFIED mykernel/kactive/completion/completion_macro.h::DECLARE_COMPLETION_ONSTACK]
```c
	#define DECLARE_COMPLETION_ONSTACK(work)	DECLARE_COMPLETION(work)
```
起始行（辅助定位）：18。

## E06 · 静态 swait 锁/队列头
[VERIFIED mykernel/kactive/swait/swait_macro.h::__SWAIT_QUEUE_HEAD_INITIALIZER]
```c
    #define __SWAIT_QUEUE_HEAD_INITIALIZER(name) {							\
				.lock			= __ARCH_SPIN_LOCK_UNLOCKED,				\
				.task_list_hdr	= LIST_HEADER_INIT((name).task_list_hdr),	\
			}
```
起始行（辅助定位）：13。

## E07 · 动态初始化 done 和 wait
[VERIFIED mykernel/kactive/completion/completion.h::init_completion]
```c
		init_completion(completion_s *x) {
			x->done = 0;
			init_swait_queue_head(&x->wait);
		}
```
起始行（辅助定位）：37。

## E08 · wait 头实际初始化
[VERIFIED mykernel/kactive/swait/swait.c::__init_swait_queue_head]
```c
void __init_swait_queue_head(swqueue_hdr_s *q)
{
	spin_lock_init(&q->lock);
	INIT_LIST_HEADER_S(&q->task_list_hdr);
}
```
起始行（辅助定位）：11。

## E09 · complete 加锁
[VERIFIED mykernel/kactive/completion/completion.c::complete]
```c
void complete(completion_s *x)
{
	ulong flags;
	spin_lock_irqsave(&x->wait.lock, flags);
```
起始行（辅助定位）：32。

## E10 · 计数饱和/唤醒请求/解锁
[VERIFIED mykernel/kactive/completion/completion.c::complete]
```c
	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, 0);
	spin_unlock_irqrestore(&x->wait.lock, flags);
```
起始行（辅助定位）：36。

## E11 · 公开 void 入口及固定参数
[VERIFIED mykernel/kactive/completion/completion.c::wait_for_completion]
```c
void wait_for_completion(completion_s *x) {
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
```
起始行（辅助定位）：53。

## E12 · wait wrapper 锁和 long 返回
[VERIFIED mykernel/kactive/completion/completion.h::wait_for_common]
```c
					spin_lock_irq(&x->wait.lock);
				} while (!x->done && timeout);
				__finish_swait(&x->wait, &wait);
				if (!x->done)
					return timeout;
```
起始行（辅助定位）：59。

## E13 · done 初始门
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
		do_wait_for_common(completion_s *x,
				long (*action)(long), long timeout, int state);

		extern long
		wait_for_common(completion_s*x, long timeout, int state);
```
起始行（辅助定位）：16。

## E14 · generic 信号门
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
					if (signal_pending_state(state, current)) {
						timeout = -ERESTARTSYS;
						break;
					}
```
起始行（辅助定位）：51。

## E15 · 准备/设状态/解锁/action/重锁
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
					__prepare_to_swait(&x->wait, &wait);
					__set_current_state(state);
					spin_unlock_irq(&x->wait.lock);
					timeout = action(timeout);
					spin_lock_irq(&x->wait.lock);
```
起始行（辅助定位）：55。

## E16 · 循环与未完成返回
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
				} while (!x->done && timeout);
				__finish_swait(&x->wait, &wait);
				if (!x->done)
					return timeout;
```
起始行（辅助定位）：60。

## E17 · 计数消费和返回表达式
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
			if (x->done != UINT_MAX)
				x->done--;
			return timeout ?: 1;
```
起始行（辅助定位）：65。

## E18 · 栈上 wait 项初始化
[VERIFIED mykernel/kactive/swait/swait_macro.h::__SWAITQUEUE_INITIALIZER]
```c
	#define __SWAITQUEUE_INITIALIZER(name) {					\
				.task			= current,						\
				.task_list		= LIST_INIT((name).task_list),	\
			}
```
起始行（辅助定位）：5。

## E19 · 待项声明宏
[VERIFIED mykernel/kactive/swait/swait_macro.h::DECLARE_SWAITQUEUE]
```c
	#define DECLARE_SWAITQUEUE(name)	\
				swqueue_s name = __SWAITQUEUE_INITIALIZER(name)
```
起始行（辅助定位）：10。

## E20 · swait 头对象成员
[VERIFIED mykernel/kactive/swait/swait_types.h::struct swait_queue_head]
```c
	struct swait_queue_head {
		spinlock_t	lock;
		List_hdr_s	task_list_hdr;
	};
```
起始行（辅助定位）：7。

## E21 · swait 待项成员
[VERIFIED mykernel/kactive/swait/swait_types.h::struct swait_queue]
```c
	struct swait_queue {
		task_s		*task;
		List_s		task_list;
	};
```
起始行（辅助定位）：12。

## E22 · 任务字段刷新和队尾入队
[VERIFIED mykernel/kactive/swait/swait.c::__prepare_to_swait]
```c
void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	wait->task = current;
	if (list_is_empty_entry(&wait->task_list))
		list_header_add_to_tail(&q->task_list_hdr, &wait->task_list);
}
```
起始行（辅助定位）：46。

## E23 · 状态设为运行及条件摘队
[VERIFIED mykernel/kactive/swait/swait.c::__finish_swait]
```c
void __finish_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	__set_current_state(TASK_RUNNING);
	if (!list_is_empty_entry(&wait->task_list))
		list_header_delete_node(&q->task_list_hdr, &wait->task_list);
}
```
起始行（辅助定位）：52。

## E24 · 空队列短路
[VERIFIED mykernel/kactive/swait/swait.c::swake_up_locked]
```c
void swake_up_locked(swqueue_hdr_s *q, int wake_flags)
{
	swqueue_s *curr;
	if (list_header_is_empty(&q->task_list_hdr))
		return;
```
起始行（辅助定位）：23。

## E25 · 非空队首、尝试唤醒和摘队
[VERIFIED mykernel/kactive/swait/swait.c::swake_up_locked]
```c
	curr = list_headr_first_container(&q->task_list_hdr, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
```
起始行（辅助定位）：28。

## E26 · 可中断状态位
[VERIFIED mykernel/sched/task/task_const.h::TASK_INTERRUPTIBLE]
```c
	#define TASK_INTERRUPTIBLE		0x00000001
```
起始行（辅助定位）：27。

## E27 · 不可中断状态位
[VERIFIED mykernel/sched/task/task_const.h::TASK_UNINTERRUPTIBLE]
```c
	#define TASK_UNINTERRUPTIBLE	0x00000002
```
起始行（辅助定位）：28。

## E28 · 致命信号醒来掩码
[VERIFIED mykernel/sched/task/task_const.h::TASK_WAKEKILL]
```c
	#define TASK_WAKEKILL			0x00000100
```
起始行（辅助定位）：38。

## E29 · 公开最大 timeout 宏
[VERIFIED mykernel/sched/task/task_const.h::MAX_SCHEDULE_TIMEOUT]
```c
	#define MAX_SCHEDULE_TIMEOUT	LONG_MAX
```
起始行（辅助定位）：86。

## E30 · 入口状态位拒绝逻辑
[VERIFIED mykernel/lock_IPC/signal/signal.h::signal_pending_state]
```c
		signal_pending_state(uint state, task_s *p) {
			if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
				return 0;
			if (!signal_pending(p))
				return 0;
```
起始行（辅助定位）：500。

## E31 · generic helper 信号返回条件
[VERIFIED mykernel/lock_IPC/signal/signal.h::signal_pending_state]
```c
			return (state & TASK_INTERRUPTIBLE) || __fatal_signal_pending(p);
```
起始行（辅助定位）：506。

## E32 · 饱和值定义
[VERIFIED mykernel/include/linux/kernel/limits.h::UINT_MAX]
```c
	#define UINT_MAX		(~0U)
```
起始行（辅助定位）：32。

## E33 · 初始化宏路由
[VERIFIED mykernel/kactive/swait/swait_macro.h::init_swait_queue_head]
```c
	#define init_swait_queue_head(q)			\
			do {								\
				__init_swait_queue_head((q));	\
			} while (0)
```
起始行（辅助定位）：21。

## E34 · 第48批静态握手对象
[VERIFIED mykernel/drivers/base/devtmpfs.c::setup_done]
```c
static __initdata DECLARE_COMPLETION(setup_done);
```
起始行（辅助定位）：344。

## E35 · 创建方等待调用点
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
		wait_for_completion(&setup_done);
```
起始行（辅助定位）：429。

## E36 · worker 通知调用点
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
	complete(&setup_done);
```
起始行（辅助定位）：399。

## 证据限界
编译条件、宏定义与实际链接路径未做 C 预处理/构建认证；`try_to_wake_up`、`schedule_timeout`、锁/中断的内部及全局对象生存期均不在本次核证范围。重复取自同一函数的不同位置不是不同根因。

