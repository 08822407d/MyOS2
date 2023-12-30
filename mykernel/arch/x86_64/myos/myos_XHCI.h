#ifndef __XHCI_H__
#define __XHCI_H__

#include <linux/kernel/types.h>


	typedef struct XHCI_HostCtrl_Cap_Regs {
		u8		CAPLENGTH;
		u8		Rsvd1;
		u16		HCIVERSION;

		union
		{
			u32		val;
			struct
			{
				u32		MaxSlots	: 8,
						MaxIntrs	: 11,
						Rsvd		: 5,
						MaxPorts	: 8;
			} def;
		} HCSPARAMS1;

		union
		{
			u32		val;
			struct
			{
				u32		IST			: 4,
						ERST_Max	: 4,
						Rsvd		: 13,
						MaxSB_Hi	: 5,
						SPR			: 1,
						MaxSB_Lo	: 5;
			} def;
		} HCSPARAMS2;

  		union
		{
			u32		val;
			struct
			{
				u32		U1_DEL		: 8,
						MaxIntrs	: 8,
						U2_DEL		: 16;
			} def;
		} HCSPARAMS3;

  		union
		{
			u32		val;
			struct
			{
				u32		AC_64		: 1,
						BNC			: 1,
						CSZ			: 1,
						PPC			: 1,
						PIND		: 1,
						LHRC		: 1,
						LTC			: 1,
						NSS			: 1,
						PAE			: 1,
						SPC			: 1,
						SEC			: 1,
						CFC			: 1,
						MaxPSASize	: 4,
						xECP		: 16;
			} def;
		} HCCPARAMS1;

		u32		DBOFF;
		u32		RTSOFF;
		
		union
		{
			u32		val;
			struct
			{
				u32		U3C			: 1,
						CMC			: 1,
						FSC			: 1,
						CTC			: 1,
						LEC			: 1,
						CIC			: 1,
						ETC			: 1,
						ETC_TSC		: 1,
						GSC			: 1,
						VTC			: 1,
						xECP		: 22;
			} def;
		} HCCPARAMS2;

		u8		*Rsvd2;
	} XHCI_HCCR_s;


	typedef struct USB_Port_Reg_Set {
		u32		PORTSC;
		u32		PORTPMSC;
		u32		PORTLI;
		u32		PORTHLPMC;
	} USB_PRS_s;

	typedef struct XHCI_HostCtrl_Ops_Regs {
		u32		USBCMD;
		u32		USBSTS;
		u32		PAGESIZE;
		u32		RsvdZ1[2];
		u32		DNCTRL;
		u32		CRCR[2];
		u32		RsvdZ2[4];
		u32		DCBAAP[2];
		u32		CONFIG;
		u32		RsvdZ3[241];

		USB_PRS_s	USB_Port_Reg_Sets[256];
	} XHCI_HCOR_s;


	typedef struct Intr_Reg_Set {
		u32		IMAN;
		u32		IMOD;
		u32		ERSTSZ;
		u32		RsvdP;
		u32		ERSTBA;
		u32		ERDP;
	} XHCI_IRS_s;

	typedef struct XHCI_HostCtrl_RunTime_Regs {
		u32		MFINDEX;
		u8		RsvdZ[28];
		XHCI_IRS_s		IRS_Arr[1024];
	} XHCI_HCRTR_s;

	void XHCI_init();
	void XHCI_exit();

#endif