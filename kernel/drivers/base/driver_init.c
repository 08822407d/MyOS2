// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 */

#include <linux/device/driver.h>
// #include <linux/init.h>
// #include <linux/memory.h>
// #include <linux/of.h>
#include <linux/drivers/base.h>


/**
 * driver_init - initialize driver model.
 *
 * Call the driver model init functions to initialize their
 * subsystems. Called early from init/main.c.
 */
void driver_init(void)
{
	/* These are the core pieces */
	devtmpfs_init();
	// devices_init();
	// buses_init();
	// classes_init();
	// firmware_init();
	// hypervisor_init();

	/* These are also core pieces, but must come after the
	//  * core core pieces.
	//  */
	// of_core_init();
	// platform_bus_init();
	// auxiliary_bus_init();
	// cpu_dev_init();
	// memory_dev_init();
	// container_dev_init();
}