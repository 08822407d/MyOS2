// SPDX-License-Identifier: GPL-2.0-only
/* Common code for 32 and 64-bit NUMA */
#include <linux/kernel/mm.h>

pg_data_t *node_data[MAX_NUMNODES] __read_mostly;