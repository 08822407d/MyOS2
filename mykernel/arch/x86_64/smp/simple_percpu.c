#include <linux/kernel/types.h>

#include <asm/sections.h>
#include <asm/fsgsbase.h>

virt_addr_t calc_pcpu_var_addr(void *proto_addr)
{
    virt_addr_t ret_val =
        (virt_addr_t)proto_addr - (virt_addr_t)&__per_cpu_load;
    return (virt_addr_t) (ret_val + rdgsbase());
}