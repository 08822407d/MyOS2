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
				u32
					MaxSlots	: 8,
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
				u32
					IST			: 4,
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
				u32
					U1_DEL		: 8,
					MaxIntrs	: 8,
					U2_DEL		: 16;
			} def;
		} HCSPARAMS3;

  		union
		{
			u32		val;
			struct
			{
				u32
					AC_64		: 1,
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
				u32
					U3C			: 1,
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
		union
		{
			u32		val;
			struct
			{
				u32
					CCS			: 1,
					PED			: 1,
					TM			: 1,
					OCA			: 1,
					PR			: 1,
					PLS			: 4,
					PP			: 1,
					Port_Speed	: 4,
					PIC			: 2,
					LWS			: 1,
					CSC			: 1,
					PEC			: 1,
					WRC			: 1,
					OCC			: 1,
					PRC			: 1,
					PLC			: 1,
					CEC			: 1,
					CAS			: 1,
					WCE			: 1,
					WDE			: 1,
					WOE			: 1,
					RsvdZ		: 2,
					DR			: 1,
					WPR			: 1;
			} def;
		} PORTSC;

		union
		{
			u32		val;
			union
			{
				struct
				{
					u32
						U1_Timeout	: 8,
						U2_Timeout	: 8,
						FLA			: 1,
						RsvdP3		: 15;
				} def;
			} USB3;

			union
			{

			} USB2;
		} PORTPMSC;

		union
		{
			u32		val;

			union
			{
				struct
				{
					u32
						LEC			: 16,
						RLC			: 4,
						TLC			: 4,
						RsvdP		: 8;
				} def;
			} USB3;

			union
			{

			} USB2;
		} PORTLI;

		u32		PORTHLPMC;
	} USB_PRS_s;

	typedef struct XHCI_HostCtrl_Ops_Regs {
		// union
		// {
		// 	u32		val;
		// 	struct
		// 	{
		// 		u32
		// 			RunStop		: 1,
		// 			HCRST		: 1,
		// 			INTE		: 1,
		// 			HSEE		: 1,
		// 			RsvdP1		: 3,
		// 			LHCRST		: 1,
		// 			CSS			: 1,
		// 			CRS			: 1,
		// 			EWE			: 1,
		// 			EU3S		: 1,
		// 			RsvdP2		: 1,
		// 			CME			: 1,
		// 			ETE			: 1,
		// 			TSC_EN		: 1,
		// 			VTIOE		: 1,
		// 			RsvdP3		: 15;
		// 	} def;
		// } USBCMD;

		u32		USBCMD;

		// union
		// {
		// 	u32		val;
		// 	struct
		// 	{
		// 		u32
		// 			HCH			: 1,
		// 			RsvdZ1		: 1,
		// 			HSE			: 1,
		// 			EINT		: 1,
		// 			PCD			: 1,
		// 			RsvdZ2		: 3,
		// 			SSS			: 1,
		// 			RSS			: 1,
		// 			SRE			: 1,
		// 			CNR			: 1,
		// 			HCE			: 1,
		// 			RsvdZ3		: 19;
		// 	} def;
		// } USBSTS;

		u32		USBSTS;
		u32		PAGESIZE;
		u32		RsvdZ1[2];
		u32		DNCTRL;

		// union
		// {
		// 	u64		val;
		// 	struct
		// 	{
		// 		u64
		// 			RCS			: 1,
		// 			CS			: 1,
		// 			CA			: 1,
		// 			CRR			: 1,
		// 			RsvdP		: 2,
		// 			CmdRing_Ptr	: 58;
		// 	} def;
		// } CRCR;

		u64		CRCR;
		u32		RsvdZ2[4];
		u64		DCBAAP;

		// union
		// {
		// 	u32		val;
		// 	struct
		// 	{
		// 		u32
		// 			MaxSlotsEn	: 8,
		// 			U3E			: 1,
		// 			CIE			: 1,
		// 			RsvdP		: 22;
		// 	} def;
		// } CONFIG;

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