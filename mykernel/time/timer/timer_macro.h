/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIMER_MACRO_H_
#define _LINUX_TIMER_MACRO_H_


	#define from_timer(var, callback_timer, timer_fieldname) \
				container_of(callback_timer, typeof(*var), timer_fieldname)

	/**
	 * timer_setup - prepare a timer for first use
	 * @timer: the timer in question
	 * @callback: the function to call when timer expires
	 * @flags: any TIMER_* flags
	 *
	 * Regular timer initialization should use either DEFINE_TIMER() above,
	 * or timer_setup(). For timers on the stack, timer_setup_on_stack() must
	 * be used and must be balanced with a call to destroy_timer_on_stack().
	 */
	#define timer_setup(timer, callback, flags)		\
				simple_init_timer_key((timer), (callback), (flags))
	#define timer_setup_on_stack(timer, callback, flags)	\
				simple_init_timer_key((timer), (callback), (flags))
	
	
	#define del_timer_sync	timer_delete_sync

	static inline void destroy_timer_on_stack(timer_list_s *timer) { }

#endif /* _LINUX_TIMER_MACRO_H_ */