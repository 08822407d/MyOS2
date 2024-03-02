#include <asm/insns.h>
#include <asm/debug_func.h>

void *func_rdgsbase()
{
    return (void *)rdgsbase();
}

void *func_rdfsbase()
{
    return (void *)rdfsbase();
}