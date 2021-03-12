#include <lib/string.h>
#include <lib/assert.h>
#include <sys/types.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "../../proto.h"

/* Storage for gdt, idt and tss. */
segdesc64_s gdt[GDT_SIZE] __aligned(SEGDESC_SIZE);
gatedesc64_s idt[IDT_SIZE] __aligned(GATEDESC_SIZE);
tss64_s tss[CONFIG_MAX_CPUS];
desctblptr64_s gdt_ptr;
desctblptr64_s idt_ptr;

void set_codeseg(uint32_t index, CommSegType type, uint8_t privil)
{
	segdesc64_s *sd = &(gdt[index]);
	// fixed bits for codeseg
	sd->Lflag	= 1;
	sd->Sflag	= 1;
	sd->Present	= 1;
	// changable bits
	sd->Type	= type;
	sd->Privil	= privil;
}

void set_dataseg(uint32_t index, CommSegType type, uint8_t privil)
{
	segdesc64_s *sd = &(gdt[index]);
	// fixed bits for dataseg
	sd->Lflag	= 0;
	sd->Sflag	= 1;
	sd->Present	= 1;
	// changable bits
	sd->Type	= type;
	sd->Privil	= privil;
}

void init_gdt()
{
	memset(gdt, 0, sizeof(gdt));

	// gdt[0] has been set to 0 by memset
	set_codeseg(KERN_CS_INDEX, E_CODE, 0);
	set_dataseg(KERN_DS_INDEX, RW_DATA, 0);
	set_codeseg(USER_CS_INDEX, E_CODE, 3);
	set_dataseg(USER_DS_INDEX, RW_DATA, 3);

	// &gdt[TSS_INDEX_FIRST]	= 0x0000000000000000;

	gdt_ptr.base = &gdt[NULL_DESC_INDEX];
	gdt_ptr.limit = sizeof(gdt) - 1;
}

void init_idt()
{
	memset(gdt, 0, sizeof(idt));
}

void prot_init(void)
{
	init_gdt();
	init_idt();


	__asm__ __volatile__("lgdt %0": : "m"(gdt_ptr));
	__asm__ __volatile__("lidt %0": : "m"(idt_ptr));

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