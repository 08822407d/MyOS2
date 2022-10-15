#include <linux/mm/page.h>
#include <linux/mm/myos_slab.h>

void myos_early_init_mm()
{
	preinit_page();
	preinit_slab();
}

void myos_init_mm()
{
	init_page();
	init_slab();
}