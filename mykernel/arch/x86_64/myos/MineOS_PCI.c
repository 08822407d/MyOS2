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
#include <uapi/asm-generic/pci_regs.h>

#include <obsolete/printk.h>
#include "MineOS_PCI.h"

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
	outl(address, 0xcfc);
}

int analysis_PCI_Config(struct PCI_Header_00 * PCI_HDR,unsigned int bus,unsigned int device,unsigned int function)
{
	unsigned int value = 0;
	unsigned int index = 0;
	unsigned int tmp = 0;

	memset(PCI_HDR,0,sizeof(struct PCI_Header_00));
	PCI_HDR->BDF = ((bus & 0xff) << 16) | ((device & 0x1f) << 11) | ((function & 0x7) << 8); 

	value = Read_PCI_Config(bus,device,function,0x00);
	PCI_HDR->VendorID = value & 0xffff;
	PCI_HDR->DeviceID = (value >> 16) & 0xffff;
	// color_printk(RED,BLACK,"PCI Bus:%#04x,Dev:%#04x,Func:%#03x,DID:%#06x,VID:%#06x\n",bus,device,function,PCI_HDR->DeviceID,PCI_HDR->VendorID);

	value = Read_PCI_Config(bus,device,function,0x04);
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

	PCI_HDR->Base32RAWData0 = Read_PCI_Config(bus,device,function,0x10);	//////BAR0
	if(PCI_HDR->Base32RAWData0 & 1)
		PCI_HDR->Base32Address0 = PCI_HDR->Base32RAWData0 & 0xfffffffc;
	else
		PCI_HDR->Base32Address0 = PCI_HDR->Base32RAWData0 & 0xfffffff0;
	if(PCI_HDR->Base32Address0)
	{
		Write_PCI_Config(bus,device,function,0x10,0xffffffff);
		tmp = Read_PCI_Config(bus,device,function,0x10);
		tmp = tmp & 0xfffffff0;
		PCI_HDR->Base32Limit0 = ~tmp + 1;
		if(PCI_HDR->Base32RAWData0 & 1)
			PCI_HDR->Base32Limit0 = PCI_HDR->Base32Limit0 & 0x0000ffff;
		Write_PCI_Config(bus,device,function,0x10,PCI_HDR->Base32RAWData0);
	}
	// color_printk(RED,BLACK,"Base Address #0:%#010x,Base Limit #0:%#010x\n",PCI_HDR->Base32Address0,PCI_HDR->Base32Limit0);

	PCI_HDR->Base32RAWData1 = Read_PCI_Config(bus,device,function,0x14);	//////BAR1
	if(PCI_HDR->Base32RAWData1 & 1)
		PCI_HDR->Base32Address1 = PCI_HDR->Base32RAWData1 & 0xfffffffc;
	else
		PCI_HDR->Base32Address1 = PCI_HDR->Base32RAWData1 & 0xfffffff0;
	if(PCI_HDR->Base32Address1)
	{
		Write_PCI_Config(bus,device,function,0x14,0xffffffff);
		tmp = Read_PCI_Config(bus,device,function,0x14);
		tmp = tmp & 0xfffffff0;
		PCI_HDR->Base32Limit1 = ~tmp + 1;
		if(PCI_HDR->Base32RAWData1 & 1)
			PCI_HDR->Base32Limit1 = PCI_HDR->Base32Limit1 & 0x0000ffff;
		Write_PCI_Config(bus,device,function,0x14,PCI_HDR->Base32RAWData1);
	}
	// color_printk(RED,BLACK,"Base Address #1:%#010x,Base Limit #1:%#010x\n",PCI_HDR->Base32Address1,PCI_HDR->Base32Limit1);

/*
	if(PCI_HDR->Base32RAWData0 & 0x4)	//base address register for 64bit memory
	{
		PCI_HDR->Base64RAWData0 = ((unsigned long)PCI_HDR->Base32RAWData1 << 32) | PCI_HDR->Base32RAWData0;
		PCI_HDR->Base64Address0 = PCI_HDR->Base64RAWData0 & (unsigned long)0xfffffffffffffff0;

		//////BAR0
		Write_PCI_Config(bus,device,function,0x10,0xffffffff);
		PCI_HDR->Base32Limit0 = Read_PCI_Config(bus,device,function,0x10);
		Write_PCI_Config(bus,device,function,0x10,PCI_HDR->Base32RAWData0);

		//////BAR1
		Write_PCI_Config(bus,device,function,0x14,0xffffffff);
		PCI_HDR->Base32Limit1 = Read_PCI_Config(bus,device,function,0x14);
		Write_PCI_Config(bus,device,function,0x14,PCI_HDR->Base32RAWData1);

		PCI_HDR->Base64Limit0 = ((unsigned long)PCI_HDR->Base32Limit1 << 32) | PCI_HDR->Base32Limit0;
		PCI_HDR->Base64Limit0 = PCI_HDR->Base64Limit0 & (unsigned long)0xfffffffffffffff0;
		PCI_HDR->Base64Limit0 = ~PCI_HDR->Base64Limit0 + 1;
	}
*/

	PCI_HDR->Base32RAWData2 = Read_PCI_Config(bus,device,function,0x18);	//////BAR2
	if(PCI_HDR->Base32RAWData2 & 1)
		PCI_HDR->Base32Address2 = PCI_HDR->Base32RAWData2 & 0xfffffffc;
	else
		PCI_HDR->Base32Address2 = PCI_HDR->Base32RAWData2 & 0xfffffff0;
	if(PCI_HDR->Base32Address2)
	{
		Write_PCI_Config(bus,device,function,0x18,0xffffffff);
		tmp = Read_PCI_Config(bus,device,function,0x18);
		tmp = tmp & 0xfffffff0;
		PCI_HDR->Base32Limit2 = ~tmp + 1;
		if(PCI_HDR->Base32RAWData2 & 1)
			PCI_HDR->Base32Limit2 = PCI_HDR->Base32Limit2 & 0x0000ffff;
		Write_PCI_Config(bus,device,function,0x18,PCI_HDR->Base32RAWData2);
	}
	// color_printk(RED,BLACK,"Base Address #2:%#010x,Base Limit #2:%#010x\n",PCI_HDR->Base32Address2,PCI_HDR->Base32Limit2);

	PCI_HDR->Base32RAWData3 = Read_PCI_Config(bus,device,function,0x1c);	//////BAR3
	if(PCI_HDR->Base32RAWData3 & 1)
		PCI_HDR->Base32Address3 = PCI_HDR->Base32RAWData3 & 0xfffffffc;
	else
		PCI_HDR->Base32Address3 = PCI_HDR->Base32RAWData3 & 0xfffffff0;
	if(PCI_HDR->Base32Address3)
	{
		Write_PCI_Config(bus,device,function,0x1c,0xffffffff);
		tmp = Read_PCI_Config(bus,device,function,0x1c);
		tmp = tmp & 0xfffffff0;
		PCI_HDR->Base32Limit3 = ~tmp + 1;
		if(PCI_HDR->Base32RAWData3 & 1)
			PCI_HDR->Base32Limit3 = PCI_HDR->Base32Limit3 & 0x0000ffff;
		Write_PCI_Config(bus,device,function,0x1c,PCI_HDR->Base32RAWData3);
	}
	// color_printk(RED,BLACK,"Base Address #3:%#010x,Base Limit #3:%#010x\n",PCI_HDR->Base32Address3,PCI_HDR->Base32Limit3);

	PCI_HDR->Base32RAWData4 = Read_PCI_Config(bus,device,function,0x20);	//////BAR4
	if(PCI_HDR->Base32RAWData4 & 1)
		PCI_HDR->Base32Address4 = PCI_HDR->Base32RAWData4 & 0xfffffffc;
	else
		PCI_HDR->Base32Address4 = PCI_HDR->Base32RAWData4 & 0xfffffff0;
	if(PCI_HDR->Base32Address4)
	{
		Write_PCI_Config(bus,device,function,0x20,0xffffffff);
		tmp = Read_PCI_Config(bus,device,function,0x20);
		tmp = tmp & 0xfffffff0;
		PCI_HDR->Base32Limit4 = ~tmp + 1;
		if(PCI_HDR->Base32RAWData4 & 1)
			PCI_HDR->Base32Limit4 = PCI_HDR->Base32Limit4 & 0x0000ffff;
		Write_PCI_Config(bus,device,function,0x20,PCI_HDR->Base32RAWData4);
	}
	// color_printk(RED,BLACK,"Base Address #4:%#010x,Base Limit #4:%#010x\n",PCI_HDR->Base32Address4,PCI_HDR->Base32Limit4);

	PCI_HDR->Base32RAWData5 = Read_PCI_Config(bus,device,function,0x24);	//////BAR5
	if(PCI_HDR->Base32RAWData5 & 1)
		PCI_HDR->Base32Address5 = PCI_HDR->Base32RAWData5 & 0xfffffffc;
	else
		PCI_HDR->Base32Address5 = PCI_HDR->Base32RAWData5 & 0xfffffff0;
	if(PCI_HDR->Base32Address5)
	{
		Write_PCI_Config(bus,device,function,0x24,0xffffffff);
		tmp = Read_PCI_Config(bus,device,function,0x24);
		tmp = tmp & 0xfffffff0;
		PCI_HDR->Base32Limit5 = ~tmp + 1;
		if(PCI_HDR->Base32RAWData5 & 1)
			PCI_HDR->Base32Limit5 = PCI_HDR->Base32Limit5 & 0x0000ffff;
		Write_PCI_Config(bus,device,function,0x24,PCI_HDR->Base32RAWData5);
	}
	// color_printk(RED,BLACK,"Base Address #5:%#010x,Base Limit #5:%#010x\n",PCI_HDR->Base32Address5,PCI_HDR->Base32Limit5);

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

	while(index != 0)
	{
		value = Read_PCI_Config(bus,device,function,index);
		// color_printk(GREEN,BLACK,"Capability ID:%#04x,Pointer:%#04x,%#06x,",value & 0xff,((value >> 8) & 0xff),(value >> 16) & 0xffff);
		// color_printk(GREEN,BLACK,"%#010x,",Read_PCI_Config(bus,device,function,index + 4));
		// color_printk(GREEN,BLACK,"%#010x,",Read_PCI_Config(bus,device,function,index + 8));
		// color_printk(GREEN,BLACK,"%#010x\n",Read_PCI_Config(bus,device,function,index + 12));

		index = (value >> 8) & 0xff;
	}

	return 1;
}

extern void NVMe_init(struct PCI_Header_00 *PCI_HDR);
void scan_PCI_devices(void)
{
	int bus,device,function;
	unsigned int index = 0;
	unsigned int value = 0;

	for(bus = 0;bus <= 255;bus++)
		for(device = 0;device <= 31;device++)
			for(function = 0;function <= 7;function++)
			{
				value = Read_PCI_Config(bus, device, function, PCI_VENDOR_ID);
				if((value & 0xffff) == 0xffff)	//NVMe_PCI_HBA.VendorID
					continue;

				struct PCI_Header_00 *PCI_HDR = kzalloc(sizeof(struct PCI_Header_00), GFP_KERNEL);
				analysis_PCI_Config(PCI_HDR,bus,device,function);

				u32 CCSCPIF = Read_PCI_Config(bus, device, function, PCI_CLASS_REVISION) >> 8;
				u16 CCSC = (PCI_HDR->ClassCode << 8) | PCI_HDR->SubClass;
				switch (CCSC)
				{
				case PCI_CLASS_STORAGE_EXPRESS:
					if (PCI_HDR->ProgIF == 2)
						NVMe_init(PCI_HDR);
					break;
				
				default:
					break;
				}
			}
}