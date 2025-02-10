#ifndef _COMPLETION_TYPES_H_
#define _COMPLETION_TYPES_H_

	#include "../kactive_type_declaration.h"


	/*
	 * completion_s - structure used to maintain state for a "completion"
	 *
	 * This is the opaque structure used to maintain the state for a "completion".
	 * Completions currently use a FIFO to queue threads that have to wait for
	 * the "completion" event.
	 *
	 * See also:  complete(), wait_for_completion() (and friends _timeout,
	 * _interruptible, _interruptible_timeout, and _killable), init_completion(),
	 * reinit_completion(), and macros DECLARE_COMPLETION(),
	 * DECLARE_COMPLETION_ONSTACK().
	 */
	struct completion {
		uint			done;
		swqueue_hdr_s	wait;
	};

#endif /* _COMPLETION_TYPES_H_ */