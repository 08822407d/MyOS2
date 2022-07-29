// SPDX-License-Identifier: GPL-2.0-only
/*
 *      sd.c Copyright (C) 1992 Drew Eckhardt
 *           Copyright (C) 1993, 1994, 1995, 1999 Eric Youngdale
 *
 *      Linux scsi disk driver
 *              Initial versions: Drew Eckhardt
 *              Subsequent revisions: Eric Youngdale
 *	Modification history:
 *       - Drew Eckhardt <drew@colorado.edu> original
 *       - Eric Youngdale <eric@andante.org> add scatter-gather, multiple 
 *         outstanding request, and other enhancements.
 *         Support loadable low-level scsi drivers.
 *       - Jirka Hanika <geo@ff.cuni.cz> support more scsi disks using 
 *         eight major numbers.
 *       - Richard Gooch <rgooch@atnf.csiro.au> support devfs.
 *	 - Torben Mathiasen <tmm@image.dk> Resource allocation fixes in 
 *	   sd_init and cleanups.
 *	 - Alex Davis <letmein@erols.com> Fix problem where partition info
 *	   not being read in sd_open. Fix problem where removable media 
 *	   could be ejected after sd_open.
 *	 - Douglas Gilbert <dgilbert@interlog.com> cleanup for lk 2.5.x
 *	 - Badari Pulavarty <pbadari@us.ibm.com>, Matthew Wilcox 
 *	   <willy@debian.org>, Kurt Garloff <garloff@suse.de>: 
 *	   Support 32k/1M disks.
 *
 *	Logging policy (needs CONFIG_SCSI_LOGGING defined):
 *	 - setting up transfer: SCSI_LOG_HLQUEUE levels 1 and 2
 *	 - end of transfer (bh + scsi_lib): SCSI_LOG_HLCOMPLETE level 1
 *	 - entering sd_ioctl: SCSI_LOG_IOCTL level 1
 *	 - entering other commands: SCSI_LOG_HLQUEUE level 3
 *	Note: when the logging level is set by the user, it must be greater
 *	than the level indicated above to trigger output.	
 */

// #include <linux/module.h>
#include <linux/fs/fs.h>
#include <linux/kernel/kernel.h>
#include <linux/mm/mm.h>
// #include <linux/bio.h>
#include <linux/block/genhd.h>
// #include <linux/hdreg.h>
// #include <linux/errno.h>
// #include <linux/idr.h>
// #include <linux/interrupt.h>
#include <linux/init/init.h>
#include <linux/block/blkdev.h>
// #include <linux/blkpg.h>
// #include <linux/blk-pm.h>
// #include <linux/delay.h>
// #include <linux/major.h>
// #include <linux/mutex.h>
// #include <linux/string_helpers.h>
// #include <linux/slab.h>
// #include <linux/sed-opal.h>
// #include <linux/pm_runtime.h>
// #include <linux/pr.h>
// #include <linux/t10-pi.h>
// #include <linux/uaccess.h>
// #include <asm/unaligned.h>

// #include <scsi/scsi.h>
// #include <scsi/scsi_cmnd.h>
// #include <scsi/scsi_dbg.h>
// #include <scsi/scsi_device.h>
// #include <scsi/scsi_driver.h>
// #include <scsi/scsi_eh.h>
// #include <scsi/scsi_host.h>
// #include <scsi/scsi_ioctl.h>
// #include <scsi/scsicam.h>

// #include "sd.h"
// #include "scsi_priv.h"
// #include "scsi_logging.h"



#include <include/obsolete/proto.h>

/**
 *	sd_probe - called during driver initialization and whenever a
 *	new scsi device is attached to the system. It is called once
 *	for each scsi device (not just disks) present.
 *	@dev: pointer to device object
 *
 *	Returns 0 if successful (or not interested in this scsi device 
 *	(e.g. scanner)); 1 when there is an error.
 *
 *	Note: this function is invoked from the scsi mid-level.
 *	This function sets up the mapping between a given 
 *	<host,channel,id,lun> (found in sdp) and new device name 
 *	(e.g. /dev/sda). More precisely it is the block device major 
 *	and minor number that is chosen here.
 *
 *	Assume sd_probe is not re-entrant (for time being)
 *	Also think about sd_probe() and sd_remove() running coincidentally.
 **/
// static int sd_probe(struct device *dev)
int ata_probe()
{
	gendisk_s *gd;
	request_queue_s *rq;
	gd = __alloc_disk_node(rq);
	if (gd == NULL)
		return -ENOMEM;

}