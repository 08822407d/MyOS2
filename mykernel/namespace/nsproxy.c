
#include <linux/kernel/mm.h>
#include <linux/kernel/nsproxy.h>
#include <linux/kernel/user_namespace.h>

static kmem_cache_s *nsproxy_cachep;

nsproxy_s init_nsproxy = {
	.count			= REFCOUNT_INIT(1),
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

static inline
nsproxy_s *create_nsproxy(void) {
	nsproxy_s *nsproxy = kmem_cache_alloc(nsproxy_cachep, GFP_KERNEL);
	if (nsproxy)
		refcount_set(&nsproxy->count, 1);
	return nsproxy;
}

/*
 * Create new nsproxy and all of its the associated namespaces.
 * Return the newly created nsproxy.  Do not attach this to the task,
 * leave it to the caller to do proper locking and attach it to task.
 */
static nsproxy_s
*create_new_namespaces(ulong flags, task_s*tsk,
	user_ns_s *user_ns, taskfs_s *new_fs) {

	nsproxy_s *new_nsp;
	int err;

	new_nsp = create_nsproxy();
	if (!new_nsp)
		return ERR_PTR(-ENOMEM);

	// new_nsp->mnt_ns = copy_mnt_ns(flags, tsk->nsproxy->mnt_ns, user_ns, new_fs);
	// if (IS_ERR(new_nsp->mnt_ns)) {
	// 	err = PTR_ERR(new_nsp->mnt_ns);
	// 	goto out_ns;
	// }

	// new_nsp->uts_ns = copy_utsname(flags, user_ns, tsk->nsproxy->uts_ns);
	// if (IS_ERR(new_nsp->uts_ns)) {
	// 	err = PTR_ERR(new_nsp->uts_ns);
	// 	goto out_uts;
	// }

	// new_nsp->ipc_ns = copy_ipcs(flags, user_ns, tsk->nsproxy->ipc_ns);
	// if (IS_ERR(new_nsp->ipc_ns)) {
	// 	err = PTR_ERR(new_nsp->ipc_ns);
	// 	goto out_ipc;
	// }

	new_nsp->pid_ns_for_children =
		copy_pid_ns(flags, user_ns, tsk->nsproxy->pid_ns_for_children);
	if (IS_ERR(new_nsp->pid_ns_for_children)) {
		err = PTR_ERR(new_nsp->pid_ns_for_children);
		goto out_pid;
	}

	// new_nsp->cgroup_ns = copy_cgroup_ns(flags, user_ns,
	// 				    tsk->nsproxy->cgroup_ns);
	// if (IS_ERR(new_nsp->cgroup_ns)) {
	// 	err = PTR_ERR(new_nsp->cgroup_ns);
	// 	goto out_cgroup;
	// }

	// new_nsp->net_ns = copy_net_ns(flags, user_ns, tsk->nsproxy->net_ns);
	// if (IS_ERR(new_nsp->net_ns)) {
	// 	err = PTR_ERR(new_nsp->net_ns);
	// 	goto out_net;
	// }

	// new_nsp->time_ns_for_children = copy_time_ns(flags, user_ns,
	// 				tsk->nsproxy->time_ns_for_children);
	// if (IS_ERR(new_nsp->time_ns_for_children)) {
	// 	err = PTR_ERR(new_nsp->time_ns_for_children);
	// 	goto out_time;
	// }
	// new_nsp->time_ns = get_time_ns(tsk->nsproxy->time_ns);

	return new_nsp;

out_time:
	// put_net(new_nsp->net_ns);
out_net:
	// put_cgroup_ns(new_nsp->cgroup_ns);
out_cgroup:
	if (new_nsp->pid_ns_for_children)
		put_pid_ns(new_nsp->pid_ns_for_children);
out_pid:
	// if (new_nsp->ipc_ns)
	// 	put_ipc_ns(new_nsp->ipc_ns);
out_ipc:
	// if (new_nsp->uts_ns)
	// 	put_uts_ns(new_nsp->uts_ns);
out_uts:
	// if (new_nsp->mnt_ns)
	// 	put_mnt_ns(new_nsp->mnt_ns);
out_ns:
	kmem_cache_free(nsproxy_cachep, new_nsp);
	return ERR_PTR(err);
}

/*
 * called from clone.  This now handles copy for nsproxy and all
 * namespaces therein.
 */
int copy_namespaces(ulong flags, task_s *tsk)
{
	nsproxy_s	*old_ns = tsk->nsproxy;
	// struct	user_namespace *user_ns = task_cred_xxx(tsk, user_ns);
	user_ns_s	*user_ns = NULL;
	nsproxy_s	*new_ns;

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

	new_ns = create_new_namespaces(flags, tsk, user_ns, tsk->fs);
	if (IS_ERR(new_ns))
		return  PTR_ERR(new_ns);

	// if ((flags & CLONE_VM) == 0)
	// 	timens_on_fork(new_ns, tsk);

	tsk->nsproxy = new_ns;
	return 0;
}



int __init nsproxy_cache_init(void)
{
	nsproxy_cachep = KMEM_CACHE(nsproxy, SLAB_PANIC|SLAB_ACCOUNT);
	return 0;
}