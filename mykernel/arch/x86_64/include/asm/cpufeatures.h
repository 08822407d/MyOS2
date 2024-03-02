// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CPUFEATURES_H
#define _ASM_X86_CPUFEATURES_H

	enum cpuid_leafs
	{
		CPUID_1_EDX		= 0,
		CPUID_8000_0001_EDX,
		CPUID_8086_0001_EDX,
		CPUID_LNX_1,
		CPUID_1_ECX,
		CPUID_C000_0001_EDX,
		CPUID_8000_0001_ECX,
		CPUID_LNX_2,
		CPUID_LNX_3,
		CPUID_7_0_EBX,
		CPUID_D_1_EAX,
		CPUID_LNX_4,
		CPUID_7_1_EAX,
		CPUID_8000_0008_EBX,
		CPUID_6_EAX,
		CPUID_8000_000A_EDX,
		CPUID_7_ECX,
		CPUID_8000_0007_EBX,
		CPUID_7_EDX,
		CPUID_8000_001F_EAX,
	};

	/*
	 * According to Intel®64 and IA-32 Manual - June 2023
	 * Volume 2, Chapter 3, 3.3 Instructions - CPUID
	 */
	struct x86_cap_bits {
		// CPUID_1_EDX - 1_0H:EDX
		struct {
			bool
				X87FPU_On_Chip			: 1,
				VME						: 1,	/* Virtual 8086 Mode Enhancements */
				Debugging_Ext			: 1,
				PageSize_Ext			: 1,
				TSC_Ins					: 1,
				MSR_Ins					: 1,
				PhysAddr_Ext			: 1,
				MCE						: 1,	/* Machine Check Exception */
				CX8_Ins					: 1,	/* CMPXCHG8B Instruction */
				APIC_On_Chip			: 1,
				SYSENTER_SYSEXIT_Ins	: 1,
				MTRR					: 1,	/* Memory Type Range Registers */
				PGE						: 1,	/* Page Global Bit */
				MCA						: 1,	/* Machine Check Architecture */
				CMOV_Ins				: 1,	/* Conditional Move Instructions */
				PAT						: 1,	/* Page Attribute Table */
				PSE_36					: 1,	/* 36-Bit Page Size Extension */
				PSN						: 1,	/* Processor Serial Number */
				CLFSH_Ins				: 1,	/* CLFLUSH Instruction */
										: 1,
				Debug_Store				: 1,
				ACPI					: 1,	/* Thermal Monitor and Software
													Controlled Clock Facilities */
				MMX						: 1,
				FXSR_Ins				: 1,
				SSE						: 1,
				SSE2					: 1,
				Self_Snoop				: 1,
				HTT						: 1,	/* Max APIC IDs reserved field is Valid */
				Thermal_Monitor			: 1,
										: 1,
				PBE						: 1;	/* Pending Break Enable */
		} __attribute__((packed));


		// CPUID_8000_0001_EDX - 8000_0001H:EDX
		struct {
			__u32						: 11;
			bool
				SYSCALL_SYSRET_Ins		: 1;
			__u32						: 8;
			bool
				Exec_Disable			: 1;	/* Execute Disable Bit available */
			__u32						: 5;
			bool
				Page_1GB				: 1,	/* 1-GByte page available */
				RDTSCP					: 1,	/* RDTSCP and IA32_TSC_AUX available */
										: 1,
				Intel64					: 1;
			__u32						: 2;
		} __attribute__((packed));

		// CPUID_8086_0001_EDX
		__u32 CPUID_8086_0001_EDX;
		// CPUID_LNX_1
		__u32 CPUID_LNX_1;

		// CPUID_1_ECX - 1_0H:ECX
		struct {
			bool
				SSE3					: 1,
				PCLMULQDQ_Ins			: 1,
				DTES64					: 1,	/* 64-bit DS Area */
				MONITOR_MWAIT			: 1,
				DS_CPL					: 1,	/* CPL Qualified Debug Store */
				VMX						: 1,	/* Virtual Machine Extensions */
				SMX						: 1,	/* Safer Mode Extensions */
				EIST					: 1,	/* Enhanced Intel SpeedStep® technology */
				Thermal_Monitor_2		: 1,
				SSSE3					: 1,
				CNXT_ID					: 1,	/* L1 Context ID */
				SDBG					: 1,	/* IA32_DEBUG_INTERFACE MSR */
				FMA						: 1,
				CX16_Ins				: 1,	/* CMPXCHG16B Instruction */
				xTPR_Update_Control		: 1,
				PDCM					: 1,	/* Perfmon and Debug Capability */
										: 1,
				PCID					: 1,	/* Process-context identifiers */
				DCA						: 1,
				SSE4_1					: 1,
				SSE4_2					: 1,
				x2APIC					: 1,
				MOVBE_Ins				: 1,
				POPCNT_Ins				: 1,
				TSC_Deadline			: 1,
				AESNI_Ext				: 1,
				XSAVE_Ins				: 1,
				OSXSAVE_Ins				: 1,
				AVX_Ext					: 1,
				F16C_Ins				: 1,
				RDRAND_Ins				: 1,
										: 1;
		} __attribute__((packed));

		// CPUID_C000_0001_EDX - C000_0001H:EDX
		__u32 CPUID_C000_0001_EDX;

		// CPUID_8000_0001_ECX
		struct {
			bool
				LAHF_SAHF				: 1;
			__u32						: 4;
			bool
				LZCNT					: 1;
			__u32						: 2;
			bool
				PREFETCHW				: 1;
			__u32						: 23;
		} __attribute__((packed));

		// CPUID_LNX_2
		__u32 CPUID_LNX_2;
		// CPUID_LNX_3
		__u32 CPUID_LNX_3;

		// CPUID_7_0_EBX - 7_0H:EBX
		struct {
			bool
				FSGSBASE				: 1,
				TSC_ADJUST_MSR			: 1,
				SGX						: 1,	/* Intel® Software Guard Extensions */
				BMI1					: 1,
				HLE						: 1,
				AVX2					: 1,
				FDP_EXCPTN_ONLY			: 1,	/* x87 FPU Data Pointer updated only on x87 exceptions */
				SMEP					: 1,	/* Supports Supervisor-Mode Execution Prevention */
				BMI2					: 1,
				Enhanced_REPMovSto		: 1,	/* Enhanced REP MOVSB/STOSB */
				INVPCID_Ins				: 1,
				RTM						: 1,
				RTD_M					: 1,	/* Intel® Resource Director Technology (Intel® RDT) */
				Deprecates_FPU_CS_DS	: 1,
				MPX_Ext					: 1,	/* Intel® Memory Protection Extensions */
				RDT_A					: 1,	/* Intel® RDT Allocation capability */
				AVX512F					: 1,
				AVX512DQ				: 1,
				RDSEED					: 1,
				ADX						: 1,
				SMAP					: 1,	/* Supervisor-Mode Access Prevention */
				AVX512_IFMA				: 1,
										: 1,
				CLFLUSHOPT				: 1,
				CLWB					: 1,
				Intel_Processor_Trace	: 1,
				AVX512PF				: 1,
				AVX512ER				: 1,
				AVX512CD				: 1,
				SHA						: 1,	/* Intel® Secure Hash Algorithm Extensions */
				AVX512BW				: 1,
				AVX512VL				: 1;
		} __attribute__((packed));
		

		// CPUID_D_1_EAX - D_1H:EAX
		struct {
			bool
				X87_State				: 1,
				SSE_State				: 1,
				AVX_State				: 1;
			__u32
				MPX_State				: 2,
				AVX_512_State			: 3;
			bool
				IA32_XSS_Usage1			: 1,
				PKRU_State				: 1;
			__u32
				IA32_XSS_Usage2			: 7;
			bool
				TILECFG_State			: 1,
				TILEDATA_State			: 1;
			__u32						: 13;
		} __attribute__((packed));

		// CPUID_LNX_4
		__u32 CPUID_LNX_4;

		// CPUID_7_1_EAX - 7_1H:EAX
		struct {
			__u32						: 4;
			bool
				AVX_VNNI				: 1,	/* versions of the Vector Neural Network Instructions */
				AVX512_BF16				: 1;	/* VNNI supporting BFLOAT16 inputs */
			__u32						: 4;
			bool
				Fast_0Len_REP_M			: 1,	/* zero-length REP MOVSB */
				Fast_Short_REP_S		: 1,	/* fast short REP STOSB */
				Fast_Short_REP_C_S		: 1;	/* fast short REP CMPSB, REP SCASB */
			__u32						: 9;
			bool
				HRESET					: 1;	/* history reset via the HRESET instruction and
													the IA32_HRESET_ENABLE MSR */
			__u32						: 9;
		} __attribute__((packed));
		

		// CPUID_8000_0008_EBX - 8000_0008H:EBX
		struct {
			__u32						: 9;
			bool
				WBNOINVD				: 1;
			__u32						: 22;
		} __attribute__((packed));

		// CPUID_6_EAX - 6_0H:EAX
		struct {
			bool
				Digital_Temp_Sensor		: 1,
				Intel_Turbo_Boost		: 1,
				ARAT					: 1,	/* APIC-Timer-always-running feature */
										: 1,
				PLN						: 1,	/* Power limit notification controls */
				ECMD					: 1,	/* Clock modulation duty cycle extension */
				PTM						: 1,	/* Package thermal management */
				HWP_Base_Reg			: 1,
				HWP_Notification		: 1,
				HWP_Activity_Window		: 1,
				HWP_EPP					: 1,	/* HWP_Energy_Performance_Preference */
				HWP_PkgLv_Req			: 1,	/* IA32_HWP_REQUEST_PKG MSR */
										: 1,
				HDC						: 1,	/* HDC base registers */
				Turbo_Boost_MT3			: 1,	/* Intel® Turbo Boost Max Technology 3.0 */
				HWP_Capabilities		: 1,	/* Highest Performance change */
				HWP_PECI_Override		: 1,
				Flexible_HWP			: 1,
				Fast_HWP_REQ_MSR		: 1,	/* Fast access mode for the IA32_HWP_REQUEST MSR */
				HW_FEEDBACK				: 1,
				Ignoring_ILP_HWP_Req	: 1;	/* Ignoring Idle Logical Processor HWP request */
			__u32						: 2;
			bool
				Thread_Director			: 1,	/* Intel® Thread Director */
				IA32_THERM_INTR_MSRb25	: 1;	/* IA32_THERM_INTERRUPT MSR bit 25 */
			__u32						: 7;
		} __attribute__((packed));

		// CPUID_8000_000A_EDX - 8000_000AH:EDX
		__u32 CPUID_8000_000A_EDX;

		// CPUID_7_ECX - 7_0H:ECX
		struct {
			bool
				PREFETCHWT1				: 1,
				AVX512_VBMI				: 1,
				UMIP					: 1,	/* User-mode instruction prevention */
				PKU						: 1,	/* Protection keys for user-mode pages */
				OSPKE					: 1,	/* OS has set CR4.PKE to enable protection keys */
				WAITPKG					: 1,
				AVX512_VBMI2			: 1,
				CET_SS					: 1,	/* CET shadow stack features */
				GFNI					: 1,
				VAES					: 1,
				VPCLMULQDQ				: 1,
				AVX512_VNNI				: 1,
				AVX512_BITALG			: 1,
				TME_EN					: 1,	/* Enable IA32_TME_CAPABILITY, IA32_TME_ACTIVATE,
													IA32_TME_EXCLUDE_MASK, and IA32_TME_EXCLUDE_BASE */
				AVX512_VPOPCNTDQ		: 1,
										: 1,
				LA57					: 1;	/* 57-bit linear addresses and five-level paging */ 
			__u32						: 5;
			bool
				RDPID_TSC_AUX			: 1,
				Key_Locker				: 1,
				BUS_LOCK_DETECT			: 1,
				CLDEMOTE				: 1,	/* cache line demote */
										: 1,
				MOVDIRI					: 1,
				MOVDIR64B				: 1,
				ENQCMD					: 1,	/* Enqueue Stores */
				SGX_LC					: 1,	/* SGX Launch Configuration */
				PKS						: 1;	/* Protection keys for supervisor-mode pages */
		} __attribute__((packed));

		// CPUID_8000_0007_EBX - 8000_0007H:EBX
		__u32 CPUID_8000_0007_EBX;

		// CPUID_7_EDX - 7_0H:EDX
		struct {
			bool						: 1,
				SGX_KEYS				: 1,	/* Attestation Services for Intel® SGX */
				AVX512_4VNNIW			: 1,
				AVX512_4FMAPS			: 1,
				Fast_Short_REP_MOV		: 1,
				UINTR					: 1;	/* User interrupts */
			__u16						: 2;
			bool
				AVX512_VP2INTERSECT		: 1,
				SRBDS_CTRL				: 1,
				MD_CLEAR				: 1,
				RTM_ALWAYS_ABORT		: 1,
										: 1,
				RTM_FORCE_ABORT			: 1,
				SERIALIZE				: 1,
				Hybrid_Part				: 1,
				TSXLDTRK				: 1,
										: 1,
				PCONFIG					: 1,
				Architectural_LBRs		: 1,
				CET_IBT					: 1,	/* CET indirect branch tracking features */
										: 1,
				AMX_BF16				: 1,	/* Tile computational operations on bfloat16 numbers */
				AVX512_FP16				: 1,
				AMX_TILE				: 1,	/* Tile architecture */
				AMX_INT8				: 1,	/* Tile computational operations on 8-bit integers */
				IBRS_IBPB				: 1,	/* Indirect Branch Restricted Speculation /
													Indirect Branch Predictor Barrier */
				STIBP					: 1,	/* Single Thread Indirect Branch Predictors */
				L1D_FLUSH				: 1,
				ARCH_CAPABILITIES_MSR	: 1,
				CORE_CAPABILITIES_MSR	: 1,
				SSBD					: 1;	/* Speculative Store Bypass Disable */
		}  __attribute__((packed));

		// CPUID_8000_001F_EAX
		__u32 CPUID_8000_001F_EAX;
	};


	/*
	 * Defines x86 CPU feature bits
	 */
	#define NCAPINTS			21	   /* N 32-bit words worth of info */
	#define NBUGINTS			1	   /* N 32-bit bug flags */

#endif /* _ASM_X86_CPUFEATURES_H */
