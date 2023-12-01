// SPDX-License-Identifier: GPL-2.0
/*
 * PCI detection and setup code
 */

#include <linux/kernel/kernel.h>
#include <linux/kernel/delay.h>
#include <linux/init/init.h>
#include <linux/device/pci.h>
// #include <linux/msi.h>
// #include <linux/of_device.h>
// #include <linux/of_pci.h>
// #include <linux/pci_hotplug.h>
#include <linux/kernel/slab.h>
// #include <linux/module.h>
// #include <linux/cpumask.h>
// #include <linux/aer.h>
// #include <linux/acpi.h>
// #include <linux/hypervisor.h>
// #include <linux/irqdomain.h>
// #include <linux/pm_runtime.h>
// #include <linux/bitfield.h>
#include "pci.h"


#include <uapi/asm-generic/pci_regs.h>


/**
 * pci_cfg_space_size_ext - Get the configuration space size of the PCI device
 * @dev: PCI device
 *
 * Regular PCI devices have 256 bytes, but PCI-X 2 and PCI Express devices
 * have 4096 bytes.  Even if the device is capable, that doesn't mean we can
 * access it.  Maybe we don't have a way to generate extended config space
 * accesses, or the device is behind a reverse Express bridge.  So we try
 * reading the dword at 0x100 which must either be 0 or a valid extended
 * capability header.
 */
static int pci_cfg_space_size_ext(pci_dev_s *dev)
{
	u32 status;
	int pos = PCI_CFG_SPACE_SIZE;

	// if (pci_read_config_dword(dev, pos, &status) != PCIBIOS_SUCCESSFUL)
	// 	return PCI_CFG_SPACE_SIZE;
	// if (PCI_POSSIBLE_ERROR(status) || pci_ext_cfg_is_aliased(dev))
	// 	return PCI_CFG_SPACE_SIZE;

	return PCI_CFG_SPACE_EXP_SIZE;
}

int pci_cfg_space_size(pci_dev_s *dev)
{
	int pos;
	u32 status;
	u16 class;

	// if (dev->bus->bus_flags & PCI_BUS_FLAGS_NO_EXTCFG)
	// 	return PCI_CFG_SPACE_SIZE;

	// class = dev->class >> 8;
	// if (class == PCI_CLASS_BRIDGE_HOST)
	// 	return pci_cfg_space_size_ext(dev);

	// if (pci_is_pcie(dev))
	// 	return pci_cfg_space_size_ext(dev);

	// pos = pci_find_capability(dev, PCI_CAP_ID_PCIX);
	// if (!pos)
	// 	return PCI_CFG_SPACE_SIZE;

	// pci_read_config_dword(dev, pos + PCI_X_STATUS, &status);
	// if (status & (PCI_X_STATUS_266MHZ | PCI_X_STATUS_533MHZ))
	// 	return pci_cfg_space_size_ext(dev);

	return PCI_CFG_SPACE_SIZE;
}

static u32 pci_class(pci_dev_s *dev)
{
	// u32 class;
	// pci_read_config_dword(dev, PCI_CLASS_REVISION, &class);
	// return class;
}

static void pci_subsystem_ids(pci_dev_s *dev, u16 *vendor, u16 *device)
{
	// pci_read_config_word(dev, PCI_SUBSYSTEM_VENDOR_ID, vendor);
	// pci_read_config_word(dev, PCI_SUBSYSTEM_ID, device);
}

static u8 pci_hdr_type(pci_dev_s *dev)
{
	// u8 hdr_type;
	// pci_read_config_byte(dev, PCI_HEADER_TYPE, &hdr_type);
	// return hdr_type;
}


/**
 * pci_setup_device - Fill in class and map information of a device
 * @dev: the device structure to fill
 *
 * Initialize the device structure with information about the device's
 * vendor,class,memory and IO-space addresses, IRQ lines etc.
 * Called at initialisation of the PCI subsystem and by CardBus services.
 * Returns 0 on success and negative if unknown type of device (not normal,
 * bridge or CardBus).
 */
int pci_setup_device(pci_dev_s *dev)
{
	u32 class;
	u16 cmd;
	u8 hdr_type;
	int err, pos = 0;
	// struct pci_bus_region region;
	// struct resource *res;

	// hdr_type = pci_hdr_type(dev);

	// dev->sysdata = dev->bus->sysdata;
	// dev->dev.parent = dev->bus->bridge;
	// dev->dev.bus = &pci_bus_type;
	dev->hdr_type = hdr_type & 0x7f;
	dev->multifunction = !!(hdr_type & 0x80);
	// dev->error_state = pci_channel_io_normal;
	// set_pcie_port_type(dev);

	// err = pci_set_of_node(dev);
	// if (err)
	// 	return err;
	// pci_set_acpi_fwnode(dev);

	// pci_dev_assign_slot(dev);

	/*
	 * Assume 32-bit PCI; let 64-bit PCI cards (which are far rarer)
	 * set this higher, assuming the system even supports it.
	 */
	dev->dma_mask = 0xffffffff;

	// dev_set_name(&dev->dev, "%04x:%02x:%02x.%d", pci_domain_nr(dev->bus),
	// 	     dev->bus->number, PCI_SLOT(dev->devfn),
	// 	     PCI_FUNC(dev->devfn));

	// class = pci_class(dev);

	dev->revision = class & 0xff;
	dev->class = class >> 8;		    /* upper 3 bytes */

	// if (pci_early_dump)
	// 	early_dump_pci_device(dev);

	// /* Need to have dev->class ready */
	// dev->cfg_size = pci_cfg_space_size(dev);

	// /* Need to have dev->cfg_size ready */
	// set_pcie_thunderbolt(dev);

	// set_pcie_untrusted(dev);

	// /* "Unknown power state" */
	// dev->current_state = PCI_UNKNOWN;

	// /* Early fixups, before probing the BARs */
	// pci_fixup_device(pci_fixup_early, dev);

	// pci_set_removable(dev);

	// pci_info(dev, "[%04x:%04x] type %02x class %#08x\n",
	// 	 dev->vendor, dev->device, dev->hdr_type, dev->class);

	// /* Device class may be changed after fixup */
	// class = dev->class >> 8;

	// if (dev->non_compliant_bars && !dev->mmio_always_on) {
	// 	pci_read_config_word(dev, PCI_COMMAND, &cmd);
	// 	if (cmd & (PCI_COMMAND_IO | PCI_COMMAND_MEMORY)) {
	// 		pci_info(dev, "device has non-compliant BARs; disabling IO/MEM decoding\n");
	// 		cmd &= ~PCI_COMMAND_IO;
	// 		cmd &= ~PCI_COMMAND_MEMORY;
	// 		pci_write_config_word(dev, PCI_COMMAND, cmd);
	// 	}
	// }

	// dev->broken_intx_masking = pci_intx_mask_broken(dev);

	// switch (dev->hdr_type) {		    /* header type */
	// case PCI_HEADER_TYPE_NORMAL:		    /* standard header */
	// 	if (class == PCI_CLASS_BRIDGE_PCI)
	// 		goto bad;
	// 	pci_read_irq(dev);
	// 	pci_read_bases(dev, 6, PCI_ROM_ADDRESS);

	// 	pci_subsystem_ids(dev, &dev->subsystem_vendor, &dev->subsystem_device);

	// 	/*
	// 	 * Do the ugly legacy mode stuff here rather than broken chip
	// 	 * quirk code. Legacy mode ATA controllers have fixed
	// 	 * addresses. These are not always echoed in BAR0-3, and
	// 	 * BAR0-3 in a few cases contain junk!
	// 	 */
	// 	if (class == PCI_CLASS_STORAGE_IDE) {
	// 		u8 progif;
	// 		pci_read_config_byte(dev, PCI_CLASS_PROG, &progif);
	// 		if ((progif & 1) == 0) {
	// 			region.start = 0x1F0;
	// 			region.end = 0x1F7;
	// 			res = &dev->resource[0];
	// 			res->flags = LEGACY_IO_RESOURCE;
	// 			pcibios_bus_to_resource(dev->bus, res, &region);
	// 			pci_info(dev, "legacy IDE quirk: reg 0x10: %pR\n",
	// 				 res);
	// 			region.start = 0x3F6;
	// 			region.end = 0x3F6;
	// 			res = &dev->resource[1];
	// 			res->flags = LEGACY_IO_RESOURCE;
	// 			pcibios_bus_to_resource(dev->bus, res, &region);
	// 			pci_info(dev, "legacy IDE quirk: reg 0x14: %pR\n",
	// 				 res);
	// 		}
	// 		if ((progif & 4) == 0) {
	// 			region.start = 0x170;
	// 			region.end = 0x177;
	// 			res = &dev->resource[2];
	// 			res->flags = LEGACY_IO_RESOURCE;
	// 			pcibios_bus_to_resource(dev->bus, res, &region);
	// 			pci_info(dev, "legacy IDE quirk: reg 0x18: %pR\n",
	// 				 res);
	// 			region.start = 0x376;
	// 			region.end = 0x376;
	// 			res = &dev->resource[3];
	// 			res->flags = LEGACY_IO_RESOURCE;
	// 			pcibios_bus_to_resource(dev->bus, res, &region);
	// 			pci_info(dev, "legacy IDE quirk: reg 0x1c: %pR\n",
	// 				 res);
	// 		}
	// 	}
	// 	break;

	// case PCI_HEADER_TYPE_BRIDGE:		    /* bridge header */
	// 	/*
	// 	 * The PCI-to-PCI bridge spec requires that subtractive
	// 	 * decoding (i.e. transparent) bridge must have programming
	// 	 * interface code of 0x01.
	// 	 */
	// 	pci_read_irq(dev);
	// 	dev->transparent = ((dev->class & 0xff) == 1);
	// 	pci_read_bases(dev, 2, PCI_ROM_ADDRESS1);
	// 	pci_read_bridge_windows(dev);
	// 	set_pcie_hotplug_bridge(dev);
	// 	pos = pci_find_capability(dev, PCI_CAP_ID_SSVID);
	// 	if (pos) {
	// 		pci_read_config_word(dev, pos + PCI_SSVID_VENDOR_ID, &dev->subsystem_vendor);
	// 		pci_read_config_word(dev, pos + PCI_SSVID_DEVICE_ID, &dev->subsystem_device);
	// 	}
	// 	break;

	// case PCI_HEADER_TYPE_CARDBUS:		    /* CardBus bridge header */
	// 	if (class != PCI_CLASS_BRIDGE_CARDBUS)
	// 		goto bad;
	// 	pci_read_irq(dev);
	// 	pci_read_bases(dev, 1, 0);
	// 	pci_read_config_word(dev, PCI_CB_SUBSYSTEM_VENDOR_ID, &dev->subsystem_vendor);
	// 	pci_read_config_word(dev, PCI_CB_SUBSYSTEM_ID, &dev->subsystem_device);
	// 	break;

	// default:				    /* unknown header */
	// 	pci_err(dev, "unknown header type %02x, ignoring device\n",
	// 		dev->hdr_type);
	// 	pci_release_of_node(dev);
	// 	return -EIO;

	// bad:
	// 	pci_err(dev, "ignoring class %#08x (doesn't match header type %02x)\n",
	// 		dev->class, dev->hdr_type);
	// 	dev->class = PCI_CLASS_NOT_DEFINED << 8;
	// }

	/* We found a fine healthy device, go go go... */
	return 0;
}