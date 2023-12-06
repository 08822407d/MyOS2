/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/
#include <linux/device/pci.h>
#include <linux/device/pci_ids.h>
#include <linux/kernel/slab.h>
#include <asm/io.h>
#include <asm/bitops.h>
#include <uapi/asm-generic/pci_regs.h>

#include <obsolete/printk.h>
#include "MineOS_PCI.h"

void Print_PCIHDR(struct PCI_Header_00 * PCI_HDR)
{
	color_printk(WHITE, BLACK, "PCI : %#04d:%#02d:%#02d - %s\n", PCI_HDR->bus, PCI_HDR->device, PCI_HDR->function, PCI_HDR->devtype_name);
	color_printk(WHITE, BLACK, "----------------------------------------------\n");
	color_printk(WHITE, BLACK, "\tBAR0 base : %#018lx , BAR0 limit : %#010x\n", PCI_HDR->BAR_base_addr[0], PCI_HDR->BAR_space_limit[0]);
	color_printk(WHITE, BLACK, "----------------------------------------------\n\n");
}

unsigned int Read_PCI_Config(unsigned int bus,unsigned int device,unsigned int function,unsigned int offset)
{
	unsigned int address = 0x80000000 | ((bus & 0xff) << 16) | ((device & 0x1f) << 11) | ((function & 0x7) << 8) | (offset & 0xfc);
	__mb();
	outl(address, 0xcf8);
	__mb();
	return inl(0xcfc);
}

void Write_PCI_Config(unsigned int bus,unsigned int device,unsigned int function,unsigned int offset,unsigned int value)
{
	unsigned int address = 0x80000000 | ((bus & 0xff) << 16) | ((device & 0x1f) << 11) | ((function & 0x7) << 8) | (offset & 0xfc);
	__mb();
	outl(address, 0xcf8);
	__mb();
	outl(value, 0xcfc);
}

int analysis_PCI_Config(struct PCI_Header_00 * PCI_HDR, unsigned int bus, unsigned int device, unsigned int function)
{
	u32 value = 0;
	u32 index = 0;
	u32 tmp = 0;
	u32 cmdstatus_bak = 0;

	PCI_HDR->bus = bus;
	PCI_HDR->device = device;
	PCI_HDR->function = function;

	PCI_HDR->BDF = ((bus & 0xff) << 16) | ((device & 0x1f) << 11) | ((function & 0x7) << 8); 

	value = Read_PCI_Config(bus,device,function,0x00);
	PCI_HDR->VendorID = value & 0xffff;
	PCI_HDR->DeviceID = (value >> 16) & 0xffff;
	// color_printk(RED,BLACK,"PCI Bus:%#04x,Dev:%#04x,Func:%#03x,DID:%#06x,VID:%#06x\n",bus,device,function,PCI_HDR->DeviceID,PCI_HDR->VendorID);

	cmdstatus_bak = value = Read_PCI_Config(bus,device,function,0x04);
	PCI_HDR->Command = value & 0xffff;
	PCI_HDR->Status = (value >> 16) & 0xffff;
	// color_printk(RED,BLACK,"Status:%#06x,Commond:%#06x,",PCI_HDR->Status,PCI_HDR->Command);

	value = Read_PCI_Config(bus,device,function,0x08);
	PCI_HDR->RevisionID = value & 0xff;
	PCI_HDR->ProgIF = (value >> 8) & 0xff;
	PCI_HDR->SubClass = (value >> 16) & 0xff;
	PCI_HDR->ClassCode = (value >> 24) & 0xff;
	// color_printk(RED,BLACK,"Class Code:%#04x,SubClass:%#04x,Prog IF:%#04x,Revision ID:%#04x,",PCI_HDR->ClassCode,PCI_HDR->SubClass,PCI_HDR->ProgIF,PCI_HDR->RevisionID);

	value = Read_PCI_Config(bus,device,function,0x0c);
	PCI_HDR->CacheLineSize = value & 0xff;
	PCI_HDR->LatencyTimer = (value >> 8) & 0xff;
	PCI_HDR->HeaderType = (value >> 16) & 0xff;
	PCI_HDR->BIST = (value >> 24) & 0xff;
	// color_printk(RED,BLACK,"BIST:%#04x,Header Type:%#04x,Latency Timer:%#04x,Cache LineSize:%#04x\n",PCI_HDR->BIST,PCI_HDR->HeaderType,PCI_HDR->LatencyTimer,PCI_HDR->CacheLineSize);

	Write_PCI_Config(bus, device, function, PCI_COMMAND, cmdstatus_bak & 0xfffc);
	int BAR_count = 6;
	if (PCI_HDR->HeaderType == 1)
		BAR_count = 2;
	for (int i = 0; i < BAR_count; i++)
	{
		u32 offset = 0x10 + i * 4;
		PCI_HDR->BAR_base_raw[i] = Read_PCI_Config(bus, device, function, offset);
		Write_PCI_Config(bus, device, function, offset, 0xffffffff);
		PCI_HDR->BAR_limit_raw[i] = Read_PCI_Config(bus, device, function, offset);
		Write_PCI_Config(bus, device, function, offset, PCI_HDR->BAR_base_raw[i]);
		// Assert BAR value not changed
		while (PCI_HDR->BAR_base_raw[i] != Read_PCI_Config(bus, device, function, offset));
	}
	Write_PCI_Config(bus, device, function, PCI_COMMAND, cmdstatus_bak);

	for (int i = 0; i < BAR_count; i++)
	{
		u32 is_IOspace = (PCI_HDR->BAR_base_raw[i] & 1);
		u32 base_mask = 0xfffffff0 | is_IOspace * 0x0c;
		PCI_HDR->BAR_base_addr[i] = PCI_HDR->BAR_base_raw[i] & base_mask;
		if (PCI_HDR->BAR_base_addr[i] != 0)
		{
			u32 limit = PCI_HDR->BAR_limit_raw[i] & 0xfffffff0;
			PCI_HDR->BAR_space_limit[i] = ~limit + 1;
		}

		// 64-bit MainMem space address
		if (value & 4)
		{
			u64 base_raw64 = (u64)PCI_HDR->BAR_base_raw[i + 1] << 32 | PCI_HDR->BAR_base_raw[i];
			u64 limit_raw64 = (u64)PCI_HDR->BAR_limit_raw[i + 1] << 32 | PCI_HDR->BAR_base_raw[i];
			PCI_HDR->BAR_base_addr[i] = base_raw64 & (u64)0xfffffffffffffff0;
			PCI_HDR->BAR_space_limit[i] = ~(limit_raw64 & (u64)0xfffffffffffffff0) + 1;

			// skip next BAR and make sure relavent value is 0
			i++;
			PCI_HDR->BAR_base_addr[i] = 0;
			PCI_HDR->BAR_space_limit[i] = 0;
		}
	}

	value = Read_PCI_Config(bus,device,function,0x28);
	PCI_HDR->CardBusCISPointer = value;
	// color_printk(RED,BLACK,"CardBus CIS Pointer:%#010x,",PCI_HDR->CardBusCISPointer);

	value = Read_PCI_Config(bus,device,function,0x2c);
	PCI_HDR->SubSystemVendorID = value & 0xffff;
	PCI_HDR->SubSystemID = (value >> 16) & 0xffff;
	// color_printk(RED,BLACK,"SubSystem ID:%#06x,SubSystem Vendor ID:%#06x\n",PCI_HDR->SubSystemID,PCI_HDR->SubSystemVendorID);

	value = Read_PCI_Config(bus,device,function,0x30);
	PCI_HDR->ExpansionROMBaseAddress = value;
	// color_printk(RED,BLACK,"Expansion ROM Base Address:%#010x,",PCI_HDR->ExpansionROMBaseAddress);

	value = Read_PCI_Config(bus,device,function,0x34);
	PCI_HDR->CapabilitiesPointer = value & 0xff;
	PCI_HDR->Reserved0 = (value >> 8) & 0xffffff;
	// color_printk(RED,BLACK,"Capabilities Pointer:%#04x\n",PCI_HDR->CapabilitiesPointer);

	PCI_HDR->Reserved1 = 0;

	value = Read_PCI_Config(bus,device,function,0x3c);
	PCI_HDR->InterruptLine = value & 0xff;
	PCI_HDR->InterruptPIN = (value >> 8) & 0xff;
	PCI_HDR->MinGrant = (value >> 16) & 0xff;
	PCI_HDR->MaxLatency = (value >> 24) & 0xff;
	// color_printk(RED,BLACK,"Max Latency:%#04x,Min Grant:%#04x,Interrupt PIN:%#04x,Interrupt Line:%#04x\n",PCI_HDR->MaxLatency,PCI_HDR->MinGrant,PCI_HDR->InterruptPIN,PCI_HDR->InterruptLine);

	index = PCI_HDR->CapabilitiesPointer;

	// while(index != 0)
	// {
	// 	value = Read_PCI_Config(bus,device,function,index);
	// 	// color_printk(GREEN,BLACK,"Capability ID:%#04x,Pointer:%#04x,%#06x,",value & 0xff,((value >> 8) & 0xff),(value >> 16) & 0xffff);
	// 	// color_printk(GREEN,BLACK,"%#010x,",Read_PCI_Config(bus,device,function,index + 4));
	// 	// color_printk(GREEN,BLACK,"%#010x,",Read_PCI_Config(bus,device,function,index + 8));
	// 	// color_printk(GREEN,BLACK,"%#010x\n",Read_PCI_Config(bus,device,function,index + 12));

	// 	index = (value >> 8) & 0xff;
	// }

	return 1;
}

extern void NVMe_init(struct PCI_Header_00 *PCI_HDR);
void scan_PCI_devices(void)
{
	int bus,device,function;
	unsigned int index = 0;
	unsigned int value = 0;
	char *DevType_name;

	for(bus = 0;bus <= 255;bus++)
		for(device = 0;device <= 31;device++)
			for(function = 0;function <= 7;function++)
			{
				value = Read_PCI_Config(bus, device, function, PCI_VENDOR_ID);
				if((value & 0xffff) == 0xffff)	//NVMe_PCI_HBA.VendorID
					continue;

				struct PCI_Header_00 *PCI_HDR = kzalloc(sizeof(struct PCI_Header_00), GFP_KERNEL);
				analysis_PCI_Config(PCI_HDR, bus, device, function);

				u32 CCSCPIF = Read_PCI_Config(bus, device, function, PCI_CLASS_REVISION) >> 8;
				u16 CCSC = (PCI_HDR->ClassCode << 8) | PCI_HDR->SubClass;
				switch (CCSC)
				{
				case PCI_CLASS_STORAGE_NVM:
					if (PCI_HDR->ProgIF == 2)
					{
						// u32 value0 = Read_PCI_Config(bus, device, function, PCI_BASE_ADDRESS_0);
						// u32 value1 = Read_PCI_Config(bus, device, function, PCI_BASE_ADDRESS_1);
						PCI_HDR->devtype_name = "NVMe storage";
						// Print_PCIHDR(PCI_HDR);
						NVMe_init(PCI_HDR);
					}
					break;
				
				case PCI_CLASS_SERIAL_USB:
					if (PCI_HDR->ProgIF == 0x00)
					{
						DevType_name = "USB_uHCI";
					}
					else if (PCI_HDR->ProgIF == 0x10)
					{
						DevType_name = "USB_oHCI";
					}
					else if (PCI_HDR->ProgIF == 0x20)
					{
						DevType_name = "USB_eHCI";
					}
					else if (PCI_HDR->ProgIF == 0x30)
					{
						DevType_name = "USB_xHCI";
					}
					break;
				
				default:
					break;
				}
			}
}