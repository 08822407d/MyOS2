#ifndef _COMPLETION_MACRO_H_
#define _COMPLETION_MACRO_H_

	#define COMPLETION_INITIALIZER(work)	\
			{ 0, __SWAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
	
	/**
	 * DECLARE_COMPLETION - declare and initialize a completion structure
	 * @work:  identifier for the completion structure
	 *
	 * This macro declares and initializes a completion structure. Generally used
	 * for static declarations. You should use the _ONSTACK variant for automatic
	 * variables.
	 */
	#define DECLARE_COMPLETION(work)	\
				completion_s work = COMPLETION_INITIALIZER(work)

	#define DECLARE_COMPLETION_ONSTACK(work)	DECLARE_COMPLETION(work)

#endif /* _COMPLETION_MACRO_H_ */