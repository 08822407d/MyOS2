/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *  libata.h - helper library for ATA
 *
 *  Copyright 2003-2004 Red Hat, Inc.  All rights reserved.
 *  Copyright 2003-2004 Jeff Garzik
 *
 *  libata documentation is available via 'make {ps|pdf}docs',
 *  as Documentation/driver-api/libata.rst
 */

#ifndef __LIBATA_H__
#define __LIBATA_H__

	// #define DRV_NAME	"libata"
	// #define DRV_VERSION	"3.00"	/* must be exactly four chars */

	// /* libata-core.c */
	// enum {
	// 	/* flags for ata_dev_read_id() */
	// 	ATA_READID_POSTRESET	= (1 << 0), /* reading ID after reset */

	// 	/* selector for ata_down_xfermask_limit() */
	// 	ATA_DNXFER_PIO		= 0,	/* speed down PIO */
	// 	ATA_DNXFER_DMA		= 1,	/* speed down DMA */
	// 	ATA_DNXFER_40C		= 2,	/* apply 40c cable limit */
	// 	ATA_DNXFER_FORCE_PIO	= 3,	/* force PIO */
	// 	ATA_DNXFER_FORCE_PIO0	= 4,	/* force PIO0 */

	// 	ATA_DNXFER_QUIET	= (1 << 31),
	// };

	// extern atomic_t ata_print_id;
	// extern int atapi_passthru16;
	// extern int libata_fua;
	// extern int libata_noacpi;
	// extern int libata_allow_tpm;
	// extern const struct device_type ata_port_type;
	// extern struct ata_link *ata_dev_phys_link(ata_dev_s *dev);
	// #ifdef CONFIG_ATA_FORCE
	// extern void ata_force_cbl(struct ata_port *ap);
	// #else
	// static inline void ata_force_cbl(struct ata_port *ap) { }
	// #endif
	extern u64 ata_tf_to_lba(const ata_tf_s *tf);
	extern u64 ata_tf_to_lba48(const ata_tf_s *tf);
	// extern ata_q_cmd_s *ata_qc_new_init(ata_dev_s *dev, int tag);
	// extern int ata_build_rw_tf(ata_tf_s *tf, ata_dev_s *dev,
	// 			u64 block, u32 n_block, unsigned int tf_flags,
	// 			unsigned int tag, int class);
	// extern u64 ata_tf_read_block(const ata_tf_s *tf,
	// 				ata_dev_s *dev);
	// extern unsigned ata_exec_internal(ata_dev_s *dev,
	// 				ata_tf_s *tf, const u8 *cdb,
	// 				int dma_dir, void *buf, unsigned int buflen,
	// 				unsigned long timeout);
	// extern unsigned ata_exec_internal_sg(ata_dev_s *dev,
	// 					ata_tf_s *tf, const u8 *cdb,
	// 					int dma_dir, struct scatterlist *sg,
	// 					unsigned int n_elem, unsigned long timeout);
	// extern int ata_wait_ready(struct ata_link *link, unsigned long deadline,
	// 			int (*check_ready)(struct ata_link *link));
	// extern int ata_dev_read_id(ata_dev_s *dev, unsigned int *p_class,
	// 			unsigned int flags, u16 *id);
	// extern int ata_dev_reread_id(ata_dev_s *dev, unsigned int readid_flags);
	// extern int ata_dev_revalidate(ata_dev_s *dev, unsigned int new_class,
	// 				unsigned int readid_flags);
	extern int ata_dev_configure(ata_dev_s *dev);
	// extern int sata_down_spd_limit(struct ata_link *link, u32 spd_limit);
	// extern int ata_down_xfermask_limit(ata_dev_s *dev, unsigned int sel);
	// extern unsigned int ata_dev_set_feature(ata_dev_s *dev,
	// 					u8 enable, u8 feature);
	// extern void ata_qc_free(ata_q_cmd_s *qc);
	// extern void ata_qc_issue(ata_q_cmd_s *qc);
	extern void myos_ata_qc_issue(ata_q_cmd_s *qc);
	// extern void __ata_qc_complete(ata_q_cmd_s *qc);
	// extern int atapi_check_dma(ata_q_cmd_s *qc);
	// extern void swap_buf_le16(u16 *buf, unsigned int buf_words);
	// extern bool ata_phys_link_online(struct ata_link *link);
	// extern bool ata_phys_link_offline(struct ata_link *link);
	// extern void ata_dev_init(ata_dev_s *dev);
	// extern void ata_link_init(struct ata_port *ap, struct ata_link *link, int pmp);
	// extern int sata_link_init_spd(struct ata_link *link);
	// extern int ata_task_ioctl(struct scsi_device *scsidev, void __user *arg);
	// extern int ata_cmd_ioctl(struct scsi_device *scsidev, void __user *arg);
	// extern struct ata_port *ata_port_alloc(struct ata_host *host);
	// extern const char *sata_spd_string(unsigned int spd);
	// extern int ata_port_probe(struct ata_port *ap);
	// extern void __ata_port_probe(struct ata_port *ap);
	// extern unsigned int ata_read_log_page(ata_dev_s *dev, u8 log,
	// 					u8 page, void *buf, unsigned int sectors);

	// #define to_ata_port(d) container_of(d, struct ata_port, tdev)

	// /* libata-sata.c */
	// #ifdef CONFIG_SATA_HOST
	// int ata_sas_allocate_tag(struct ata_port *ap);
	// void ata_sas_free_tag(unsigned int tag, struct ata_port *ap);
	// #else
	// static inline int ata_sas_allocate_tag(struct ata_port *ap)
	// {
	// 	return -EOPNOTSUPP;
	// }
	// static inline void ata_sas_free_tag(unsigned int tag, struct ata_port *ap) { }
	// #endif

	// /* libata-acpi.c */
	// #ifdef CONFIG_ATA_ACPI
	// extern unsigned int ata_acpi_gtf_filter;
	// extern void ata_acpi_dissociate(struct ata_host *host);
	// extern int ata_acpi_on_suspend(struct ata_port *ap);
	// extern void ata_acpi_on_resume(struct ata_port *ap);
	// extern int ata_acpi_on_devcfg(ata_dev_s *dev);
	// extern void ata_acpi_on_disable(ata_dev_s *dev);
	// extern void ata_acpi_set_state(struct ata_port *ap, pm_message_t state);
	// extern void ata_acpi_bind_port(struct ata_port *ap);
	// extern void ata_acpi_bind_dev(ata_dev_s *dev);
	// extern acpi_handle ata_dev_acpi_handle(ata_dev_s *dev);
	// #else
	// static inline void ata_acpi_dissociate(struct ata_host *host) { }
	// static inline int ata_acpi_on_suspend(struct ata_port *ap) { return 0; }
	// static inline void ata_acpi_on_resume(struct ata_port *ap) { }
	// static inline int ata_acpi_on_devcfg(ata_dev_s *dev) { return 0; }
	// static inline void ata_acpi_on_disable(ata_dev_s *dev) { }
	// static inline void ata_acpi_set_state(struct ata_port *ap,
	// 					pm_message_t state) { }
	// static inline void ata_acpi_bind_port(struct ata_port *ap) {}
	// static inline void ata_acpi_bind_dev(ata_dev_s *dev) {}
	// #endif

	// /* libata-scsi.c */
	// extern ata_dev_s *ata_scsi_find_dev(struct ata_port *ap,
	// 						const struct scsi_device *scsidev);
	// extern int ata_scsi_add_hosts(struct ata_host *host,
	// 				struct scsi_host_template *sht);
	// extern void ata_scsi_scan_host(struct ata_port *ap, int sync);
	// extern int ata_scsi_offline_dev(ata_dev_s *dev);
	// extern void ata_scsi_set_sense(ata_dev_s *dev,
	// 				struct scsi_cmnd *cmd, u8 sk, u8 asc, u8 ascq);
	// extern void ata_scsi_set_sense_information(ata_dev_s *dev,
	// 					struct scsi_cmnd *cmd,
	// 					const ata_tf_s *tf);
	// extern void ata_scsi_media_change_notify(ata_dev_s *dev);
	// extern void ata_scsi_hotplug(struct work_struct *work);
	// extern void ata_schedule_scsi_eh(struct Scsi_Host *shost);
	// extern void ata_scsi_dev_rescan(struct work_struct *work);
	// extern int ata_bus_probe(struct ata_port *ap);
	// extern int ata_scsi_user_scan(struct Scsi_Host *shost, unsigned int channel,
	// 				unsigned int id, u64 lun);
	// void ata_scsi_sdev_config(struct scsi_device *sdev);
	// int ata_scsi_dev_config(struct scsi_device *sdev, ata_dev_s *dev);
	// int __ata_scsi_queuecmd(struct scsi_cmnd *scmd, ata_dev_s *dev);

	// /* libata-eh.c */
	// extern unsigned long ata_internal_cmd_timeout(ata_dev_s *dev, u8 cmd);
	// extern void ata_internal_cmd_timed_out(ata_dev_s *dev, u8 cmd);
	// extern void ata_eh_acquire(struct ata_port *ap);
	// extern void ata_eh_release(struct ata_port *ap);
	// extern void ata_scsi_error(struct Scsi_Host *host);
	// extern void ata_eh_fastdrain_timerfn(struct timer_list *t);
	// extern void ata_qc_schedule_eh(ata_q_cmd_s *qc);
	// extern void ata_dev_disable(ata_dev_s *dev);
	// extern void ata_eh_detach_dev(ata_dev_s *dev);
	// extern void ata_eh_about_to_do(struct ata_link *link, ata_dev_s *dev,
	// 				unsigned int action);
	// extern void ata_eh_done(struct ata_link *link, ata_dev_s *dev,
	// 			unsigned int action);
	// extern void ata_eh_autopsy(struct ata_port *ap);
	// const char *ata_get_cmd_name(u8 command);
	// extern void ata_eh_report(struct ata_port *ap);
	// extern int ata_eh_reset(struct ata_link *link, int classify,
	// 			ata_prereset_fn_t prereset, ata_reset_fn_t softreset,
	// 			ata_reset_fn_t hardreset, ata_postreset_fn_t postreset);
	// extern int ata_set_mode(struct ata_link *link, ata_dev_s **r_failed_dev);
	// extern int ata_eh_recover(struct ata_port *ap, ata_prereset_fn_t prereset,
	// 			ata_reset_fn_t softreset, ata_reset_fn_t hardreset,
	// 			ata_postreset_fn_t postreset,
	// 			struct ata_link **r_failed_disk);
	// extern void ata_eh_finish(struct ata_port *ap);
	// extern int ata_ering_map(struct ata_ering *ering,
	// 			int (*map_fn)(struct ata_ering_entry *, void *),
	// 			void *arg);
	// extern unsigned int atapi_eh_tur(ata_dev_s *dev, u8 *r_sense_key);
	// extern unsigned int atapi_eh_request_sense(ata_dev_s *dev,
	// 					u8 *sense_buf, u8 dfl_sense_key);

	// /* libata-pmp.c */
	// #ifdef CONFIG_SATA_PMP
	// extern int sata_pmp_scr_read(struct ata_link *link, int reg, u32 *val);
	// extern int sata_pmp_scr_write(struct ata_link *link, int reg, u32 val);
	// extern int sata_pmp_set_lpm(struct ata_link *link, enum ata_lpm_policy policy,
	// 				unsigned hints);
	// extern int sata_pmp_attach(ata_dev_s *dev);
	// #else /* CONFIG_SATA_PMP */
	// static inline int sata_pmp_scr_read(struct ata_link *link, int reg, u32 *val)
	// {
	// 	return -EINVAL;
	// }

	// static inline int sata_pmp_scr_write(struct ata_link *link, int reg, u32 val)
	// {
	// 	return -EINVAL;
	// }

	// static inline int sata_pmp_set_lpm(struct ata_link *link,
	// 				enum ata_lpm_policy policy, unsigned hints)
	// {
	// 	return -EINVAL;
	// }

	// static inline int sata_pmp_attach(ata_dev_s *dev)
	// {
	// 	return -EINVAL;
	// }
	// #endif /* CONFIG_SATA_PMP */

	// /* libata-sff.c */
	// #ifdef CONFIG_ATA_SFF
	// extern void ata_sff_flush_pio_task(struct ata_port *ap);
	// extern void ata_sff_port_init(struct ata_port *ap);
	// extern int ata_sff_init(void);
	// extern void ata_sff_exit(void);
	// #else /* CONFIG_ATA_SFF */
	// static inline void ata_sff_flush_pio_task(struct ata_port *ap)
	// { }
	// static inline void ata_sff_port_init(struct ata_port *ap)
	// { }
	// static inline int ata_sff_init(void)
	// { return 0; }
	// static inline void ata_sff_exit(void)
	// { }
	// #endif /* CONFIG_ATA_SFF */

	// /* libata-zpodd.c */
	// #ifdef CONFIG_SATA_ZPODD
	// void zpodd_init(ata_dev_s *dev);
	// void zpodd_exit(ata_dev_s *dev);
	// static inline bool zpodd_dev_enabled(ata_dev_s *dev)
	// {
	// 	return dev->zpodd != NULL;
	// }
	// void zpodd_on_suspend(ata_dev_s *dev);
	// bool zpodd_zpready(ata_dev_s *dev);
	// void zpodd_enable_run_wake(ata_dev_s *dev);
	// void zpodd_disable_run_wake(ata_dev_s *dev);
	// void zpodd_post_poweron(ata_dev_s *dev);
	// #else /* CONFIG_SATA_ZPODD */
	// static inline void zpodd_init(ata_dev_s *dev) {}
	// static inline void zpodd_exit(ata_dev_s *dev) {}
	// static inline bool zpodd_dev_enabled(ata_dev_s *dev) { return false; }
	// static inline void zpodd_on_suspend(ata_dev_s *dev) {}
	// static inline bool zpodd_zpready(ata_dev_s *dev) { return false; }
	// static inline void zpodd_enable_run_wake(ata_dev_s *dev) {}
	// static inline void zpodd_disable_run_wake(ata_dev_s *dev) {}
	// static inline void zpodd_post_poweron(ata_dev_s *dev) {}
	// #endif /* CONFIG_SATA_ZPODD */

	int myos_ata_dev_read_id(ata_dev_s *dev, u16 *id);

#endif /* __LIBATA_H__ */
