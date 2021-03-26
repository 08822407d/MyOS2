#ifndef _ADM64_PROTO_H_
#define _AMD64_PROTO_H_

#include <sys/types.h>
#include "archtypes.h"
#include "../../../include/ktypes.h"

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

typedef struct __attribute__((packed)) {
	reg_t ds;
	reg_t es;
	reg_t r15;
	reg_t r14;
	reg_t r13;
	reg_t r12;
	reg_t r11;
	reg_t r10;
	reg_t r9;
	reg_t r8;
	reg_t rsi;
	reg_t rdi;
	reg_t rbp;
	reg_t rax;
	reg_t rbx;
	reg_t rcx;
	reg_t rdx;
	reg_t vec_nr;
	reg_t err_code;
	reg_t rip;
	reg_t cs;
	reg_t rflags;
	reg_t rsp;
	reg_t ss;
} stack_frame_s;

void amd64_excep_handler(stack_frame_s *);

typedef struct {
	void	(*gate_entry) (void);
	uint8_t	vec_nr;
	uint8_t	type;
	uint8_t	DPL;
} gate_table_s;

/* protect.c */
phy_addr vir2phy(vir_addr);
vir_addr phy2vir(phy_addr);

/* pg_util.c */
void pg_clear(void);
void pg_load_cr3(PML4E *);
void pg_flush_tlb(void);
void pg_domap(vir_addr, phy_addr, uint64_t);
void pg_unmap(vir_addr);

#endif /* _AMD64_PROTO_H_ */