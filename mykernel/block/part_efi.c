// block/partitions/efi.c

// SPDX-License-Identifier: GPL-2.0-or-later
/************************************************************
 * EFI GUID Partition Table handling
 *
 * http://www.uefi.org/specs/
 * http://www.intel.com/technology/efi/
 *
 * efi.[ch] by Matt Domsch <Matt_Domsch@dell.com>
 *   Copyright 2000,2001,2002,2004 Dell Inc.
 *
 * TODO:
 *
 * Changelog:
 * Mon August 5th, 2013 Davidlohr Bueso <davidlohr@hp.com>
 * - detect hybrid MBRs, tighter pMBR checking & cleanups.
 *
 * Mon Nov 09 2004 Matt Domsch <Matt_Domsch@dell.com>
 * - test for valid PMBR and valid PGPT before ever reading
 *   AGPT, allow override with 'gpt' kernel command line option.
 * - check for first/last_usable_lba outside of size of disk
 *
 * Tue  Mar 26 2002 Matt Domsch <Matt_Domsch@dell.com>
 * - Ported to 2.5.7-pre1 and 2.5.7-dj2
 * - Applied patch to avoid fault in alternate header handling
 * - cleaned up find_valid_gpt
 * - On-disk structure and copy in memory is *always* LE now - 
 *   swab fields as needed
 * - remove print_gpt_header()
 * - only use first max_p partition entries, to keep the kernel minor number
 *   and partition numbers tied.
 *
 * Mon  Feb 04 2002 Matt Domsch <Matt_Domsch@dell.com>
 * - Removed __PRIPTR_PREFIX - not being used
 *
 * Mon  Jan 14 2002 Matt Domsch <Matt_Domsch@dell.com>
 * - Ported to 2.5.2-pre11 + library crc32 patch Linus applied
 *
 * Thu Dec 6 2001 Matt Domsch <Matt_Domsch@dell.com>
 * - Added compare_gpts().
 * - moved le_efi_guid_to_cpus() back into this file.  GPT is the only
 *   thing that keeps EFI GUIDs on disk.
 * - Changed gpt structure names and members to be simpler and more Linux-like.
 * 
 * Wed Oct 17 2001 Matt Domsch <Matt_Domsch@dell.com>
 * - Removed CONFIG_DEVFS_VOLUMES_UUID code entirely per Martin Wilck
 *
 * Wed Oct 10 2001 Matt Domsch <Matt_Domsch@dell.com>
 * - Changed function comments to DocBook style per Andreas Dilger suggestion.
 *
 * Mon Oct 08 2001 Matt Domsch <Matt_Domsch@dell.com>
 * - Change read_lba() to use the page cache per Al Viro's work.
 * - print u64s properly on all architectures
 * - fixed debug_printk(), now Dprintk()
 *
 * Mon Oct 01 2001 Matt Domsch <Matt_Domsch@dell.com>
 * - Style cleanups
 * - made most functions static
 * - Endianness addition
 * - remove test for second alternate header, as it's not per spec,
 *   and is unnecessary.  There's now a method to read/write the last
 *   sector of an odd-sized disk from user space.  No tools have ever
 *   been released which used this code, so it's effectively dead.
 * - Per Asit Mallick of Intel, added a test for a valid PMBR.
 * - Added kernel command line option 'gpt' to override valid PMBR test.
 *
 * Wed Jun  6 2001 Martin Wilck <Martin.Wilck@Fujitsu-Siemens.com>
 * - added devfs volume UUID support (/dev/volumes/uuids) for
 *   mounting file systems by the partition GUID. 
 *
 * Tue Dec  5 2000 Matt Domsch <Matt_Domsch@dell.com>
 * - Moved crc32() to linux/lib, added efi_crc32().
 *
 * Thu Nov 30 2000 Matt Domsch <Matt_Domsch@dell.com>
 * - Replaced Intel's CRC32 function with an equivalent
 *   non-license-restricted version.
 *
 * Wed Oct 25 2000 Matt Domsch <Matt_Domsch@dell.com>
 * - Fixed the last_lba() call to return the proper last block
 *
 * Thu Oct 12 2000 Matt Domsch <Matt_Domsch@dell.com>
 * - Thanks to Andries Brouwer for his debugging assistance.
 * - Code works, detects all the partitions.
 *
 ************************************************************/
#include <linux/kernel/kernel.h>
// #include <linux/crc32.h>
#include <linux/kernel/ctype.h>
#include <linux/kernel/math64.h>
// #include <linux/kernel/slab.h>
#include "part_check.h"
#include "efi.h"


/**
 * last_lba(): return number of last logical block of device
 * @disk: block device
 * 
 * Description: Returns last LBA value on success, 0 on error.
 * This is stored (by sd and ide-geometry) in
 *  the part[0] entry for this disk, and is the number of
 *  physical sectors available on the disk.
 */
static u64
last_lba(gendisk_s *disk) {
	// return div_u64(bdev_nr_bytes(disk->part0),
	// 	       queue_logical_block_size(disk->queue)) - 1ULL;
}


/**
 * read_lba(): Read bytes from disk, starting at given LBA
 * @state: disk parsed partitions
 * @lba: the Logical Block Address of the partition table
 * @buffer: destination buffer
 * @count: bytes to read
 *
 * Description: Reads @count bytes from @state->disk into @buffer.
 * Returns number of bytes read on success, 0 on error.
 */
static size_t
read_lba(parsed_parts_s *state, u64 lba, u8 *buffer, size_t count)
{
	size_t totalreadcount = 0;
	// sector_t n = lba *
	// 	(queue_logical_block_size(state->disk->queue) / 512);

	// if (!buffer || lba > last_lba(state->disk))
    //             return 0;

	// while (count) {
	// 	int copied = 512;
	// 	Sector sect;
	// 	unsigned char *data = read_part_sector(state, n++, &sect);
	// 	if (!data)
	// 		break;
	// 	if (copied > count)
	// 		copied = count;
	// 	memcpy(buffer, data, copied);
	// 	put_dev_sector(sect);
	// 	buffer += copied;
	// 	totalreadcount +=copied;
	// 	count -= copied;
	// }
	// return totalreadcount;
}


/**
 * find_valid_gpt() - Search disk for valid GPT headers and PTEs
 * @state: disk parsed partitions
 * @gpt: GPT header ptr, filled on return.
 * @ptes: PTEs ptr, filled on return.
 *
 * Description: Returns 1 if valid, 0 on error.
 * If valid, returns pointers to newly allocated GPT header and PTEs.
 * Validity depends on PMBR being valid (or being overridden by the
 * 'gpt' kernel command line option) and finding either the Primary
 * GPT header and PTEs valid, or the Alternate GPT header and PTEs
 * valid.  If the Primary GPT header is not valid, the Alternate GPT header
 * is not checked unless the 'gpt' kernel command line option is passed.
 * This protects against devices which misreport their size, and forces
 * the user to decide to use the Alternate GPT.
 */
static int
find_valid_gpt(parsed_parts_s *state, gpt_header **gpt, gpt_entry **ptes)
{
	int good_pgpt = 0, good_agpt = 0, good_pmbr = 0;
	gpt_header *pgpt = NULL, *agpt = NULL;
	gpt_entry *pptes = NULL, *aptes = NULL;
	legacy_mbr *legacymbr;
	gendisk_s *disk = state->disk;
	const blk_dev_ops_s *fops = disk->fops;
	sector_t total_sectors = get_capacity(state->disk);
	u64 lastlba;

	if (!ptes)
		return 0;

	lastlba = last_lba(state->disk);
// 		if (!force_gpt) {
// 		/* This will be added to the EFI Spec. per Intel after v1.02. */
// 		legacymbr = kzalloc(sizeof(*legacymbr), GFP_KERNEL);
// 		if (!legacymbr)
// 			goto fail;

		read_lba(state, 0, (u8 *)legacymbr, sizeof(*legacymbr));
// 		good_pmbr = is_pmbr_valid(legacymbr, total_sectors);
// 		kfree(legacymbr);

// 		if (!good_pmbr)
// 			goto fail;

// 		pr_debug("Device has a %s MBR\n",
// 			 good_pmbr == GPT_MBR_PROTECTIVE ?
// 						"protective" : "hybrid");
// 	}

// 	good_pgpt = is_gpt_valid(state, GPT_PRIMARY_PARTITION_TABLE_LBA,
// 				 &pgpt, &pptes);
// 		if (good_pgpt)
// 		good_agpt = is_gpt_valid(state,
// 					 le64_to_cpu(pgpt->alternate_lba),
// 					 &agpt, &aptes);
// 		if (!good_agpt && force_gpt)
// 				good_agpt = is_gpt_valid(state, lastlba, &agpt, &aptes);

// 	if (!good_agpt && force_gpt && fops->alternative_gpt_sector) {
// 		sector_t agpt_sector;
// 		int err;

// 		err = fops->alternative_gpt_sector(disk, &agpt_sector);
// 		if (!err)
// 			good_agpt = is_gpt_valid(state, agpt_sector,
// 						 &agpt, &aptes);
// 	}

// 		/* The obviously unsuccessful case */
// 		if (!good_pgpt && !good_agpt)
// 				goto fail;

// 		compare_gpts(pgpt, agpt, lastlba);

// 		/* The good cases */
// 		if (good_pgpt) {
// 				*gpt  = pgpt;
// 				*ptes = pptes;
// 				kfree(agpt);
// 				kfree(aptes);
// 		if (!good_agpt)
// 						pr_warn("Alternate GPT is invalid, using primary GPT.\n");
// 				return 1;
// 		}
// 		else if (good_agpt) {
// 				*gpt  = agpt;
// 				*ptes = aptes;
// 				kfree(pgpt);
// 				kfree(pptes);
// 		pr_warn("Primary GPT is invalid, using alternate GPT.\n");
// 				return 1;
// 		}

//  fail:
// 		kfree(pgpt);
// 		kfree(agpt);
// 		kfree(pptes);
// 		kfree(aptes);
// 		*gpt = NULL;
// 		*ptes = NULL;
// 		return 0;
}

// /**
//  * utf16_le_to_7bit(): Naively converts a UTF-16LE string to 7-bit ASCII characters
//  * @in: input UTF-16LE string
//  * @size: size of the input string
//  * @out: output string ptr, should be capable to store @size+1 characters
//  *
//  * Description: Converts @size UTF16-LE symbols from @in string to 7-bit
//  * ASCII characters and stores them to @out. Adds trailing zero to @out array.
//  */
// static void utf16_le_to_7bit(const __le16 *in, unsigned int size, u8 *out)
// {
// 	unsigned int i = 0;

// 	out[size] = 0;

// 	while (i < size) {
// 		u8 c = le16_to_cpu(in[i]) & 0xff;

// 		if (c && !isprint(c))
// 			c = '!';
// 		out[i] = c;
// 		i++;
// 	}
// }

/**
 * efi_partition - scan for GPT partitions
 * @state: disk parsed partitions
 *
 * Description: called from check.c, if the disk contains GPT
 * partitions, sets up partition entries in the kernel.
 *
 * If the first block on the disk is a legacy MBR,
 * it will get handled by msdos_partition().
 * If it's a Protective MBR, we'll handle it here.
 *
 * We do not create a Linux partition for GPT, but
 * only for the actual data partitions.
 * Returns:
 * -1 if unable to read the partition table
 *  0 if this isn't our partition table
 *  1 if successful
 *
 */
int efi_partition(parsed_parts_s *state)
{
	gpt_header *gpt = NULL;
	gpt_entry *ptes = NULL;
	u32 i;
	// unsigned ssz = queue_logical_block_size(state->disk->queue) / 512;

	if (!find_valid_gpt(state, &gpt, &ptes) || !gpt || !ptes) {
		kfree(gpt);
		kfree(ptes);
		return 0;
	}

	// pr_debug("GUID Partition Table is valid!  Yea!\n");

	// for (i = 0; i < le32_to_cpu(gpt->num_partition_entries) && i < state->limit-1; i++) {
	// 	struct partition_meta_info *info;
	// 	unsigned label_max;
	// 	u64 start = le64_to_cpu(ptes[i].starting_lba);
	// 	u64 size = le64_to_cpu(ptes[i].ending_lba) -
	// 		   le64_to_cpu(ptes[i].starting_lba) + 1ULL;

	// 	if (!is_pte_valid(&ptes[i], last_lba(state->disk)))
	// 		continue;

	// 	put_partition(state, i+1, start * ssz, size * ssz);

	// 	/* If this is a RAID volume, tell md */
	// 	if (!efi_guidcmp(ptes[i].partition_type_guid, PARTITION_LINUX_RAID_GUID))
	// 		state->parts[i + 1].flags = ADDPART_FLAG_RAID;

	// 	info = &state->parts[i + 1].info;
	// 	efi_guid_to_str(&ptes[i].unique_partition_guid, info->uuid);

	// 	/* Naively convert UTF16-LE to 7 bits. */
	// 	label_max = min(ARRAY_SIZE(info->volname) - 1,
	// 			ARRAY_SIZE(ptes[i].partition_name));
	// 	utf16_le_to_7bit(ptes[i].partition_name, label_max, info->volname);
	// 	state->parts[i + 1].has_info = true;
	// }
	// kfree(ptes);
	// kfree(gpt);
	// strlcat(state->pp_buf, "\n", PAGE_SIZE);
	// return 1;
}