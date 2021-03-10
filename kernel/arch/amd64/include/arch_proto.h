#ifndef _ADM64_PROTO_H_
#define _AMD64_PROTO_H_

/* Exception handlers (real or protected mode), in numerical order. */
void int00(void), divide_error (void);
void int01(void), single_step_exception (void);
void int02(void), nmi (void);
void int03(void), breakpoint_exception (void);
void int04(void), overflow (void);
void int05(void), bounds_check (void);
void int06(void), inval_opcode (void);
void int07(void), copr_not_available (void);
void double_fault(void);
void copr_seg_overrun(void);
void inval_tss(void);
void segment_not_present(void);
void stack_exception(void);
void general_protection(void);
void page_fault(void);
void copr_error(void);
void alignment_check(void);
void machine_check(void);
void simd_exception(void);

#endif /* _AMD64_PROTO_H_ */