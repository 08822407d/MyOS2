

// static const cpu_dev_s default_cpu = {
// 	.c_init			= default_init,
// 	.c_vendor		= "Unknown",
// 	.c_x86_vendor	= X86_VENDOR_UNKNOWN,
// };

// static const cpu_dev_s cyrix_cpu_dev = {
// 	.c_vendor		= "Cyrix",
// 	.c_ident		= {"CyrixInstead"},
// 	.c_early_init	= early_init_cyrix,
// 	.c_init			= init_cyrix,
// 	.c_identify		= cyrix_identify,
// 	.c_x86_vendor	= X86_VENDOR_CYRIX,
// };

// static const cpu_dev_s nsc_cpu_dev = {
// 	.c_vendor		= "NSC",
// 	.c_ident		= {"Geode by NSC"},
// 	.c_init			= init_nsc,
// 	.c_x86_vendor	= X86_VENDOR_NSC,
// };

// static const cpu_dev_s hygon_cpu_dev = {
// 	.c_vendor		= "Hygon",
// 	.c_ident		= {"HygonGenuine"},
// 	.c_early_init	= early_init_hygon,
// 	.c_detect_tlb	= cpu_detect_tlb_hygon,
// 	.c_bsp_init		= bsp_init_hygon,
// 	.c_init			= init_hygon,
// 	.c_x86_vendor	= X86_VENDOR_HYGON,
// };

// static const cpu_dev_s transmeta_cpu_dev = {
// 	.c_vendor		= "Transmeta",
// 	.c_ident		= {"GenuineTMx86", "TransmetaCPU"},
// 	.c_early_init	= early_init_transmeta,
// 	.c_init			= init_transmeta,
// 	.c_x86_vendor	= X86_VENDOR_TRANSMETA,
// };

// static const cpu_dev_s vortex_cpu_dev = {
// 	.c_vendor		= "Vortex",
// 	.c_ident		= {"Vortex86 SoC"},
// 	.legacy_models	= {
// 		{
// 			.family			= 5,
// 			.model_names	= {
// 				[2] = "Vortex86DX",
// 				[8] = "Vortex86MX",
// 			},
// 		},
// 		{
// 			.family			= 6,
// 			.model_names	= {
// 				/*
// 				 * Both the Vortex86EX and the Vortex86EX2
// 				 * have the same family and model id.
// 				 *
// 				 * However, the -EX2 supports the product name
// 				 * CPUID call, so this name will only be used
// 				 * for the -EX, which does not.
// 				 */
// 				[0] = "Vortex86EX",
// 			},
// 		},
// 	},
// 	.c_x86_vendor	= X86_VENDOR_VORTEX,
// };