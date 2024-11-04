
#include <linux/kernel/mm.h>
#include <linux/kernel/nsproxy.h>

static kmem_cache_s *nsproxy_cachep;

struct nsproxy init_nsproxy = {
// 	.count			= REFCOUNT_INIT(1),
// 	.uts_ns			= &init_uts_ns,
// #if defined(CONFIG_POSIX_MQUEUE) || defined(CONFIG_SYSVIPC)
// 	.ipc_ns			= &init_ipc_ns,
// #endif
// 	.mnt_ns			= NULL,
	.pid_ns_for_children	= &init_pid_ns,
// #ifdef CONFIG_NET
// 	.net_ns			= &init_net,
// #endif
// #ifdef CONFIG_CGROUPS
// 	.cgroup_ns		= &init_cgroup_ns,
// #endif
// #ifdef CONFIG_TIME_NS
// 	.time_ns		= &init_time_ns,
// 	.time_ns_for_children	= &init_time_ns,
// #endif
};




/*
 * called from clone.  This now handles copy for nsproxy and all
 * namespaces therein.
 */
int copy_namespaces(ulong flags, task_s *tsk)
{
	nsproxy_s *old_ns = tsk->nsproxy;
	// struct user_namespace *user_ns = task_cred_xxx(tsk, user_ns);
	nsproxy_s *new_ns;

	// if (likely(!(flags & (CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWIPC |
	// 			CLONE_NEWPID | CLONE_NEWNET |
	// 			CLONE_NEWCGROUP | CLONE_NEWTIME)))) {
	// 	if ((flags & CLONE_VM) ||
	// 	    likely(old_ns->time_ns_for_children == old_ns->time_ns)) {
	// 		get_nsproxy(old_ns);
	// 		return 0;
	// 	}
	// } else if (!ns_capable(user_ns, CAP_SYS_ADMIN))
	// 	return -EPERM;

	/*
	 * CLONE_NEWIPC must detach from the undolist: after switching
	 * to a new ipc namespace, the semaphore arrays from the old
	 * namespace are unreachable.  In clone parlance, CLONE_SYSVSEM
	 * means share undolist with parent, so we must forbid using
	 * it along with CLONE_NEWIPC.
	 */
	if ((flags & (CLONE_NEWIPC | CLONE_SYSVSEM)) ==
			(CLONE_NEWIPC | CLONE_SYSVSEM))
		return -EINVAL;

	// new_ns = create_new_namespaces(flags, tsk, user_ns, tsk->fs);
	// if (IS_ERR(new_ns))
	// 	return  PTR_ERR(new_ns);

	// if ((flags & CLONE_VM) == 0)
	// 	timens_on_fork(new_ns, tsk);

	// tsk->nsproxy = new_ns;
	return 0;
}