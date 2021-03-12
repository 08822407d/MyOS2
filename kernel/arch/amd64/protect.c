#include <lib/string.h>
#include <lib/assert.h>
#include <sys/types.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "../../proto.h"

/* Storage for gdt, idt and tss. */
segdesc64_s gdt[GDT_SIZE] __aligned(SEGDESC_SIZE);
descselector_s gdtslctr[GDT_SIZE] __aligned(SEGDESC_SIZE);
gatedesc64_s idt[IDT_SIZE] __aligned(GATEDESC_SIZE);
tss64_s tss[CONFIG_MAX_CPUS];
desctblptr64_s gdt_ptr;
desctblptr64_s idt_ptr;

void set_commseg(uint32_t index, CommSegType type, uint8_t privil, uint8_t Lflag)
{
	segdesc64_s *sd = &(gdt[index]);
	sd->Sflag	= 1;
	sd->Present	= 1;
	// changable bits
	sd->Lflag	= Lflag;
	sd->Type	= type;
	sd->Privil	= privil;

	descselector_s *ds = &(gdtslctr[index]);
	ds->Index	= index;
	ds->RPL		= privil;
}

void set_codeseg(uint32_t index, CommSegType type, uint8_t privil)
{
	set_commseg(index, type, privil, 1);
}

void set_dataseg(uint32_t index, CommSegType type, uint8_t privil)
{
	set_commseg(index, type, privil, 0);
}	

void init_gdt()
{
	memset(gdt, 0, sizeof(gdt));
	memset(gdtslctr, 0, sizeof(gdtslctr));

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


	__asm__ __volatile__("	lgdt	%0			\n\
							mov		%1, %%ds	\n\
							mov		%1, %%es	\n\
							mov		%1, %%fs	\n\
							mov		%1, %%gs	\n\
							mov		%1, %%ss	\n	"
						 :
						 :	"m"(gdt_ptr),
						 	"a"(gdtslctr[KERN_DS_INDEX]));

	__asm__ __volatile__("lidt %0": : "m"(idt_ptr));

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