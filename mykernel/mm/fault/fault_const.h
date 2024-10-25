#ifndef _MMFAULT_CONST_H_
#define _MMFAULT_CONST_H_


	#define VM_FAULT_ERROR (VM_FAULT_OOM | VM_FAULT_SIGBUS |       \
							VM_FAULT_SIGSEGV | VM_FAULT_HWPOISON | \
							VM_FAULT_HWPOISON_LARGE | VM_FAULT_FALLBACK)

	/*
	 * The default fault flags that should be used by most of the
	 * arch-specific page fault handlers.
	 */
	#define FAULT_FLAG_DEFAULT  (FAULT_FLAG_ALLOW_RETRY | \
					FAULT_FLAG_KILLABLE | \
					FAULT_FLAG_INTERRUPTIBLE)

#endif /* _MMFAULT_CONST_H_ */