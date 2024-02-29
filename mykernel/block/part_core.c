// block/partitions/core.c

// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 1991-1998  Linus Torvalds
 * Re-organised Feb 1998 Russell King
 * Copyright (C) 2020 Christoph Hellwig
 */
#include <linux/fs/fs.h>
#include <uapi/linux/major.h>
// #include <linux/kernel/slab.h>
#include <linux/kernel/ctype.h>
#include <linux/block/genhd.h>
// #include <linux/vmalloc.h>
// #include <linux/blktrace_api.h>
// #include <linux/raid/detect.h>
#include "part_check.h"


static int (*check_part[])(parsed_parts_s *) = {
	/*
	 * Probe partition formats with tables at disk address 0
	 * that also have an ADFS boot block at 0xdc0.
	 */
#ifdef CONFIG_ACORN_PARTITION_ICS
	adfspart_check_ICS,
#endif
#ifdef CONFIG_ACORN_PARTITION_POWERTEC
	adfspart_check_POWERTEC,
#endif
#ifdef CONFIG_ACORN_PARTITION_EESOX
	adfspart_check_EESOX,
#endif

	/*
	 * Now move on to formats that only have partition info at
	 * disk address 0xdc0.  Since these may also have stale
	 * PC/BIOS partition tables, they need to come before
	 * the msdos entry.
	 */
#ifdef CONFIG_ACORN_PARTITION_CUMANA
	adfspart_check_CUMANA,
#endif
#ifdef CONFIG_ACORN_PARTITION_ADFS
	adfspart_check_ADFS,
#endif

#ifdef CONFIG_CMDLINE_PARTITION
	cmdline_partition,
#endif
// #ifdef CONFIG_EFI_PARTITION
	efi_partition,		/* this must come before msdos */
// #endif
#ifdef CONFIG_SGI_PARTITION
	sgi_partition,
#endif
#ifdef CONFIG_LDM_PARTITION
	ldm_partition,		/* this must come before msdos */
#endif
#ifdef CONFIG_MSDOS_PARTITION
	msdos_partition,
#endif
#ifdef CONFIG_OSF_PARTITION
	osf_partition,
#endif
#ifdef CONFIG_SUN_PARTITION
	sun_partition,
#endif
#ifdef CONFIG_AMIGA_PARTITION
	amiga_partition,
#endif
#ifdef CONFIG_ATARI_PARTITION
	atari_partition,
#endif
#ifdef CONFIG_MAC_PARTITION
	mac_partition,
#endif
#ifdef CONFIG_ULTRIX_PARTITION
	ultrix_partition,
#endif
#ifdef CONFIG_IBM_PARTITION
	ibm_partition,
#endif
#ifdef CONFIG_KARMA_PARTITION
	karma_partition,
#endif
#ifdef CONFIG_SYSV68_PARTITION
	sysv68_partition,
#endif
	NULL
};

static void bdev_set_nr_sectors(blk_dev_s *bdev, sector_t sectors)
{
	// spin_lock(&bdev->bd_size_lock);
	i_size_write(bdev->bd_inode, (loff_t)sectors << SECTOR_SHIFT);
	bdev->bd_nr_sectors = sectors;
	// spin_unlock(&bdev->bd_size_lock);
}

static parsed_parts_s *allocate_partitions(gendisk_s *hd)
{
	parsed_parts_s *state;

	state = kzalloc(sizeof(*state), GFP_KERNEL);
	if (!state)
		return NULL;

	state->parts = kzalloc(
			DISK_MAX_PARTS * sizeof(state->parts[0]), GFP_KERNEL);
	if (!state->parts) {
		kfree(state);
		return NULL;
	}

	state->limit = DISK_MAX_PARTS;

	return state;
}

static void free_partitions(parsed_parts_s *state)
{
	// vfree(state->parts);
	kfree(state->parts);
	kfree(state);
}

static parsed_parts_s *check_partition(gendisk_s *hd)
{
	parsed_parts_s *state;
	int i, res, err;

	state = allocate_partitions(hd);
	if (!state)
		return NULL;
	// state->pp_buf = (char *)__get_free_page(GFP_KERNEL);
	// if (!state->pp_buf) {
	// 	free_partitions(state);
	// 	return NULL;
	// }
	// state->pp_buf[0] = '\0';

	state->disk = hd;
	// snprintf(state->name, BDEVNAME_SIZE, "%s", hd->disk_name);
	// snprintf(state->pp_buf, PAGE_SIZE, " %s:", state->name);
	// if (isdigit(state->name[strlen(state->name)-1]))
	// 	sprintf(state->name, "p");

	i = res = err = 0;
	// while (!res && check_part[i]) {
	// 	memset(state->parts, 0, state->limit * sizeof(state->parts[0]));
	// 	res = check_part[i++](state);
	// 	if (res < 0) {
	// 		/*
	// 		 * We have hit an I/O error which we don't report now.
	// 		 * But record it, and let the others do their job.
	// 		 */
	// 		err = res;
	// 		res = 0;
	// 	}

	// }
	// if (res > 0) {
	// 	printk(KERN_INFO "%s", state->pp_buf);

	// 	free_page((unsigned long)state->pp_buf);
	// 	return state;
	// }
	// if (state->access_beyond_eod)
	// 	err = -ENOSPC;
	// /*
	//  * The partition is unrecognized. So report I/O errors if there were any
	//  */
	// if (err)
	// 	res = err;
	// if (res) {
	// 	strlcat(state->pp_buf,
	// 		" unable to read partition table\n", PAGE_SIZE);
	// 	printk(KERN_INFO "%s", state->pp_buf);
	// }

	// free_page((unsigned long)state->pp_buf);
	// free_partitions(state);
	// return ERR_PTR(res);
}


static int blk_add_partitions(gendisk_s *disk)
{
	parsed_parts_s *state;
	int ret = -EAGAIN, p;

// 	if (disk->flags & GENHD_FL_NO_PART)
// 		return 0;

	state = check_partition(disk);
	if (!state)
		return 0;
// 	if (IS_ERR(state)) {
// 		/*
// 		 * I/O error reading the partition table.  If we tried to read
// 		 * beyond EOD, retry after unlocking the native capacity.
// 		 */
// 		if (PTR_ERR(state) == -ENOSPC) {
// 			printk(KERN_WARNING "%s: partition table beyond EOD, ",
// 			       disk->disk_name);
// 			if (disk_unlock_native_capacity(disk))
// 				return -EAGAIN;
// 		}
// 		return -EIO;
// 	}

// 	/*
// 	 * Partitions are not supported on host managed zoned block devices.
// 	 */
// 	if (disk->queue->limits.zoned == BLK_ZONED_HM) {
// 		pr_warn("%s: ignoring partition table on host managed zoned block device\n",
// 			disk->disk_name);
// 		ret = 0;
// 		goto out_free_state;
// 	}

// 	/*
// 	 * If we read beyond EOD, try unlocking native capacity even if the
// 	 * partition table was successfully read as we could be missing some
// 	 * partitions.
// 	 */
// 	if (state->access_beyond_eod) {
// 		printk(KERN_WARNING
// 		       "%s: partition table partially beyond EOD, ",
// 		       disk->disk_name);
// 		if (disk_unlock_native_capacity(disk))
// 			goto out_free_state;
// 	}

// 	/* tell userspace that the media / partition table may have changed */
// 	kobject_uevent(&disk_to_dev(disk)->kobj, KOBJ_CHANGE);

// 	for (p = 1; p < state->limit; p++)
// 		if (!blk_add_partition(disk, state, p))
// 			goto out_free_state;

// 	ret = 0;
// out_free_state:
// 	free_partitions(state);
// 	return ret;
}

int bdev_disk_changed(gendisk_s *disk, bool invalidate)
{
	int ret = 0;

// 	lockdep_assert_held(&disk->open_mutex);

// 	if (!disk_live(disk))
// 		return -ENXIO;

rescan:
// 	if (disk->open_partitions)
// 		return -EBUSY;
// 	sync_blockdev(disk->part0);
// 	invalidate_bdev(disk->part0);
// 	blk_drop_partitions(disk);

// 	clear_bit(GD_NEED_PART_SCAN, &disk->state);

// 	/*
// 	 * Historically we only set the capacity to zero for devices that
// 	 * support partitions (independ of actually having partitions created).
// 	 * Doing that is rather inconsistent, but changing it broke legacy
// 	 * udisks polling for legacy ide-cdrom devices.  Use the crude check
// 	 * below to get the sane behavior for most device while not breaking
// 	 * userspace for this particular setup.
// 	 */
// 	if (invalidate) {
// 		if (!(disk->flags & GENHD_FL_NO_PART) ||
// 		    !(disk->flags & GENHD_FL_REMOVABLE))
// 			set_capacity(disk, 0);
// 	}

	if (get_capacity(disk)) {
		ret = blk_add_partitions(disk);
		if (ret == -EAGAIN)
			goto rescan;
	// } else if (invalidate) {
	// 	/*
	// 	 * Tell userspace that the media / partition table may have
	// 	 * changed.
	// 	 */
	// 	kobject_uevent(&disk_to_dev(disk)->kobj, KOBJ_CHANGE);
	}

	return ret;
}