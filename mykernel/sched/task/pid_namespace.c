#define PID_NAMESPACE_DEFINATION
#include "pid_namespace.h"

#include <linux/init/init.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/export.h>


// static DEFINE_MUTEX(pid_caches_mutex);
static kmem_cache_s *pid_ns_cachep;
/* Write once array, filled from the beginning. */
static kmem_cache_s *pid_cache[MAX_PID_NS_LEVEL];

/*
 * creates the kmem cache to allocate pids from.
 * @level: pid namespace level
 */

static kmem_cache_s
*create_pid_cachep(uint level) {
	/* Level 0 is init_pid_ns.pid_cachep */
	kmem_cache_s **pkc = &pid_cache[level - 1];
	kmem_cache_s *kc;
	char name[4 + 10 + 1];
	uint len;

	kc = READ_ONCE(*pkc);
	if (kc)
		return kc;

	snprintf(name, sizeof(name), "pid_%u", level + 1);
	len = struct_size_t(pid_s, numbers, level + 1);
	// mutex_lock(&pid_caches_mutex);
	/* Name collision forces to do allocation under mutex. */
	if (!*pkc)
		*pkc =  kmem_cache_create(name,
                    len, 0,
					SLAB_HWCACHE_ALIGN | SLAB_ACCOUNT,
                    NULL);
	// mutex_unlock(&pid_caches_mutex);
	/* current can fail, but someone else can succeed. */
	return READ_ONCE(*pkc);
}


static pid_ns_s
*create_pid_namespace(user_ns_s *user_ns, pid_ns_s *parent_pid_ns) {
	pid_ns_s *ns;
	uint level = parent_pid_ns->level + 1;
	// struct ucounts *ucounts;
	int err;

	// err = -EINVAL;
	// if (!in_userns(parent_pid_ns->user_ns, user_ns))
	// 	goto out;

	err = -ENOSPC;
	if (level > MAX_PID_NS_LEVEL)
		goto out;
	// ucounts = inc_pid_namespaces(user_ns);
	// if (!ucounts)
	// 	goto out;

	err = -ENOMEM;
	ns = kmem_cache_zalloc(pid_ns_cachep, GFP_KERNEL);
	if (ns == NULL)
		goto out_dec;

	idr_init(&ns->idr);

	ns->pid_cachep = create_pid_cachep(level);
	if (ns->pid_cachep == NULL)
		goto out_free_idr;

	// err = ns_alloc_inum(&ns->ns);
	// if (err)
	// 	goto out_free_idr;
	// ns->ns.ops = &pidns_operations;

	// refcount_set(&ns->ns.count, 1);
	ns->level = level;
	ns->parent = get_pid_ns(parent_pid_ns);
	// ns->user_ns = get_user_ns(user_ns);
	// ns->ucounts = ucounts;
	ns->pid_allocated = PIDNS_ADDING;
// #if defined(CONFIG_SYSCTL) && defined(CONFIG_MEMFD_CREATE)
// 	ns->memfd_noexec_scope = pidns_memfd_noexec_scope(parent_pid_ns);
// #endif
	return ns;

out_free_idr:
	// idr_destroy(&ns->idr);
	kmem_cache_free(pid_ns_cachep, ns);
out_dec:
	// dec_pid_namespaces(ucounts);
out:
	return ERR_PTR(err);
}

pid_ns_s
*copy_pid_ns(ulong flags, user_ns_s *user_ns, pid_ns_s *old_ns)
{
	if (!(flags & CLONE_NEWPID))
		return get_pid_ns(old_ns);
	if (task_active_pid_ns(current) != old_ns)
		return ERR_PTR(-EINVAL);
	return create_pid_namespace(user_ns, old_ns);
}

void put_pid_ns(pid_ns_s *ns)
{
	pid_ns_s *parent;

	while (ns != &init_pid_ns) {
		// parent = ns->parent;
		// if (!refcount_dec_and_test(&ns->ns.count))
		// 	break;
		// destroy_pid_namespace(ns);
		// ns = parent;
	}
}
EXPORT_SYMBOL_GPL(put_pid_ns);


__init int pid_namespaces_init(void)
{
	pid_ns_cachep = KMEM_CACHE(pid_namespace, SLAB_PANIC | SLAB_ACCOUNT);

// #ifdef CONFIG_CHECKPOINT_RESTORE
// 	register_sysctl_init("kernel", pid_ns_ctl_table);
// #endif

// 	register_pid_ns_sysctl_table_vm();
	return 0;
}
__initcall(pid_namespaces_init);