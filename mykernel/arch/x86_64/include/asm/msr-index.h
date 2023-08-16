// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MSR_INDEX_H
#define _ASM_X86_MSR_INDEX_H

	// #include <linux/bits.h>

	/*
	 * CPU model specific register (MSR) numbers.
	 *
	 * Do not add new entries to this file unless the definitions are shared
	 * between multiple compilation units.
	 */

	/* x86-64 specific MSRs */
	#define MSR_EFER			0xc0000080 /* extended feature register */
	#define MSR_STAR			0xc0000081 /* legacy mode SYSCALL target */
	#define MSR_LSTAR			0xc0000082 /* long mode SYSCALL target */
	#define MSR_CSTAR			0xc0000083 /* compat mode SYSCALL target */
	#define MSR_SYSCALL_MASK	0xc0000084 /* EFLAGS mask for syscall */
	#define MSR_FS_BASE			0xc0000100 /* 64bit FS base */
	#define MSR_GS_BASE			0xc0000101 /* 64bit GS base */
	#define MSR_KERNEL_GS_BASE	0xc0000102 /* SwapGS GS shadow */
	#define MSR_TSC_AUX			0xc0000103 /* Auxiliary TSC */

	/* EFER bits: */
	#define _EFER_SCE			0  /* SYSCALL/SYSRET */
	#define _EFER_LME			8  /* Long mode enable */
	#define _EFER_LMA			10 /* Long mode active (read-only) */
	#define _EFER_NX			11 /* No execute enable */
	#define _EFER_SVME			12 /* Enable virtualization */
	#define _EFER_LMSLE			13 /* Long Mode Segment Limit Enable */
	#define _EFER_FFXSR			14 /* Enable Fast FXSAVE/FXRSTOR */
	#define _EFER_AUTOIBRS		21 /* Enable Automatic IBRS */

	#define EFER_SCE			(1<<_EFER_SCE)
	#define EFER_LME			(1<<_EFER_LME)
	#define EFER_LMA			(1<<_EFER_LMA)
	#define EFER_NX				(1<<_EFER_NX)
	#define EFER_SVME			(1<<_EFER_SVME)
	#define EFER_LMSLE			(1<<_EFER_LMSLE)
	#define EFER_FFXSR			(1<<_EFER_FFXSR)
	#define EFER_AUTOIBRS		(1<<_EFER_AUTOIBRS)

	// /* Intel MSRs. Some also available on other CPUs */

	// #define MSR_TEST_CTRL				0x00000033
	// #define MSR_TEST_CTRL_SPLIT_LOCK_DETECT_BIT	29
	// #define MSR_TEST_CTRL_SPLIT_LOCK_DETECT		BIT(MSR_TEST_CTRL_SPLIT_LOCK_DETECT_BIT)

	// #define MSR_IA32_SPEC_CTRL		0x00000048 /* Speculation Control */
	// #define SPEC_CTRL_IBRS			BIT(0)	   /* Indirect Branch Restricted Speculation */
	// #define SPEC_CTRL_STIBP_SHIFT		1	   /* Single Thread Indirect Branch Predictor (STIBP) bit */
	// #define SPEC_CTRL_STIBP			BIT(SPEC_CTRL_STIBP_SHIFT)	/* STIBP mask */
	// #define SPEC_CTRL_SSBD_SHIFT		2	   /* Speculative Store Bypass Disable bit */
	// #define SPEC_CTRL_SSBD			BIT(SPEC_CTRL_SSBD_SHIFT)	/* Speculative Store Bypass Disable */
	// #define SPEC_CTRL_RRSBA_DIS_S_SHIFT	6	   /* Disable RRSBA behavior */
	// #define SPEC_CTRL_RRSBA_DIS_S		BIT(SPEC_CTRL_RRSBA_DIS_S_SHIFT)

	// /* A mask for bits which the kernel toggles when controlling mitigations */
	// #define SPEC_CTRL_MITIGATIONS_MASK	(SPEC_CTRL_IBRS | SPEC_CTRL_STIBP | SPEC_CTRL_SSBD \
	// 							| SPEC_CTRL_RRSBA_DIS_S)

	// #define MSR_IA32_PRED_CMD		0x00000049 /* Prediction Command */
	// #define PRED_CMD_IBPB			BIT(0)	   /* Indirect Branch Prediction Barrier */

	// #define MSR_PPIN_CTL			0x0000004e
	// #define MSR_PPIN			0x0000004f

	// #define MSR_IA32_PERFCTR0		0x000000c1
	// #define MSR_IA32_PERFCTR1		0x000000c2
	// #define MSR_FSB_FREQ			0x000000cd
	// #define MSR_PLATFORM_INFO		0x000000ce
	// #define MSR_PLATFORM_INFO_CPUID_FAULT_BIT	31
	// #define MSR_PLATFORM_INFO_CPUID_FAULT		BIT_ULL(MSR_PLATFORM_INFO_CPUID_FAULT_BIT)

	// #define MSR_IA32_UMWAIT_CONTROL			0xe1
	// #define MSR_IA32_UMWAIT_CONTROL_C02_DISABLE	BIT(0)
	// #define MSR_IA32_UMWAIT_CONTROL_RESERVED	BIT(1)
	// /*
	// * The time field is bit[31:2], but representing a 32bit value with
	// * bit[1:0] zero.
	// */
	// #define MSR_IA32_UMWAIT_CONTROL_TIME_MASK	(~0x03U)

	// /* Abbreviated from Intel SDM name IA32_CORE_CAPABILITIES */
	// #define MSR_IA32_CORE_CAPS			  0x000000cf
	// #define MSR_IA32_CORE_CAPS_INTEGRITY_CAPS_BIT	  2
	// #define MSR_IA32_CORE_CAPS_INTEGRITY_CAPS	  BIT(MSR_IA32_CORE_CAPS_INTEGRITY_CAPS_BIT)
	// #define MSR_IA32_CORE_CAPS_SPLIT_LOCK_DETECT_BIT  5
	// #define MSR_IA32_CORE_CAPS_SPLIT_LOCK_DETECT	  BIT(MSR_IA32_CORE_CAPS_SPLIT_LOCK_DETECT_BIT)

	// #define MSR_PKG_CST_CONFIG_CONTROL	0x000000e2
	// #define NHM_C3_AUTO_DEMOTE		(1UL << 25)
	// #define NHM_C1_AUTO_DEMOTE		(1UL << 26)
	// #define ATM_LNC_C6_AUTO_DEMOTE		(1UL << 25)
	// #define SNB_C3_AUTO_UNDEMOTE		(1UL << 27)
	// #define SNB_C1_AUTO_UNDEMOTE		(1UL << 28)

	// #define MSR_MTRRcap			0x000000fe

	// #define MSR_IA32_ARCH_CAPABILITIES	0x0000010a
	// #define ARCH_CAP_RDCL_NO		BIT(0)	/* Not susceptible to Meltdown */
	// #define ARCH_CAP_IBRS_ALL		BIT(1)	/* Enhanced IBRS support */
	// #define ARCH_CAP_RSBA			BIT(2)	/* RET may use alternative branch predictors */
	// #define ARCH_CAP_SKIP_VMENTRY_L1DFLUSH	BIT(3)	/* Skip L1D flush on vmentry */
	// #define ARCH_CAP_SSB_NO			BIT(4)	/*
	// 						* Not susceptible to Speculative Store Bypass
	// 						* attack, so no Speculative Store Bypass
	// 						* control required.
	// 						*/
	// #define ARCH_CAP_MDS_NO			BIT(5)   /*
	// 						* Not susceptible to
	// 						* Microarchitectural Data
	// 						* Sampling (MDS) vulnerabilities.
	// 						*/
	// #define ARCH_CAP_PSCHANGE_MC_NO		BIT(6)	 /*
	// 						* The processor is not susceptible to a
	// 						* machine check error due to modifying the
	// 						* code page size along with either the
	// 						* physical address or cache type
	// 						* without TLB invalidation.
	// 						*/
	// #define ARCH_CAP_TSX_CTRL_MSR		BIT(7)	/* MSR for TSX control is available. */
	// #define ARCH_CAP_TAA_NO			BIT(8)	/*
	// 						* Not susceptible to
	// 						* TSX Async Abort (TAA) vulnerabilities.
	// 						*/
	// #define ARCH_CAP_SBDR_SSDP_NO		BIT(13)	/*
	// 						* Not susceptible to SBDR and SSDP
	// 						* variants of Processor MMIO stale data
	// 						* vulnerabilities.
	// 						*/
	// #define ARCH_CAP_FBSDP_NO		BIT(14)	/*
	// 						* Not susceptible to FBSDP variant of
	// 						* Processor MMIO stale data
	// 						* vulnerabilities.
	// 						*/
	// #define ARCH_CAP_PSDP_NO		BIT(15)	/*
	// 						* Not susceptible to PSDP variant of
	// 						* Processor MMIO stale data
	// 						* vulnerabilities.
	// 						*/
	// #define ARCH_CAP_FB_CLEAR		BIT(17)	/*
	// 						* VERW clears CPU fill buffer
	// 						* even on MDS_NO CPUs.
	// 						*/
	// #define ARCH_CAP_FB_CLEAR_CTRL		BIT(18)	/*
	// 						* MSR_IA32_MCU_OPT_CTRL[FB_CLEAR_DIS]
	// 						* bit available to control VERW
	// 						* behavior.
	// 						*/
	// #define ARCH_CAP_RRSBA			BIT(19)	/*
	// 						* Indicates RET may use predictors
	// 						* other than the RSB. With eIBRS
	// 						* enabled predictions in kernel mode
	// 						* are restricted to targets in
	// 						* kernel.
	// 						*/
	// #define ARCH_CAP_PBRSB_NO		BIT(24)	/*
	// 						* Not susceptible to Post-Barrier
	// 						* Return Stack Buffer Predictions.
	// 						*/

	// #define ARCH_CAP_XAPIC_DISABLE		BIT(21)	/*
	// 						* IA32_XAPIC_DISABLE_STATUS MSR
	// 						* supported
	// 						*/

	// #define MSR_IA32_FLUSH_CMD		0x0000010b
	// #define L1D_FLUSH			BIT(0)	/*
	// 						* Writeback and invalidate the
	// 						* L1 data cache.
	// 						*/

	// #define MSR_IA32_BBL_CR_CTL		0x00000119
	// #define MSR_IA32_BBL_CR_CTL3		0x0000011e

	// #define MSR_IA32_TSX_CTRL		0x00000122
	// #define TSX_CTRL_RTM_DISABLE		BIT(0)	/* Disable RTM feature */
	// #define TSX_CTRL_CPUID_CLEAR		BIT(1)	/* Disable TSX enumeration */

	// #define MSR_IA32_MCU_OPT_CTRL		0x00000123
	// #define RNGDS_MITG_DIS			BIT(0)	/* SRBDS support */
	// #define RTM_ALLOW			BIT(1)	/* TSX development mode */
	// #define FB_CLEAR_DIS			BIT(3)	/* CPU Fill buffer clear disable */

	#define MSR_IA32_SYSENTER_CS		0x00000174
	#define MSR_IA32_SYSENTER_ESP		0x00000175
	#define MSR_IA32_SYSENTER_EIP		0x00000176

	// #define MSR_IA32_MCG_CAP		0x00000179
	// #define MSR_IA32_MCG_STATUS		0x0000017a
	// #define MSR_IA32_MCG_CTL		0x0000017b
	// #define MSR_ERROR_CONTROL		0x0000017f
	// #define MSR_IA32_MCG_EXT_CTL		0x000004d0

	// #define MSR_OFFCORE_RSP_0		0x000001a6
	// #define MSR_OFFCORE_RSP_1		0x000001a7
	// #define MSR_TURBO_RATIO_LIMIT		0x000001ad
	// #define MSR_TURBO_RATIO_LIMIT1		0x000001ae
	// #define MSR_TURBO_RATIO_LIMIT2		0x000001af

	// #define MSR_SNOOP_RSP_0			0x00001328
	// #define MSR_SNOOP_RSP_1			0x00001329

	// #define MSR_LBR_SELECT			0x000001c8
	// #define MSR_LBR_TOS			0x000001c9

	// #define MSR_IA32_POWER_CTL		0x000001fc
	// #define MSR_IA32_POWER_CTL_BIT_EE	19

	// /* Abbreviated from Intel SDM name IA32_INTEGRITY_CAPABILITIES */
	// #define MSR_INTEGRITY_CAPS			0x000002d9
	// #define MSR_INTEGRITY_CAPS_ARRAY_BIST_BIT      2
	// #define MSR_INTEGRITY_CAPS_ARRAY_BIST          BIT(MSR_INTEGRITY_CAPS_ARRAY_BIST_BIT)
	// #define MSR_INTEGRITY_CAPS_PERIODIC_BIST_BIT	4
	// #define MSR_INTEGRITY_CAPS_PERIODIC_BIST	BIT(MSR_INTEGRITY_CAPS_PERIODIC_BIST_BIT)

	// #define MSR_LBR_NHM_FROM		0x00000680
	// #define MSR_LBR_NHM_TO			0x000006c0
	// #define MSR_LBR_CORE_FROM		0x00000040
	// #define MSR_LBR_CORE_TO			0x00000060

	// #define MSR_LBR_INFO_0			0x00000dc0 /* ... 0xddf for _31 */
	// #define LBR_INFO_MISPRED		BIT_ULL(63)
	// #define LBR_INFO_IN_TX			BIT_ULL(62)
	// #define LBR_INFO_ABORT			BIT_ULL(61)
	// #define LBR_INFO_CYC_CNT_VALID		BIT_ULL(60)
	// #define LBR_INFO_CYCLES			0xffff
	// #define LBR_INFO_BR_TYPE_OFFSET		56
	// #define LBR_INFO_BR_TYPE		(0xfull << LBR_INFO_BR_TYPE_OFFSET)

	// #define MSR_ARCH_LBR_CTL		0x000014ce
	// #define ARCH_LBR_CTL_LBREN		BIT(0)
	// #define ARCH_LBR_CTL_CPL_OFFSET		1
	// #define ARCH_LBR_CTL_CPL		(0x3ull << ARCH_LBR_CTL_CPL_OFFSET)
	// #define ARCH_LBR_CTL_STACK_OFFSET	3
	// #define ARCH_LBR_CTL_STACK		(0x1ull << ARCH_LBR_CTL_STACK_OFFSET)
	// #define ARCH_LBR_CTL_FILTER_OFFSET	16
	// #define ARCH_LBR_CTL_FILTER		(0x7full << ARCH_LBR_CTL_FILTER_OFFSET)
	// #define MSR_ARCH_LBR_DEPTH		0x000014cf
	// #define MSR_ARCH_LBR_FROM_0		0x00001500
	// #define MSR_ARCH_LBR_TO_0		0x00001600
	// #define MSR_ARCH_LBR_INFO_0		0x00001200

	// #define MSR_IA32_PEBS_ENABLE		0x000003f1
	// #define MSR_PEBS_DATA_CFG		0x000003f2
	// #define MSR_IA32_DS_AREA		0x00000600
	// #define MSR_IA32_PERF_CAPABILITIES	0x00000345
	// #define PERF_CAP_METRICS_IDX		15
	// #define PERF_CAP_PT_IDX			16

	// #define MSR_PEBS_LD_LAT_THRESHOLD	0x000003f6
	// #define PERF_CAP_PEBS_TRAP             BIT_ULL(6)
	// #define PERF_CAP_ARCH_REG              BIT_ULL(7)
	// #define PERF_CAP_PEBS_FORMAT           0xf00
	// #define PERF_CAP_PEBS_BASELINE         BIT_ULL(14)
	// #define PERF_CAP_PEBS_MASK	(PERF_CAP_PEBS_TRAP | PERF_CAP_ARCH_REG | \
	// 				PERF_CAP_PEBS_FORMAT | PERF_CAP_PEBS_BASELINE)

	// #define MSR_IA32_RTIT_CTL		0x00000570
	// #define RTIT_CTL_TRACEEN		BIT(0)
	// #define RTIT_CTL_CYCLEACC		BIT(1)
	// #define RTIT_CTL_OS			BIT(2)
	// #define RTIT_CTL_USR			BIT(3)
	// #define RTIT_CTL_PWR_EVT_EN		BIT(4)
	// #define RTIT_CTL_FUP_ON_PTW		BIT(5)
	// #define RTIT_CTL_FABRIC_EN		BIT(6)
	// #define RTIT_CTL_CR3EN			BIT(7)
	// #define RTIT_CTL_TOPA			BIT(8)
	// #define RTIT_CTL_MTC_EN			BIT(9)
	// #define RTIT_CTL_TSC_EN			BIT(10)
	// #define RTIT_CTL_DISRETC		BIT(11)
	// #define RTIT_CTL_PTW_EN			BIT(12)
	// #define RTIT_CTL_BRANCH_EN		BIT(13)
	// #define RTIT_CTL_EVENT_EN		BIT(31)
	// #define RTIT_CTL_NOTNT			BIT_ULL(55)
	// #define RTIT_CTL_MTC_RANGE_OFFSET	14
	// #define RTIT_CTL_MTC_RANGE		(0x0full << RTIT_CTL_MTC_RANGE_OFFSET)
	// #define RTIT_CTL_CYC_THRESH_OFFSET	19
	// #define RTIT_CTL_CYC_THRESH		(0x0full << RTIT_CTL_CYC_THRESH_OFFSET)
	// #define RTIT_CTL_PSB_FREQ_OFFSET	24
	// #define RTIT_CTL_PSB_FREQ		(0x0full << RTIT_CTL_PSB_FREQ_OFFSET)
	// #define RTIT_CTL_ADDR0_OFFSET		32
	// #define RTIT_CTL_ADDR0			(0x0full << RTIT_CTL_ADDR0_OFFSET)
	// #define RTIT_CTL_ADDR1_OFFSET		36
	// #define RTIT_CTL_ADDR1			(0x0full << RTIT_CTL_ADDR1_OFFSET)
	// #define RTIT_CTL_ADDR2_OFFSET		40
	// #define RTIT_CTL_ADDR2			(0x0full << RTIT_CTL_ADDR2_OFFSET)
	// #define RTIT_CTL_ADDR3_OFFSET		44
	// #define RTIT_CTL_ADDR3			(0x0full << RTIT_CTL_ADDR3_OFFSET)
	// #define MSR_IA32_RTIT_STATUS		0x00000571
	// #define RTIT_STATUS_FILTEREN		BIT(0)
	// #define RTIT_STATUS_CONTEXTEN		BIT(1)
	// #define RTIT_STATUS_TRIGGEREN		BIT(2)
	// #define RTIT_STATUS_BUFFOVF		BIT(3)
	// #define RTIT_STATUS_ERROR		BIT(4)
	// #define RTIT_STATUS_STOPPED		BIT(5)
	// #define RTIT_STATUS_BYTECNT_OFFSET	32
	// #define RTIT_STATUS_BYTECNT		(0x1ffffull << RTIT_STATUS_BYTECNT_OFFSET)
	// #define MSR_IA32_RTIT_ADDR0_A		0x00000580
	// #define MSR_IA32_RTIT_ADDR0_B		0x00000581
	// #define MSR_IA32_RTIT_ADDR1_A		0x00000582
	// #define MSR_IA32_RTIT_ADDR1_B		0x00000583
	// #define MSR_IA32_RTIT_ADDR2_A		0x00000584
	// #define MSR_IA32_RTIT_ADDR2_B		0x00000585
	// #define MSR_IA32_RTIT_ADDR3_A		0x00000586
	// #define MSR_IA32_RTIT_ADDR3_B		0x00000587
	// #define MSR_IA32_RTIT_CR3_MATCH		0x00000572
	// #define MSR_IA32_RTIT_OUTPUT_BASE	0x00000560
	// #define MSR_IA32_RTIT_OUTPUT_MASK	0x00000561

	// #define MSR_MTRRfix64K_00000		0x00000250
	// #define MSR_MTRRfix16K_80000		0x00000258
	// #define MSR_MTRRfix16K_A0000		0x00000259
	// #define MSR_MTRRfix4K_C0000		0x00000268
	// #define MSR_MTRRfix4K_C8000		0x00000269
	// #define MSR_MTRRfix4K_D0000		0x0000026a
	// #define MSR_MTRRfix4K_D8000		0x0000026b
	// #define MSR_MTRRfix4K_E0000		0x0000026c
	// #define MSR_MTRRfix4K_E8000		0x0000026d
	// #define MSR_MTRRfix4K_F0000		0x0000026e
	// #define MSR_MTRRfix4K_F8000		0x0000026f
	// #define MSR_MTRRdefType			0x000002ff

	// #define MSR_IA32_CR_PAT			0x00000277

	// #define MSR_IA32_DEBUGCTLMSR		0x000001d9
	// #define MSR_IA32_LASTBRANCHFROMIP	0x000001db
	// #define MSR_IA32_LASTBRANCHTOIP		0x000001dc
	// #define MSR_IA32_LASTINTFROMIP		0x000001dd
	// #define MSR_IA32_LASTINTTOIP		0x000001de

	// #define MSR_IA32_PASID			0x00000d93
	// #define MSR_IA32_PASID_VALID		BIT_ULL(31)

	// /* DEBUGCTLMSR bits (others vary by model): */
	// #define DEBUGCTLMSR_LBR			(1UL <<  0) /* last branch recording */
	// #define DEBUGCTLMSR_BTF_SHIFT		1
	// #define DEBUGCTLMSR_BTF			(1UL <<  1) /* single-step on branches */
	// #define DEBUGCTLMSR_BUS_LOCK_DETECT	(1UL <<  2)
	// #define DEBUGCTLMSR_TR			(1UL <<  6)
	// #define DEBUGCTLMSR_BTS			(1UL <<  7)
	// #define DEBUGCTLMSR_BTINT		(1UL <<  8)
	// #define DEBUGCTLMSR_BTS_OFF_OS		(1UL <<  9)
	// #define DEBUGCTLMSR_BTS_OFF_USR		(1UL << 10)
	// #define DEBUGCTLMSR_FREEZE_LBRS_ON_PMI	(1UL << 11)
	// #define DEBUGCTLMSR_FREEZE_PERFMON_ON_PMI	(1UL << 12)
	// #define DEBUGCTLMSR_FREEZE_IN_SMM_BIT	14
	// #define DEBUGCTLMSR_FREEZE_IN_SMM	(1UL << DEBUGCTLMSR_FREEZE_IN_SMM_BIT)

	// #define MSR_PEBS_FRONTEND		0x000003f7

	// #define MSR_IA32_MC0_CTL		0x00000400
	// #define MSR_IA32_MC0_STATUS		0x00000401
	// #define MSR_IA32_MC0_ADDR		0x00000402
	// #define MSR_IA32_MC0_MISC		0x00000403

	// /* C-state Residency Counters */
	// #define MSR_PKG_C3_RESIDENCY		0x000003f8
	// #define MSR_PKG_C6_RESIDENCY		0x000003f9
	// #define MSR_ATOM_PKG_C6_RESIDENCY	0x000003fa
	// #define MSR_PKG_C7_RESIDENCY		0x000003fa
	// #define MSR_CORE_C3_RESIDENCY		0x000003fc
	// #define MSR_CORE_C6_RESIDENCY		0x000003fd
	// #define MSR_CORE_C7_RESIDENCY		0x000003fe
	// #define MSR_KNL_CORE_C6_RESIDENCY	0x000003ff
	// #define MSR_PKG_C2_RESIDENCY		0x0000060d
	// #define MSR_PKG_C8_RESIDENCY		0x00000630
	// #define MSR_PKG_C9_RESIDENCY		0x00000631
	// #define MSR_PKG_C10_RESIDENCY		0x00000632

	// /* Interrupt Response Limit */
	// #define MSR_PKGC3_IRTL			0x0000060a
	// #define MSR_PKGC6_IRTL			0x0000060b
	// #define MSR_PKGC7_IRTL			0x0000060c
	// #define MSR_PKGC8_IRTL			0x00000633
	// #define MSR_PKGC9_IRTL			0x00000634
	// #define MSR_PKGC10_IRTL			0x00000635

	// /* Run Time Average Power Limiting (RAPL) Interface */

	// #define MSR_VR_CURRENT_CONFIG	0x00000601
	// #define MSR_RAPL_POWER_UNIT		0x00000606

	// #define MSR_PKG_POWER_LIMIT		0x00000610
	// #define MSR_PKG_ENERGY_STATUS		0x00000611
	// #define MSR_PKG_PERF_STATUS		0x00000613
	// #define MSR_PKG_POWER_INFO		0x00000614

	// #define MSR_DRAM_POWER_LIMIT		0x00000618
	// #define MSR_DRAM_ENERGY_STATUS		0x00000619
	// #define MSR_DRAM_PERF_STATUS		0x0000061b
	// #define MSR_DRAM_POWER_INFO		0x0000061c

	// #define MSR_PP0_POWER_LIMIT		0x00000638
	// #define MSR_PP0_ENERGY_STATUS		0x00000639
	// #define MSR_PP0_POLICY			0x0000063a
	// #define MSR_PP0_PERF_STATUS		0x0000063b

	// #define MSR_PP1_POWER_LIMIT		0x00000640
	// #define MSR_PP1_ENERGY_STATUS		0x00000641
	// #define MSR_PP1_POLICY			0x00000642

	// #define MSR_AMD_RAPL_POWER_UNIT		0xc0010299
	// #define MSR_AMD_CORE_ENERGY_STATUS		0xc001029a
	// #define MSR_AMD_PKG_ENERGY_STATUS	0xc001029b

	// /* Config TDP MSRs */
	// #define MSR_CONFIG_TDP_NOMINAL		0x00000648
	// #define MSR_CONFIG_TDP_LEVEL_1		0x00000649
	// #define MSR_CONFIG_TDP_LEVEL_2		0x0000064A
	// #define MSR_CONFIG_TDP_CONTROL		0x0000064B
	// #define MSR_TURBO_ACTIVATION_RATIO	0x0000064C

	// #define MSR_PLATFORM_ENERGY_STATUS	0x0000064D
	// #define MSR_SECONDARY_TURBO_RATIO_LIMIT	0x00000650

	// #define MSR_PKG_WEIGHTED_CORE_C0_RES	0x00000658
	// #define MSR_PKG_ANY_CORE_C0_RES		0x00000659
	// #define MSR_PKG_ANY_GFXE_C0_RES		0x0000065A
	// #define MSR_PKG_BOTH_CORE_GFXE_C0_RES	0x0000065B

	// #define MSR_CORE_C1_RES			0x00000660
	// #define MSR_MODULE_C6_RES_MS		0x00000664

	// #define MSR_CC6_DEMOTION_POLICY_CONFIG	0x00000668
	// #define MSR_MC6_DEMOTION_POLICY_CONFIG	0x00000669

	// #define MSR_ATOM_CORE_RATIOS		0x0000066a
	// #define MSR_ATOM_CORE_VIDS		0x0000066b
	// #define MSR_ATOM_CORE_TURBO_RATIOS	0x0000066c
	// #define MSR_ATOM_CORE_TURBO_VIDS	0x0000066d

	// #define MSR_CORE_PERF_LIMIT_REASONS	0x00000690
	// #define MSR_GFX_PERF_LIMIT_REASONS	0x000006B0
	// #define MSR_RING_PERF_LIMIT_REASONS	0x000006B1

	// /* Control-flow Enforcement Technology MSRs */
	// #define MSR_IA32_U_CET			0x000006a0 /* user mode cet */
	// #define MSR_IA32_S_CET			0x000006a2 /* kernel mode cet */
	// #define CET_SHSTK_EN			BIT_ULL(0)
	// #define CET_WRSS_EN			BIT_ULL(1)
	// #define CET_ENDBR_EN			BIT_ULL(2)
	// #define CET_LEG_IW_EN			BIT_ULL(3)
	// #define CET_NO_TRACK_EN			BIT_ULL(4)
	// #define CET_SUPPRESS_DISABLE		BIT_ULL(5)
	// #define CET_RESERVED			(BIT_ULL(6) | BIT_ULL(7) | BIT_ULL(8) | BIT_ULL(9))
	// #define CET_SUPPRESS			BIT_ULL(10)
	// #define CET_WAIT_ENDBR			BIT_ULL(11)

	// #define MSR_IA32_PL0_SSP		0x000006a4 /* ring-0 shadow stack pointer */
	// #define MSR_IA32_PL1_SSP		0x000006a5 /* ring-1 shadow stack pointer */
	// #define MSR_IA32_PL2_SSP		0x000006a6 /* ring-2 shadow stack pointer */
	// #define MSR_IA32_PL3_SSP		0x000006a7 /* ring-3 shadow stack pointer */
	// #define MSR_IA32_INT_SSP_TAB		0x000006a8 /* exception shadow stack table */

	// /* Hardware P state interface */
	// #define MSR_PPERF			0x0000064e
	// #define MSR_PERF_LIMIT_REASONS		0x0000064f
	// #define MSR_PM_ENABLE			0x00000770
	// #define MSR_HWP_CAPABILITIES		0x00000771
	// #define MSR_HWP_REQUEST_PKG		0x00000772
	// #define MSR_HWP_INTERRUPT		0x00000773
	// #define MSR_HWP_REQUEST 		0x00000774
	// #define MSR_HWP_STATUS			0x00000777

	// /* CPUID.6.EAX */
	// #define HWP_BASE_BIT			(1<<7)
	// #define HWP_NOTIFICATIONS_BIT		(1<<8)
	// #define HWP_ACTIVITY_WINDOW_BIT		(1<<9)
	// #define HWP_ENERGY_PERF_PREFERENCE_BIT	(1<<10)
	// #define HWP_PACKAGE_LEVEL_REQUEST_BIT	(1<<11)

	// /* IA32_HWP_CAPABILITIES */
	// #define HWP_HIGHEST_PERF(x)		(((x) >> 0) & 0xff)
	// #define HWP_GUARANTEED_PERF(x)		(((x) >> 8) & 0xff)
	// #define HWP_MOSTEFFICIENT_PERF(x)	(((x) >> 16) & 0xff)
	// #define HWP_LOWEST_PERF(x)		(((x) >> 24) & 0xff)

	// /* IA32_HWP_REQUEST */
	// #define HWP_MIN_PERF(x) 		(x & 0xff)
	// #define HWP_MAX_PERF(x) 		((x & 0xff) << 8)
	// #define HWP_DESIRED_PERF(x)		((x & 0xff) << 16)
	// #define HWP_ENERGY_PERF_PREFERENCE(x)	(((unsigned long long) x & 0xff) << 24)
	// #define HWP_EPP_PERFORMANCE		0x00
	// #define HWP_EPP_BALANCE_PERFORMANCE	0x80
	// #define HWP_EPP_BALANCE_POWERSAVE	0xC0
	// #define HWP_EPP_POWERSAVE		0xFF
	// #define HWP_ACTIVITY_WINDOW(x)		((unsigned long long)(x & 0xff3) << 32)
	// #define HWP_PACKAGE_CONTROL(x)		((unsigned long long)(x & 0x1) << 42)

	// /* IA32_HWP_STATUS */
	// #define HWP_GUARANTEED_CHANGE(x)	(x & 0x1)
	// #define HWP_EXCURSION_TO_MINIMUM(x)	(x & 0x4)

	// /* IA32_HWP_INTERRUPT */
	// #define HWP_CHANGE_TO_GUARANTEED_INT(x)	(x & 0x1)
	// #define HWP_EXCURSION_TO_MINIMUM_INT(x)	(x & 0x2)

	// #define MSR_AMD64_MC0_MASK		0xc0010044

	// #define MSR_IA32_MCx_CTL(x)		(MSR_IA32_MC0_CTL + 4*(x))
	// #define MSR_IA32_MCx_STATUS(x)		(MSR_IA32_MC0_STATUS + 4*(x))
	// #define MSR_IA32_MCx_ADDR(x)		(MSR_IA32_MC0_ADDR + 4*(x))
	// #define MSR_IA32_MCx_MISC(x)		(MSR_IA32_MC0_MISC + 4*(x))

	// #define MSR_AMD64_MCx_MASK(x)		(MSR_AMD64_MC0_MASK + (x))

	// /* These are consecutive and not in the normal 4er MCE bank block */
	// #define MSR_IA32_MC0_CTL2		0x00000280
	// #define MSR_IA32_MCx_CTL2(x)		(MSR_IA32_MC0_CTL2 + (x))

	// #define MSR_P6_PERFCTR0			0x000000c1
	// #define MSR_P6_PERFCTR1			0x000000c2
	// #define MSR_P6_EVNTSEL0			0x00000186
	// #define MSR_P6_EVNTSEL1			0x00000187

	// #define MSR_KNC_PERFCTR0               0x00000020
	// #define MSR_KNC_PERFCTR1               0x00000021
	// #define MSR_KNC_EVNTSEL0               0x00000028
	// #define MSR_KNC_EVNTSEL1               0x00000029

	// /* Alternative perfctr range with full access. */
	// #define MSR_IA32_PMC0			0x000004c1

	// /* Auto-reload via MSR instead of DS area */
	// #define MSR_RELOAD_PMC0			0x000014c1
	// #define MSR_RELOAD_FIXED_CTR0		0x00001309

	// /*
	// * AMD64 MSRs. Not complete. See the architecture manual for a more
	// * complete list.
	// */
	// #define MSR_AMD64_PATCH_LEVEL		0x0000008b
	// #define MSR_AMD64_TSC_RATIO		0xc0000104
	// #define MSR_AMD64_NB_CFG		0xc001001f
	// #define MSR_AMD64_PATCH_LOADER		0xc0010020
	// #define MSR_AMD_PERF_CTL		0xc0010062
	// #define MSR_AMD_PERF_STATUS		0xc0010063
	// #define MSR_AMD_PSTATE_DEF_BASE		0xc0010064
	// #define MSR_AMD64_OSVW_ID_LENGTH	0xc0010140
	// #define MSR_AMD64_OSVW_STATUS		0xc0010141
	// #define MSR_AMD_PPIN_CTL		0xc00102f0
	// #define MSR_AMD_PPIN			0xc00102f1
	// #define MSR_AMD64_CPUID_FN_1		0xc0011004
	// #define MSR_AMD64_LS_CFG		0xc0011020
	// #define MSR_AMD64_DC_CFG		0xc0011022

	// #define MSR_AMD64_DE_CFG		0xc0011029
	// #define MSR_AMD64_DE_CFG_LFENCE_SERIALIZE_BIT	 1
	// #define MSR_AMD64_DE_CFG_LFENCE_SERIALIZE	BIT_ULL(MSR_AMD64_DE_CFG_LFENCE_SERIALIZE_BIT)

	// #define MSR_AMD64_BU_CFG2		0xc001102a
	// #define MSR_AMD64_IBSFETCHCTL		0xc0011030
	// #define MSR_AMD64_IBSFETCHLINAD		0xc0011031
	// #define MSR_AMD64_IBSFETCHPHYSAD	0xc0011032
	// #define MSR_AMD64_IBSFETCH_REG_COUNT	3
	// #define MSR_AMD64_IBSFETCH_REG_MASK	((1UL<<MSR_AMD64_IBSFETCH_REG_COUNT)-1)
	// #define MSR_AMD64_IBSOPCTL		0xc0011033
	// #define MSR_AMD64_IBSOPRIP		0xc0011034
	// #define MSR_AMD64_IBSOPDATA		0xc0011035
	// #define MSR_AMD64_IBSOPDATA2		0xc0011036
	// #define MSR_AMD64_IBSOPDATA3		0xc0011037
	// #define MSR_AMD64_IBSDCLINAD		0xc0011038
	// #define MSR_AMD64_IBSDCPHYSAD		0xc0011039
	// #define MSR_AMD64_IBSOP_REG_COUNT	7
	// #define MSR_AMD64_IBSOP_REG_MASK	((1UL<<MSR_AMD64_IBSOP_REG_COUNT)-1)
	// #define MSR_AMD64_IBSCTL		0xc001103a
	// #define MSR_AMD64_IBSBRTARGET		0xc001103b
	// #define MSR_AMD64_ICIBSEXTDCTL		0xc001103c
	// #define MSR_AMD64_IBSOPDATA4		0xc001103d
	// #define MSR_AMD64_IBS_REG_COUNT_MAX	8 /* includes MSR_AMD64_IBSBRTARGET */
	// #define MSR_AMD64_SVM_AVIC_DOORBELL	0xc001011b
	// #define MSR_AMD64_VM_PAGE_FLUSH		0xc001011e
	// #define MSR_AMD64_SEV_ES_GHCB		0xc0010130
	// #define MSR_AMD64_SEV			0xc0010131
	// #define MSR_AMD64_SEV_ENABLED_BIT	0
	// #define MSR_AMD64_SEV_ES_ENABLED_BIT	1
	// #define MSR_AMD64_SEV_SNP_ENABLED_BIT	2
	// #define MSR_AMD64_SEV_ENABLED		BIT_ULL(MSR_AMD64_SEV_ENABLED_BIT)
	// #define MSR_AMD64_SEV_ES_ENABLED	BIT_ULL(MSR_AMD64_SEV_ES_ENABLED_BIT)
	// #define MSR_AMD64_SEV_SNP_ENABLED	BIT_ULL(MSR_AMD64_SEV_SNP_ENABLED_BIT)

	// /* SNP feature bits enabled by the hypervisor */
	// #define MSR_AMD64_SNP_VTOM			BIT_ULL(3)
	// #define MSR_AMD64_SNP_REFLECT_VC		BIT_ULL(4)
	// #define MSR_AMD64_SNP_RESTRICTED_INJ		BIT_ULL(5)
	// #define MSR_AMD64_SNP_ALT_INJ			BIT_ULL(6)
	// #define MSR_AMD64_SNP_DEBUG_SWAP		BIT_ULL(7)
	// #define MSR_AMD64_SNP_PREVENT_HOST_IBS		BIT_ULL(8)
	// #define MSR_AMD64_SNP_BTB_ISOLATION		BIT_ULL(9)
	// #define MSR_AMD64_SNP_VMPL_SSS			BIT_ULL(10)
	// #define MSR_AMD64_SNP_SECURE_TSC		BIT_ULL(11)
	// #define MSR_AMD64_SNP_VMGEXIT_PARAM		BIT_ULL(12)
	// #define MSR_AMD64_SNP_IBS_VIRT			BIT_ULL(14)
	// #define MSR_AMD64_SNP_VMSA_REG_PROTECTION	BIT_ULL(16)
	// #define MSR_AMD64_SNP_SMT_PROTECTION		BIT_ULL(17)

	// /* SNP feature bits reserved for future use. */
	// #define MSR_AMD64_SNP_RESERVED_BIT13		BIT_ULL(13)
	// #define MSR_AMD64_SNP_RESERVED_BIT15		BIT_ULL(15)
	// #define MSR_AMD64_SNP_RESERVED_MASK		GENMASK_ULL(63, 18)

	// #define MSR_AMD64_VIRT_SPEC_CTRL	0xc001011f

	// /* AMD Collaborative Processor Performance Control MSRs */
	// #define MSR_AMD_CPPC_CAP1		0xc00102b0
	// #define MSR_AMD_CPPC_ENABLE		0xc00102b1
	// #define MSR_AMD_CPPC_CAP2		0xc00102b2
	// #define MSR_AMD_CPPC_REQ		0xc00102b3
	// #define MSR_AMD_CPPC_STATUS		0xc00102b4

	// #define AMD_CPPC_LOWEST_PERF(x)		(((x) >> 0) & 0xff)
	// #define AMD_CPPC_LOWNONLIN_PERF(x)	(((x) >> 8) & 0xff)
	// #define AMD_CPPC_NOMINAL_PERF(x)	(((x) >> 16) & 0xff)
	// #define AMD_CPPC_HIGHEST_PERF(x)	(((x) >> 24) & 0xff)

	// #define AMD_CPPC_MAX_PERF(x)		(((x) & 0xff) << 0)
	// #define AMD_CPPC_MIN_PERF(x)		(((x) & 0xff) << 8)
	// #define AMD_CPPC_DES_PERF(x)		(((x) & 0xff) << 16)
	// #define AMD_CPPC_ENERGY_PERF_PREF(x)	(((x) & 0xff) << 24)

	// /* AMD Performance Counter Global Status and Control MSRs */
	// #define MSR_AMD64_PERF_CNTR_GLOBAL_STATUS	0xc0000300
	// #define MSR_AMD64_PERF_CNTR_GLOBAL_CTL		0xc0000301
	// #define MSR_AMD64_PERF_CNTR_GLOBAL_STATUS_CLR	0xc0000302

	// /* AMD Last Branch Record MSRs */
	// #define MSR_AMD64_LBR_SELECT			0xc000010e

	// /* Fam 17h MSRs */
	// #define MSR_F17H_IRPERF			0xc00000e9

	// #define MSR_ZEN2_SPECTRAL_CHICKEN	0xc00110e3
	// #define MSR_ZEN2_SPECTRAL_CHICKEN_BIT	BIT_ULL(1)

	// /* Fam 16h MSRs */
	// #define MSR_F16H_L2I_PERF_CTL		0xc0010230
	// #define MSR_F16H_L2I_PERF_CTR		0xc0010231
	// #define MSR_F16H_DR1_ADDR_MASK		0xc0011019
	// #define MSR_F16H_DR2_ADDR_MASK		0xc001101a
	// #define MSR_F16H_DR3_ADDR_MASK		0xc001101b
	// #define MSR_F16H_DR0_ADDR_MASK		0xc0011027

	// /* Fam 15h MSRs */
	// #define MSR_F15H_CU_PWR_ACCUMULATOR     0xc001007a
	// #define MSR_F15H_CU_MAX_PWR_ACCUMULATOR 0xc001007b
	// #define MSR_F15H_PERF_CTL		0xc0010200
	// #define MSR_F15H_PERF_CTL0		MSR_F15H_PERF_CTL
	// #define MSR_F15H_PERF_CTL1		(MSR_F15H_PERF_CTL + 2)
	// #define MSR_F15H_PERF_CTL2		(MSR_F15H_PERF_CTL + 4)
	// #define MSR_F15H_PERF_CTL3		(MSR_F15H_PERF_CTL + 6)
	// #define MSR_F15H_PERF_CTL4		(MSR_F15H_PERF_CTL + 8)
	// #define MSR_F15H_PERF_CTL5		(MSR_F15H_PERF_CTL + 10)

	// #define MSR_F15H_PERF_CTR		0xc0010201
	// #define MSR_F15H_PERF_CTR0		MSR_F15H_PERF_CTR
	// #define MSR_F15H_PERF_CTR1		(MSR_F15H_PERF_CTR + 2)
	// #define MSR_F15H_PERF_CTR2		(MSR_F15H_PERF_CTR + 4)
	// #define MSR_F15H_PERF_CTR3		(MSR_F15H_PERF_CTR + 6)
	// #define MSR_F15H_PERF_CTR4		(MSR_F15H_PERF_CTR + 8)
	// #define MSR_F15H_PERF_CTR5		(MSR_F15H_PERF_CTR + 10)

	// #define MSR_F15H_NB_PERF_CTL		0xc0010240
	// #define MSR_F15H_NB_PERF_CTR		0xc0010241
	// #define MSR_F15H_PTSC			0xc0010280
	// #define MSR_F15H_IC_CFG			0xc0011021
	// #define MSR_F15H_EX_CFG			0xc001102c

	// /* Fam 10h MSRs */
	// #define MSR_FAM10H_MMIO_CONF_BASE	0xc0010058
	// #define FAM10H_MMIO_CONF_ENABLE		(1<<0)
	// #define FAM10H_MMIO_CONF_BUSRANGE_MASK	0xf
	// #define FAM10H_MMIO_CONF_BUSRANGE_SHIFT 2
	// #define FAM10H_MMIO_CONF_BASE_MASK	0xfffffffULL
	// #define FAM10H_MMIO_CONF_BASE_SHIFT	20
	// #define MSR_FAM10H_NODE_ID		0xc001100c

	// /* K8 MSRs */
	// #define MSR_K8_TOP_MEM1			0xc001001a
	// #define MSR_K8_TOP_MEM2			0xc001001d
	// #define MSR_AMD64_SYSCFG		0xc0010010
	// #define MSR_AMD64_SYSCFG_MEM_ENCRYPT_BIT	23
	// #define MSR_AMD64_SYSCFG_MEM_ENCRYPT	BIT_ULL(MSR_AMD64_SYSCFG_MEM_ENCRYPT_BIT)
	// #define MSR_K8_INT_PENDING_MSG		0xc0010055
	// /* C1E active bits in int pending message */
	// #define K8_INTP_C1E_ACTIVE_MASK		0x18000000
	// #define MSR_K8_TSEG_ADDR		0xc0010112
	// #define MSR_K8_TSEG_MASK		0xc0010113
	// #define K8_MTRRFIXRANGE_DRAM_ENABLE	0x00040000 /* MtrrFixDramEn bit    */
	// #define K8_MTRRFIXRANGE_DRAM_MODIFY	0x00080000 /* MtrrFixDramModEn bit */
	// #define K8_MTRR_RDMEM_WRMEM_MASK	0x18181818 /* Mask: RdMem|WrMem    */

	// /* K7 MSRs */
	// #define MSR_K7_EVNTSEL0			0xc0010000
	// #define MSR_K7_PERFCTR0			0xc0010004
	// #define MSR_K7_EVNTSEL1			0xc0010001
	// #define MSR_K7_PERFCTR1			0xc0010005
	// #define MSR_K7_EVNTSEL2			0xc0010002
	// #define MSR_K7_PERFCTR2			0xc0010006
	// #define MSR_K7_EVNTSEL3			0xc0010003
	// #define MSR_K7_PERFCTR3			0xc0010007
	// #define MSR_K7_CLK_CTL			0xc001001b
	// #define MSR_K7_HWCR			0xc0010015
	// #define MSR_K7_HWCR_SMMLOCK_BIT		0
	// #define MSR_K7_HWCR_SMMLOCK		BIT_ULL(MSR_K7_HWCR_SMMLOCK_BIT)
	// #define MSR_K7_HWCR_IRPERF_EN_BIT	30
	// #define MSR_K7_HWCR_IRPERF_EN		BIT_ULL(MSR_K7_HWCR_IRPERF_EN_BIT)
	// #define MSR_K7_FID_VID_CTL		0xc0010041
	// #define MSR_K7_FID_VID_STATUS		0xc0010042

	// /* K6 MSRs */
	// #define MSR_K6_WHCR			0xc0000082
	// #define MSR_K6_UWCCR			0xc0000085
	// #define MSR_K6_EPMR			0xc0000086
	// #define MSR_K6_PSOR			0xc0000087
	// #define MSR_K6_PFIR			0xc0000088

	// /* Centaur-Hauls/IDT defined MSRs. */
	// #define MSR_IDT_FCR1			0x00000107
	// #define MSR_IDT_FCR2			0x00000108
	// #define MSR_IDT_FCR3			0x00000109
	// #define MSR_IDT_FCR4			0x0000010a

	// #define MSR_IDT_MCR0			0x00000110
	// #define MSR_IDT_MCR1			0x00000111
	// #define MSR_IDT_MCR2			0x00000112
	// #define MSR_IDT_MCR3			0x00000113
	// #define MSR_IDT_MCR4			0x00000114
	// #define MSR_IDT_MCR5			0x00000115
	// #define MSR_IDT_MCR6			0x00000116
	// #define MSR_IDT_MCR7			0x00000117
	// #define MSR_IDT_MCR_CTRL		0x00000120

	// /* VIA Cyrix defined MSRs*/
	// #define MSR_VIA_FCR			0x00001107
	// #define MSR_VIA_LONGHAUL		0x0000110a
	// #define MSR_VIA_RNG			0x0000110b
	// #define MSR_VIA_BCR2			0x00001147

	// /* Transmeta defined MSRs */
	// #define MSR_TMTA_LONGRUN_CTRL		0x80868010
	// #define MSR_TMTA_LONGRUN_FLAGS		0x80868011
	// #define MSR_TMTA_LRTI_READOUT		0x80868018
	// #define MSR_TMTA_LRTI_VOLT_MHZ		0x8086801a

	// /* Intel defined MSRs. */
	// #define MSR_IA32_P5_MC_ADDR		0x00000000
	// #define MSR_IA32_P5_MC_TYPE		0x00000001
	// #define MSR_IA32_TSC			0x00000010
	// #define MSR_IA32_PLATFORM_ID		0x00000017
	// #define MSR_IA32_EBL_CR_POWERON		0x0000002a
	// #define MSR_EBC_FREQUENCY_ID		0x0000002c
	// #define MSR_SMI_COUNT			0x00000034

	// /* Referred to as IA32_FEATURE_CONTROL in Intel's SDM. */
	// #define MSR_IA32_FEAT_CTL		0x0000003a
	// #define FEAT_CTL_LOCKED				BIT(0)
	// #define FEAT_CTL_VMX_ENABLED_INSIDE_SMX		BIT(1)
	// #define FEAT_CTL_VMX_ENABLED_OUTSIDE_SMX	BIT(2)
	// #define FEAT_CTL_SGX_LC_ENABLED			BIT(17)
	// #define FEAT_CTL_SGX_ENABLED			BIT(18)
	// #define FEAT_CTL_LMCE_ENABLED			BIT(20)

	// #define MSR_IA32_TSC_ADJUST             0x0000003b
	// #define MSR_IA32_BNDCFGS		0x00000d90

	// #define MSR_IA32_BNDCFGS_RSVD		0x00000ffc

	// #define MSR_IA32_XFD			0x000001c4
	// #define MSR_IA32_XFD_ERR		0x000001c5
	// #define MSR_IA32_XSS			0x00000da0

	#define MSR_IA32_APICBASE			0x0000001b
	#define MSR_IA32_APICBASE_BSP		(1<<8)
	#define MSR_IA32_APICBASE_ENABLE	(1<<11)
	#define MSR_IA32_APICBASE_BASE		(0xfffff<<12)

	// #define MSR_IA32_UCODE_WRITE		0x00000079
	// #define MSR_IA32_UCODE_REV		0x0000008b

	// /* Intel SGX Launch Enclave Public Key Hash MSRs */
	// #define MSR_IA32_SGXLEPUBKEYHASH0	0x0000008C
	// #define MSR_IA32_SGXLEPUBKEYHASH1	0x0000008D
	// #define MSR_IA32_SGXLEPUBKEYHASH2	0x0000008E
	// #define MSR_IA32_SGXLEPUBKEYHASH3	0x0000008F

	// #define MSR_IA32_SMM_MONITOR_CTL	0x0000009b
	// #define MSR_IA32_SMBASE			0x0000009e

	// #define MSR_IA32_PERF_STATUS		0x00000198
	// #define MSR_IA32_PERF_CTL		0x00000199
	// #define INTEL_PERF_CTL_MASK		0xffff

	// /* AMD Branch Sampling configuration */
	// #define MSR_AMD_DBG_EXTN_CFG		0xc000010f
	// #define MSR_AMD_SAMP_BR_FROM		0xc0010300

	// #define DBG_EXTN_CFG_LBRV2EN		BIT_ULL(6)

	// #define MSR_IA32_MPERF			0x000000e7
	// #define MSR_IA32_APERF			0x000000e8

	// #define MSR_IA32_THERM_CONTROL		0x0000019a
	// #define MSR_IA32_THERM_INTERRUPT	0x0000019b

	// #define THERM_INT_HIGH_ENABLE		(1 << 0)
	// #define THERM_INT_LOW_ENABLE		(1 << 1)
	// #define THERM_INT_PLN_ENABLE		(1 << 24)

	// #define MSR_IA32_THERM_STATUS		0x0000019c

	// #define THERM_STATUS_PROCHOT		(1 << 0)
	// #define THERM_STATUS_POWER_LIMIT	(1 << 10)

	// #define MSR_THERM2_CTL			0x0000019d

	// #define MSR_THERM2_CTL_TM_SELECT	(1ULL << 16)

	// #define MSR_IA32_MISC_ENABLE		0x000001a0

	// #define MSR_IA32_TEMPERATURE_TARGET	0x000001a2

	// #define MSR_MISC_FEATURE_CONTROL	0x000001a4
	// #define MSR_MISC_PWR_MGMT		0x000001aa

	// #define MSR_IA32_ENERGY_PERF_BIAS	0x000001b0
	// #define ENERGY_PERF_BIAS_PERFORMANCE		0
	// #define ENERGY_PERF_BIAS_BALANCE_PERFORMANCE	4
	// #define ENERGY_PERF_BIAS_NORMAL			6
	// #define ENERGY_PERF_BIAS_NORMAL_POWERSAVE	7
	// #define ENERGY_PERF_BIAS_BALANCE_POWERSAVE	8
	// #define ENERGY_PERF_BIAS_POWERSAVE		15

	// #define MSR_IA32_PACKAGE_THERM_STATUS		0x000001b1

	// #define PACKAGE_THERM_STATUS_PROCHOT		(1 << 0)
	// #define PACKAGE_THERM_STATUS_POWER_LIMIT	(1 << 10)
	// #define PACKAGE_THERM_STATUS_HFI_UPDATED	(1 << 26)

	// #define MSR_IA32_PACKAGE_THERM_INTERRUPT	0x000001b2

	// #define PACKAGE_THERM_INT_HIGH_ENABLE		(1 << 0)
	// #define PACKAGE_THERM_INT_LOW_ENABLE		(1 << 1)
	// #define PACKAGE_THERM_INT_PLN_ENABLE		(1 << 24)
	// #define PACKAGE_THERM_INT_HFI_ENABLE		(1 << 25)

	// /* Thermal Thresholds Support */
	// #define THERM_INT_THRESHOLD0_ENABLE    (1 << 15)
	// #define THERM_SHIFT_THRESHOLD0        8
	// #define THERM_MASK_THRESHOLD0          (0x7f << THERM_SHIFT_THRESHOLD0)
	// #define THERM_INT_THRESHOLD1_ENABLE    (1 << 23)
	// #define THERM_SHIFT_THRESHOLD1        16
	// #define THERM_MASK_THRESHOLD1          (0x7f << THERM_SHIFT_THRESHOLD1)
	// #define THERM_STATUS_THRESHOLD0        (1 << 6)
	// #define THERM_LOG_THRESHOLD0           (1 << 7)
	// #define THERM_STATUS_THRESHOLD1        (1 << 8)
	// #define THERM_LOG_THRESHOLD1           (1 << 9)

	// /* MISC_ENABLE bits: architectural */
	// #define MSR_IA32_MISC_ENABLE_FAST_STRING_BIT		0
	// #define MSR_IA32_MISC_ENABLE_FAST_STRING		(1ULL << MSR_IA32_MISC_ENABLE_FAST_STRING_BIT)
	// #define MSR_IA32_MISC_ENABLE_TCC_BIT			1
	// #define MSR_IA32_MISC_ENABLE_TCC			(1ULL << MSR_IA32_MISC_ENABLE_TCC_BIT)
	// #define MSR_IA32_MISC_ENABLE_EMON_BIT			7
	// #define MSR_IA32_MISC_ENABLE_EMON			(1ULL << MSR_IA32_MISC_ENABLE_EMON_BIT)
	// #define MSR_IA32_MISC_ENABLE_BTS_UNAVAIL_BIT		11
	// #define MSR_IA32_MISC_ENABLE_BTS_UNAVAIL		(1ULL << MSR_IA32_MISC_ENABLE_BTS_UNAVAIL_BIT)
	// #define MSR_IA32_MISC_ENABLE_PEBS_UNAVAIL_BIT		12
	// #define MSR_IA32_MISC_ENABLE_PEBS_UNAVAIL		(1ULL << MSR_IA32_MISC_ENABLE_PEBS_UNAVAIL_BIT)
	// #define MSR_IA32_MISC_ENABLE_ENHANCED_SPEEDSTEP_BIT	16
	// #define MSR_IA32_MISC_ENABLE_ENHANCED_SPEEDSTEP		(1ULL << MSR_IA32_MISC_ENABLE_ENHANCED_SPEEDSTEP_BIT)
	// #define MSR_IA32_MISC_ENABLE_MWAIT_BIT			18
	// #define MSR_IA32_MISC_ENABLE_MWAIT			(1ULL << MSR_IA32_MISC_ENABLE_MWAIT_BIT)
	// #define MSR_IA32_MISC_ENABLE_LIMIT_CPUID_BIT		22
	// #define MSR_IA32_MISC_ENABLE_LIMIT_CPUID		(1ULL << MSR_IA32_MISC_ENABLE_LIMIT_CPUID_BIT)
	// #define MSR_IA32_MISC_ENABLE_XTPR_DISABLE_BIT		23
	// #define MSR_IA32_MISC_ENABLE_XTPR_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_XTPR_DISABLE_BIT)
	// #define MSR_IA32_MISC_ENABLE_XD_DISABLE_BIT		34
	// #define MSR_IA32_MISC_ENABLE_XD_DISABLE			(1ULL << MSR_IA32_MISC_ENABLE_XD_DISABLE_BIT)

	// /* MISC_ENABLE bits: model-specific, meaning may vary from core to core */
	// #define MSR_IA32_MISC_ENABLE_X87_COMPAT_BIT		2
	// #define MSR_IA32_MISC_ENABLE_X87_COMPAT			(1ULL << MSR_IA32_MISC_ENABLE_X87_COMPAT_BIT)
	// #define MSR_IA32_MISC_ENABLE_TM1_BIT			3
	// #define MSR_IA32_MISC_ENABLE_TM1			(1ULL << MSR_IA32_MISC_ENABLE_TM1_BIT)
	// #define MSR_IA32_MISC_ENABLE_SPLIT_LOCK_DISABLE_BIT	4
	// #define MSR_IA32_MISC_ENABLE_SPLIT_LOCK_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_SPLIT_LOCK_DISABLE_BIT)
	// #define MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE_BIT	6
	// #define MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE_BIT)
	// #define MSR_IA32_MISC_ENABLE_SUPPRESS_LOCK_BIT		8
	// #define MSR_IA32_MISC_ENABLE_SUPPRESS_LOCK		(1ULL << MSR_IA32_MISC_ENABLE_SUPPRESS_LOCK_BIT)
	// #define MSR_IA32_MISC_ENABLE_PREFETCH_DISABLE_BIT	9
	// #define MSR_IA32_MISC_ENABLE_PREFETCH_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_PREFETCH_DISABLE_BIT)
	// #define MSR_IA32_MISC_ENABLE_FERR_BIT			10
	// #define MSR_IA32_MISC_ENABLE_FERR			(1ULL << MSR_IA32_MISC_ENABLE_FERR_BIT)
	// #define MSR_IA32_MISC_ENABLE_FERR_MULTIPLEX_BIT		10
	// #define MSR_IA32_MISC_ENABLE_FERR_MULTIPLEX		(1ULL << MSR_IA32_MISC_ENABLE_FERR_MULTIPLEX_BIT)
	// #define MSR_IA32_MISC_ENABLE_TM2_BIT			13
	// #define MSR_IA32_MISC_ENABLE_TM2			(1ULL << MSR_IA32_MISC_ENABLE_TM2_BIT)
	// #define MSR_IA32_MISC_ENABLE_ADJ_PREF_DISABLE_BIT	19
	// #define MSR_IA32_MISC_ENABLE_ADJ_PREF_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_ADJ_PREF_DISABLE_BIT)
	// #define MSR_IA32_MISC_ENABLE_SPEEDSTEP_LOCK_BIT		20
	// #define MSR_IA32_MISC_ENABLE_SPEEDSTEP_LOCK		(1ULL << MSR_IA32_MISC_ENABLE_SPEEDSTEP_LOCK_BIT)
	// #define MSR_IA32_MISC_ENABLE_L1D_CONTEXT_BIT		24
	// #define MSR_IA32_MISC_ENABLE_L1D_CONTEXT		(1ULL << MSR_IA32_MISC_ENABLE_L1D_CONTEXT_BIT)
	// #define MSR_IA32_MISC_ENABLE_DCU_PREF_DISABLE_BIT	37
	// #define MSR_IA32_MISC_ENABLE_DCU_PREF_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_DCU_PREF_DISABLE_BIT)
	// #define MSR_IA32_MISC_ENABLE_TURBO_DISABLE_BIT		38
	// #define MSR_IA32_MISC_ENABLE_TURBO_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_TURBO_DISABLE_BIT)
	// #define MSR_IA32_MISC_ENABLE_IP_PREF_DISABLE_BIT	39
	// #define MSR_IA32_MISC_ENABLE_IP_PREF_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_IP_PREF_DISABLE_BIT)

	// /* MISC_FEATURES_ENABLES non-architectural features */
	// #define MSR_MISC_FEATURES_ENABLES	0x00000140

	// #define MSR_MISC_FEATURES_ENABLES_CPUID_FAULT_BIT	0
	// #define MSR_MISC_FEATURES_ENABLES_CPUID_FAULT		BIT_ULL(MSR_MISC_FEATURES_ENABLES_CPUID_FAULT_BIT)
	// #define MSR_MISC_FEATURES_ENABLES_RING3MWAIT_BIT	1

	// #define MSR_IA32_TSC_DEADLINE		0x000006E0


	// #define MSR_TSX_FORCE_ABORT		0x0000010F

	// #define MSR_TFA_RTM_FORCE_ABORT_BIT	0
	// #define MSR_TFA_RTM_FORCE_ABORT		BIT_ULL(MSR_TFA_RTM_FORCE_ABORT_BIT)
	// #define MSR_TFA_TSX_CPUID_CLEAR_BIT	1
	// #define MSR_TFA_TSX_CPUID_CLEAR		BIT_ULL(MSR_TFA_TSX_CPUID_CLEAR_BIT)
	// #define MSR_TFA_SDV_ENABLE_RTM_BIT	2
	// #define MSR_TFA_SDV_ENABLE_RTM		BIT_ULL(MSR_TFA_SDV_ENABLE_RTM_BIT)

	// /* P4/Xeon+ specific */
	// #define MSR_IA32_MCG_EAX		0x00000180
	// #define MSR_IA32_MCG_EBX		0x00000181
	// #define MSR_IA32_MCG_ECX		0x00000182
	// #define MSR_IA32_MCG_EDX		0x00000183
	// #define MSR_IA32_MCG_ESI		0x00000184
	// #define MSR_IA32_MCG_EDI		0x00000185
	// #define MSR_IA32_MCG_EBP		0x00000186
	// #define MSR_IA32_MCG_ESP		0x00000187
	// #define MSR_IA32_MCG_EFLAGS		0x00000188
	// #define MSR_IA32_MCG_EIP		0x00000189
	// #define MSR_IA32_MCG_RESERVED		0x0000018a

	// /* Pentium IV performance counter MSRs */
	// #define MSR_P4_BPU_PERFCTR0		0x00000300
	// #define MSR_P4_BPU_PERFCTR1		0x00000301
	// #define MSR_P4_BPU_PERFCTR2		0x00000302
	// #define MSR_P4_BPU_PERFCTR3		0x00000303
	// #define MSR_P4_MS_PERFCTR0		0x00000304
	// #define MSR_P4_MS_PERFCTR1		0x00000305
	// #define MSR_P4_MS_PERFCTR2		0x00000306
	// #define MSR_P4_MS_PERFCTR3		0x00000307
	// #define MSR_P4_FLAME_PERFCTR0		0x00000308
	// #define MSR_P4_FLAME_PERFCTR1		0x00000309
	// #define MSR_P4_FLAME_PERFCTR2		0x0000030a
	// #define MSR_P4_FLAME_PERFCTR3		0x0000030b
	// #define MSR_P4_IQ_PERFCTR0		0x0000030c
	// #define MSR_P4_IQ_PERFCTR1		0x0000030d
	// #define MSR_P4_IQ_PERFCTR2		0x0000030e
	// #define MSR_P4_IQ_PERFCTR3		0x0000030f
	// #define MSR_P4_IQ_PERFCTR4		0x00000310
	// #define MSR_P4_IQ_PERFCTR5		0x00000311
	// #define MSR_P4_BPU_CCCR0		0x00000360
	// #define MSR_P4_BPU_CCCR1		0x00000361
	// #define MSR_P4_BPU_CCCR2		0x00000362
	// #define MSR_P4_BPU_CCCR3		0x00000363
	// #define MSR_P4_MS_CCCR0			0x00000364
	// #define MSR_P4_MS_CCCR1			0x00000365
	// #define MSR_P4_MS_CCCR2			0x00000366
	// #define MSR_P4_MS_CCCR3			0x00000367
	// #define MSR_P4_FLAME_CCCR0		0x00000368
	// #define MSR_P4_FLAME_CCCR1		0x00000369
	// #define MSR_P4_FLAME_CCCR2		0x0000036a
	// #define MSR_P4_FLAME_CCCR3		0x0000036b
	// #define MSR_P4_IQ_CCCR0			0x0000036c
	// #define MSR_P4_IQ_CCCR1			0x0000036d
	// #define MSR_P4_IQ_CCCR2			0x0000036e
	// #define MSR_P4_IQ_CCCR3			0x0000036f
	// #define MSR_P4_IQ_CCCR4			0x00000370
	// #define MSR_P4_IQ_CCCR5			0x00000371
	// #define MSR_P4_ALF_ESCR0		0x000003ca
	// #define MSR_P4_ALF_ESCR1		0x000003cb
	// #define MSR_P4_BPU_ESCR0		0x000003b2
	// #define MSR_P4_BPU_ESCR1		0x000003b3
	// #define MSR_P4_BSU_ESCR0		0x000003a0
	// #define MSR_P4_BSU_ESCR1		0x000003a1
	// #define MSR_P4_CRU_ESCR0		0x000003b8
	// #define MSR_P4_CRU_ESCR1		0x000003b9
	// #define MSR_P4_CRU_ESCR2		0x000003cc
	// #define MSR_P4_CRU_ESCR3		0x000003cd
	// #define MSR_P4_CRU_ESCR4		0x000003e0
	// #define MSR_P4_CRU_ESCR5		0x000003e1
	// #define MSR_P4_DAC_ESCR0		0x000003a8
	// #define MSR_P4_DAC_ESCR1		0x000003a9
	// #define MSR_P4_FIRM_ESCR0		0x000003a4
	// #define MSR_P4_FIRM_ESCR1		0x000003a5
	// #define MSR_P4_FLAME_ESCR0		0x000003a6
	// #define MSR_P4_FLAME_ESCR1		0x000003a7
	// #define MSR_P4_FSB_ESCR0		0x000003a2
	// #define MSR_P4_FSB_ESCR1		0x000003a3
	// #define MSR_P4_IQ_ESCR0			0x000003ba
	// #define MSR_P4_IQ_ESCR1			0x000003bb
	// #define MSR_P4_IS_ESCR0			0x000003b4
	// #define MSR_P4_IS_ESCR1			0x000003b5
	// #define MSR_P4_ITLB_ESCR0		0x000003b6
	// #define MSR_P4_ITLB_ESCR1		0x000003b7
	// #define MSR_P4_IX_ESCR0			0x000003c8
	// #define MSR_P4_IX_ESCR1			0x000003c9
	// #define MSR_P4_MOB_ESCR0		0x000003aa
	// #define MSR_P4_MOB_ESCR1		0x000003ab
	// #define MSR_P4_MS_ESCR0			0x000003c0
	// #define MSR_P4_MS_ESCR1			0x000003c1
	// #define MSR_P4_PMH_ESCR0		0x000003ac
	// #define MSR_P4_PMH_ESCR1		0x000003ad
	// #define MSR_P4_RAT_ESCR0		0x000003bc
	// #define MSR_P4_RAT_ESCR1		0x000003bd
	// #define MSR_P4_SAAT_ESCR0		0x000003ae
	// #define MSR_P4_SAAT_ESCR1		0x000003af
	// #define MSR_P4_SSU_ESCR0		0x000003be
	// #define MSR_P4_SSU_ESCR1		0x000003bf /* guess: not in manual */

	// #define MSR_P4_TBPU_ESCR0		0x000003c2
	// #define MSR_P4_TBPU_ESCR1		0x000003c3
	// #define MSR_P4_TC_ESCR0			0x000003c4
	// #define MSR_P4_TC_ESCR1			0x000003c5
	// #define MSR_P4_U2L_ESCR0		0x000003b0
	// #define MSR_P4_U2L_ESCR1		0x000003b1

	// #define MSR_P4_PEBS_MATRIX_VERT		0x000003f2

	// /* Intel Core-based CPU performance counters */
	// #define MSR_CORE_PERF_FIXED_CTR0	0x00000309
	// #define MSR_CORE_PERF_FIXED_CTR1	0x0000030a
	// #define MSR_CORE_PERF_FIXED_CTR2	0x0000030b
	// #define MSR_CORE_PERF_FIXED_CTR3	0x0000030c
	// #define MSR_CORE_PERF_FIXED_CTR_CTRL	0x0000038d
	// #define MSR_CORE_PERF_GLOBAL_STATUS	0x0000038e
	// #define MSR_CORE_PERF_GLOBAL_CTRL	0x0000038f
	// #define MSR_CORE_PERF_GLOBAL_OVF_CTRL	0x00000390

	// #define MSR_PERF_METRICS		0x00000329

	// /* PERF_GLOBAL_OVF_CTL bits */
	// #define MSR_CORE_PERF_GLOBAL_OVF_CTRL_TRACE_TOPA_PMI_BIT	55
	// #define MSR_CORE_PERF_GLOBAL_OVF_CTRL_TRACE_TOPA_PMI		(1ULL << MSR_CORE_PERF_GLOBAL_OVF_CTRL_TRACE_TOPA_PMI_BIT)
	// #define MSR_CORE_PERF_GLOBAL_OVF_CTRL_OVF_BUF_BIT		62
	// #define MSR_CORE_PERF_GLOBAL_OVF_CTRL_OVF_BUF			(1ULL <<  MSR_CORE_PERF_GLOBAL_OVF_CTRL_OVF_BUF_BIT)
	// #define MSR_CORE_PERF_GLOBAL_OVF_CTRL_COND_CHGD_BIT		63
	// #define MSR_CORE_PERF_GLOBAL_OVF_CTRL_COND_CHGD			(1ULL << MSR_CORE_PERF_GLOBAL_OVF_CTRL_COND_CHGD_BIT)

	// /* Geode defined MSRs */
	// #define MSR_GEODE_BUSCONT_CONF0		0x00001900

	// /* Intel VT MSRs */
	// #define MSR_IA32_VMX_BASIC              0x00000480
	// #define MSR_IA32_VMX_PINBASED_CTLS      0x00000481
	// #define MSR_IA32_VMX_PROCBASED_CTLS     0x00000482
	// #define MSR_IA32_VMX_EXIT_CTLS          0x00000483
	// #define MSR_IA32_VMX_ENTRY_CTLS         0x00000484
	// #define MSR_IA32_VMX_MISC               0x00000485
	// #define MSR_IA32_VMX_CR0_FIXED0         0x00000486
	// #define MSR_IA32_VMX_CR0_FIXED1         0x00000487
	// #define MSR_IA32_VMX_CR4_FIXED0         0x00000488
	// #define MSR_IA32_VMX_CR4_FIXED1         0x00000489
	// #define MSR_IA32_VMX_VMCS_ENUM          0x0000048a
	// #define MSR_IA32_VMX_PROCBASED_CTLS2    0x0000048b
	// #define MSR_IA32_VMX_EPT_VPID_CAP       0x0000048c
	// #define MSR_IA32_VMX_TRUE_PINBASED_CTLS  0x0000048d
	// #define MSR_IA32_VMX_TRUE_PROCBASED_CTLS 0x0000048e
	// #define MSR_IA32_VMX_TRUE_EXIT_CTLS      0x0000048f
	// #define MSR_IA32_VMX_TRUE_ENTRY_CTLS     0x00000490
	// #define MSR_IA32_VMX_VMFUNC             0x00000491
	// #define MSR_IA32_VMX_PROCBASED_CTLS3	0x00000492

	// /* VMX_BASIC bits and bitmasks */
	// #define VMX_BASIC_VMCS_SIZE_SHIFT	32
	// #define VMX_BASIC_TRUE_CTLS		(1ULL << 55)
	// #define VMX_BASIC_64		0x0001000000000000LLU
	// #define VMX_BASIC_MEM_TYPE_SHIFT	50
	// #define VMX_BASIC_MEM_TYPE_MASK	0x003c000000000000LLU
	// #define VMX_BASIC_MEM_TYPE_WB	6LLU
	// #define VMX_BASIC_INOUT		0x0040000000000000LLU

	// /* Resctrl MSRs: */
	// /* - Intel: */
	// #define MSR_IA32_L3_QOS_CFG		0xc81
	// #define MSR_IA32_L2_QOS_CFG		0xc82
	// #define MSR_IA32_QM_EVTSEL		0xc8d
	// #define MSR_IA32_QM_CTR			0xc8e
	// #define MSR_IA32_PQR_ASSOC		0xc8f
	// #define MSR_IA32_L3_CBM_BASE		0xc90
	// #define MSR_IA32_L2_CBM_BASE		0xd10
	// #define MSR_IA32_MBA_THRTL_BASE		0xd50

	// /* - AMD: */
	// #define MSR_IA32_MBA_BW_BASE		0xc0000200
	// #define MSR_IA32_SMBA_BW_BASE		0xc0000280
	// #define MSR_IA32_EVT_CFG_BASE		0xc0000400

	// /* MSR_IA32_VMX_MISC bits */
	// #define MSR_IA32_VMX_MISC_INTEL_PT                 (1ULL << 14)
	// #define MSR_IA32_VMX_MISC_VMWRITE_SHADOW_RO_FIELDS (1ULL << 29)
	// #define MSR_IA32_VMX_MISC_PREEMPTION_TIMER_SCALE   0x1F
	// /* AMD-V MSRs */

	// #define MSR_VM_CR                       0xc0010114
	// #define MSR_VM_IGNNE                    0xc0010115
	// #define MSR_VM_HSAVE_PA                 0xc0010117

	// /* Hardware Feedback Interface */
	// #define MSR_IA32_HW_FEEDBACK_PTR        0x17d0
	// #define MSR_IA32_HW_FEEDBACK_CONFIG     0x17d1

	// /* x2APIC locked status */
	// #define MSR_IA32_XAPIC_DISABLE_STATUS	0xBD
	// #define LEGACY_XAPIC_DISABLED		BIT(0) /*
	// 						* x2APIC mode is locked and
	// 						* disabling x2APIC will cause
	// 						* a #GP
	// 						*/

#endif /* _ASM_X86_MSR_INDEX_H */
