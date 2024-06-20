/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PROTO_H
#define _ASM_X86_PROTO_H

// #include <asm/ldt.h>

struct task_struct;

/* misc architecture specific prototypes */

void syscall_init(void);

void entry_SYSCALL_64(void);
void entry_SYSCALL_64_safe_stack(void);
void entry_SYSRETQ_unsafe_stack(void);
void entry_SYSRETQ_end(void);
long do_arch_prctl_64(struct task_struct *task, int option, unsigned long arg2);

void x86_configure_nx(void);

extern int reboot_force;

long do_arch_prctl_common(int option, unsigned long arg2);

#endif /* _ASM_X86_PROTO_H */