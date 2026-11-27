# 第157批｜固定源证据与限定排重

固定time a039d9803ade；各正式窗1–5连续物理行，定义或实际宏/命令实例内原文。位置窗仅用于注释、包含、guard等，不计P6。NONE窗末尾为Markdown围栏分隔换行，不冒称源有LF。引文不等于预处理、编译或运行。三组实际固定源码逐个重开核：15/18/12路径；120/72/13正式、18/17/6普通位置，全数匹配；P6=205/205/0；行尾账 {"P_LF":89,"E_LF":494,"E_NONE":1}。

## 历史限定查询

在 lead@1937b1b16805 完整2067项树限定路径名检索 `signal|sigmask|pending|queue`（不区分大小写、仅合同/reviews/checkpoints）。实际集合：26项：
- 125-user-workqueue-work-lifecycle-contract.md
- 126-user-workqueue-pool-bootstrap-contract.md
- 185-user-runqueue-clock-update-contract.md
- 188-user-new-task-runqueue-entry-contract.md
- 23-user-return-signal-contract.md
- 27-user-signal-send-wake-contract.md
- checkpoints/2026-10-28-c03-stdio08-reviewed-return-signal09-ready.md
- checkpoints/2026-10-30-chat-executor-prepared-signal09-reviewed.md
- checkpoints/2026-10-31-signal-send-wake10-ready.md
- checkpoints/2026-10-31-signal10-reviewed-pid11-ready.md
- checkpoints/2026-11-05-files43-reviewed-forkfiles44-publication-pending.md
- checkpoints/2026-11-15-percpu87-reviewed-workqueue88-ready.md
- checkpoints/2026-11-15-workqueue88-reviewed-pool89-ready.md
- checkpoints/2026-11-15-workqueue89-reviewed-id90-ready.md
- checkpoints/2026-11-24-c10-recovery-ready-140-pending-review.md
- reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-D00-135-review.md
- reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md
- reviews/CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01-review.md
- reviews/CORE-USER-RETURN-SIGNAL-09-review.md
- reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-D00-132-review.md
- reviews/CORE-USER-RUNQUEUE-CLOCK-UPDATE-133-review.md
- reviews/CORE-USER-SIGNAL-SEND-WAKE-10-review.md
- reviews/CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89-CORRECTION-CLOSURE-review.md
- reviews/CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89-review.md
- reviews/CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88-DOC-CLOSURE-review.md
- reviews/CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88-review.md

定点消费原件（均冻结，不重验旧源码配额）：
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/user-return-signal.md / 5885d136414a / D02-a L245–269; AD2-16/19/20/21/22; L271–276 RS-01：旧大小门、旧快照、新旧flags过滤、不可屏蔽位过滤、动作写表、共享清理、mask合成、copyout顺序和from/to不对称已有；本批只加具体队列/别名/提交后错误见证。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/facts-and-boundaries.yaml / 30fbb6511c76 / ipc.signal.action_mask_syscalls L417–451：保留现有接口及未知；不重复立根因。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/deferred-findings.yaml / 969e25ac017d / RS-01 L30–32, RS-11 L621–623, RS-12 L704–706, RS-13 L744–746, RS-14 L785–787：SA_RESTORER未预处理、from方向缺门、sighand/私有pending/同步出队旧限；本批flags0、合法对象与GIVEN不裁定真实生产者。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-send-wake-10/signal-send-wake.md / 66f0a4a931b7 / S02 L197,220; L290–294：共享pending/collect_signal/__sigqueue_free和位节点区别既有；这里只从设置动作后的flush入口消费，非kill、出队、唤醒。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-uaccess-copy-63/uaccess-copy.md / b6abe15d0dcd / A/G01 L30–34, B/G02 L38–42：选择x86地址门，拒绝保留n，数值真假不等映射；本批不重做汇编或原语。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-uaccess-copy-63/deferred-findings.yaml / 36d1b2c24868 / UC63-F01–F06；尤其 UC63-F04 L132：拒绝门及残量根因既有，不重复编号。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-uaccess-recovery-64/uaccess-recovery.md / a2dbc30c8727 / A/B/C L44–78及L80–84：异常表、缺页和alternative未证真实恢复；本批高位数值拒绝不进入raw。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/MANIFEST.md / 1a13243ea029 / followup L4, inputs L22, coverage L45–46：work提交、pool链、scheduled/回调；不是信号动作表。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-pool-bootstrap-89/MANIFEST.md / a36374b4c0a1 / followup L4, inputs L23, L41：静态池/worker/manager，不等于信号队列。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-pool-bootstrap-89/workqueue-pool-bootstrap.md / 7d1d70cfe86c / G01/A L36, G02/B L48, G03/C L60：定点区分三专题。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/MANIFEST.md / f89f871e6ce1 / followup L4, inputs L31, coverage L85, L101–103：rq时钟和picker。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-runqueue-clock-update-133/runqueue-clock-update.md / 58a078f96e91 / A/B/C L43/57/71：时钟生产与连续采样。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md / 11c8d72208bd / L4, L140, L150, L214 signal.h：源包含signal.h但工作是调度入队。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry.md / 786abe3e28ac / D00 R135-02 L38–40; A/B/C L50/54/65：K.signal.pids[PIDTYPE_TGID]写槽，sched_fork/CPU0队首/picker非动作表/flush。
- def3b82d9191 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rw-position-consumption-155/rw-position-consumption.md / 20ba154636e0 / A/B/C：旧文件位置课题，不换名复用数值模型。
- 2d134f987c21 / agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kernel-write-consumption-156/kernel-write-consumption.md / 8fa368cb3ad7 / A/B/C；合读本次156-review §5：旧kernel_write局部模型冻结；核收三注记不倒改。

## 新源码引文

### SG157-P001 / entry_includes

[POSITION_ONLY mykernel/lock_IPC/syscall.c::include]
```
#include <linux/kernel/syscalls.h>
#include <linux/lib/errno.h>
#include <linux/kernel/uaccess.h>
```

mykernel/lock_IPC/syscall.c:1–3；blob c0969d606b84；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L1-L3

### SG157-E001 / act_entry

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigaction)]
```
MYOS_SYSCALL_DEFINE4(rt_sigaction,
		int, sig, const sigaction_s __user *, act,
		sigaction_s __user *, oact, size_t, sigsetsize)
{
	k_sigaction_s new_sa, old_sa;
```

mykernel/lock_IPC/syscall.c:22–26；blob c0969d606b84；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L22-L26

### SG157-E002 / act_size

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigaction)]
```
	if (sigsetsize != sizeof(sigset_t))
		return -EINVAL;
```

mykernel/lock_IPC/syscall.c:30–31；blob c0969d606b84；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L30-L31

### SG157-E003 / act_in

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigaction)]
```
	if (act && copy_from_user(&new_sa.sa, act, sizeof(new_sa.sa)))
		return -EFAULT;
```

mykernel/lock_IPC/syscall.c:33–34；blob c0969d606b84；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L33-L34

### SG157-E004 / act_call

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigaction)]
```
	ret = do_sigaction(sig, act ? &new_sa : NULL, oact ? &old_sa : NULL);
	if (ret)
		return ret;
```

mykernel/lock_IPC/syscall.c:36–38；blob c0969d606b84；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L36-L38

### SG157-E005 / act_out

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigaction)]
```
	if (oact && copy_to_user(oact, &old_sa.sa, sizeof(old_sa.sa)))
		return -EFAULT;

	return 0;
}
```

mykernel/lock_IPC/syscall.c:40–44；blob c0969d606b84；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L40-L44

### SG157-E006 / mask_entry

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigprocmask)]
```
MYOS_SYSCALL_DEFINE4(rt_sigprocmask, int, how, sigset_t __user *, nset, sigset_t __user *, oset, size_t, sigsetsize)
{
	sigset_t old_set, new_set;
	int error;
```

mykernel/lock_IPC/syscall.c:55–58；blob c0969d606b84；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L55-L58

### SG157-E007 / mask_size

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigprocmask)]
```
	if (sigsetsize != sizeof(sigset_t))
		return -EINVAL;
```

mykernel/lock_IPC/syscall.c:61–62；blob c0969d606b84；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L61-L62

### SG157-E008 / mask_old

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigprocmask)]
```
	old_set = current->blocked;
```

mykernel/lock_IPC/syscall.c:64–64；blob c0969d606b84；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L64-L64

### SG157-E009 / mask_in

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigprocmask)]
```
	if (nset) {
		if (copy_from_user(&new_set, nset, sizeof(sigset_t)))
			return -EFAULT;
		sigdelsetmask(&new_set, sigmask(SIGKILL)|sigmask(SIGSTOP));
```

mykernel/lock_IPC/syscall.c:66–69；blob c0969d606b84；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L66-L69

### SG157-E010 / mask_call

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigprocmask)]
```
		error = sigprocmask(how, &new_set, NULL);
		if (error)
			return error;
	}
```

mykernel/lock_IPC/syscall.c:71–74；blob c0969d606b84；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L71-L74

### SG157-E011 / mask_out

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigprocmask)]
```
	if (oset) {
		if (copy_to_user(oset, &old_set, sizeof(sigset_t)))
			return -EFAULT;
	}
```

mykernel/lock_IPC/syscall.c:76–79；blob c0969d606b84；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L76-L79

### SG157-E012 / mask_ret

[VERIFIED mykernel/lock_IPC/syscall.c::MYOS_SYSCALL_DEFINE4(rt_sigprocmask)]
```
	return 0;
}
```

mykernel/lock_IPC/syscall.c:81–82；blob c0969d606b84；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/syscall.c#L81-L82

### SG157-E013 / signal_define

[VERIFIED mykernel/lock_IPC/signal/signal.c::SIGNAL_DEFINATION]
```
#define SIGNAL_DEFINATION
```

mykernel/lock_IPC/signal/signal.c:1–1；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1-L1

### SG157-P002 / signal_inc

[POSITION_ONLY mykernel/lock_IPC/signal/signal.c::include]
```
#include "signal.h"
```

mykernel/lock_IPC/signal/signal.c:2–2；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L2-L2

### SG157-E014 / cache

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigqueue_cachep]
```
static kmem_cache_s *sigqueue_cachep;
```

mykernel/lock_IPC/signal/signal.c:12–12；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L12-L12

### SG157-E015 / free_gate

[VERIFIED mykernel/lock_IPC/signal/signal.c::__sigqueue_free]
```
static void
__sigqueue_free(sigqueue_s *q) {
	if (q->flags & SIGQUEUE_PREALLOC)
		return;
```

mykernel/lock_IPC/signal/signal.c:60–63；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L60-L63

### SG157-E016 / free_request

[VERIFIED mykernel/lock_IPC/signal/signal.c::__sigqueue_free]
```
	kmem_cache_free(sigqueue_cachep, q);
}
```

mykernel/lock_IPC/signal/signal.c:68–69；blob 252357bc7a70；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L68-L69

### SG157-P003 / free_comment

[POSITION_ONLY mykernel/lock_IPC/signal/signal.c::__sigqueue_free]
```
	// if (q->ucounts) {
	// 	dec_rlimit_put_ucounts(q->ucounts, UCOUNT_RLIMIT_SIGPENDING);
	// 	q->ucounts = NULL;
	// }
```

mykernel/lock_IPC/signal/signal.c:64–67；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L64-L67

### SG157-E017 / flush_head

[VERIFIED mykernel/lock_IPC/signal/signal.c::flush_sigqueue_mask]
```
static void
flush_sigqueue_mask(sigset_t *mask, sigpending_s *pending) {
	sigqueue_s *q, *n;
	sigset_t m;
```

mykernel/lock_IPC/signal/signal.c:387–390；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L387-L390

### SG157-E018 / flush_inter

[VERIFIED mykernel/lock_IPC/signal/signal.c::flush_sigqueue_mask]
```
	sigandsets(&m, mask, &pending->signal);
	if (sigisemptyset(&m))
		return;
```

mykernel/lock_IPC/signal/signal.c:392–394；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L392-L394

### SG157-E019 / flush_clear

[VERIFIED mykernel/lock_IPC/signal/signal.c::flush_sigqueue_mask]
```
	sigandnsets(&pending->signal, &pending->signal, mask);
```

mykernel/lock_IPC/signal/signal.c:396–396；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L396-L396

### SG157-P004 / flush_old_iter

[POSITION_ONLY mykernel/lock_IPC/signal/signal.c::flush_sigqueue_mask]
```
	// list_for_each_entry_safe(q, n, &s->list, list) {
```

mykernel/lock_IPC/signal/signal.c:397–397；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L397-L397

### SG157-E020 / flush_loop

[VERIFIED mykernel/lock_IPC/signal/signal.c::flush_sigqueue_mask]
```
	list_header_for_each_container_safe(q, n, &pending->list, list) {
		if (sigismember(mask, q->info.si_signo)) {
			list_header_delete_node(&pending->list, &q->list);
			__sigqueue_free(q);
		}
```

mykernel/lock_IPC/signal/signal.c:398–402；blob 252357bc7a70；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L398-L402

### SG157-E021 / flush_end

[VERIFIED mykernel/lock_IPC/signal/signal.c::flush_sigqueue_mask]
```
	}
}
```

mykernel/lock_IPC/signal/signal.c:403–404；blob 252357bc7a70；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L403-L404

### SG157-E022 / task_block_head

[VERIFIED mykernel/lock_IPC/signal/signal.c::__set_task_blocked]
```
static void
__set_task_blocked(task_s *tsk, const sigset_t *newset) {
	if (task_sigpending(tsk) && !thread_group_empty(tsk)) {
		sigset_t newblocked;
```

mykernel/lock_IPC/signal/signal.c:1085–1088；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1085-L1088

### SG157-E023 / task_block_unreached

[VERIFIED mykernel/lock_IPC/signal/signal.c::__set_task_blocked]
```
		/* A set of now blocked but previously unblocked signals. */
		sigandnsets(&newblocked, newset, &current->blocked);
		// retarget_shared_pending(tsk, &newblocked);
	}
```

mykernel/lock_IPC/signal/signal.c:1089–1092；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1089-L1092

### SG157-E024 / task_block_store

[VERIFIED mykernel/lock_IPC/signal/signal.c::__set_task_blocked]
```
	tsk->blocked = *newset;
	// recalc_sigpending();
}
```

mykernel/lock_IPC/signal/signal.c:1093–1095；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1093-L1095

### SG157-P005 / task_block_retarget

[POSITION_ONLY mykernel/lock_IPC/signal/signal.c::__set_task_blocked]
```
		// retarget_shared_pending(tsk, &newblocked);
```

mykernel/lock_IPC/signal/signal.c:1091–1091；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1091-L1091

### SG157-P006 / task_block_recalc

[POSITION_ONLY mykernel/lock_IPC/signal/signal.c::__set_task_blocked]
```
	// recalc_sigpending();
```

mykernel/lock_IPC/signal/signal.c:1094–1094；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1094-L1094

### SG157-E025 / curr_head

[VERIFIED mykernel/lock_IPC/signal/signal.c::__set_current_blocked]
```
void __set_current_blocked(const sigset_t *newset)
{
	task_s *tsk = current;
```

mykernel/lock_IPC/signal/signal.c:1097–1099；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1097-L1099

### SG157-E026 / curr_equal

[VERIFIED mykernel/lock_IPC/signal/signal.c::__set_current_blocked]
```
	if (sigequalsets(&tsk->blocked, newset))
		return;
```

mykernel/lock_IPC/signal/signal.c:1105–1106；blob 252357bc7a70；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1105-L1106

### SG157-E027 / curr_lock

[VERIFIED mykernel/lock_IPC/signal/signal.c::__set_current_blocked]
```
	spin_lock_irq(&tsk->sighand->siglock);
	__set_task_blocked(tsk, newset);
	spin_unlock_irq(&tsk->sighand->siglock);
}
```

mykernel/lock_IPC/signal/signal.c:1108–1111；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1108-L1111

### SG157-E028 / proc_head

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigprocmask]
```
int sigprocmask(int how, sigset_t *set, sigset_t *oldset)
{
	task_s *tsk = current;
	sigset_t newset;
```

mykernel/lock_IPC/signal/signal.c:1134–1137；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1134-L1137

### SG157-E029 / proc_old

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigprocmask]
```
	if (oldset)
		*oldset = tsk->blocked;
```

mykernel/lock_IPC/signal/signal.c:1140–1141；blob 252357bc7a70；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1140-L1141

### SG157-E030 / proc_block

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigprocmask]
```
	switch (how) {
	case SIG_BLOCK:
		sigorsets(&newset, &tsk->blocked, set);
		break;
```

mykernel/lock_IPC/signal/signal.c:1143–1146；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1143-L1146

### SG157-E031 / proc_unblock

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigprocmask]
```
	case SIG_UNBLOCK:
		sigandnsets(&newset, &tsk->blocked, set);
		break;
```

mykernel/lock_IPC/signal/signal.c:1147–1149；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1147-L1149

### SG157-E032 / proc_set

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigprocmask]
```
	case SIG_SETMASK:
		newset = *set;
		break;
```

mykernel/lock_IPC/signal/signal.c:1150–1152；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1150-L1152

### SG157-E033 / proc_default

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigprocmask]
```
	default:
		return -EINVAL;
	}
```

mykernel/lock_IPC/signal/signal.c:1153–1155；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1153-L1155

### SG157-E034 / proc_commit

[VERIFIED mykernel/lock_IPC/signal/signal.c::sigprocmask]
```
	__set_current_blocked(&newset);
	return 0;
}
```

mykernel/lock_IPC/signal/signal.c:1157–1159；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1157-L1159

### SG157-E035 / do_head

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
int do_sigaction(int sig, k_sigaction_s *act, k_sigaction_s *oact)
{
	task_s *p = current, *t;
	k_sigaction_s *k;
	sigset_t mask;
```

mykernel/lock_IPC/signal/signal.c:1164–1168；blob 252357bc7a70；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1164-L1168

### SG157-E036 / do_valid

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	if (!valid_signal(sig) || sig < 1 || (act && sig_kernel_only(sig)))
		return -EINVAL;
```

mykernel/lock_IPC/signal/signal.c:1170–1171；blob 252357bc7a70；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1170-L1171

### SG157-E037 / do_k

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	k = &p->sighand->action[sig-1];

	spin_lock_irq(&p->sighand->siglock);
```

mykernel/lock_IPC/signal/signal.c:1173–1175；blob 252357bc7a70；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1173-L1175

### SG157-E038 / do_immutable

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	if (k->sa.sa_flags & SA_IMMUTABLE) {
		spin_unlock_irq(&p->sighand->siglock);
		return -EINVAL;
	}
```

mykernel/lock_IPC/signal/signal.c:1176–1179；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1176-L1179

### SG157-E039 / do_old

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	if (oact)
		*oact = *k;
```

mykernel/lock_IPC/signal/signal.c:1180–1181；blob 252357bc7a70；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1180-L1181

### SG157-E040 / do_build

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	BUILD_BUG_ON(UAPI_SA_FLAGS & SA_UNSUPPORTED);
```

mykernel/lock_IPC/signal/signal.c:1187–1187；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1187-L1187

### SG157-E041 / do_flags

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	if (act)
		act->sa.sa_flags &= UAPI_SA_FLAGS;
	if (oact)
		oact->sa.sa_flags &= UAPI_SA_FLAGS;
```

mykernel/lock_IPC/signal/signal.c:1194–1197；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1194-L1197

### SG157-P007 / do_compat

[POSITION_ONLY mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	// sigaction_compat_abi(act, oact);
```

mykernel/lock_IPC/signal/signal.c:1199–1199；blob 252357bc7a70；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1199-L1199

### SG157-E042 / do_write

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	if (act) {
		sigdelsetmask(&act->sa.sa_mask,
				sigmask(SIGKILL) | sigmask(SIGSTOP));
		*k = *act;
```

mykernel/lock_IPC/signal/signal.c:1201–1204；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1201-L1204

### SG157-E043 / do_ignore

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
		if (sig_handler_ignored(sig_handler(p, sig), sig)) {
			sigemptyset(&mask);
			sigaddset(&mask, sig);
			flush_sigqueue_mask(&mask, &p->signal->shared_pending);
```

mykernel/lock_IPC/signal/signal.c:1216–1219；blob 252357bc7a70；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1216-L1219

### SG157-P008 / do_private

[POSITION_ONLY mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
			// for_each_thread(p, t)
			// 	flush_sigqueue_mask(&mask, &t->pending);
```

mykernel/lock_IPC/signal/signal.c:1220–1221；blob 252357bc7a70；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1220-L1221

### SG157-E044 / do_ret

[VERIFIED mykernel/lock_IPC/signal/signal.c::do_sigaction]
```
	}

	spin_unlock_irq(&p->sighand->siglock);
	return 0;
}
```

mykernel/lock_IPC/signal/signal.c:1223–1227；blob 252357bc7a70；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.c#L1223-L1227

### SG157-P009 / sig_inc_local

[POSITION_ONLY mykernel/lock_IPC/signal/signal.h::include]
```
	#include <linux/compiler/myos_debug_option.h>

	#include "../lock_ipc_const.h"
	#include "../lock_ipc_types.h"
	#include "../lock_ipc_api.h"
```

mykernel/lock_IPC/signal/signal.h:4–8；blob 09a5f6ad3c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L4-L8

### SG157-P010 / sig_inc_uapi

[POSITION_ONLY mykernel/lock_IPC/signal/signal.h::include]
```
	#include <uapi/linux/signal.h>
```

mykernel/lock_IPC/signal/signal.h:10–10；blob 09a5f6ad3c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L10-L10

### SG157-P011 / sig_macro_inc

[POSITION_ONLY mykernel/lock_IPC/signal/signal.h::include / guard]
```
	#include "signal_macro.h"
	
	#if defined(SIGNAL_DEFINATION) || !(DEBUG)
```

mykernel/lock_IPC/signal/signal.h:130–132；blob 09a5f6ad3c48；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L130-L132

### SG157-E045 / handler

[VERIFIED mykernel/lock_IPC/signal/signal.h::sig_handler]
```
		PREFIX_STATIC_INLINE
		void __user
		*sig_handler(task_s *t, int sig) {
			return t->sighand->action[sig - 1].sa.sa_handler;
		}
```

mykernel/lock_IPC/signal/signal.h:134–138；blob 09a5f6ad3c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L134-L138

### SG157-E046 / ignored

[VERIFIED mykernel/lock_IPC/signal/signal.h::sig_handler_ignored]
```
		sig_handler_ignored(void __user *handler, int sig) {
			/* Is it explicitly or implicitly ignored? */
			return handler == SIG_IGN ||
				(handler == SIG_DFL && sig_kernel_ignore(sig));
		}
```

mykernel/lock_IPC/signal/signal.h:142–146；blob 09a5f6ad3c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L142-L146

### SG157-E047 / addset

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigaddset]
```
		sigaddset(sigset_t *set, int _sig) {
			ulong sig = _sig - 1;
			if (_NSIG_WORDS == 1)
				set->sig[0] |= 1UL << sig;
```

mykernel/lock_IPC/signal/signal.h:247–250；blob 09a5f6ad3c48；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L247-L250

### SG157-E048 / member

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigismember]
```
		sigismember(sigset_t *set, int _sig) {
			ulong sig = _sig - 1;
			if (_NSIG_WORDS == 1)
				return 1 & (set->sig[0] >> sig);
```

mykernel/lock_IPC/signal/signal.h:267–270；blob 09a5f6ad3c48；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L267-L270

### SG157-E049 / empty_head

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigisemptyset]
```
		sigisemptyset(sigset_t *set) {
			switch (_NSIG_WORDS) {
```

mykernel/lock_IPC/signal/signal.h:277–278；blob 09a5f6ad3c48；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L277-L278

### SG157-E050 / empty_case

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigisemptyset]
```
				case 1:
					return set->sig[0] == 0;
```

mykernel/lock_IPC/signal/signal.h:284–285；blob 09a5f6ad3c48；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L284-L285

### SG157-E051 / equal_head

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigequalsets]
```
		sigequalsets(const sigset_t *set1, const sigset_t *set2) {
			switch (_NSIG_WORDS) {
```

mykernel/lock_IPC/signal/signal.h:294–295；blob 09a5f6ad3c48；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L294-L295

### SG157-E052 / equal_case

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigequalsets]
```
				case 1:
					return	set1->sig[0] == set2->sig[0];
			}
			return 0;
		}
```

mykernel/lock_IPC/signal/signal.h:304–308；blob 09a5f6ad3c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L304-L308

### SG157-E053 / emptyset_head

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigemptyset]
```
		sigemptyset(sigset_t *set) {
			switch (_NSIG_WORDS) {
```

mykernel/lock_IPC/signal/signal.h:313–314；blob 09a5f6ad3c48；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L313-L314

### SG157-E054 / emptyset_case

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigemptyset]
```
			case 2: set->sig[1] = 0;
				fallthrough;
			case 1:	set->sig[0] = 0;
				break;
			}
```

mykernel/lock_IPC/signal/signal.h:318–322；blob 09a5f6ad3c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L318-L322

### SG157-E055 / delmask

[VERIFIED mykernel/lock_IPC/signal/signal.h::sigdelsetmask]
```
		sigdelsetmask(sigset_t *set, ulong mask) {
			set->sig[0] &= ~mask;
		}
```

mykernel/lock_IPC/signal/signal.h:348–350；blob 09a5f6ad3c48；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L348-L350

### SG157-E056 / valid

[VERIFIED mykernel/lock_IPC/signal/signal.h::valid_signal]
```
		valid_signal(ulong sig) {
			return sig <= _NSIG ? 1 : 0;
		}
```

mykernel/lock_IPC/signal/signal.h:397–399；blob 09a5f6ad3c48；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L397-L399

### SG157-E057 / taskpending

[VERIFIED mykernel/lock_IPC/signal/signal.h::task_sigpending]
```
		task_sigpending(task_s *p) {
			return unlikely(test_tsk_thread_flag(p,TIF_SIGPENDING));
		}
```

mykernel/lock_IPC/signal/signal.h:469–471；blob 09a5f6ad3c48；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L469-L471

### SG157-E058 / or

[VERIFIED mykernel/lock_IPC/signal/signal.h::_sig_or]
```
	#define _sig_or(x,y)	((x) | (y))
```

mykernel/lock_IPC/signal/signal.h:574–574；blob 09a5f6ad3c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L574-L574

### SG157-E059 / or_instance

[VERIFIED mykernel/lock_IPC/signal/signal.h::_SIG_SET_BINOP(sigorsets, _sig_or)]
```
			_SIG_SET_BINOP(sigorsets, _sig_or)
```

mykernel/lock_IPC/signal/signal.h:575–575；blob 09a5f6ad3c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L575-L575

### SG157-E060 / and

[VERIFIED mykernel/lock_IPC/signal/signal.h::_sig_and]
```
	#define _sig_and(x,y)	((x) & (y))
```

mykernel/lock_IPC/signal/signal.h:577–577；blob 09a5f6ad3c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L577-L577

### SG157-E061 / and_instance

[VERIFIED mykernel/lock_IPC/signal/signal.h::_SIG_SET_BINOP(sigandsets, _sig_and)]
```
			_SIG_SET_BINOP(sigandsets, _sig_and)
```

mykernel/lock_IPC/signal/signal.h:578–578；blob 09a5f6ad3c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L578-L578

### SG157-E062 / andn

[VERIFIED mykernel/lock_IPC/signal/signal.h::_sig_andn]
```
	#define _sig_andn(x,y)	((x) & ~(y))
```

mykernel/lock_IPC/signal/signal.h:580–580；blob 09a5f6ad3c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L580-L580

### SG157-E063 / andn_instance

[VERIFIED mykernel/lock_IPC/signal/signal.h::_SIG_SET_BINOP(sigandnsets, _sig_andn)]
```
			_SIG_SET_BINOP(sigandnsets, _sig_andn)
```

mykernel/lock_IPC/signal/signal.h:581–581；blob 09a5f6ad3c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal.h#L581-L581

### SG157-E064 / sigmask

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::sigmask]
```
	#define sigmask(sig)		(1UL << ((sig) - 1))
```

mykernel/lock_IPC/signal/signal_macro.h:19–19；blob 2465e96c2db6；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L19-L19

### SG157-P012 / rt_guard

[POSITION_ONLY mykernel/lock_IPC/signal/signal_macro.h::SIGRTMIN > BITS_PER_LONG]
```
	#if SIGRTMIN > BITS_PER_LONG
	#  define rt_sigmask(sig)	(1ULL << ((sig)-1))
	#else
	#  define rt_sigmask(sig)	sigmask(sig)
	#endif
```

mykernel/lock_IPC/signal/signal_macro.h:101–105；blob 2465e96c2db6；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L101-L105

### SG157-E065 / rt_sigmask

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::rt_sigmask]
```
	#  define rt_sigmask(sig)	sigmask(sig)
```

mykernel/lock_IPC/signal/signal_macro.h:104–104；blob 2465e96c2db6；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L104-L104

### SG157-E066 / siginmask

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::siginmask]
```
	#define siginmask(sig, mask) \
			((sig) > 0 && (sig) < SIGRTMIN && (rt_sigmask(sig) & (mask)))
```

mykernel/lock_IPC/signal/signal_macro.h:107–108；blob 2465e96c2db6；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L107-L108

### SG157-E067 / onlymask

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::SIG_KERNEL_ONLY_MASK]
```
	#define SIG_KERNEL_ONLY_MASK (\
			rt_sigmask(SIGKILL)   |  rt_sigmask(SIGSTOP))
```

mykernel/lock_IPC/signal/signal_macro.h:110–111；blob 2465e96c2db6；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L110-L111

### SG157-E068 / ignoremask

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::SIG_KERNEL_IGNORE_MASK]
```
	#define SIG_KERNEL_IGNORE_MASK (\
	        rt_sigmask(SIGCONT)   |  rt_sigmask(SIGCHLD)   | \
			rt_sigmask(SIGWINCH)  |  rt_sigmask(SIGURG)    )
```

mykernel/lock_IPC/signal/signal_macro.h:125–127；blob 2465e96c2db6；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L125-L127

### SG157-E069 / only

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::sig_kernel_only]
```
	#define sig_kernel_only(sig)		siginmask(sig, SIG_KERNEL_ONLY_MASK)
```

mykernel/lock_IPC/signal/signal_macro.h:136–136；blob 2465e96c2db6；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L136-L136

### SG157-E070 / ignore

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::sig_kernel_ignore]
```
	#define sig_kernel_ignore(sig)		siginmask(sig, SIG_KERNEL_IGNORE_MASK)
```

mykernel/lock_IPC/signal/signal_macro.h:138–138；blob 2465e96c2db6；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L138-L138

### SG157-E071 / binop_head

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::_SIG_SET_BINOP]
```
	#define _SIG_SET_BINOP(name, op)						\
			static inline void								\
			name(sigset_t *r, const sigset_t *a,			\
					const sigset_t *b) {					\
```

mykernel/lock_IPC/signal/signal_macro.h:147–150；blob 2465e96c2db6；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L147-L150

### SG157-E072 / binop_switch

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::_SIG_SET_BINOP]
```
				ulong a0, a1, a2, a3, b0, b1, b2, b3;		\
				switch (_NSIG_WORDS) {						\
```

mykernel/lock_IPC/signal/signal_macro.h:152–153；blob 2465e96c2db6；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L152-L153

### SG157-E073 / binop_one

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::_SIG_SET_BINOP]
```
					case 1:									\
						a0 = a->sig[0]; b0 = b->sig[0];		\
						r->sig[0] = op(a0, b0);				\
						break;								\
```

mykernel/lock_IPC/signal/signal_macro.h:164–167；blob 2465e96c2db6；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L164-L167

### SG157-E074 / binop_end

[VERIFIED mykernel/lock_IPC/signal/signal_macro.h::_SIG_SET_BINOP]
```
					default:								\
						BUILD_BUG();						\
				}											\
			}
```

mykernel/lock_IPC/signal/signal_macro.h:168–171；blob 2465e96c2db6；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_macro.h#L168-L171

### SG157-E075 / prealloc

[VERIFIED mykernel/lock_IPC/signal/signal_const.h::SIGQUEUE_PREALLOC]
```
	#define SIGQUEUE_PREALLOC		1
```

mykernel/lock_IPC/signal/signal_const.h:84–84；blob ca576d47d65d；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_const.h#L84-L84

### SG157-E076 / immutable

[VERIFIED mykernel/lock_IPC/signal/signal_const.h::SA_IMMUTABLE]
```
	#define SA_IMMUTABLE			0x00800000
```

mykernel/lock_IPC/signal/signal_const.h:87–87；blob ca576d47d65d；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_const.h#L87-L87

### SG157-P013 / arch_flags_guard

[POSITION_ONLY mykernel/lock_IPC/signal/signal_const.h::__ARCH_UAPI_SA_FLAGS]
```
	#ifndef __ARCH_UAPI_SA_FLAGS
	#  ifdef SA_RESTORER
	#    define __ARCH_UAPI_SA_FLAGS	SA_RESTORER
	#  else
	#    define __ARCH_UAPI_SA_FLAGS	0
```

mykernel/lock_IPC/signal/signal_const.h:89–93；blob ca576d47d65d；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_const.h#L89-L93

### SG157-E077 / uapi_flags1

[VERIFIED mykernel/lock_IPC/signal/signal_const.h::UAPI_SA_FLAGS]
```
	#define UAPI_SA_FLAGS	(								\
				SA_NOCLDSTOP | SA_NOCLDWAIT | SA_SIGINFO |	\
				SA_ONSTACK | SA_RESTART | SA_NODEFER |		\
				SA_RESETHAND | SA_EXPOSE_TAGBITS |			\
```

mykernel/lock_IPC/signal/signal_const.h:97–100；blob ca576d47d65d；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_const.h#L97-L100

### SG157-E078 / uapi_flags2

[VERIFIED mykernel/lock_IPC/signal/signal_const.h::UAPI_SA_FLAGS]
```
				__ARCH_UAPI_SA_FLAGS						\
			)
```

mykernel/lock_IPC/signal/signal_const.h:101–102；blob ca576d47d65d；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_const.h#L101-L102

### SG157-E079 / signo

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::kernel_siginfo]
```
	struct kernel_siginfo {
		int					si_signo;
		int					si_errno;
		int					si_code;
		union __sifields	_sifields;
```

mykernel/lock_IPC/signal/signal_types.h:25–29；blob e715cf681555；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L25-L29

### SG157-E080 / sigqueue

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::sigqueue]
```
	struct sigqueue {
		List_s				list;
		int					flags;
		kernel_siginfo_t	info;
		// struct ucounts		*ucounts;
```

mykernel/lock_IPC/signal/signal_types.h:36–40；blob e715cf681555；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L36-L40

### SG157-E081 / pending

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::sigpending]
```
	struct sigpending {
		List_hdr_s	list;
		sigset_t	signal;
	};
```

mykernel/lock_IPC/signal/signal_types.h:43–46；blob e715cf681555；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L43-L46

### SG157-P014 / sa_guard

[POSITION_ONLY mykernel/lock_IPC/signal/signal_types.h::sigaction]
```
	struct sigaction {
	#ifndef __ARCH_HAS_IRIX_SIGACTION
```

mykernel/lock_IPC/signal/signal_types.h:48–49；blob e715cf681555；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L48-L49

### SG157-E082 / sa_fields

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::sigaction]
```
		__sighandler_t	sa_handler;
		ulong			sa_flags;
```

mykernel/lock_IPC/signal/signal_types.h:50–51；blob e715cf681555；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L50-L51

### SG157-E083 / sa_other

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::sigaction]
```
		__sigrestore_t	sa_restorer;
		sigset_t		sa_mask;	/* mask last for extensibility */
	};
```

mykernel/lock_IPC/signal/signal_types.h:56–58；blob e715cf681555；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L56-L58

### SG157-E084 / ksa

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::k_sigaction]
```
	struct k_sigaction {
		sigaction_s		sa;
```

mykernel/lock_IPC/signal/signal_types.h:60–61；blob e715cf681555；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L60-L61

### SG157-P015 / ksa_guard

[POSITION_ONLY mykernel/lock_IPC/signal/signal_types.h::k_sigaction]
```
	#ifdef __ARCH_HAS_KA_RESTORER
		__sigrestore_t	ka_restorer;
	#endif
	};
```

mykernel/lock_IPC/signal/signal_types.h:62–65；blob e715cf681555；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L62-L65

### SG157-E085 / shared_pending

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::signal_struct]
```
		sigpending_s	shared_pending;
```

mykernel/lock_IPC/signal/signal_types.h:92–92；blob e715cf681555；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L92-L92

### SG157-E086 / sighand

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::sighand_struct]
```
	struct sighand_struct {
		spinlock_t		siglock;
		refcount_t		count;
		// wait_queue_head_t	signalfd_wqh;
		k_sigaction_s	action[_NSIG];
```

mykernel/lock_IPC/signal/signal_types.h:240–244；blob e715cf681555；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/signal/signal_types.h#L240-L244

### SG157-P016 / list_inc

[POSITION_ONLY mykernel/lib/list/double_list.h::include]
```
	#include <linux/compiler/myos_debug_option.h>

	#include "../lib_const.h"
	#include "../lib_types.h"
	#include "../lib_api.h"
```

mykernel/lib/list/double_list.h:5–9；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L5-L9

### SG157-E087 / init_alias

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_HEAD]
```
		#define INIT_LIST_HEAD(list) INIT_LIST_S(list)
```

mykernel/lib/list/double_list.h:125–125；blob 6e3e1aa8e724；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L125-L125

### SG157-E088 / head_alias

[VERIFIED mykernel/lib/list/double_list.h::list_is_head]
```
		#define list_is_head list_is_head_anchor
```

mykernel/lib/list/double_list.h:132–132；blob 6e3e1aa8e724；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L132-L132

### SG157-P017 / list_guard

[POSITION_ONLY mykernel/lib/list/double_list.h::include / LIST_DEFINATION]
```
	#include "double_list_macro.h"
	
	#if defined(LIST_DEFINATION) || !(DEBUG)
```

mykernel/lib/list/double_list.h:136–138；blob 6e3e1aa8e724；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L136-L138

### SG157-E089 / list_self

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_S]
```
		INIT_LIST_S(List_s *list) {
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
		}
```

mykernel/lib/list/double_list.h:149–152；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L149-L152

### SG157-E090 / del_valid_head

[VERIFIED mykernel/lib/list/double_list.h::__list_del_entry_valid]
```
		__list_del_entry_valid(List_s *entry) {
			List_s	*prev = entry->prev,
					*next = entry->next;
```

mykernel/lib/list/double_list.h:171–173；blob 6e3e1aa8e724；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L171-L173

### SG157-E091 / del_valid

[VERIFIED mykernel/lib/list/double_list.h::__list_del_entry_valid]
```
			while ((next == NULL) || (prev == NULL) ||
					(next == LIST_POISON1) || (prev == LIST_POISON2) ||
					(prev->next != entry) || (next->prev != entry));

			return true;
```

mykernel/lib/list/double_list.h:174–178；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L174-L178

### SG157-E092 / del_neighbors

[VERIFIED mykernel/lib/list/double_list.h::__list_del]
```
		__list_del(List_s * prev, List_s * next) {
			next->prev = prev;
			WRITE_ONCE(prev->next, next);
		}
```

mykernel/lib/list/double_list.h:235–238；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L235-L238

### SG157-E093 / del_entry

[VERIFIED mykernel/lib/list/double_list.h::__list_del_entry]
```
		__list_del_entry(List_s *entry) {
			if (!__list_del_entry_valid(entry))
				return;

			__list_del(entry->prev, entry->next);
```

mykernel/lib/list/double_list.h:255–259；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L255-L259

### SG157-E094 / del_init

[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
		}
```

mykernel/lib/list/double_list.h:280–283；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L280-L283

### SG157-E095 / head_equal

[VERIFIED mykernel/lib/list/double_list.h::list_is_head_anchor]
```
		list_is_head_anchor(const List_s *list, const List_s *head) {
			return list == head;
		}
```

mykernel/lib/list/double_list.h:421–423；blob 6e3e1aa8e724；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L421-L423

### SG157-E096 / contains

[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
```

mykernel/lib/list/double_list.h:635–639；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L635-L639

### SG157-E097 / contains_ret

[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```
			}
			return false;
		}
```

mykernel/lib/list/double_list.h:640–642；blob 6e3e1aa8e724；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L640-L642

### SG157-E098 / delete

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```
		*list_header_delete_node(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
```

mykernel/lib/list/double_list.h:683–687；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L683-L687

### SG157-E099 / delete_miss

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```
			}

			return NULL;
		}
```

mykernel/lib/list/double_list.h:688–691；blob 6e3e1aa8e724；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.h#L688-L691

### SG157-E100 / container

[VERIFIED mykernel/lib/list/double_list_macro.h::list_container]
```
	#define list_container(ptr, type, member) \
				container_of(ptr, type, member)
```

mykernel/lib/list/double_list_macro.h:25–26；blob 985a8262e578；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L25-L26

### SG157-E101 / entry_alias

[VERIFIED mykernel/lib/list/double_list_macro.h::list_entry]
```
	#define list_entry list_container
```

mykernel/lib/list/double_list_macro.h:27–27；blob 985a8262e578；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L27-L27

### SG157-E102 / first_entry

[VERIFIED mykernel/lib/list/double_list_macro.h::list_first_entry]
```
	#define list_first_entry(ptr, type, member) \
				list_entry((ptr)->next, type, member)
```

mykernel/lib/list/double_list_macro.h:37–38；blob 985a8262e578；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L37-L38

### SG157-E103 / next_entry

[VERIFIED mykernel/lib/list/double_list_macro.h::list_next_container]
```
	#define list_next_container(pos, member) \
				list_container((pos)->member.next, typeof(*(pos)), member)
```

mykernel/lib/list/double_list_macro.h:70–71；blob 985a8262e578；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L70-L71

### SG157-E104 / next_alias

[VERIFIED mykernel/lib/list/double_list_macro.h::list_next_entry]
```
	#define list_next_entry list_next_container
```

mykernel/lib/list/double_list_macro.h:72–72；blob 985a8262e578；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L72-L72

### SG157-E105 / for_each

[VERIFIED mykernel/lib/list/double_list_macro.h::list_for_each]
```
	#define list_for_each(pos, head)			\
				for (pos = (head)->next;		\
					!list_is_head(pos, (head));	\
					pos = pos->next)
```

mykernel/lib/list/double_list_macro.h:88–91；blob 985a8262e578；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L88-L91

### SG157-E106 / head_test

[VERIFIED mykernel/lib/list/double_list_macro.h::list_entry_is_head]
```
	#define list_entry_is_head(pos, head, member)				\
				(&pos->member == (head))
```

mykernel/lib/list/double_list_macro.h:164–165；blob 985a8262e578；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L164-L165

### SG157-E107 / safe_for

[VERIFIED mykernel/lib/list/double_list_macro.h::list_for_each_entry_safe]
```
	#define list_for_each_entry_safe(pos, n, head, member)					\
				for (pos = list_first_entry(head, typeof(*pos), member),	\
					n = list_next_entry(pos, member);						\
					!list_entry_is_head(pos, head, member); 				\
					pos = n, n = list_next_entry(n, member))
```

mykernel/lib/list/double_list_macro.h:260–264；blob 985a8262e578；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L260-L264

### SG157-E108 / header_foreach

[VERIFIED mykernel/lib/list/double_list_macro.h::list_header_foreach]
```
	#define list_header_foreach(pos, lhdr)	\
				list_for_each(pos, &((lhdr)->anchor))
```

mykernel/lib/list/double_list_macro.h:340–341；blob 985a8262e578；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L340-L341

### SG157-E109 / header_safe

[VERIFIED mykernel/lib/list/double_list_macro.h::list_header_for_each_container_safe]
```
	#define list_header_for_each_container_safe(pos, n, header, member)	\
				list_for_each_entry_safe(pos, n, &((header)->anchor), member)
```

mykernel/lib/list/double_list_macro.h:349–350；blob 985a8262e578；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_macro.h#L349-L350

### SG157-E110 / list_head

[VERIFIED mykernel/lib/list/double_list_types.h::list_head]
```
	struct list_head {
		List_s		*next;
		List_s		*prev;
	};
```

mykernel/lib/list/double_list_types.h:8–11；blob 2ef93df3fe63；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_types.h#L8-L11

### SG157-E111 / list_header

[VERIFIED mykernel/lib/list/double_list_types.h::list_hdr]
```
	struct list_hdr {
		List_s		anchor;
		ulong		count;
	};
```

mykernel/lib/list/double_list_types.h:13–16；blob 2ef93df3fe63；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_types.h#L13-L16

### SG157-E112 / poison1

[VERIFIED mykernel/lib/list/double_list_const.h::LIST_POISON1]
```
	#define LIST_POISON1  ((void *) 0x100)
```

mykernel/lib/list/double_list_const.h:5–5；blob 32656c6d8cda；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_const.h#L5-L5

### SG157-E113 / poison2

[VERIFIED mykernel/lib/list/double_list_const.h::LIST_POISON2]
```
	#define LIST_POISON2  ((void *) 0x122)
```

mykernel/lib/list/double_list_const.h:6–6；blob 32656c6d8cda；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list_const.h#L6-L6

### SG157-P018 / container_guard

[POSITION_ONLY mykernel/include/uapi/linux/stddef.h::_CONTAINER_OF]
```
	#ifndef _CONTAINER_OF
	#define _CONTAINER_OF
```

mykernel/include/uapi/linux/stddef.h:7–8；blob c183d8b445e6；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/stddef.h#L7-L8

### SG157-E114 / container_of

[VERIFIED mykernel/include/uapi/linux/stddef.h::container_of]
```
	#	define container_of(member_ptr, container_type, member_name) ({				\
					typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
					(container_type *)((unsigned long)p -							\
						(unsigned long)&(((container_type *)0)->member_name));		\
				})
```

mykernel/include/uapi/linux/stddef.h:9–13；blob c183d8b445e6；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/stddef.h#L9-L13

### SG157-E115 / task_thread

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```
		thread_info_s	thread_info;
```

mykernel/sched/task/task_types.h:18–18；blob 87518a05fa49；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_types.h#L18-L18

### SG157-E116 / task_signals

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```
		signal_s		*signal;
		sighand_s __rcu	*sighand;
		sigset_t		blocked;
		sigset_t		real_blocked;
```

mykernel/sched/task/task_types.h:352–355；blob 87518a05fa49；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_types.h#L352-L355

### SG157-E117 / task_pending

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```
		sigset_t		saved_sigmask;
		sigpending_s	pending;
```

mykernel/sched/task/task_types.h:357–358；blob 87518a05fa49；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task_types.h#L357-L358

### SG157-E118 / test_tsk

[VERIFIED mykernel/sched/task/task.h::test_tsk_thread_flag]
```
		test_tsk_thread_flag(task_s *tsk, int flag) {
			// while (IS_INVAL_PTR(tsk));
			return arch_test_bit(flag, &(tsk->thread_info.flags));
		}
```

mykernel/sched/task/task.h:75–78；blob 6f085938ffa2；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/task/task.h#L75-L78

### SG157-E119 / tif

[VERIFIED mykernel/arch/x86_64/sched/context/thread_info_const_arch.h::TIF_SIGPENDING]
```
	#define TIF_SIGPENDING				2	/* signal pending */
```

mykernel/arch/x86_64/sched/context/thread_info_const_arch.h:38–38；blob eb89478d021e；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/sched/context/thread_info_const_arch.h#L38-L38

### SG157-E120 / threadflags

[VERIFIED mykernel/arch/x86_64/sched/context/thread_info_types_arch.h::thread_info]
```
	struct thread_info {
		ulong	flags;		/* low level flags */
```

mykernel/arch/x86_64/sched/context/thread_info_types_arch.h:19–20；blob a854b3cbd4f7；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/sched/context/thread_info_types_arch.h#L19-L20

### SG157-P019 / uaccess_include

[POSITION_ONLY mykernel/include/linux/kernel/uaccess.h::include]
```
	#include <asm/uaccess.h>
```

mykernel/include/linux/kernel/uaccess.h:11–11；blob ab39e43ad2f7；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L11-L11

### SG157-E121 / from_head

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```
	static inline __must_check ulong
	_inline_copy_from_user(void *to, const void __user *from, ulong n) {
		ulong res = n;
		// might_fault();
		// if (!should_fail_usercopy() && likely(access_ok(from, n))) {
```

mykernel/include/linux/kernel/uaccess.h:149–153；blob ab39e43ad2f7；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L149-L153

### SG157-P020 / from_guard_comment

[POSITION_ONLY mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```
		// might_fault();
		// if (!should_fail_usercopy() && likely(access_ok(from, n))) {
```

mykernel/include/linux/kernel/uaccess.h:152–153；blob ab39e43ad2f7；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L152-L153

### SG157-E122 / from_raw

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```
		// 	instrument_copy_from_user_before(to, from, n);
			res = raw_copy_from_user(to, from, n);
		// 	instrument_copy_from_user_after(to, from, n, res);
		// }
		if (unlikely(res))
```

mykernel/include/linux/kernel/uaccess.h:160–164；blob ab39e43ad2f7；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L160-L164

### SG157-E123 / from_ret

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```
			memset(to + (n - res), 0, res);
		return res;
	}
```

mykernel/include/linux/kernel/uaccess.h:165–167；blob ab39e43ad2f7；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L165-L167

### SG157-E124 / to_head

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```
	static inline __must_check ulong
	_inline_copy_to_user(void __user *to, const void *from, ulong n) {
		// might_fault();
		// if (should_fail_usercopy())
		// 	return n;
```

mykernel/include/linux/kernel/uaccess.h:171–175；blob ab39e43ad2f7；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L171-L175

### SG157-E125 / to_gate

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```

mykernel/include/linux/kernel/uaccess.h:176–180；blob ab39e43ad2f7；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L176-L180

### SG157-E126 / from_wrapper

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_from_user]
```
	static __always_inline ulong __must_check
	copy_from_user(void *to, const void __user *from, ulong n) {
```

mykernel/include/linux/kernel/uaccess.h:185–186；blob ab39e43ad2f7；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L185-L186

### SG157-E127 / from_inline

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_from_user]
```
		return _inline_copy_from_user(to, from, n);
```

mykernel/include/linux/kernel/uaccess.h:190–190；blob ab39e43ad2f7；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L190-L190

### SG157-E128 / to_wrapper

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```
	static __always_inline ulong __must_check
	copy_to_user(void __user *to, const void *from, ulong n) {
```

mykernel/include/linux/kernel/uaccess.h:196–197；blob ab39e43ad2f7；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L196-L197

### SG157-E129 / to_inline

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```
		return _inline_copy_to_user(to, from, n);
```

mykernel/include/linux/kernel/uaccess.h:201–201；blob ab39e43ad2f7；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/uaccess.h#L201-L201

### SG157-P021 / arch_includes

[POSITION_ONLY mykernel/arch/x86_64/include/asm/uaccess.h::include]
```
	#include <asm/uaccess_64.h>

	#include <uapi/asm-generic/access_ok.h>
```

mykernel/arch/x86_64/include/asm/uaccess.h:14–16；blob 27e6beb3efc2；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uaccess.h#L14-L16

### SG157-E130 / valid_addr

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::valid_user_address]
```
	#define valid_user_address(x) ((__force long)(x) >= 0)
```

mykernel/arch/x86_64/include/asm/uaccess_64.h:55–55；blob 90ca89193f9c；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uaccess_64.h#L55-L55

### SG157-E131 / access_small

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```
	static inline bool
    __access_ok(const void __user *ptr, ulong size) {
		if (__builtin_constant_p(size <= PAGE_SIZE) && size <= PAGE_SIZE) {
			return valid_user_address(ptr);
		} else {
```

mykernel/arch/x86_64/include/asm/uaccess_64.h:83–87；blob 90ca89193f9c；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uaccess_64.h#L83-L87

### SG157-E132 / access_general

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```
			ulong sum = size + (__force ulong)ptr;
			return valid_user_address(sum) && sum >= (__force ulong)ptr;
		}
	}
```

mykernel/arch/x86_64/include/asm/uaccess_64.h:88–91；blob 90ca89193f9c；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uaccess_64.h#L88-L91

### SG157-E133 / access_marker

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```
	#define __access_ok __access_ok
```

mykernel/arch/x86_64/include/asm/uaccess_64.h:92–92；blob 90ca89193f9c；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uaccess_64.h#L92-L92

### SG157-P022 / generic_guard

[POSITION_ONLY mykernel/include/uapi/asm-generic/access_ok.h::__access_ok guard]
```
	#ifndef __access_ok
```

mykernel/include/uapi/asm-generic/access_ok.h:19–19；blob 6045b6cae442；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/access_ok.h#L19-L19

### SG157-P023 / generic_end_guard

[POSITION_ONLY mykernel/include/uapi/asm-generic/access_ok.h::__access_ok guard]
```
	#endif
```

mykernel/include/uapi/asm-generic/access_ok.h:38–38；blob 6045b6cae442；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/access_ok.h#L38-L38

### SG157-E134 / access_wrapper

[VERIFIED mykernel/include/uapi/asm-generic/access_ok.h::access_ok]
```
	#  define access_ok(addr, size) likely(__access_ok(addr, size))
```

mykernel/include/uapi/asm-generic/access_ok.h:41–41；blob 6045b6cae442；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/access_ok.h#L41-L41

### SG157-E135 / nsig

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::_NSIG]
```
	#define _NSIG		64
```

mykernel/include/uapi/asm-generic/signal.h:7–7；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L7-L7

### SG157-E136 / nsig_bpw

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::_NSIG_BPW]
```
	#define _NSIG_BPW	__BITS_PER_LONG
```

mykernel/include/uapi/asm-generic/signal.h:8–8；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L8-L8

### SG157-E137 / nsig_words

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::_NSIG_WORDS]
```
	#define _NSIG_WORDS	(_NSIG / _NSIG_BPW)
```

mykernel/include/uapi/asm-generic/signal.h:9–9；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L9-L9

### SG157-E138 / sigint

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGINT]
```
	#define SIGINT		 2
```

mykernel/include/uapi/asm-generic/signal.h:12–12；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L12-L12

### SG157-E139 / sigkill

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGKILL]
```
	#define SIGKILL		 9
```

mykernel/include/uapi/asm-generic/signal.h:20–20；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L20-L20

### SG157-E140 / sigusr1

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGUSR1]
```
	#define SIGUSR1		10
```

mykernel/include/uapi/asm-generic/signal.h:21–21；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L21-L21

### SG157-E141 / sigusr2

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGUSR2]
```
	#define SIGUSR2		12
```

mykernel/include/uapi/asm-generic/signal.h:23–23；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L23-L23

### SG157-E142 / sigterm

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGTERM]
```
	#define SIGTERM		15
```

mykernel/include/uapi/asm-generic/signal.h:26–26；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L26-L26

### SG157-E143 / sigchld

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGCHLD]
```
	#define SIGCHLD		17
```

mykernel/include/uapi/asm-generic/signal.h:28–28；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L28-L28

### SG157-E144 / sigcont

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGCONT]
```
	#define SIGCONT		18
```

mykernel/include/uapi/asm-generic/signal.h:29–29；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L29-L29

### SG157-E145 / sigstop

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGSTOP]
```
	#define SIGSTOP		19
```

mykernel/include/uapi/asm-generic/signal.h:30–30；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L30-L30

### SG157-E146 / sigurg

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGURG]
```
	#define SIGURG		23
```

mykernel/include/uapi/asm-generic/signal.h:34–34；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L34-L34

### SG157-E147 / sigwinch

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGWINCH]
```
	#define SIGWINCH	28
```

mykernel/include/uapi/asm-generic/signal.h:39–39；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L39-L39

### SG157-E148 / sigrtmin

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::SIGRTMIN]
```
	#define SIGRTMIN	32
```

mykernel/include/uapi/asm-generic/signal.h:50–50；blob c7b7ef8cc387；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L50-L50

### SG157-E149 / sigset

[VERIFIED mykernel/include/uapi/asm-generic/signal.h::sigset_t]
```
	typedef struct {
		unsigned long sig[_NSIG_WORDS];
	} sigset_t;
```

mykernel/include/uapi/asm-generic/signal.h:63–65；blob c7b7ef8cc387；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal.h#L63-L65

### SG157-E150 / sa_unsupported

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::SA_UNSUPPORTED]
```
	#define SA_UNSUPPORTED		0x00000400
```

mykernel/include/uapi/asm-generic/signal-defs.h:45–45；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L45-L45

### SG157-E151 / block_const

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::SIG_BLOCK]
```
	#  define SIG_BLOCK			0	/* for blocking signals */
```

mykernel/include/uapi/asm-generic/signal-defs.h:72–72；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L72-L72

### SG157-E152 / unblock_const

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::SIG_UNBLOCK]
```
	#  define SIG_UNBLOCK		1	/* for unblocking signals */
```

mykernel/include/uapi/asm-generic/signal-defs.h:75–75；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L75-L75

### SG157-E153 / setmask_const

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::SIG_SETMASK]
```
	#  define SIG_SETMASK		2	/* for setting the signal mask */
```

mykernel/include/uapi/asm-generic/signal-defs.h:78–78；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L78-L78

### SG157-E154 / handler_fn

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::__signalfn_t]
```
	typedef void __signalfn_t(int);
```

mykernel/include/uapi/asm-generic/signal-defs.h:82–82；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L82-L82

### SG157-E155 / handler_ptr

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::__sighandler_t]
```
	typedef __signalfn_t __user *__sighandler_t;
```

mykernel/include/uapi/asm-generic/signal-defs.h:83–83；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L83-L83

### SG157-E156 / restore_fn

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::__restorefn_t]
```
	typedef void __restorefn_t(void);
```

mykernel/include/uapi/asm-generic/signal-defs.h:85–85；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L85-L85

### SG157-E157 / restore_ptr

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::__sigrestore_t]
```
	typedef __restorefn_t __user *__sigrestore_t;
```

mykernel/include/uapi/asm-generic/signal-defs.h:86–86；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L86-L86

### SG157-E158 / sig_dfl

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::SIG_DFL]
```
	#  define SIG_DFL	((__force __sighandler_t)0)		/* default signal handling */
```

mykernel/include/uapi/asm-generic/signal-defs.h:88–88；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L88-L88

### SG157-E159 / sig_ign

[VERIFIED mykernel/include/uapi/asm-generic/signal-defs.h::SIG_IGN]
```
	#  define SIG_IGN	((__force __sighandler_t)1)		/* ignore signal */
```

mykernel/include/uapi/asm-generic/signal-defs.h:89–89；blob 5423067f1d44；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/signal-defs.h#L89-L89

### SG157-P024 / asm_sig_include

[POSITION_ONLY mykernel/arch/x86_64/include/asm/signal.h::include]
```
#include <uapi/asm-generic/signal.h>
```

mykernel/arch/x86_64/include/asm/signal.h:5–5；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L5-L5

### SG157-P025 / asm_user_guard

[POSITION_ONLY mykernel/arch/x86_64/include/asm/signal.h::__KERNEL__]
```
	#	ifndef __KERNEL__
	/* Here we must cater to libcs that poke about in kernel headers.  */
	#		define NSIG		32
			typedef unsigned long sigset_t;
	#	endif /* __KERNEL__ */
```

mykernel/arch/x86_64/include/asm/signal.h:15–19；blob 4086c1b4ebd5；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L15-L19

### SG157-E160 / asm_sigkill

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGKILL]
```
	#define SIGKILL			 9
```

mykernel/arch/x86_64/include/asm/signal.h:32–32；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L32-L32

### SG157-E161 / asm_sigusr1

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGUSR1]
```
	#define SIGUSR1			10
```

mykernel/arch/x86_64/include/asm/signal.h:33–33；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L33-L33

### SG157-E162 / asm_sigusr2

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGUSR2]
```
	#define SIGUSR2			12
```

mykernel/arch/x86_64/include/asm/signal.h:35–35；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L35-L35

### SG157-E163 / asm_sigterm

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGTERM]
```
	#define SIGTERM			15
```

mykernel/arch/x86_64/include/asm/signal.h:38–38；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L38-L38

### SG157-E164 / asm_sigchld

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGCHLD]
```
	#define SIGCHLD			17
```

mykernel/arch/x86_64/include/asm/signal.h:40–40；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L40-L40

### SG157-E165 / asm_sigcont

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGCONT]
```
	#define SIGCONT			18
```

mykernel/arch/x86_64/include/asm/signal.h:41–41；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L41-L41

### SG157-E166 / asm_sigstop

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGSTOP]
```
	#define SIGSTOP			19
```

mykernel/arch/x86_64/include/asm/signal.h:42–42；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L42-L42

### SG157-E167 / asm_sigurg

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGURG]
```
	#define SIGURG			23
```

mykernel/arch/x86_64/include/asm/signal.h:46–46；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L46-L46

### SG157-E168 / asm_sigwinch

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGWINCH]
```
	#define SIGWINCH		28
```

mykernel/arch/x86_64/include/asm/signal.h:51–51；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L51-L51

### SG157-E169 / asm_sigrtmin

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SIGRTMIN]
```
	#define SIGRTMIN		32
```

mykernel/arch/x86_64/include/asm/signal.h:62–62；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L62-L62

### SG157-E170 / asm_restorer

[VERIFIED mykernel/arch/x86_64/include/asm/signal.h::SA_RESTORER]
```
	#define SA_RESTORER		0x04000000
```

mykernel/arch/x86_64/include/asm/signal.h:65–65；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L65-L65

### SG157-P026 / asm_defs_include

[POSITION_ONLY mykernel/arch/x86_64/include/asm/signal.h::include]
```
	#include <uapi/asm-generic/signal-defs.h>
```

mykernel/arch/x86_64/include/asm/signal.h:70–70；blob 4086c1b4ebd5；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/signal.h#L70-L70

### SG157-P027 / uapi_sig_include

[POSITION_ONLY mykernel/include/uapi/linux/signal.h::include]
```
	#include <asm/signal.h>
```

mykernel/include/uapi/linux/signal.h:5–5；blob 5bf46f731de0；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/signal.h#L5-L5

### SG157-P028 / arch_bits_guard

[POSITION_ONLY mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h::__x86_64__ / __ILP32__]
```
	#if defined(__x86_64__) && !defined(__ILP32__)
	#	define __BITS_PER_LONG	64
	#else
	#	define __BITS_PER_LONG	32
	#endif
```

mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h:7–11；blob 4870cf573ea1；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h#L7-L11

### SG157-E171 / arch_bits

[VERIFIED mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h::__BITS_PER_LONG]
```
	#	define __BITS_PER_LONG	64
```

mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h:8–8；blob 4870cf573ea1；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h#L8-L8

### SG157-P029 / arch_bits_inc

[POSITION_ONLY mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h::include]
```
	#include <asm-generic/bitsperlong.h>
```

mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h:13–13；blob 4870cf573ea1；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/uapi_bitsperlong.h#L13-L13

### SG157-P030 / kernel_bits_include

[POSITION_ONLY mykernel/include/linux/kernel/asm-generic/bitsperlong.h::include]
```
	#include <uapi/asm-generic/bitsperlong.h>
```

mykernel/include/linux/kernel/asm-generic/bitsperlong.h:5–5；blob 56f71a53ad49；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/asm-generic/bitsperlong.h#L5-L5

### SG157-E172 / kernel_bits

[VERIFIED mykernel/include/linux/kernel/asm-generic/bitsperlong.h::BITS_PER_LONG]
```
	#define BITS_PER_LONG	64
```

mykernel/include/linux/kernel/asm-generic/bitsperlong.h:7–7；blob 56f71a53ad49；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/asm-generic/bitsperlong.h#L7-L7

### SG157-P031 / bits_guard

[POSITION_ONLY mykernel/include/uapi/asm-generic/bitsperlong.h::__BITS_PER_LONG]
```
	#ifndef __BITS_PER_LONG
	#	define __BITS_PER_LONG	64
	#endif
```

mykernel/include/uapi/asm-generic/bitsperlong.h:12–14；blob 37add8b79b1a；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/bitsperlong.h#L12-L14

### SG157-E173 / bits_fallback

[VERIFIED mykernel/include/uapi/asm-generic/bitsperlong.h::__BITS_PER_LONG]
```
	#	define __BITS_PER_LONG	64
```

mykernel/include/uapi/asm-generic/bitsperlong.h:13–13；blob 37add8b79b1a；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/bitsperlong.h#L13-L13

### SG157-P032 / sigconst_include

[POSITION_ONLY mykernel/lock_IPC/lock_ipc_const.h::include]
```
	#include "signal/signal_const.h"
```

mykernel/lock_IPC/lock_ipc_const.h:9–9；blob 8854a82437bb；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/lock_ipc_const.h#L9-L9

### SG157-P033 / sigtypes_include

[POSITION_ONLY mykernel/lock_IPC/lock_ipc_types.h::include]
```
	#include "signal/signal_types.h"
```

mykernel/lock_IPC/lock_ipc_types.h:11–11；blob 3db4487cd4f0；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/lock_ipc_types.h#L11-L11

### SG157-P034 / syscall_guard

[POSITION_ONLY mykernel/include/linux/kernel/syscalls.h::CONFIG_ARCH_HAS_SYSCALL_WRAPPER]
```
	#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
```

mykernel/include/linux/kernel/syscalls.h:93–93；blob 6cc2f85fec90；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L93-L93

### SG157-P035 / syscall_include

[POSITION_ONLY mykernel/include/linux/kernel/syscalls.h::include]
```
	#	include <asm/syscall_wrapper.h>
	#endif /* CONFIG_ARCH_HAS_SYSCALL_WRAPPER */
```

mykernel/include/linux/kernel/syscalls.h:101–102；blob 6cc2f85fec90；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L101-L102

### SG157-E174 / map4

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__MAP4]
```
	#define __MAP4(m, t, a, ...)	m(t, a), __MAP3(m, __VA_ARGS__)
```

mykernel/include/linux/kernel/syscalls.h:117–117；blob 6cc2f85fec90；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L117-L117

### SG157-E175 / map

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__MAP]
```
	#define __MAP(n, ...)			__MAP##n(__VA_ARGS__)
```

mykernel/include/linux/kernel/syscalls.h:120–120；blob 6cc2f85fec90；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L120-L120

### SG157-E176 / scdecl

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__SC_DECL]
```
	#define __SC_DECL(t, a)		t a
```

mykernel/include/linux/kernel/syscalls.h:122–122；blob 6cc2f85fec90；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L122-L122

### SG157-E177 / sccast

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__SC_CAST]
```
	#define __SC_CAST(t, a)		(__force t) a
```

mykernel/include/linux/kernel/syscalls.h:128–128；blob 6cc2f85fec90；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L128-L128

### SG157-E178 / define4

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE4]
```
	#define MYOS_SYSCALL_DEFINE4(name, ...)	__MYOS_SYSCALL_DEFINEx(4, _##name, __VA_ARGS__)
```

mykernel/include/linux/kernel/syscalls.h:1387–1387；blob 6cc2f85fec90；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L1387-L1387

### SG157-E179 / archmap4

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__X86_64_MAP4]
```
	#define __X86_64_MAP4(m, t1, a1, t2, a2, t3, a3, t4, a4)					\
				m(t1, regs->di), m(t2, regs->si), m(t3, regs->dx),				\
				m(t4, regs->r10)
```

mykernel/arch/x86_64/include/asm/syscall_wrapper.h:21–23；blob 4224440c2c48；终止符 LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L21-L23

### SG157-E180 / archmap

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__ARCH_MAP]
```
	#define __ARCH_MAP(n, ...)	__X86_64_MAP##n(__VA_ARGS__)
```

mykernel/arch/x86_64/include/asm/syscall_wrapper.h:30–30；blob 4224440c2c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L30-L30

### SG157-E181 / stub1

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```
	#define __SYS_STUBx(abi, name, ...)							\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```

mykernel/arch/x86_64/include/asm/syscall_wrapper.h:104–108；blob 4224440c2c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L104-L108

### SG157-E182 / stub2

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```
				}	
```

mykernel/arch/x86_64/include/asm/syscall_wrapper.h:109–109；blob 4224440c2c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L109-L109

### SG157-E183 / defx1

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
				long __se_sys##name(const pt_regs_s *regs)				\
```

mykernel/arch/x86_64/include/asm/syscall_wrapper.h:241–245；blob 4224440c2c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241-L245

### SG157-E184 / defx2

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
				}														\
```

mykernel/arch/x86_64/include/asm/syscall_wrapper.h:246–250；blob 4224440c2c48；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L246-L250

### SG157-E185 / defx3

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))
```

mykernel/arch/x86_64/include/asm/syscall_wrapper.h:251–251；blob 4224440c2c48；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L251-L251

### SG157-E186 / sys_act

[VERIFIED mykernel/arch/x86_64/include/asm/syscalls_64.h::__SYSCALL(__NR_rt_sigaction, sys_rt_sigaction)]
```
__SYSCALL(__NR_rt_sigaction, sys_rt_sigaction)
```

mykernel/arch/x86_64/include/asm/syscalls_64.h:16–16；blob 544cfdb93d7e；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L16-L16

### SG157-E187 / sys_mask

[VERIFIED mykernel/arch/x86_64/include/asm/syscalls_64.h::__SYSCALL(__NR_rt_sigprocmask, sys_rt_sigprocmask)]
```
__SYSCALL(__NR_rt_sigprocmask, sys_rt_sigprocmask)
```

mykernel/arch/x86_64/include/asm/syscalls_64.h:17–17；blob 544cfdb93d7e；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L17-L17

### SG157-E188 / nr_act

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_rt_sigaction]
```
	#define __NR_rt_sigaction	13
```

mykernel/arch/x86_64/include/asm/unistd_64.h:19–19；blob 3b54c09e5d3a；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/unistd_64.h#L19-L19

### SG157-E189 / nr_mask

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_rt_sigprocmask]
```
	#define __NR_rt_sigprocmask	14
```

mykernel/arch/x86_64/include/asm/unistd_64.h:20–20；blob 3b54c09e5d3a；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/unistd_64.h#L20-L20

### SG157-E190 / dispatch_macro

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL (switch case instance)]
```
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

mykernel/arch/x86_64/entry/syscall_64.c:26–26；blob a21461322a9f；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L26-L26

### SG157-E191 / dispatch_body

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

mykernel/arch/x86_64/entry/syscall_64.c:27–31；blob a21461322a9f；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L27-L31

### SG157-E192 / dispatch_end

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```
	}
};
```

mykernel/arch/x86_64/entry/syscall_64.c:32–33；blob a21461322a9f；终止符 LF/NONE
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L32-L33

### SG157-E193 / flags64

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

mykernel/scripts/options_flags.cmake:39–43；blob 6019436417ad；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L39-L43

### SG157-E194 / flagsabi

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```
	-mabi=sysv \
	-mno-red-zone \
```

mykernel/scripts/options_flags.cmake:49–50；blob 6019436417ad；终止符 LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L49-L50

### SG157-E195 / flagsconfig

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

mykernel/scripts/options_flags.cmake:58–62；blob 6019436417ad；终止符 LF/LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L58-L62

### SG157-E196 / flagwrapper

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

mykernel/scripts/options_flags.cmake:63–66；blob 6019436417ad；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L63-L66

### SG157-E197 / pageshift

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SHIFT]
```
	#define PAGE_SHIFT				12
```

mykernel/arch/x86_64/mm/page/page_const_arch.h:9–9；blob b10e280f26f0；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/mm/page/page_const_arch.h#L9-L9

### SG157-E198 / pagesize

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```

mykernel/arch/x86_64/mm/page/page_const_arch.h:10–10；blob b10e280f26f0；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/mm/page/page_const_arch.h#L10-L10

### SG157-P036 / ac_guard

[POSITION_ONLY mykernel/include/uapi/linux/const.h::__ASSEMBLY__]
```
	#ifdef __ASSEMBLY__
	#	define _AC(X,Y)		X
	#	define _AT(T,X)		X
	#else
```

mykernel/include/uapi/linux/const.h:16–19；blob d8bcaf7e3151；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/const.h#L16-L19

### SG157-E199 / ac

[VERIFIED mykernel/include/uapi/linux/const.h::_AC]
```
	#	define _AC(X,Y)		(X##Y)
```

mykernel/include/uapi/linux/const.h:20–20；blob d8bcaf7e3151；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/const.h#L20-L20

### SG157-E200 / uint

[VERIFIED mykernel/include/linux/kernel/types.h::uint]
```
		typedef unsigned int		uint;
```

mykernel/include/linux/kernel/types.h:104–104；blob ef531baa8317；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/types.h#L104-L104

### SG157-E201 / ulong

[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```
		typedef unsigned long		ulong;
```

mykernel/include/linux/kernel/types.h:105–105；blob ef531baa8317；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/types.h#L105-L105

### SG157-P037 / errnoinc

[POSITION_ONLY mykernel/include/linux/lib/errno.h::include]
```
	#include <uapi/linux/errno.h>
```

mykernel/include/linux/lib/errno.h:5–5；blob e58cab61b391；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/lib/errno.h#L5-L5

### SG157-P038 / errnoinc2

[POSITION_ONLY mykernel/include/uapi/linux/errno.h::include]
```
#include <uapi/asm-generic/errno.h>
```

mykernel/include/uapi/linux/errno.h:1–1；blob 73481623d27e；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/errno.h#L1-L1

### SG157-P039 / errnoinc3

[POSITION_ONLY mykernel/include/uapi/asm-generic/errno.h::include]
```
	#include <uapi/asm-generic/errno-base.h>
```

mykernel/include/uapi/asm-generic/errno.h:5–5；blob 300dd57bc24b；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/errno.h#L5-L5

### SG157-E202 / efault

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EFAULT]
```
	#define	EFAULT		14	/* Bad address */
```

mykernel/include/uapi/asm-generic/errno-base.h:20–20；blob 8705e60aee51；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/errno-base.h#L20-L20

### SG157-E203 / einval

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EINVAL]
```
	#define	EINVAL		22	/* Invalid argument */
```

mykernel/include/uapi/asm-generic/errno-base.h:28–28；blob 8705e60aee51；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/errno-base.h#L28-L28

### SG157-E204 / test_all

[VERIFIED myinitramfs/myshell/tests.c::test_all]
```
void test_all()
{
	signal(SIGTERM, sigaction_SIGKILL_test);

```

myinitramfs/myshell/tests.c:5–8；blob 71e7681d050b；终止符 LF/LF/LF/LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/tests.c#L5-L8

### SG157-P040 / test_disabled

[POSITION_ONLY myinitramfs/myshell/myshell.c::main]
```
	// test_all();
```

myinitramfs/myshell/myshell.c:81–81；blob 5e177066dc16；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L81-L81

### SG157-E205 / canary

[VERIFIED mykernel/debug/panic.c::panic]
```
	this_cpu = smp_processor_id();
```

mykernel/debug/panic.c:79–79；blob 2e9d5b195559；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/debug/panic.c#L79-L79

### SG157-P041 / list_define

[POSITION_ONLY mykernel/lib/list/double_list.c::LIST_DEFINATION]
```
#define LIST_DEFINATION
```

mykernel/lib/list/double_list.c:1–1；blob 3b9ab682a5e3；终止符 LF
https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/double_list.c#L1-L1

## 有限模型与边界

仅自写纯整数/对象/文本辅助：从各独立输入重建有序差量，逐个比较from/to与合同终点，核链表邻接、count、alias、errno先后及图全属性。297项实际断言均过；计数仅为本次作者辅助，不并入P6，不代311合同READY或主线独审。模型终点和实际读取/未达见facts.input_rows、terminal_boundaries。没有实际执行C/ASM、宿主信号夹具、项目程序、预处理、构建、Graphviz、QEMU或实机；完整本地P9 NOT_RUN。
