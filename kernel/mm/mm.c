#include <include/proto.h>
#include <include/page.h>
#include <include/slab.h>

void preinit_mm()
{
	preinit_page();
	preinit_slab();
}

void init_mm()
{
	init_page();
	init_slab();
}