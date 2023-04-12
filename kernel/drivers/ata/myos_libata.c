#include <linux/kernel/slab.h>
#include <linux/drivers/libata.h>
#include <linux/block/genhd.h>
#include <linux/block/blkdev.h>
#include <uapi/linux/major.h>

#include "libata.h"


#include <linux/fs/fs.h>
#include <obsolete/ide.h>
#include <obsolete/printk.h>

#define MAX_IDE_DEV_NR (MYOS_ATA_MAX_PORTS * ATA_MAX_DEVICES)

// ata_iops_s ioaddr_master =
// {
// 	.cmd_addr		= (void *)0x1f0,
// 	.data_addr		= (void *)0x1f0,
// 	.error_addr		= (void *)0x1f1,
// 	.feature_addr	= (void *)0x1f1,
// 	.nsect_addr		= (void *)0x1f2,
// 	.lbal_addr		= (void *)0x1f3,
// 	.lbam_addr		= (void *)0x1f4,
// 	.lbah_addr		= (void *)0x1f5,
// 	.device_addr	= (void *)0x1f6,
// 	.status_addr	= (void *)0x1f7,
// 	.command_addr	= (void *)0x1f7,
// 	.altstatus_addr	= (void *)0x3f6,
// 	.ctl_addr		= (void *)0x3f6,
// };

// ata_iops_s ioaddr_slave =
// {
// 	.cmd_addr		= (void *)0x170,
// 	.data_addr		= (void *)0x170,
// 	.error_addr		= (void *)0x171,
// 	.feature_addr	= (void *)0x171,
// 	.nsect_addr		= (void *)0x172,
// 	.lbal_addr		= (void *)0x173,
// 	.lbam_addr		= (void *)0x174,
// 	.lbah_addr		= (void *)0x175,
// 	.device_addr	= (void *)0x176,
// 	.status_addr	= (void *)0x177,
// 	.command_addr	= (void *)0x177,
// 	.altstatus_addr	= (void *)0x376,
// 	.ctl_addr		= (void *)0x376,
// };

#define atasff_pops (ata_port_ops_s *)&ata_sff_port_ops
ata_port_s	ide_ports[MYOS_ATA_MAX_PORTS] = {
	//	MASTER CONTROLLER
	{
		.port_no	= MASTER,
		.ops		= atasff_pops,
		.ioaddr		=
		{
			.cmd_addr		= 0x1f0,
			.data_addr		= 0x1f0,
			.error_addr		= 0x1f1,
			.feature_addr	= 0x1f1,
			.nsect_addr		= 0x1f2,
			.lbal_addr		= 0x1f3,
			.lbam_addr		= 0x1f4,
			.lbah_addr		= 0x1f5,
			.device_addr	= 0x1f6,
			.status_addr	= 0x1f7,
			.command_addr	= 0x1f7,
			.altstatus_addr	= 0x3f6,
			.ctl_addr		= 0x3f6,
		},
	},
	//	SLAVE CONTROLLER
	{
		.port_no	= SLAVE,
		.ops		= atasff_pops,
		.ioaddr		=
		{
			.cmd_addr		= 0x170,
			.data_addr		= 0x170,
			.error_addr		= 0x171,
			.feature_addr	= 0x171,
			.nsect_addr		= 0x172,
			.lbal_addr		= 0x173,
			.lbam_addr		= 0x174,
			.lbah_addr		= 0x175,
			.device_addr	= 0x176,
			.status_addr	= 0x177,
			.command_addr	= 0x177,
			.altstatus_addr	= 0x376,
			.ctl_addr		= 0x376,
		},
	},
};

ata_dev_s	ide_devs[MYOS_ATA_MAX_PORTS][ATA_MAX_DEVICES] = {
	//	MASTER CONTROLLER
	{
		// MASTER DISK
		{
			.devno	= MASTER,
			.name	= "hda",
			.ap		= &(ide_ports[MASTER]),
		},
		// SLAVE DISK
		{
			.devno	= SLAVE,
			.name	= "hdb",
			.ap		= &(ide_ports[MASTER]),
		}
	},
	//	SLAVE CONTROLLER
	{
		// MASTER DISK
		{
			.devno	= MASTER,
			.name	= "hdc",
			.ap		= &(ide_ports[SLAVE]),
		},
		// SLAVE DISK
		{
			.devno	= SLAVE,
			.name	= "hdd",
			.ap		= &(ide_ports[SLAVE]),
		}
	}
};

static const blk_dev_ops_s ata_fops = {

};


long ATA_disk_transfer(unsigned controller, unsigned disk, long cmd,
		unsigned long blk_idx, long count, unsigned char * buffer);
static char *__myos_ide_read_block(blk_dev_s *bdev, sector_t start, sector_t nr)
{
	ata_dev_s *ad;
	ata_port_s *ap;
	char *tmp_buf;

	tmp_buf = kmalloc(nr * SECTOR_SIZE, GFP_KERNEL);
	if (tmp_buf == NULL)
		return ERR_PTR(-ENOMEM);

	ad = myos_gendisk_to_atadev(bdev->bd_disk);
	ap = ad->ap;
	ATA_disk_transfer(ap->port_no, ad->devno, ATA_READ_CMD,
			start + bdev->bd_start_sect, nr, tmp_buf);

	return tmp_buf;
}

static void __myos_ide_write_block(blk_dev_s *bdev, char *buf, sector_t start, sector_t nr)
{
	ata_dev_s *ad;
	ata_port_s *ap;

	ad = myos_gendisk_to_atadev(bdev->bd_disk);
	ap = ad->ap;
	ATA_disk_transfer(ap->port_no, ad->devno, ATA_WRITE_CMD,
			start + bdev->bd_start_sect, nr, buf);
}

ssize_t myos_ide_read(file_s *file, char *buf, size_t size, loff_t *pos)
{
	loff_t offset;
	loff_t end;
	sector_t sec_start;
	sector_t sec_nr;
	inode_s *inode;
	blk_dev_s *bdev;
	char *blkbuf;

	offset = *pos % SECTOR_SIZE;
	end = ((*pos + size + SECTOR_SIZE - 1) & SECTOR_ALIGN) - (*pos & SECTOR_ALIGN);
	sec_start = *pos >> SECTOR_SHIFT;
	sec_nr = end / SECTOR_SIZE;
	inode = file->f_path.dentry->d_inode;
	bdev = blkdev_get_by_dev(inode->i_rdev, file->f_mode);
	blkbuf = __myos_ide_read_block(bdev, sec_start, sec_nr);

	memcpy(buf, blkbuf + offset, size);
}

ssize_t myos_ide_write(file_s *file, const char *buf, size_t size, loff_t *pos)
{
	loff_t offset;
	loff_t end;
	sector_t sec_start;
	sector_t sec_nr;
	inode_s *inode;
	blk_dev_s *bdev;
	char *blkbuf;

	offset = *pos % SECTOR_SIZE;
	end = ((*pos + size + SECTOR_SIZE - 1) & SECTOR_ALIGN) - (*pos & SECTOR_ALIGN);
	sec_start = *pos >> SECTOR_SHIFT;
	sec_nr = end / SECTOR_SIZE;
	inode = file->f_path.dentry->d_inode;
	bdev = blkdev_get_by_dev(inode->i_rdev, file->f_mode);
	blkbuf = __myos_ide_read_block(bdev, sec_start, sec_nr);

	memcpy(blkbuf + offset, buf, size);

	__myos_ide_write_block(bdev, blkbuf, sec_start, sec_nr);
}

static const file_ops_s myos_ide_fops = {
	.read = myos_ide_read,
	.write = myos_ide_write,
};

/**
 *	sd_revalidate_disk - called the first time a new disk is seen,
 *	performs disk spin up, read_capacity, etc.
 *	@disk: struct gendisk we care about
 **/
static int myos_ata_revalidate_disk(gendisk_s *gd, ata_dev_s *disk)
{
// 	struct scsi_disk *sdkp = scsi_disk(disk);
// 	struct scsi_device *sdp = sdkp->device;
// 	struct request_queue *q = sdkp->disk->queue;
// 	sector_t old_capacity = sdkp->capacity;
// 	unsigned char *buffer;
// 	unsigned int dev_max, rw_max;

// 	SCSI_LOG_HLQUEUE(3, sd_printk(KERN_INFO, sdkp,
// 				      "sd_revalidate_disk\n"));

// 	/*
// 	 * If the device is offline, don't try and read capacity or any
// 	 * of the other niceties.
// 	 */
// 	if (!scsi_device_online(sdp))
// 		goto out;

// 	buffer = kmalloc(SD_BUF_SIZE, GFP_KERNEL);
// 	if (!buffer) {
// 		sd_printk(KERN_WARNING, sdkp, "sd_revalidate_disk: Memory "
// 			  "allocation failure.\n");
// 		goto out;
// 	}

// 	sd_spinup_disk(sdkp);

// 	/*
// 	 * Without media there is no reason to ask; moreover, some devices
// 	 * react badly if we do.
// 	 */
// 	if (sdkp->media_present) {
// 		sd_read_capacity(sdkp, buffer);

// 		/*
// 		 * set the default to rotational.  All non-rotational devices
// 		 * support the block characteristics VPD page, which will
// 		 * cause this to be updated correctly and any device which
// 		 * doesn't support it should be treated as rotational.
// 		 */
// 		blk_queue_flag_clear(QUEUE_FLAG_NONROT, q);
// 		blk_queue_flag_set(QUEUE_FLAG_ADD_RANDOM, q);

// 		if (scsi_device_supports_vpd(sdp)) {
// 			sd_read_block_provisioning(sdkp);
// 			sd_read_block_limits(sdkp);
// 			sd_read_block_characteristics(sdkp);
// 			sd_zbc_read_zones(sdkp, buffer);
// 			sd_read_cpr(sdkp);
// 		}

// 		sd_print_capacity(sdkp, old_capacity);

// 		sd_read_write_protect_flag(sdkp, buffer);
// 		sd_read_cache_type(sdkp, buffer);
// 		sd_read_app_tag_own(sdkp, buffer);
// 		sd_read_write_same(sdkp, buffer);
// 		sd_read_security(sdkp, buffer);
// 	}

// 	/*
// 	 * We now have all cache related info, determine how we deal
// 	 * with flush requests.
// 	 */
// 	sd_set_flush_flag(sdkp);

// 	/* Initial block count limit based on CDB TRANSFER LENGTH field size. */
// 	dev_max = sdp->use_16_for_rw ? SD_MAX_XFER_BLOCKS : SD_DEF_XFER_BLOCKS;

// 	/* Some devices report a maximum block count for READ/WRITE requests. */
// 	dev_max = min_not_zero(dev_max, sdkp->max_xfer_blocks);
// 	q->limits.max_dev_sectors = logical_to_sectors(sdp, dev_max);

// 	if (sd_validate_opt_xfer_size(sdkp, dev_max)) {
// 		q->limits.io_opt = logical_to_bytes(sdp, sdkp->opt_xfer_blocks);
// 		rw_max = logical_to_sectors(sdp, sdkp->opt_xfer_blocks);
// 	} else {
// 		q->limits.io_opt = 0;
// 		rw_max = min_not_zero(logical_to_sectors(sdp, dev_max),
// 				      (sector_t)BLK_DEF_MAX_SECTORS);
// 	}

// 	/* Do not exceed controller limit */
// 	rw_max = min(rw_max, queue_max_hw_sectors(q));

// 	/*
// 	 * Only update max_sectors if previously unset or if the current value
// 	 * exceeds the capabilities of the hardware.
// 	 */
// 	if (sdkp->first_scan ||
// 	    q->limits.max_sectors > q->limits.max_dev_sectors ||
// 	    q->limits.max_sectors > q->limits.max_hw_sectors)
// 		q->limits.max_sectors = rw_max;

// 	sdkp->first_scan = 0;

// 	set_capacity_and_notify(disk, logical_to_sectors(sdp, sdkp->capacity));
	set_capacity(gd, disk->n_sectors);
// 	sd_config_write_same(sdkp);
// 	kfree(buffer);

// 	/*
// 	 * For a zoned drive, revalidating the zones can be done only once
// 	 * the gendisk capacity is set. So if this fails, set back the gendisk
// 	 * capacity to 0.
// 	 */
// 	if (sd_zbc_revalidate_zones(sdkp))
// 		set_capacity_and_notify(disk, 0);

//  out:
// 	return 0;
}

/**
 *	sd_format_disk_name - format disk name
 *	@prefix: name prefix - ie. "sd" for SCSI disks
 *	@index: index of the disk to format name for
 *	@buf: output buffer
 *	@buflen: length of the output buffer
 *
 *	SCSI disk names starts at sda.  The 26th device is sdz and the
 *	27th is sdaa.  The last one for two lettered suffix is sdzz
 *	which is followed by sdaaa.
 *
 *	This is basically 26 base counting with one extra 'nil' entry
 *	at the beginning from the second digit on and can be
 *	determined using similar method as 26 base conversion with the
 *	index shifted -1 after each digit is computed.
 *
 *	CONTEXT:
 *	Don't care.
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
static int myos_ata_format_disk_name(char *prefix,
		int index, char *buf, int buflen)
{
	const int base = 'z' - 'a' + 1;
	char *begin = buf + strlen(prefix);
	char *end = buf + buflen;
	char *p;
	int unit;

	p = end - 1;
	*p = '\0';
	unit = base;
	do {
		if (p == begin)
			return -EINVAL;
		*--p = 'a' + (index % unit);
		index = (index / unit) - 1;
	} while (index >= 0);

	memmove(begin, p, end - p);
	memcpy(buf, prefix, strlen(prefix));

	return 0;
}

int myos_ata_port_probe(ata_dev_s *disk)
{
	gendisk_s *gd;
	int error;

	u16 *ataid = disk->id;
	ata_port_s *ap = disk->ap;
	ata_iops_s *ioaddr = &(ap->ioaddr);
	outb(0, ioaddr->ctl_addr);

	error = -ENOMEM;
	// gd = __myos_alloc_disk(NULL);
	// if (gd == NULL)
	// 	goto out_free;
	gd = &disk->gd;
	__myos_init_disk(gd, NULL);

	memcpy(gd->disk_name, disk->name, strlen(disk->name));
	if (ap->port_no == MASTER)
		gd->major = IDE0_MAJOR;
	else if (ap->port_no == SLAVE)
		gd->major = IDE1_MAJOR;
	else
		while (1);
	gd->first_minor = disk->devno;
	gd->minors = MAX_IDE_DEV_NR;	
	gd->fops = &ata_fops;
	gd->myos_bd_fops = &myos_ide_fops;

	// 	ATA_disk_transfer(MASTER, MASTER, ATA_INFO_CMD, 0, 0,
	// 					(unsigned char *)&ide_disk_info[0]);
	ATA_disk_transfer(ap->port_no, disk->devno, ATA_CMD_STANDBYNOW1,
			0, 0, (unsigned char *)ataid);
	// myos_ata_dev_read_id(disk, ataid);

	ata_dev_configure(disk);

	myos_ata_revalidate_disk(gd, disk);

	myos_device_add_disk(gd);

	return 0;

 out_free_index:
	// ida_free(&sd_index_ida, index);
 out_put:
	// put_disk(gd);
 out_free:
	// sd_zbc_release_disk(sdkp);
	// kfree(sdkp);
 out:
	// scsi_autopm_put_device(sdp);
	return error;
}

void myos_ata_probe()
{
	for (int i = 0; i < MYOS_ATA_MAX_PORTS; i++)
	{
		ata_port_s *ap = &(ide_ports[i]);
		for (int j = 0; j < ATA_MAX_DEVICES; j++)
		{
			if (ata_devchk(ap, ap->port_no))
			{
				ata_dev_s *disk = &(ide_devs[i][j]);
				myos_ata_port_probe(disk);
			}
		}
	}
}