#include <linux/device/pci.h>
#include <linux/mm/mm.h>
#include <uapi/asm-generic/pci_regs.h>
#include "pci.h"

#define PCIe_CONFSPACE_MMIO_ADDR    0xE0000000
#define PCIe_EXCONFSPACE_SIZE       0x1000
#define FUNC_PER_DEVICE             8
#define DEVICE_PER_BUS              32
#define BUS_PER_HOST                256

PCI_comp_confspace_s *pci_get_comp_confspace(int bus, int dev, int func);

void detect_PCIe_devs()
{
    myos_kernel_physical_mapping_init(PCIe_CONFSPACE_MMIO_ADDR, PCIe_CONFSPACE_MMIO_ADDR + SZ_256M);

    int bus, dev, func;
    for (bus = 0; bus < BUS_PER_HOST; bus++) {
        for (dev = 0; dev < DEVICE_PER_BUS; dev++) {
            for (func = 0; func < FUNC_PER_DEVICE; func++) {
                PCI_comp_confspace_s *conf = pci_get_comp_confspace(bus, dev, func);
                int type = conf->Header_Type;
            }
        }
    }
}

PCI_comp_confspace_s *pci_get_comp_confspace(int bus, int dev, int func)
{
    return (PCI_comp_confspace_s *)phys_to_virt(PCIe_CONFSPACE_MMIO_ADDR) +
        (bus * DEVICE_PER_BUS * FUNC_PER_DEVICE + dev * FUNC_PER_DEVICE + func);
}