
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