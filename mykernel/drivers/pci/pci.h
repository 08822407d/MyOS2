/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DRIVERS_PCI_H
#define DRIVERS_PCI_H

	#include <linux/device/pci.h>

	// /* Number of possible devfns: 0.0 to 1f.7 inclusive */
	// #define MAX_NR_DEVFNS 256

	// #define PCI_FIND_CAP_TTL	48

	// #define PCI_VSEC_ID_INTEL_TBT	0x1234	/* Thunderbolt */

	// extern const unsigned char pcie_link_speed[];
	// extern bool pci_early_dump;

	// bool pcie_cap_has_lnkctl(const pci_dev_s *dev);
	// bool pcie_cap_has_lnkctl2(const pci_dev_s *dev);
	// bool pcie_cap_has_rtctl(const pci_dev_s *dev);

	// /* Functions internal to the PCI core code */

	// int pci_create_sysfs_dev_files(pci_dev_s *pdev);
	// void pci_remove_sysfs_dev_files(pci_dev_s *pdev);
	// void pci_cleanup_rom(pci_dev_s *dev);
	// #ifdef CONFIG_DMI
	// extern const struct attribute_group pci_dev_smbios_attr_group;
	// #endif

	// enum pci_mmap_api {
	// 	PCI_MMAP_SYSFS,	/* mmap on /sys/bus/pci/devices/<BDF>/resource<N> */
	// 	PCI_MMAP_PROCFS	/* mmap on /proc/bus/pci/<BDF> */
	// };
	// int pci_mmap_fits(pci_dev_s *pdev, int resno, struct vm_area_struct *vmai,
	// 		  enum pci_mmap_api mmap_api);

	// bool pci_reset_supported(pci_dev_s *dev);
	// void pci_init_reset_methods(pci_dev_s *dev);
	// int pci_bridge_secondary_bus_reset(pci_dev_s *dev);
	// int pci_bus_error_reset(pci_dev_s *dev);

	// struct pci_cap_saved_data {
	// 	u16		cap_nr;
	// 	bool		cap_extended;
	// 	unsigned int	size;
	// 	u32		data[];
	// };

	// struct pci_cap_saved_state {
	// 	HList_s		next;
	// 	struct pci_cap_saved_data	cap;
	// };

	// void pci_allocate_cap_save_buffers(pci_dev_s *dev);
	// void pci_free_cap_save_buffers(pci_dev_s *dev);
	// int pci_add_cap_save_buffer(pci_dev_s *dev, char cap, unsigned int size);
	// int pci_add_ext_cap_save_buffer(pci_dev_s *dev,
	// 				u16 cap, unsigned int size);
	// struct pci_cap_saved_state *pci_find_saved_cap(pci_dev_s *dev, char cap);
	// struct pci_cap_saved_state *pci_find_saved_ext_cap(pci_dev_s *dev,
	// 						   u16 cap);

	// #define PCI_PM_D2_DELAY         200	/* usec; see PCIe r4.0, sec 5.9.1 */
	// #define PCI_PM_D3HOT_WAIT       10	/* msec */
	// #define PCI_PM_D3COLD_WAIT      100	/* msec */

	// /*
	//  * Following exit from Conventional Reset, devices must be ready within 1 sec
	//  * (PCIe r6.0 sec 6.6.1).  A D3cold to D0 transition implies a Conventional
	//  * Reset (PCIe r6.0 sec 5.8).
	//  */
	// #define PCI_RESET_WAIT		1000	/* msec */

	// void pci_update_current_state(pci_dev_s *dev, pci_power_t state);
	// void pci_refresh_power_state(pci_dev_s *dev);
	// int pci_power_up(pci_dev_s *dev);
	// void pci_disable_enabled_device(pci_dev_s *dev);
	// int pci_finish_runtime_suspend(pci_dev_s *dev);
	// void pcie_clear_device_status(pci_dev_s *dev);
	// void pcie_clear_root_pme_status(pci_dev_s *dev);
	// bool pci_check_pme_status(pci_dev_s *dev);
	// void pci_pme_wakeup_bus(pci_bus_s *bus);
	// int __pci_pme_wakeup(pci_dev_s *dev, void *ign);
	// void pci_pme_restore(pci_dev_s *dev);
	// bool pci_dev_need_resume(pci_dev_s *dev);
	// void pci_dev_adjust_pme(pci_dev_s *dev);
	// void pci_dev_complete_resume(pci_dev_s *pci_dev);
	// void pci_config_pm_runtime_get(pci_dev_s *dev);
	// void pci_config_pm_runtime_put(pci_dev_s *dev);
	// void pci_pm_init(pci_dev_s *dev);
	// void pci_ea_init(pci_dev_s *dev);
	// void pci_msi_init(pci_dev_s *dev);
	// void pci_msix_init(pci_dev_s *dev);
	// bool pci_bridge_d3_possible(pci_dev_s *dev);
	// void pci_bridge_d3_update(pci_dev_s *dev);
	// void pci_bridge_reconfigure_ltr(pci_dev_s *dev);
	// int pci_bridge_wait_for_secondary_bus(pci_dev_s *dev, char *reset_type);

	// static inline void pci_wakeup_event(pci_dev_s *dev)
	// {
	// 	/* Wait 100 ms before the system can be put into a sleep state. */
	// 	pm_wakeup_event(&dev->dev, 100);
	// }

	// static inline bool pci_has_subordinate(pci_dev_s *pci_dev)
	// {
	// 	return !!(pci_dev->subordinate);
	// }

	// static inline bool pci_power_manageable(pci_dev_s *pci_dev)
	// {
	// 	/*
	// 	 * Currently we allow normal PCI devices and PCI bridges transition
	// 	 * into D3 if their bridge_d3 is set.
	// 	 */
	// 	return !pci_has_subordinate(pci_dev) || pci_dev->bridge_d3;
	// }

	// static inline bool pcie_downstream_port(const pci_dev_s *dev)
	// {
	// 	int type = pci_pcie_type(dev);

	// 	return type == PCI_EXP_TYPE_ROOT_PORT ||
	// 		   type == PCI_EXP_TYPE_DOWNSTREAM ||
	// 		   type == PCI_EXP_TYPE_PCIE_BRIDGE;
	// }

	// void pci_vpd_init(pci_dev_s *dev);
	// void pci_vpd_release(pci_dev_s *dev);
	// extern const struct attribute_group pci_dev_vpd_attr_group;

	// /* PCI Virtual Channel */
	// int pci_save_vc_state(pci_dev_s *dev);
	// void pci_restore_vc_state(pci_dev_s *dev);
	// void pci_allocate_vc_save_buffers(pci_dev_s *dev);

	// /* PCI /proc functions */
	// #ifdef CONFIG_PROC_FS
	// int pci_proc_attach_device(pci_dev_s *dev);
	// int pci_proc_detach_device(pci_dev_s *dev);
	// int pci_proc_detach_bus(pci_bus_s *bus);
	// #else
	// static inline int pci_proc_attach_device(pci_dev_s *dev) { return 0; }
	// static inline int pci_proc_detach_device(pci_dev_s *dev) { return 0; }
	// static inline int pci_proc_detach_bus(pci_bus_s *bus) { return 0; }
	// #endif

	// /* Functions for PCI Hotplug drivers to use */
	// int pci_hp_add_bridge(pci_dev_s *dev);

	// #ifdef HAVE_PCI_LEGACY
	// void pci_create_legacy_files(pci_bus_s *bus);
	// void pci_remove_legacy_files(pci_bus_s *bus);
	// #else
	// static inline void pci_create_legacy_files(pci_bus_s *bus) { return; }
	// static inline void pci_remove_legacy_files(pci_bus_s *bus) { return; }
	// #endif

	// /* Lock for read/write access to pci device and bus lists */
	// extern rwsem_t pci_bus_sem;
	// extern struct mutex pci_slot_mutex;

	// extern raw_spinlock_t pci_lock;

	// extern unsigned int pci_pm_d3hot_delay;

	// #ifdef CONFIG_PCI_MSI
	// void pci_no_msi(void);
	// #else
	// static inline void pci_no_msi(void) { }
	// #endif

	// void pci_realloc_get_opt(char *);

	// static inline int pci_no_d1d2(pci_dev_s *dev)
	// {
	// 	unsigned int parent_dstates = 0;

	// 	if (dev->bus->self)
	// 		parent_dstates = dev->bus->self->no_d1d2;
	// 	return (dev->no_d1d2 || parent_dstates);

	// }
	// extern const struct attribute_group *pci_dev_groups[];
	// extern const struct attribute_group *pcibus_groups[];
	// extern const struct device_type pci_dev_type;
	// extern const struct attribute_group *pci_bus_groups[];

	// extern unsigned long pci_hotplug_io_size;
	// extern unsigned long pci_hotplug_mmio_size;
	// extern unsigned long pci_hotplug_mmio_pref_size;
	// extern unsigned long pci_hotplug_bus_size;

	// /**
	//  * pci_match_one_device - Tell if a PCI device structure has a matching
	//  *			  PCI device id structure
	//  * @id: single PCI device id structure to match
	//  * @dev: the PCI device structure to match against
	//  *
	//  * Returns the matching pci_device_id structure or %NULL if there is no match.
	//  */
	// static inline const struct pci_device_id *
	// pci_match_one_device(const struct pci_device_id *id, const pci_dev_s *dev)
	// {
	// 	if ((id->vendor == PCI_ANY_ID || id->vendor == dev->vendor) &&
	// 		(id->device == PCI_ANY_ID || id->device == dev->device) &&
	// 		(id->subvendor == PCI_ANY_ID || id->subvendor == dev->subsystem_vendor) &&
	// 		(id->subdevice == PCI_ANY_ID || id->subdevice == dev->subsystem_device) &&
	// 		!((id->class ^ dev->class) & id->class_mask))
	// 		return id;
	// 	return NULL;
	// }

	// /* PCI slot sysfs helper code */
	// #define to_pci_slot(s) container_of(s, struct pci_slot, kobj)

	// extern struct kset *pci_slots_kset;

	// struct pci_slot_attribute {
	// 	struct attribute attr;
	// 	ssize_t (*show)(struct pci_slot *, char *);
	// 	ssize_t (*store)(struct pci_slot *, const char *, size_t);
	// };
	// #define to_pci_slot_attr(s) container_of(s, struct pci_slot_attribute, attr)

	// enum pci_bar_type {
	// 	pci_bar_unknown,	/* Standard PCI BAR probe */
	// 	pci_bar_io,		/* An I/O port BAR */
	// 	pci_bar_mem32,		/* A 32-bit memory BAR */
	// 	pci_bar_mem64,		/* A 64-bit memory BAR */
	// };

	// struct device *pci_get_host_bridge_device(pci_dev_s *dev);
	// void pci_put_host_bridge_device(struct device *dev);

	// int pci_configure_extended_tags(pci_dev_s *dev, void *ign);
	// bool pci_bus_read_dev_vendor_id(pci_bus_s *bus, int devfn, u32 *pl,
	// 				int crs_timeout);
	// bool pci_bus_generic_read_dev_vendor_id(pci_bus_s *bus, int devfn, u32 *pl,
	// 					int crs_timeout);
	// int pci_idt_bus_quirk(pci_bus_s *bus, int devfn, u32 *pl, int crs_timeout);

	// int pci_setup_device(pci_dev_s *dev);
	// int __pci_read_base(pci_dev_s *dev, enum pci_bar_type type,
	// 			struct resource *res, unsigned int reg);
	// void pci_configure_ari(pci_dev_s *dev);
	// void __pci_bus_size_bridges(pci_bus_s *bus,
	// 			struct list_head *realloc_head);
	// void __pci_bus_assign_resources(const pci_bus_s *bus,
	// 				struct list_head *realloc_head,
	// 				struct list_head *fail_head);
	// bool pci_bus_clip_resource(pci_dev_s *dev, int idx);

	// void pci_reassigndev_resource_alignment(pci_dev_s *dev);
	// void pci_disable_bridge_window(pci_dev_s *dev);
	// pci_bus_s *pci_bus_get(pci_bus_s *bus);
	// void pci_bus_put(pci_bus_s *bus);

	// /* PCIe link information from Link Capabilities 2 */
	// #define PCIE_LNKCAP2_SLS2SPEED(lnkcap2) \
	// 	((lnkcap2) & PCI_EXP_LNKCAP2_SLS_64_0GB ? PCIE_SPEED_64_0GT : \
	// 	 (lnkcap2) & PCI_EXP_LNKCAP2_SLS_32_0GB ? PCIE_SPEED_32_0GT : \
	// 	 (lnkcap2) & PCI_EXP_LNKCAP2_SLS_16_0GB ? PCIE_SPEED_16_0GT : \
	// 	 (lnkcap2) & PCI_EXP_LNKCAP2_SLS_8_0GB ? PCIE_SPEED_8_0GT : \
	// 	 (lnkcap2) & PCI_EXP_LNKCAP2_SLS_5_0GB ? PCIE_SPEED_5_0GT : \
	// 	 (lnkcap2) & PCI_EXP_LNKCAP2_SLS_2_5GB ? PCIE_SPEED_2_5GT : \
	// 	 PCI_SPEED_UNKNOWN)

	// /* PCIe speed to Mb/s reduced by encoding overhead */
	// #define PCIE_SPEED2MBS_ENC(speed) \
	// 	((speed) == PCIE_SPEED_64_0GT ? 64000*128/130 : \
	// 	 (speed) == PCIE_SPEED_32_0GT ? 32000*128/130 : \
	// 	 (speed) == PCIE_SPEED_16_0GT ? 16000*128/130 : \
	// 	 (speed) == PCIE_SPEED_8_0GT  ?  8000*128/130 : \
	// 	 (speed) == PCIE_SPEED_5_0GT  ?  5000*8/10 : \
	// 	 (speed) == PCIE_SPEED_2_5GT  ?  2500*8/10 : \
	// 	 0)

	// const char *pci_speed_string(enum pci_bus_speed speed);
	// enum pci_bus_speed pcie_get_speed_cap(pci_dev_s *dev);
	// enum pcie_link_width pcie_get_width_cap(pci_dev_s *dev);
	// u32 pcie_bandwidth_capable(pci_dev_s *dev, enum pci_bus_speed *speed,
	// 			   enum pcie_link_width *width);
	// void __pcie_print_link_status(pci_dev_s *dev, bool verbose);
	// void pcie_report_downtraining(pci_dev_s *dev);
	// void pcie_update_link_speed(pci_bus_s *bus, u16 link_status);

	// /* Single Root I/O Virtualization */
	// struct pci_sriov {
	// 	int		pos;		/* Capability position */
	// 	int		nres;		/* Number of resources */
	// 	u32		cap;		/* SR-IOV Capabilities */
	// 	u16		ctrl;		/* SR-IOV Control */
	// 	u16		total_VFs;	/* Total VFs associated with the PF */
	// 	u16		initial_VFs;	/* Initial VFs associated with the PF */
	// 	u16		num_VFs;	/* Number of VFs available */
	// 	u16		offset;		/* First VF Routing ID offset */
	// 	u16		stride;		/* Following VF stride */
	// 	u16		vf_device;	/* VF device ID */
	// 	u32		pgsz;		/* Page size for BAR alignment */
	// 	u8		link;		/* Function Dependency Link */
	// 	u8		max_VF_buses;	/* Max buses consumed by VFs */
	// 	u16		driver_max_VFs;	/* Max num VFs driver supports */
	// 	pci_dev_s	*dev;		/* Lowest numbered PF */
	// 	pci_dev_s	*self;		/* This PF */
	// 	u32		class;		/* VF device */
	// 	u8		hdr_type;	/* VF header type */
	// 	u16		subsystem_vendor; /* VF subsystem vendor */
	// 	u16		subsystem_device; /* VF subsystem device */
	// 	resource_size_t	barsz[PCI_SRIOV_NUM_BARS];	/* VF BAR size */
	// 	bool		drivers_autoprobe; /* Auto probing of VFs by driver */
	// };

	// #ifdef CONFIG_PCI_DOE
	// void pci_doe_init(pci_dev_s *pdev);
	// void pci_doe_destroy(pci_dev_s *pdev);
	// void pci_doe_disconnected(pci_dev_s *pdev);
	// #else
	// static inline void pci_doe_init(pci_dev_s *pdev) { }
	// static inline void pci_doe_destroy(pci_dev_s *pdev) { }
	// static inline void pci_doe_disconnected(pci_dev_s *pdev) { }
	// #endif

	// /**
	//  * pci_dev_set_io_state - Set the new error state if possible.
	//  *
	//  * @dev: PCI device to set new error_state
	//  * @new: the state we want dev to be in
	//  *
	//  * If the device is experiencing perm_failure, it has to remain in that state.
	//  * Any other transition is allowed.
	//  *
	//  * Returns true if state has been changed to the requested state.
	//  */
	// static inline bool pci_dev_set_io_state(pci_dev_s *dev,
	// 					pci_channel_state_t new)
	// {
	// 	pci_channel_state_t old;

	// 	switch (new) {
	// 	case pci_channel_io_perm_failure:
	// 		xchg(&dev->error_state, pci_channel_io_perm_failure);
	// 		return true;
	// 	case pci_channel_io_frozen:
	// 		old = cmpxchg(&dev->error_state, pci_channel_io_normal,
	// 				  pci_channel_io_frozen);
	// 		return old != pci_channel_io_perm_failure;
	// 	case pci_channel_io_normal:
	// 		old = cmpxchg(&dev->error_state, pci_channel_io_frozen,
	// 				  pci_channel_io_normal);
	// 		return old != pci_channel_io_perm_failure;
	// 	default:
	// 		return false;
	// 	}
	// }

	// static inline int pci_dev_set_disconnected(pci_dev_s *dev, void *unused)
	// {
	// 	pci_dev_set_io_state(dev, pci_channel_io_perm_failure);
	// 	pci_doe_disconnected(dev);

	// 	return 0;
	// }

	// static inline bool pci_dev_is_disconnected(const pci_dev_s *dev)
	// {
	// 	return dev->error_state == pci_channel_io_perm_failure;
	// }

	// /* pci_dev priv_flags */
	// #define PCI_DEV_ADDED 0
	// #define PCI_DPC_RECOVERED 1
	// #define PCI_DPC_RECOVERING 2

	// static inline void pci_dev_assign_added(pci_dev_s *dev, bool added)
	// {
	// 	assign_bit(PCI_DEV_ADDED, &dev->priv_flags, added);
	// }

	// static inline bool pci_dev_is_added(const pci_dev_s *dev)
	// {
	// 	return test_bit(PCI_DEV_ADDED, &dev->priv_flags);
	// }

	// #ifdef CONFIG_PCIEAER
	// #include <linux/aer.h>

	// #define AER_MAX_MULTI_ERR_DEVICES	5	/* Not likely to have more */

	// struct aer_err_info {
	// 	pci_dev_s *dev[AER_MAX_MULTI_ERR_DEVICES];
	// 	int error_dev_num;

	// 	unsigned int id:16;

	// 	unsigned int severity:2;	/* 0:NONFATAL | 1:FATAL | 2:COR */
	// 	unsigned int __pad1:5;
	// 	unsigned int multi_error_valid:1;

	// 	unsigned int first_error:5;
	// 	unsigned int __pad2:2;
	// 	unsigned int tlp_header_valid:1;

	// 	unsigned int status;		/* COR/UNCOR Error Status */
	// 	unsigned int mask;		/* COR/UNCOR Error Mask */
	// 	struct aer_header_log_regs tlp;	/* TLP Header */
	// };

	// int aer_get_device_error_info(pci_dev_s *dev, struct aer_err_info *info);
	// void aer_print_error(pci_dev_s *dev, struct aer_err_info *info);
	// #endif	/* CONFIG_PCIEAER */

	// #ifdef CONFIG_PCIEPORTBUS
	// /* Cached RCEC Endpoint Association */
	// struct rcec_ea {
	// 	u8		nextbusn;
	// 	u8		lastbusn;
	// 	u32		bitmap;
	// };
	// #endif

	// #ifdef CONFIG_PCIE_DPC
	// void pci_save_dpc_state(pci_dev_s *dev);
	// void pci_restore_dpc_state(pci_dev_s *dev);
	// void pci_dpc_init(pci_dev_s *pdev);
	// void dpc_process_error(pci_dev_s *pdev);
	// pci_ers_result_t dpc_reset_link(pci_dev_s *pdev);
	// bool pci_dpc_recovered(pci_dev_s *pdev);
	// #else
	// static inline void pci_save_dpc_state(pci_dev_s *dev) {}
	// static inline void pci_restore_dpc_state(pci_dev_s *dev) {}
	// static inline void pci_dpc_init(pci_dev_s *pdev) {}
	// static inline bool pci_dpc_recovered(pci_dev_s *pdev) { return false; }
	// #endif

	// #ifdef CONFIG_PCIEPORTBUS
	// void pci_rcec_init(pci_dev_s *dev);
	// void pci_rcec_exit(pci_dev_s *dev);
	// void pcie_link_rcec(pci_dev_s *rcec);
	// void pcie_walk_rcec(pci_dev_s *rcec,
	// 			int (*cb)(pci_dev_s *, void *),
	// 			void *userdata);
	// #else
	// static inline void pci_rcec_init(pci_dev_s *dev) {}
	// static inline void pci_rcec_exit(pci_dev_s *dev) {}
	// static inline void pcie_link_rcec(pci_dev_s *rcec) {}
	// static inline void pcie_walk_rcec(pci_dev_s *rcec,
	// 				  int (*cb)(pci_dev_s *, void *),
	// 				  void *userdata) {}
	// #endif

	// #ifdef CONFIG_PCI_ATS
	// /* Address Translation Service */
	// void pci_ats_init(pci_dev_s *dev);
	// void pci_restore_ats_state(pci_dev_s *dev);
	// #else
	// static inline void pci_ats_init(pci_dev_s *d) { }
	// static inline void pci_restore_ats_state(pci_dev_s *dev) { }
	// #endif /* CONFIG_PCI_ATS */

	// #ifdef CONFIG_PCI_PRI
	// void pci_pri_init(pci_dev_s *dev);
	// void pci_restore_pri_state(pci_dev_s *pdev);
	// #else
	// static inline void pci_pri_init(pci_dev_s *dev) { }
	// static inline void pci_restore_pri_state(pci_dev_s *pdev) { }
	// #endif

	// #ifdef CONFIG_PCI_PASID
	// void pci_pasid_init(pci_dev_s *dev);
	// void pci_restore_pasid_state(pci_dev_s *pdev);
	// #else
	// static inline void pci_pasid_init(pci_dev_s *dev) { }
	// static inline void pci_restore_pasid_state(pci_dev_s *pdev) { }
	// #endif

	// #ifdef CONFIG_PCI_IOV
	// int pci_iov_init(pci_dev_s *dev);
	// void pci_iov_release(pci_dev_s *dev);
	// void pci_iov_remove(pci_dev_s *dev);
	// void pci_iov_update_resource(pci_dev_s *dev, int resno);
	// resource_size_t pci_sriov_resource_alignment(pci_dev_s *dev, int resno);
	// void pci_restore_iov_state(pci_dev_s *dev);
	// int pci_iov_bus_range(pci_bus_s *bus);
	// extern const struct attribute_group sriov_pf_dev_attr_group;
	// extern const struct attribute_group sriov_vf_dev_attr_group;
	// #else
	// static inline int pci_iov_init(pci_dev_s *dev)
	// {
	// 	return -ENODEV;
	// }
	// static inline void pci_iov_release(pci_dev_s *dev)

	// {
	// }
	// static inline void pci_iov_remove(pci_dev_s *dev)
	// {
	// }
	// static inline void pci_restore_iov_state(pci_dev_s *dev)
	// {
	// }
	// static inline int pci_iov_bus_range(pci_bus_s *bus)
	// {
	// 	return 0;
	// }

	// #endif /* CONFIG_PCI_IOV */

	// #ifdef CONFIG_PCIE_PTM
	// void pci_ptm_init(pci_dev_s *dev);
	// void pci_save_ptm_state(pci_dev_s *dev);
	// void pci_restore_ptm_state(pci_dev_s *dev);
	// void pci_suspend_ptm(pci_dev_s *dev);
	// void pci_resume_ptm(pci_dev_s *dev);
	// #else
	// static inline void pci_ptm_init(pci_dev_s *dev) { }
	// static inline void pci_save_ptm_state(pci_dev_s *dev) { }
	// static inline void pci_restore_ptm_state(pci_dev_s *dev) { }
	// static inline void pci_suspend_ptm(pci_dev_s *dev) { }
	// static inline void pci_resume_ptm(pci_dev_s *dev) { }
	// #endif

	// unsigned long pci_cardbus_resource_alignment(struct resource *);

	// static inline resource_size_t pci_resource_alignment(pci_dev_s *dev,
	// 							 struct resource *res)
	// {
	// #ifdef CONFIG_PCI_IOV
	// 	int resno = res - dev->resource;

	// 	if (resno >= PCI_IOV_RESOURCES && resno <= PCI_IOV_RESOURCE_END)
	// 		return pci_sriov_resource_alignment(dev, resno);
	// #endif
	// 	if (dev->class >> 8 == PCI_CLASS_BRIDGE_CARDBUS)
	// 		return pci_cardbus_resource_alignment(res);
	// 	return resource_alignment(res);
	// }

	// void pci_acs_init(pci_dev_s *dev);
	// #ifdef CONFIG_PCI_QUIRKS
	// int pci_dev_specific_acs_enabled(pci_dev_s *dev, u16 acs_flags);
	// int pci_dev_specific_enable_acs(pci_dev_s *dev);
	// int pci_dev_specific_disable_acs_redir(pci_dev_s *dev);
	// #else
	// static inline int pci_dev_specific_acs_enabled(pci_dev_s *dev,
	// 						   u16 acs_flags)
	// {
	// 	return -ENOTTY;
	// }
	// static inline int pci_dev_specific_enable_acs(pci_dev_s *dev)
	// {
	// 	return -ENOTTY;
	// }
	// static inline int pci_dev_specific_disable_acs_redir(pci_dev_s *dev)
	// {
	// 	return -ENOTTY;
	// }
	// #endif

	// /* PCI error reporting and recovery */
	// pci_ers_result_t pcie_do_recovery(pci_dev_s *dev,
	// 		pci_channel_state_t state,
	// 		pci_ers_result_t (*reset_subordinates)(pci_dev_s *pdev));

	// bool pcie_wait_for_link(pci_dev_s *pdev, bool active);
	// #ifdef CONFIG_PCIEASPM
	// void pcie_aspm_init_link_state(pci_dev_s *pdev);
	// void pcie_aspm_exit_link_state(pci_dev_s *pdev);
	// void pcie_aspm_powersave_config_link(pci_dev_s *pdev);
	// #else
	// static inline void pcie_aspm_init_link_state(pci_dev_s *pdev) { }
	// static inline void pcie_aspm_exit_link_state(pci_dev_s *pdev) { }
	// static inline void pcie_aspm_powersave_config_link(pci_dev_s *pdev) { }
	// #endif

	// #ifdef CONFIG_PCIE_ECRC
	// void pcie_set_ecrc_checking(pci_dev_s *dev);
	// void pcie_ecrc_get_policy(char *str);
	// #else
	// static inline void pcie_set_ecrc_checking(pci_dev_s *dev) { }
	// static inline void pcie_ecrc_get_policy(char *str) { }
	// #endif

	// struct pci_dev_reset_methods {
	// 	u16 vendor;
	// 	u16 device;
	// 	int (*reset)(pci_dev_s *dev, bool probe);
	// };

	// struct pci_reset_fn_method {
	// 	int (*reset_fn)(pci_dev_s *pdev, bool probe);
	// 	char *name;
	// };

	// #ifdef CONFIG_PCI_QUIRKS
	// int pci_dev_specific_reset(pci_dev_s *dev, bool probe);
	// #else
	// static inline int pci_dev_specific_reset(pci_dev_s *dev, bool probe)
	// {
	// 	return -ENOTTY;
	// }
	// #endif

	// #if defined(CONFIG_PCI_QUIRKS) && defined(CONFIG_ARM64)
	// int acpi_get_rc_resources(struct device *dev, const char *hid, u16 segment,
	// 			  struct resource *res);
	// #else
	// static inline int acpi_get_rc_resources(struct device *dev, const char *hid,
	// 					u16 segment, struct resource *res)
	// {
	// 	return -ENODEV;
	// }
	// #endif

	// int pci_rebar_get_current_size(pci_dev_s *pdev, int bar);
	// int pci_rebar_set_size(pci_dev_s *pdev, int bar, int size);
	// static inline u64 pci_rebar_size_to_bytes(int size)
	// {
	// 	return 1ULL << (size + 20);
	// }

	// struct device_node;

	// #ifdef CONFIG_OF
	// int of_pci_parse_bus_range(struct device_node *node, struct resource *res);
	// int of_get_pci_domain_nr(struct device_node *node);
	// int of_pci_get_max_link_speed(struct device_node *node);
	// u32 of_pci_get_slot_power_limit(struct device_node *node,
	// 				u8 *slot_power_limit_value,
	// 				u8 *slot_power_limit_scale);
	// int pci_set_of_node(pci_dev_s *dev);
	// void pci_release_of_node(pci_dev_s *dev);
	// void pci_set_bus_of_node(pci_bus_s *bus);
	// void pci_release_bus_of_node(pci_bus_s *bus);

	// int devm_of_pci_bridge_init(struct device *dev, struct pci_host_bridge *bridge);

	// #else
	// static inline int
	// of_pci_parse_bus_range(struct device_node *node, struct resource *res)
	// {
	// 	return -EINVAL;
	// }

	// static inline int
	// of_get_pci_domain_nr(struct device_node *node)
	// {
	// 	return -1;
	// }

	// static inline int
	// of_pci_get_max_link_speed(struct device_node *node)
	// {
	// 	return -EINVAL;
	// }

	// static inline u32
	// of_pci_get_slot_power_limit(struct device_node *node,
	// 				u8 *slot_power_limit_value,
	// 				u8 *slot_power_limit_scale)
	// {
	// 	if (slot_power_limit_value)
	// 		*slot_power_limit_value = 0;
	// 	if (slot_power_limit_scale)
	// 		*slot_power_limit_scale = 0;
	// 	return 0;
	// }

	// static inline int pci_set_of_node(pci_dev_s *dev) { return 0; }
	// static inline void pci_release_of_node(pci_dev_s *dev) { }
	// static inline void pci_set_bus_of_node(pci_bus_s *bus) { }
	// static inline void pci_release_bus_of_node(pci_bus_s *bus) { }

	// static inline int devm_of_pci_bridge_init(struct device *dev, struct pci_host_bridge *bridge)
	// {
	// 	return 0;
	// }

	// #endif /* CONFIG_OF */

	// #ifdef CONFIG_PCIEAER
	// void pci_no_aer(void);
	// void pci_aer_init(pci_dev_s *dev);
	// void pci_aer_exit(pci_dev_s *dev);
	// extern const struct attribute_group aer_stats_attr_group;
	// void pci_aer_clear_fatal_status(pci_dev_s *dev);
	// int pci_aer_clear_status(pci_dev_s *dev);
	// int pci_aer_raw_clear_status(pci_dev_s *dev);
	// #else
	// static inline void pci_no_aer(void) { }
	// static inline void pci_aer_init(pci_dev_s *d) { }
	// static inline void pci_aer_exit(pci_dev_s *d) { }
	// static inline void pci_aer_clear_fatal_status(pci_dev_s *dev) { }
	// static inline int pci_aer_clear_status(pci_dev_s *dev) { return -EINVAL; }
	// static inline int pci_aer_raw_clear_status(pci_dev_s *dev) { return -EINVAL; }
	// #endif

	// #ifdef CONFIG_ACPI
	// int pci_acpi_program_hp_params(pci_dev_s *dev);
	// extern const struct attribute_group pci_dev_acpi_attr_group;
	// void pci_set_acpi_fwnode(pci_dev_s *dev);
	// int pci_dev_acpi_reset(pci_dev_s *dev, bool probe);
	// bool acpi_pci_power_manageable(pci_dev_s *dev);
	// bool acpi_pci_bridge_d3(pci_dev_s *dev);
	// int acpi_pci_set_power_state(pci_dev_s *dev, pci_power_t state);
	// pci_power_t acpi_pci_get_power_state(pci_dev_s *dev);
	// void acpi_pci_refresh_power_state(pci_dev_s *dev);
	// int acpi_pci_wakeup(pci_dev_s *dev, bool enable);
	// bool acpi_pci_need_resume(pci_dev_s *dev);
	// pci_power_t acpi_pci_choose_state(pci_dev_s *pdev);
	// #else
	// static inline int pci_dev_acpi_reset(pci_dev_s *dev, bool probe)
	// {
	// 	return -ENOTTY;
	// }
	// static inline void pci_set_acpi_fwnode(pci_dev_s *dev) {}
	// static inline int pci_acpi_program_hp_params(pci_dev_s *dev)
	// {
	// 	return -ENODEV;
	// }
	// static inline bool acpi_pci_power_manageable(pci_dev_s *dev)
	// {
	// 	return false;
	// }
	// static inline bool acpi_pci_bridge_d3(pci_dev_s *dev)
	// {
	// 	return false;
	// }
	// static inline int acpi_pci_set_power_state(pci_dev_s *dev, pci_power_t state)
	// {
	// 	return -ENODEV;
	// }
	// static inline pci_power_t acpi_pci_get_power_state(pci_dev_s *dev)
	// {
	// 	return PCI_UNKNOWN;
	// }
	// static inline void acpi_pci_refresh_power_state(pci_dev_s *dev) {}
	// static inline int acpi_pci_wakeup(pci_dev_s *dev, bool enable)
	// {
	// 	return -ENODEV;
	// }
	// static inline bool acpi_pci_need_resume(pci_dev_s *dev)
	// {
	// 	return false;
	// }
	// static inline pci_power_t acpi_pci_choose_state(pci_dev_s *pdev)
	// {
	// 	return PCI_POWER_ERROR;
	// }
	// #endif

	// #ifdef CONFIG_PCIEASPM
	// extern const struct attribute_group aspm_ctrl_attr_group;
	// #endif

	// extern const struct attribute_group pci_dev_reset_method_attr_group;

	// #ifdef CONFIG_X86_INTEL_MID
	// bool pci_use_mid_pm(void);
	// int mid_pci_set_power_state(pci_dev_s *pdev, pci_power_t state);
	// pci_power_t mid_pci_get_power_state(pci_dev_s *pdev);
	// #else
	// static inline bool pci_use_mid_pm(void)
	// {
	// 	return false;
	// }
	// static inline int mid_pci_set_power_state(pci_dev_s *pdev, pci_power_t state)
	// {
	// 	return -ENODEV;
	// }
	// static inline pci_power_t mid_pci_get_power_state(pci_dev_s *pdev)
	// {
	// 	return PCI_UNKNOWN;
	// }
	// #endif

	// /*
	//  * Config Address for PCI Configuration Mechanism #1
	//  *
	//  * See PCI Local Bus Specification, Revision 3.0,
	//  * Section 3.2.2.3.2, Figure 3-2, p. 50.
	//  */

	// #define PCI_CONF1_BUS_SHIFT	16 /* Bus number */
	// #define PCI_CONF1_DEV_SHIFT	11 /* Device number */
	// #define PCI_CONF1_FUNC_SHIFT	8  /* Function number */

	// #define PCI_CONF1_BUS_MASK	0xff
	// #define PCI_CONF1_DEV_MASK	0x1f
	// #define PCI_CONF1_FUNC_MASK	0x7
	// #define PCI_CONF1_REG_MASK	0xfc /* Limit aligned offset to a maximum of 256B */

	// #define PCI_CONF1_ENABLE	BIT(31)
	// #define PCI_CONF1_BUS(x)	(((x) & PCI_CONF1_BUS_MASK) << PCI_CONF1_BUS_SHIFT)
	// #define PCI_CONF1_DEV(x)	(((x) & PCI_CONF1_DEV_MASK) << PCI_CONF1_DEV_SHIFT)
	// #define PCI_CONF1_FUNC(x)	(((x) & PCI_CONF1_FUNC_MASK) << PCI_CONF1_FUNC_SHIFT)
	// #define PCI_CONF1_REG(x)	((x) & PCI_CONF1_REG_MASK)

	// #define PCI_CONF1_ADDRESS(bus, dev, func, reg) \
	// 	(PCI_CONF1_ENABLE | \
	// 	 PCI_CONF1_BUS(bus) | \
	// 	 PCI_CONF1_DEV(dev) | \
	// 	 PCI_CONF1_FUNC(func) | \
	// 	 PCI_CONF1_REG(reg))

	// /*
	//  * Extension of PCI Config Address for accessing extended PCIe registers
	//  *
	//  * No standardized specification, but used on lot of non-ECAM-compliant ARM SoCs
	//  * or on AMD Barcelona and new CPUs. Reserved bits [27:24] of PCI Config Address
	//  * are used for specifying additional 4 high bits of PCI Express register.
	//  */

	// #define PCI_CONF1_EXT_REG_SHIFT	16
	// #define PCI_CONF1_EXT_REG_MASK	0xf00
	// #define PCI_CONF1_EXT_REG(x)	(((x) & PCI_CONF1_EXT_REG_MASK) << PCI_CONF1_EXT_REG_SHIFT)

	// #define PCI_CONF1_EXT_ADDRESS(bus, dev, func, reg) \
	// 	(PCI_CONF1_ADDRESS(bus, dev, func, reg) | \
	// 	 PCI_CONF1_EXT_REG(reg))

#endif /* DRIVERS_PCI_H */
