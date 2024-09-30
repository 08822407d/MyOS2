#ifndef _LINUX_READWRITE_SEMAPHORE_CONST_H_
#define _LINUX_READWRITE_SEMAPHORE_CONST_H_


	#define RWSEM_UNLOCKED_VALUE	0UL
	#define RWSEM_WRITER_LOCKED		(1UL << 0)

	/*
	 * The least significant 2 bits of the owner value has the following
	 * meanings when set.
	 *  - Bit 0: RWSEM_READER_OWNED - rwsem may be owned by readers (just a hint)
	 *  - Bit 1: RWSEM_NONSPINNABLE - Cannot spin on a reader-owned lock
	 *
	 * When the rwsem is reader-owned and a spinning writer has timed out,
	 * the nonspinnable bit will be set to disable optimistic spinning.

	 * When a writer acquires a rwsem, it puts its task_struct pointer
	 * into the owner field. It is cleared after an unlock.
	 *
	 * When a reader acquires a rwsem, it will also puts its task_struct
	 * pointer into the owner field with the RWSEM_READER_OWNED bit set.
	 * On unlock, the owner field will largely be left untouched. So
	 * for a free or reader-owned rwsem, the owner value may contain
	 * information about the last reader that acquires the rwsem.
	 *
	 * That information may be helpful in debugging cases where the system
	 * seems to hang on a reader owned rwsem especially if only one reader
	 * is involved. Ideally we would like to track all the readers that own
	 * a rwsem, but the overhead is simply too big.
	 *
	 * A fast path reader optimistic lock stealing is supported when the rwsem
	 * is previously owned by a writer and the following conditions are met:
	 *  - rwsem is not currently writer owned
	 *  - the handoff isn't set.
	 */
	#define RWSEM_READER_OWNED		(1UL << 0)
	#define RWSEM_NONSPINNABLE		(1UL << 1)
	#define RWSEM_OWNER_FLAGS_MASK	(RWSEM_READER_OWNED | RWSEM_NONSPINNABLE)

	/*
	 * On 64-bit architectures, the bit definitions of the count are:
	 *
	 * Bit  0    - writer locked bit
	 * Bit  1    - waiters present bit
	 * Bit  2    - lock handoff bit
	 * Bits 3-7  - reserved
	 * Bits 8-62 - 55-bit reader count
	 * Bit  63   - read fail bit
	 *
	 * On 32-bit architectures, the bit definitions of the count are:
	 *
	 * Bit  0    - writer locked bit
	 * Bit  1    - waiters present bit
	 * Bit  2    - lock handoff bit
	 * Bits 3-7  - reserved
	 * Bits 8-30 - 23-bit reader count
	 * Bit  31   - read fail bit
	 *
	 * It is not likely that the most significant bit (read fail bit) will ever
	 * be set. This guard bit is still checked anyway in the down_read() fastpath
	 * just in case we need to use up more of the reader bits for other purpose
	 * in the future.
	 *
	 * atomic_long_fetch_add() is used to obtain reader lock, whereas
	 * atomic_long_cmpxchg() will be used to obtain writer lock.
	 *
	 * There are three places where the lock handoff bit may be set or cleared.
	 * 1) rwsem_mark_wake() for readers		-- set, clear
	 * 2) rwsem_try_write_lock() for writers	-- set, clear
	 * 3) rwsem_del_waiter()			-- clear
	 *
	 * For all the above cases, wait_lock will be held. A writer must also
	 * be the first one in the wait_list to be eligible for setting the handoff
	 * bit. So concurrent setting/clearing of handoff bit is not possible.
	 */
	#define RWSEM_WRITER_LOCKED		(1UL << 0)
	#define RWSEM_FLAG_WAITERS		(1UL << 1)
	#define RWSEM_FLAG_HANDOFF		(1UL << 2)
	#define RWSEM_FLAG_READFAIL		(1UL << (BITS_PER_LONG - 1))

	#define RWSEM_READER_SHIFT		8
	#define RWSEM_READER_BIAS		(1UL << RWSEM_READER_SHIFT)
	#define RWSEM_READER_MASK		(~(RWSEM_READER_BIAS - 1))
	#define RWSEM_WRITER_MASK		RWSEM_WRITER_LOCKED
	#define RWSEM_LOCK_MASK			(RWSEM_WRITER_MASK|RWSEM_READER_MASK)
	#define RWSEM_READ_FAILED_MASK	(RWSEM_WRITER_MASK|RWSEM_FLAG_WAITERS|\
										RWSEM_FLAG_HANDOFF|RWSEM_FLAG_READFAIL)

#endif /* _LINUX_READWRITE_SEMAPHORE_CONST_H_ */