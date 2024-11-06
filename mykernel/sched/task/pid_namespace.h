/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PID_NAMESPACE_H_
#define _LINUX_PID_NAMESPACE_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../sched_const.h"
	#include "../sched_types.h"
	#include "../sched_api.h"


	#ifdef DEBUG


		extern pid_ns_s
		*get_pid_ns(pid_ns_s *ns);

	#endif
	
	#if defined(PID_NAMESPACE_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		pid_ns_s
		*get_pid_ns(pid_ns_s *ns) {
			// if (ns != &init_pid_ns)
			// 	refcount_inc(&ns->ns.count);
			return ns;
		}


	#endif /* !DEBUG */

#endif /* _LINUX_PID_NAMESPACE_H_ */
