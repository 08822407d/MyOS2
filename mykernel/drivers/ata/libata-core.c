// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  libata-core.c - helper library for ATA
 *
 *  Copyright 2003-2004 Red Hat, Inc.  All rights reserved.
 *  Copyright 2003-2004 Jeff Garzik
 *
 *  libata documentation is available via 'make {ps|pdf}docs',
 *  as Documentation/driver-api/libata.rst
 *
 *  Hardware documentation available from http://www.t13.org/ and
 *  http://www.sata-io.org/
 *
 *  Standards documents from:
 *	http://www.t13.org (ATA standards, PCI DMA IDE spec)
 *	http://www.t10.org (SCSI MMC - for ATAPI MMC)
 *	http://www.sata-io.org (SATA)
 *	http://www.compactflash.org (CF)
 *	http://www.qic.org (QIC157 - Tape and DSC)
 *	http://www.ce-ata.org (CE-ATA: not supported)
 *
 * libata is essentially a library of internal helper functions for
 * low-level ATA host controller drivers.  As such, the API/ABI is
 * likely to change as new drivers are added and updated.
 * Do not depend on ABI/API stability.
 */

#include <linux/kernel/kernel.h>
// #include <linux/module.h>
#include <linux/device/pci.h>
#include <linux/init/init.h>
#include <linux/kernel/lib.h>
#include <linux/kernel/mm.h>
#include <linux/kernel/lock_ipc.h>
#include <linux/block/blkdev.h>
#include <linux/kernel/delay.h>
// #include <linux/timer.h>
#include <linux/kernel/time.h>
// #include <linux/interrupt.h>
// #include <linux/suspend.h>
#include <linux/kernel/kactive_api.h>
// #include <linux/scatterlist.h>
#include <linux/kernel/log2.h>
#include <linux/kernel/mm_api.h>
// #include <linux/glob.h>
// #include <scsi/scsi.h>
// #include <scsi/scsi_cmnd.h>
// #include <scsi/scsi_host.h>
#include <linux/drivers/libata.h>
// #include <asm/byteorder.h>
#include <asm-generic/unaligned.h>
// #include <linux/cdrom.h>
// #include <linux/ratelimit.h>
// #include <linux/leds.h>
// #include <linux/pm_runtime.h>
// #include <linux/platform_device.h>
#include <asm/setup.h>

// #define CREATE_TRACE_POINTS
// #include <trace/events/libata.h>

#include "libata.h"
// #include "libata-transport.h"


static u64 ata_id_n_sectors(const u16 *id)
{
	if (ata_id_has_lba(id)) {
		if (ata_id_has_lba48(id))
			return ata_id_u64(id, ATA_ID_LBA_CAPACITY_2);
		else
			return ata_id_u32(id, ATA_ID_LBA_CAPACITY);
	} else {
		if (ata_id_current_chs_valid(id))
			return id[ATA_ID_CUR_CYLS] * id[ATA_ID_CUR_HEADS] *
				   id[ATA_ID_CUR_SECTORS];
		else
			return id[ATA_ID_CYLS] * id[ATA_ID_HEADS] *
				   id[ATA_ID_SECTORS];
	}
}

u64 ata_tf_to_lba48(const ata_tf_s *tf)
{
	u64 sectors = 0;

	sectors |= ((u64)(tf->hob_lbah & 0xff)) << 40;
	sectors |= ((u64)(tf->hob_lbam & 0xff)) << 32;
	sectors |= ((u64)(tf->hob_lbal & 0xff)) << 24;
	sectors |= (tf->lbah & 0xff) << 16;
	sectors |= (tf->lbam & 0xff) << 8;
	sectors |= (tf->lbal & 0xff);

	return sectors;
}

u64 ata_tf_to_lba(const ata_tf_s *tf)
{
	u64 sectors = 0;

	sectors |= (tf->device & 0x0f) << 24;
	sectors |= (tf->lbah & 0xff) << 16;
	sectors |= (tf->lbam & 0xff) << 8;
	sectors |= (tf->lbal & 0xff);

	return sectors;
}


unsigned myos_ata_exec_internal(ata_dev_s *dev, ata_tf_s *tf, void *buf ,size_t len)
{
	ata_port_s *ap = dev->ap;
	ata_q_cmd_s *qc = &(ap->qcmd[ATA_TAG_INTERNAL]);

	qc->ap = ap;
	qc->dev = dev;
	ata_qc_reinit(qc);

	qc->tf = *tf;
	qc->buf = buf;
	qc->nbytes = len;

	myos_ata_qc_issue(qc);
}

int myos_ata_dev_read_id(ata_dev_s *dev, u16 *id)
{
	ata_tf_s tf;
	ata_tf_init(dev, &tf);

	tf.command = ATA_CMD_ID_ATA;

	tf.protocol = ATA_PROT_PIO;
	/* Some devices choke if TF registers contain garbage.  Make
	 * sure those are properly initialized.
	 */
	tf.flags |= ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE;
	/* Device presence detection is unreliable on some
	 * controllers.  Always poll IDENTIFY if available.
	 */
	tf.flags |= ATA_TFLAG_POLLING;

	myos_ata_exec_internal(dev, &tf, (void *)id, SECTOR_SIZE);
}



static int ata_dev_config_lba(ata_dev_s *dev)
{
	const u16 *id = dev->id;
	const char *lba_desc;
	char ncq_desc[24];
	int ret = 0;

	dev->flags |= ATA_DFLAG_LBA;

	if (ata_id_has_lba48(id)) {
		lba_desc = "LBA48";
		dev->flags |= ATA_DFLAG_LBA48;
		if (dev->n_sectors >= (1UL << 28) &&
			ata_id_has_flush_ext(id))
			dev->flags |= ATA_DFLAG_FLUSH_EXT;
	} else {
		lba_desc = "LBA";
	}

	// /* config NCQ */
	// ret = ata_dev_config_ncq(dev, ncq_desc, sizeof(ncq_desc));

	// /* print device info to dmesg */
	// if (ata_dev_print_info(dev))
	// 	ata_dev_info(dev,
	// 		     "%llu sectors, multi %u: %s %s\n",
	// 		     (unsigned long long)dev->n_sectors,
	// 		     dev->multi_count, lba_desc, ncq_desc);

	return ret;
}


static void ata_dev_config_chs(ata_dev_s *dev)
{
	const u16 *id = dev->id;

	if (ata_id_current_chs_valid(id)) {
		/* Current CHS translation is valid. */
		dev->cylinders = id[54];
		dev->heads     = id[55];
		dev->sectors   = id[56];
	} else {
		/* Default translation */
		dev->cylinders	= id[1];
		dev->heads	= id[3];
		dev->sectors	= id[6];
	}

	// /* print device info to dmesg */
	// if (ata_dev_print_info(dev))
	// 	ata_dev_info(dev,
	// 		     "%llu sectors, multi %u, CHS %u/%u/%u\n",
	// 		     (unsigned long long)dev->n_sectors,
	// 		     dev->multi_count, dev->cylinders,
	// 		     dev->heads, dev->sectors);
}


/**
 *	ata_dev_configure - Configure the specified ATA/ATAPI device
 *	@dev: Target device to configure
 *
 *	Configure @dev according to @dev->id.  Generic and low-level
 *	driver specific fixups are also applied.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, -errno otherwise
 */
int ata_dev_configure(ata_dev_s *dev)
{
	ata_port_s *ap = dev->ap;
	// bool print_info = ata_dev_print_info(dev);
	const u16 *id = dev->id;
	// unsigned long xfer_mask;
	// unsigned int err_mask;
	// char revbuf[7];		/* XYZ-99\0 */
	// char fwrevbuf[ATA_ID_FW_REV_LEN+1];
	// char modelbuf[ATA_ID_PROD_LEN+1];
	int rc;

	// if (!ata_dev_enabled(dev)) {
	// 	ata_dev_dbg(dev, "no device\n");
	// 	return 0;
	// }

	// /* set horkage */
	// dev->horkage |= ata_dev_blacklisted(dev);
	// ata_force_horkage(dev);

	// if (dev->horkage & ATA_HORKAGE_DISABLE) {
	// 	ata_dev_info(dev, "unsupported device, disabling\n");
	// 	ata_dev_disable(dev);
	// 	return 0;
	// }

	// if ((!atapi_enabled || (ap->flags & ATA_FLAG_NO_ATAPI)) &&
	// 	dev->class == ATA_DEV_ATAPI) {
	// 	ata_dev_warn(dev, "WARNING: ATAPI is %s, device ignored\n",
	// 			 atapi_enabled ? "not supported with this driver"
	// 			 : "disabled");
	// 	ata_dev_disable(dev);
	// 	return 0;
	// }

	// rc = ata_do_link_spd_horkage(dev);
	// if (rc)
	// 	return rc;

	// /* some WD SATA-1 drives have issues with LPM, turn on NOLPM for them */
	// if ((dev->horkage & ATA_HORKAGE_WD_BROKEN_LPM) &&
	// 	(id[ATA_ID_SATA_CAPABILITY] & 0xe) == 0x2)
	// 	dev->horkage |= ATA_HORKAGE_NOLPM;

	// if (ap->flags & ATA_FLAG_NO_LPM)
	// 	dev->horkage |= ATA_HORKAGE_NOLPM;

	// if (dev->horkage & ATA_HORKAGE_NOLPM) {
	// 	ata_dev_warn(dev, "LPM support broken, forcing max_power\n");
	// 	dev->link->ap->target_lpm_policy = ATA_LPM_MAX_POWER;
	// }

	// /* let ACPI work its magic */
	// rc = ata_acpi_on_devcfg(dev);
	// if (rc)
	// 	return rc;

	// /* massage HPA, do it early as it might change IDENTIFY data */
	// rc = ata_hpa_resize(dev);
	// if (rc)
	// 	return rc;

	// /* print device capabilities */
	// ata_dev_dbg(dev,
	// 		"%s: cfg 49:%04x 82:%04x 83:%04x 84:%04x "
	// 		"85:%04x 86:%04x 87:%04x 88:%04x\n",
	// 		__func__,
	// 		id[49], id[82], id[83], id[84],
	// 		id[85], id[86], id[87], id[88]);

	/* initialize to-be-configured parameters */
	dev->flags			&= ~ATA_DFLAG_CFG_MASK;
	// dev->max_sectors	= 0;
	// dev->cdb_len		= 0;
	dev->n_sectors		= 0;
	dev->cylinders		= 0;
	dev->heads			= 0;
	dev->sectors		= 0;
	// dev->multi_count	= 0;

	/*
	 * common ATA, ATAPI feature tests
	 */

	// /* find max transfer mode; for printk only */
	// xfer_mask = ata_id_xfermask(id);

	// ata_dump_id(dev, id);

	// /* SCSI only uses 4-char revisions, dump full 8 chars from ATA */
	// ata_id_c_string(dev->id, fwrevbuf, ATA_ID_FW_REV,
	// 		sizeof(fwrevbuf));

	// ata_id_c_string(dev->id, modelbuf, ATA_ID_PROD,
	// 		sizeof(modelbuf));

	// /* ATA-specific feature tests */
	// if (dev->class == ATA_DEV_ATA || dev->class == ATA_DEV_ZAC) {
	// 	if (ata_id_is_cfa(id)) {
	// 		/* CPRM may make this media unusable */
	// 		if (id[ATA_ID_CFA_KEY_MGMT] & 1)
	// 			ata_dev_warn(dev,
	// "supports DRM functions and may not be fully accessible\n");
	// 		snprintf(revbuf, 7, "CFA");
	// 	} else {
	// 		snprintf(revbuf, 7, "ATA-%d", ata_id_major_version(id));
	// 		/* Warn the user if the device has TPM extensions */
	// 		if (ata_id_has_tpm(id))
	// 			ata_dev_warn(dev,
	// "supports DRM functions and may not be fully accessible\n");
	// 	}

		dev->n_sectors = ata_id_n_sectors(id);

		/* get current R/W Multiple count setting */
		if ((dev->id[47] >> 8) == 0x80 && (dev->id[59] & 0x100)) {
			unsigned int max = dev->id[47] & 0xff;
			unsigned int cnt = dev->id[59] & 0xff;
			/* only recognize/allow powers of two here */
			if (is_power_of_2(max) && is_power_of_2(cnt))
				if (cnt <= max)
					dev->multi_count = cnt;
		}

		// /* print device info to dmesg */
		// if (print_info)
		// 	ata_dev_info(dev, "%s: %s, %s, max %s\n",
		// 			 revbuf, modelbuf, fwrevbuf,
		// 			 ata_mode_string(xfer_mask));

		if (ata_id_has_lba(id)) {
			rc = ata_dev_config_lba(dev);
			if (rc)
				return rc;
		} else {
			ata_dev_config_chs(dev);
		}

		// ata_dev_config_devslp(dev);
		// ata_dev_config_sense_reporting(dev);
		// ata_dev_config_zac(dev);
		// ata_dev_config_trusted(dev);
		// ata_dev_config_cpr(dev);
		// dev->cdb_len = 32;

// 		if (print_info)
// 			ata_dev_print_features(dev);
// 	}

// 	/* ATAPI-specific feature tests */
// 	else if (dev->class == ATA_DEV_ATAPI) {
// 		const char *cdb_intr_string = "";
// 		const char *atapi_an_string = "";
// 		const char *dma_dir_string = "";
// 		u32 sntf;

// 		rc = atapi_cdb_len(id);
// 		if ((rc < 12) || (rc > ATAPI_CDB_LEN)) {
// 			ata_dev_warn(dev, "unsupported CDB len %d\n", rc);
// 			rc = -EINVAL;
// 			goto err_out_nosup;
// 		}
// 		dev->cdb_len = (unsigned int) rc;

// 		/* Enable ATAPI AN if both the host and device have
// 		 * the support.  If PMP is attached, SNTF is required
// 		 * to enable ATAPI AN to discern between PHY status
// 		 * changed notifications and ATAPI ANs.
// 		 */
// 		if (atapi_an &&
// 			(ap->flags & ATA_FLAG_AN) && ata_id_has_atapi_AN(id) &&
// 			(!sata_pmp_attached(ap) ||
// 			 sata_scr_read(&ap->link, SCR_NOTIFICATION, &sntf) == 0)) {
// 			/* issue SET feature command to turn this on */
// 			err_mask = ata_dev_set_feature(dev,
// 					SETFEATURES_SATA_ENABLE, SATA_AN);
// 			if (err_mask)
// 				ata_dev_err(dev,
// 						"failed to enable ATAPI AN (err_mask=0x%x)\n",
// 						err_mask);
// 			else {
// 				dev->flags |= ATA_DFLAG_AN;
// 				atapi_an_string = ", ATAPI AN";
// 			}
// 		}

// 		if (ata_id_cdb_intr(dev->id)) {
// 			dev->flags |= ATA_DFLAG_CDB_INTR;
// 			cdb_intr_string = ", CDB intr";
// 		}

// 		if (atapi_dmadir || (dev->horkage & ATA_HORKAGE_ATAPI_DMADIR) || atapi_id_dmadir(dev->id)) {
// 			dev->flags |= ATA_DFLAG_DMADIR;
// 			dma_dir_string = ", DMADIR";
// 		}

// 		if (ata_id_has_da(dev->id)) {
// 			dev->flags |= ATA_DFLAG_DA;
// 			zpodd_init(dev);
// 		}

// 		/* print device info to dmesg */
// 		if (print_info)
// 			ata_dev_info(dev,
// 					 "ATAPI: %s, %s, max %s%s%s%s\n",
// 					 modelbuf, fwrevbuf,
// 					 ata_mode_string(xfer_mask),
// 					 cdb_intr_string, atapi_an_string,
// 					 dma_dir_string);
// 	}

	/* determine max_sectors */
	dev->max_sectors = ATA_MAX_SECTORS;
	if (dev->flags & ATA_DFLAG_LBA48)
		dev->max_sectors = ATA_MAX_SECTORS_LBA48;

// 	/* Limit PATA drive on SATA cable bridge transfers to udma5,
// 	   200 sectors */
// 	if (ata_dev_knobble(dev)) {
// 		if (print_info)
// 			ata_dev_info(dev, "applying bridge limits\n");
// 		dev->udma_mask &= ATA_UDMA5;
// 		dev->max_sectors = ATA_MAX_SECTORS;
// 	}

// 	if ((dev->class == ATA_DEV_ATAPI) &&
// 		(atapi_command_packet_set(id) == TYPE_TAPE)) {
// 		dev->max_sectors = ATA_MAX_SECTORS_TAPE;
// 		dev->horkage |= ATA_HORKAGE_STUCK_ERR;
// 	}

// 	if (dev->horkage & ATA_HORKAGE_MAX_SEC_128)
// 		dev->max_sectors = min_t(unsigned int, ATA_MAX_SECTORS_128,
// 					 dev->max_sectors);

// 	if (dev->horkage & ATA_HORKAGE_MAX_SEC_1024)
// 		dev->max_sectors = min_t(unsigned int, ATA_MAX_SECTORS_1024,
// 					 dev->max_sectors);

// 	if (dev->horkage & ATA_HORKAGE_MAX_SEC_LBA48)
// 		dev->max_sectors = ATA_MAX_SECTORS_LBA48;

// 	if (ap->ops->dev_config)
// 		ap->ops->dev_config(dev);

// 	if (dev->horkage & ATA_HORKAGE_DIAGNOSTIC) {
// 		/* Let the user know. We don't want to disallow opens for
// 		   rescue purposes, or in case the vendor is just a blithering
// 		   idiot. Do this after the dev_config call as some controllers
// 		   with buggy firmware may want to avoid reporting false device
// 		   bugs */

// 		if (print_info) {
// 			ata_dev_warn(dev,
// "Drive reports diagnostics failure. This may indicate a drive\n");
// 			ata_dev_warn(dev,
// "fault or invalid emulation. Contact drive vendor for information.\n");
// 		}
// 	}

// 	if ((dev->horkage & ATA_HORKAGE_FIRMWARE_WARN) && print_info) {
// 		ata_dev_warn(dev, "WARNING: device requires firmware update to be fully functional\n");
// 		ata_dev_warn(dev, "         contact the vendor or visit http://ata.wiki.kernel.org\n");
// 	}

	return 0;

// err_out_nosup:
// 	return rc;
}



/**
 *	ata_qc_issue - issue taskfile to device
 *	@qc: command to issue to device
 *
 *	Prepare an ATA command to submission to device.
 *	This includes mapping the data into a DMA-able
 *	area, filling in the S/G table, and finally
 *	writing the taskfile to hardware, starting the command.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */
// void ata_qc_issue(struct ata_queued_cmd *qc)
void myos_ata_qc_issue(ata_q_cmd_s *qc)
{
	unsigned int err = 0;
	/* defer PIO handling to sff_qc_issue */
	if (!ata_is_dma(qc->tf.protocol))
		err = myos_ata_pio_qc_issue(qc);
	else
		err = myos_ata_dma_qc_issue(qc);
}