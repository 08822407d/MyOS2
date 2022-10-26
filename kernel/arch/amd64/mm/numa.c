// SPDX-License-Identifier: GPL-2.0-only
/* Common code for 32 and 64-bit NUMA */
// #include <linux/acpi.h>
#include <linux/kernel/kernel.h>
#include <linux/mm/mm.h>
#include <linux/lib/string.h>
#include <linux/init/init.h>
#include <linux/mm/memblock.h>
#include <linux/mm/mmzone.h>
// #include <linux/ctype.h>
// #include <linux/nodemask.h>
// #include <linux/sched.h>
// #include <linux/topology.h>

#include <asm/e820-api.h>
// #include <asm/proto.h>
// #include <asm/dma.h>
// #include <asm/amd_nb.h>

// #include "numa_internal.h"

pg_data_t *node_data[MAX_NUMNODES] __read_mostly;