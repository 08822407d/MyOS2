#ifndef _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_
#define _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_

    #include <linux/kernel/types.h>

    #include <linux/kernel/lock_ipc_type_declaration.h>

    /* arch spinlock */
	struct tspinlock;
	typedef struct tspinlock arch_spinlock_t;

#endif /* _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_ */