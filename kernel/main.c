#include <lib/string.h>
#include <lib/assert.h>

#include "proto.h"
#include "arch/amd64/include/arch_proto.h"

void cstart(void)
{
    prot_init();
}

void kmain(void)
{
    static int bss_test;

    /* bss sanity check */
    // assert(bss_test == 0);
    bss_test = 1;

    cstart();
}