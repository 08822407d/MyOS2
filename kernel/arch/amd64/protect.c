#include <lib/string.h>
#include <lib/assert.h>
#include <sys/types.h>

#include "../../proto.h"

void prot_init(void)
{
	// memset(gdt, 0, sizeof(gdt));
	// memset(idt, 0, sizeof(idt));

	// /* Build GDT, IDT, IDT descriptors. */
	// gdt_desc.base = (u32_t) gdt;
	// gdt_desc.limit = sizeof(gdt)-1;
	// idt_desc.base = (u32_t) idt;
	// idt_desc.limit = sizeof(idt)-1;
	// tss_init(0, &k_boot_stktop);

	// /* Build GDT */
	// init_param_dataseg(&gdt[LDT_INDEX],
	// (phys_bytes) 0, 0, INTR_PRIVILEGE); /* unusable LDT */
	// gdt[LDT_INDEX].access = PRESENT | LDT;
	// init_codeseg(KERN_CS_INDEX, INTR_PRIVILEGE);
	// init_dataseg(KERN_DS_INDEX, INTR_PRIVILEGE);
	// init_codeseg(USER_CS_INDEX, USER_PRIVILEGE);
	// init_dataseg(USER_DS_INDEX, USER_PRIVILEGE);

	// /* Currently the multiboot segments are loaded; which is fine, but
	// * let's replace them with the ones from our own GDT so we test
	// * right away whether they work as expected.
	// */
	// prot_load_selectors();

	// /* Set up a new post-relocate bootstrap pagetable so that
	// * we can map in VM, and we no longer rely on pre-relocated
	// * data.
	// */

	// pg_clear();
	// pg_identity(&kinfo); /* Still need 1:1 for lapic and video mem and such. */
	// pg_mapkernel();
	// pg_load();

	// prot_init_done = 1;
}