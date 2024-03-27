#include <linux/kernel/sched.h>
#include <linux/debug/bug.h>

#include <obsolete/printk.h>




/*
 * Send signals to all our closest relatives so that they know
 * to properly mourn us..
 */
static void exit_notify(task_s *tsk, int group_dead)
{
	while (tsk->children.count != 0)
	{
		List_s * child_lp = list_header_pop(&tsk->children);
		while (child_lp == 0);

		task_s *task_initd = myos_find_task_by_pid(1);
		list_header_enqueue(&task_initd->children, child_lp);
	}


	// bool autoreap;
	// task_s *p, *n;
	// LIST_HEAD(dead);

	// write_lock_irq(&tasklist_lock);
	// forget_original_parent(tsk, &dead);

	// if (group_dead)
	// 	kill_orphaned_pgrp(tsk->group_leader, NULL);

	// tsk->exit_state = EXIT_ZOMBIE;
	// if (unlikely(tsk->ptrace)) {
	// 	int sig = thread_group_leader(tsk) &&
	// 			thread_group_empty(tsk) &&
	// 			!ptrace_reparented(tsk) ?
	// 		tsk->exit_signal : SIGCHLD;
	// 	autoreap = do_notify_parent(tsk, sig);
	// } else if (thread_group_leader(tsk)) {
	// 	autoreap = thread_group_empty(tsk) &&
	// 		do_notify_parent(tsk, tsk->exit_signal);
	// } else {
	// 	autoreap = true;
	// }

	// if (autoreap) {
	// 	tsk->exit_state = EXIT_DEAD;
	// 	list_add_to_next(&tsk->ptrace_entry, &dead);
	// }

	// /* mt-exec, de_thread() is waiting for group leader */
	// if (unlikely(tsk->signal->notify_count < 0))
	// 	wake_up_process(tsk->signal->group_exec_task);
	// write_unlock_irq(&tasklist_lock);

	// list_for_each_entry_safe(p, n, &dead, ptrace_entry) {
	// 	list_del_init(&p->ptrace_entry);
	// 	release_task(p);
	// }
}


void __noreturn do_exit(long code)
{
	task_s *tsk = current;
	int group_dead;

	// WARN_ON(irqs_disabled());

	// synchronize_group_exit(tsk, code);

	// WARN_ON(tsk->plug);

	// kcov_task_exit(tsk);
	// kmsan_task_exit(tsk);

	// coredump_task_exit(tsk);
	// ptrace_event(PTRACE_EVENT_EXIT, code);
	// user_events_exit(tsk);

	// validate_creds_for_do_exit(tsk);

	// io_uring_files_cancel();
	// exit_signals(tsk);  /* sets PF_EXITING */

	// /* sync mm's RSS info before statistics gathering */
	// if (tsk->mm)
	// 	sync_mm_rss(tsk->mm);
	// acct_update_integrals(tsk);
	// group_dead = atomic_dec_and_test(&tsk->signal->live);
	// if (group_dead) {
	// 	/*
	// 	 * If the last thread of global init has exited, panic
	// 	 * immediately to get a useable coredump.
	// 	 */
	// 	if (unlikely(is_global_init(tsk)))
	// 		panic("Attempted to kill init! exitcode=0x%08x\n",
	// 			tsk->signal->group_exit_code ?: (int)code);

// #ifdef CONFIG_POSIX_TIMERS
// 		hrtimer_cancel(&tsk->signal->real_timer);
// 		exit_itimers(tsk);
// #endif
	// 	if (tsk->mm)
	// 		setmax_mm_hiwater_rss(&tsk->signal->maxrss, tsk->mm);
	// }
	// acct_collect(code, group_dead);
	// if (group_dead)
	// 	tty_audit_exit();
	// audit_free(tsk);

	tsk->exit_code = code;
	// taskstats_exit(tsk, group_dead);

	// exit_mm();

	// if (group_dead)
	// 	acct_process();
	// trace_sched_process_exit(tsk);

	// exit_sem(tsk);
	// exit_shm(tsk);
	exit_files(tsk);
	exit_fs(tsk);
	// if (group_dead)
	// 	disassociate_ctty(1);
	// exit_task_namespaces(tsk);
	// exit_task_work(tsk);
	exit_thread(tsk);

	// /*
	//  * Flush inherited counters to the parent - before the parent
	//  * gets woken up by child-exit notifications.
	//  *
	//  * because of cgroup mode, must be called before cgroup_exit()
	//  */
	// perf_event_exit_task(tsk);

	// sched_autogroup_exit_task(tsk);
	// cgroup_exit(tsk);

	// /*
	//  * FIXME: do that only when needed, using sched_exit tracepoint
	//  */
	// flush_ptrace_hw_breakpoint(tsk);

	// exit_tasks_rcu_start();
	exit_notify(tsk, group_dead);
	// proc_exit_connector(tsk);
	// mpol_put_task_policy(tsk);
// #ifdef CONFIG_FUTEX
// 	if (unlikely(current->pi_state_cache))
// 		kfree(current->pi_state_cache);
// #endif
	// /*
	//  * Make sure we are holding no locks:
	//  */
	// debug_check_no_locks_held();

	// if (tsk->io_context)
	// 	exit_io_context(tsk);

	// if (tsk->splice_pipe)
	// 	free_pipe_info(tsk->splice_pipe);

	// if (tsk->task_frag.page)
	// 	put_page(tsk->task_frag.page);

	// validate_creds_for_do_exit(tsk);
	// exit_task_stack_account(tsk);

	// check_stack_usage();
	preempt_disable();
	// if (tsk->nr_dirtied)
	// 	__this_cpu_add(dirty_throttle_leaks, tsk->nr_dirtied);
	// exit_rcu();
	// exit_tasks_rcu_finish();

	// lockdep_free_task(tsk);
	do_task_dead();
}


