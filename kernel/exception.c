/*===========================================================================*
 *				exception				     *
 *===========================================================================*/
void exception_handler(unsigned long errcode)
{
	unsigned long code = errcode;
	while (1)
	{
		/* code */
	}
	
// /* An exception or unexpected interrupt has occurred. */
//   register struct ex_s *ep;
//   struct proc *saved_proc;

//   /* Save proc_ptr, because it may be changed by debug statements. */
//   saved_proc = get_cpulocal_var(proc_ptr);
  
//   ep = &ex_data[frame->vector];

//   if (frame->vector == 2) {		/* spurious NMI on some machines */
// 	printf("got spurious NMI\n");
// 	return;
//   }

//   /*
//    * handle special cases for nested problems as they might be tricky or filter
//    * them out quickly if the traps are not nested
//    */
//   if (is_nested) {
// 	/*
// 	 * if a problem occured while copying a message from userspace because
// 	 * of a wrong pointer supplied by userland, handle it the only way we
// 	 * can handle it ...
// 	 */
// 	if (((void*)frame->eip >= (void*)copy_msg_to_user &&
// 			(void*)frame->eip <= (void*)__copy_msg_to_user_end) ||
// 			((void*)frame->eip >= (void*)copy_msg_from_user &&
// 			(void*)frame->eip <= (void*)__copy_msg_from_user_end)) {
// 		switch(frame->vector) {
// 		/* these error are expected */
// 		case PAGE_FAULT_VECTOR:
// 		case PROTECTION_VECTOR:
// 			frame->eip = (reg_t) __user_copy_msg_pointer_failure;
// 			return;
// 		default:
// 			panic("Copy involving a user pointer failed unexpectedly!");
// 		}
// 	}

// 	/* Pass any error resulting from restoring FPU state, as a FPU
// 	 * exception to the process.
// 	 */
// 	if (((void*)frame->eip >= (void*)fxrstor &&
// 			(void *)frame->eip <= (void*)__fxrstor_end) ||
// 			((void*)frame->eip >= (void*)frstor &&
// 			(void *)frame->eip <= (void*)__frstor_end)) {
// 		frame->eip = (reg_t) __frstor_failure;
// 		return;
// 	}

//   	if(frame->vector == DEBUG_VECTOR
// 		&& (saved_proc->p_reg.psw & TRACEBIT)
// 		&& (saved_proc->p_seg.p_kern_trap_style == KTS_NONE)) {
// 		/* Getting a debug trap in the kernel is legitimate
// 		 * if a traced process entered the kernel using sysenter
// 		 * or syscall; the trap flag is not cleared then.
// 		 *
// 		 * It triggers on the first kernel entry so the trap
// 		 * style is still KTS_NONE.
// 		 */

// 		frame->eflags &= ~TRACEBIT;

// 		return;

// 		/* If control passes, this case is not recognized as legitimate
// 		 * and we panic later on after all.
// 		 */
// 	}
//   }

//   if(frame->vector == PAGE_FAULT_VECTOR) {
// 	pagefault(saved_proc, frame, is_nested);
// 	return;
//   }

//   /* If an exception occurs while running a process, the is_nested variable
//    * will be zero. Exceptions in interrupt handlers or system traps will make
//    * is_nested non-zero.
//    */
//   if (is_nested == 0 && ! iskernelp(saved_proc)) {
// #if 0
// 	{

//   		printf(
//   "vec_nr= %d, trap_errno= 0x%lx, eip= 0x%lx, cs= 0x%x, eflags= 0x%lx\n",
// 			frame->vector, (unsigned long)frame->errcode,
// 			(unsigned long)frame->eip, frame->cs,
// 			(unsigned long)frame->eflags);
// 		proc_stacktrace(saved_proc);
// 	}

// #endif
// 	cause_sig(proc_nr(saved_proc), ep->signum);
// 	return;
//   }

//   /* Exception in system code. This is not supposed to happen. */
//   inkernel_disaster(saved_proc, frame, ep, is_nested);

//   panic("return from inkernel_disaster");
}