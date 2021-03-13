#ifndef _ADM64_PROTO_H_
#define _AMD64_PROTO_H_

/* Exception handlers (real or protected mode), in numerical order. */
void divide_error (void);
void debug(void);
void nmi (void);
void breakpoint_exception (void);
void overflow (void);
void bounds_exceed (void);
void invalid_opcode (void);
void dev_not_available (void);
void double_fault(void);
void coproc_seg_overrun(void);
void invalid_tss(void);
void segment_not_present(void);
void stack_segfault(void);
void general_protection(void);
void page_fault(void);
// EXCEPTION 15 INTEL RESERVED
void x87_fpu_error(void);
void alignment_check(void);
void machine_check(void);
void simd_exception(void);
void virtualization_exception(void);
void control_protection_exception(void);

typedef struct {
	void	(*gate_entry) (void);
	uint8_t	vec_nr;
	uint8_t	type;
	uint8_t	DPL;
} gate_table_s;

#endif /* _AMD64_PROTO_H_ */