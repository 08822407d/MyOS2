/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IDR_MACROS_H_
#define _LINUX_IDR_MACROS_H_


	#define IDR_INIT_BASE(name, base) {			\
		.idr_base	= (base),					\
		.idr_next	= 0,						\
		.initiated	= false,					\
	}

	/**
	 * IDR_INIT() - Initialise an IDR.
	 * @name: Name of IDR.
	 *
	 * A freshly-initialised IDR contains no IDs.
	 */
	#define IDR_INIT(name)		IDR_INIT_BASE(name, 0)

	/**
	 * DEFINE_IDR() - Define a statically-allocated IDR.
	 * @name: Name of IDR.
	 *
	 * An IDR defined using this macro is ready for use with no additional
	 * initialisation required.  It contains no IDs.
	 */
	#define DEFINE_IDR(name)	idr_s name = IDR_INIT(name)


	#define IDA_INIT(name)	{					\
		.initiated	= false,					\
	}
	#define DEFINE_IDA(name)	ida_s name = IDA_INIT(name)


#endif /* _LINUX_IDR_MACROS_H_ */