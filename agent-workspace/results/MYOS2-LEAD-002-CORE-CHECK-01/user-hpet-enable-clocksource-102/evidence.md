# 第102批固定源码短证据：HPET启用与clocksource

来源：`time@a039d9803ade`，此证据文件是本批独立取回的 GitHub 固定源码片段，不继承第101批作者的合格锚判断。编号 E001–E020 对应原 IR101 的20项定义体资格退回；其余为新批次102自己的源码命题。每段连续1–5行，标题中的符号或宏为定义边界，源码字面和语义/运行合格性独立判断。空白、注释与独立宏各自按实际定义范围处理。全部复核不包含预处理、编译、ELF、QEMU、硬件、真实并发，full_local_P9: NOT_RUN。

## 逐条证据

### HC102-E001 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
	myos_irq_desc_s *p = &irq_descriptors[irq];
	p->controller = controller;
	p->irq_name = irq_name;
	p->parameter = parameter;
	p->flags = 0;
```
范围：D00-E017; 原101 IR101-E017 追加纠正；固定time第182–186行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E002 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::unregister_irq]
```c
	myos_irq_desc_s *p = &irq_descriptors[irq];

	if (p->controller != NULL)
	{
		if (p->controller->disable != NULL)
```
范围：D00-E022; 原101 IR101-E022 追加纠正；固定time第200–204行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E003 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::hwint_irq_handler]
```c
	unsigned long vec = (unsigned long)regs->irq_nr;
	int irq_nr = 0;
	myos_irq_desc_s *irq_d = &irq_descriptors[vec];	
```
范围：D00-E026; 原101 IR101-E026 追加纠正；固定time第162–164行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E004 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_enable]
```c
	unsigned long value = 0;
	value = ioapic_rte_read((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10);
	value = value & (~0x10000UL); 
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, value);
```
范围：D00-E032; 原101 IR101-E032 追加纠正；固定time第33–36行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E005 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_disable]
```c
	unsigned long value = 0;
	value = ioapic_rte_read((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10);
	value = value | 0x10000UL; 
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, value);
```
范围：D00-E034; 原101 IR101-E034 追加纠正；固定time第41–44行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E006 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_install]
```c
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, *(unsigned long *)arg);
	return 1;
```
范围：D00-E036; 原101 IR101-E036 追加纠正；固定time第49–50行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E007 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_uninstall]
```c
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, 0x10000UL);
```
范围：D00-E037; 原101 IR101-E037 追加纠正；固定time第55–55行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E008 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_level_ack]
```c
	IOAPIC_edge_ack(irq_nr);
				
	*ioapic_map.virt_EOI_addr = irq_nr;
```
范围：D00-E038; 原101 IR101-E038 追加纠正；固定time第60–62行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E009 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_edge_ack]
```c
	apic_msr_write(APIC_EOI, 0);
```
范围：D00-E040; 原101 IR101-E040 追加纠正；固定time第67–67行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E010 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_read]
```c
	*ioapic_map.virt_idx_addr = index + 1;
	mb();
	ret = *ioapic_map.virt_data_addr;
	ret <<= 32;
	mb();
```
范围：D00-E041; 原101 IR101-E041 追加纠正；固定time第74–78行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E011 [VERIFIED mykernel/arch/x86_64/include/asm/apic.h::apic_msr_write]
```c
		if (reg == APIC_DFR || reg == APIC_ID || reg == APIC_LDR ||
			reg == APIC_LVR)
			return;
		wrmsr(APIC_BASE_MSR + (reg >> 4), v, 0);
```
范围：D00-E044; 原101 IR101-E044 追加纠正；固定time第157–160行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E012 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_write]
```c
	*ioapic_map.virt_idx_addr = index;
	mb();
	*ioapic_map.virt_data_addr = value & 0xffffffff;
	mb();
```
范围：D00-E045; 原101 IR101-E045 追加纠正；固定time第92–95行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E013 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_init]
```c
	uint8_t *IOAPIC_addr =
		(uint8_t *)phys_to_virt((phys_addr_t)IO_APIC_DEFAULT_PHYS_BASE);

	ioapic_map.phys_addr = (phys_addr_t)IO_APIC_DEFAULT_PHYS_BASE;
	ioapic_map.virt_idx_addr = IOAPIC_addr;
```
范围：D00-E048; 原101 IR101-E048 追加纠正；固定time第112–116行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E014 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::myos_init_bsp_intr]
```c
	i8259_disable();
	//init ioapic
	IOAPIC_init();
```
范围：D00-E051; 原101 IR101-E051 追加纠正；固定time第224–226行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E015 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::myos_init_percpu_intr]
```c
	LVT_timer_init();
```
范围：D00-E061; 原101 IR101-E061 追加纠正；固定time第231–231行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E016 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::HPET_handler]
```c
	jiffies++;
	do_timer(1);
```
范围：D00-E065; 原101 IR101-E065 追加纠正；固定time第527–528行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E017 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
	ioapic_retentry_T entry;
	//init I/O APIC IRQ2 => HPET Timer 0
	entry.vector = HPET_TIMER0_IRQ;
	entry.deliver_mode = APIC_DELIVERY_MODE_FIXED;
	entry.mask = APIC_LVT_MASKED >> 16;
```
范围：D00-E067; 原101 IR101-E067 追加纠正；固定time第541–545行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E018 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_handler]
```c
	lvt_count++;
	if ((lvt_count % SCALE) == 0)
	{
```
范围：D00-E076; 原101 IR101-E076 追加纠正；固定time第33–35行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E019 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_init]
```c
	*((u32 *)&lvt_timer) = LAPIC_LVT_TIMER_IRQ;
	*((u32 *)&lvt_timer) |= APIC_LVT_TIMER_PERIODIC;
```
范围：D00-E078; 原101 IR101-E078 追加纠正；固定time第45–46行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E020 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_ack]
```c
	apic_msr_write(APIC_TMICT, 0xAFF);
	apic_msr_write(APIC_LVTT, *((u32 *)&lvt_timer));
	apic_msr_write(APIC_TDCR, APIC_TDR_DIV_128);
	IOAPIC_edge_ack(param);
```
范围：D00-E081; 原101 IR101-E081 追加纠正；固定time第57–60行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E021 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::hwint_irq_handler]
```c
	if(irq_d->handler != NULL)
		irq_d->handler(irq_d->parameter, regs);
	if(irq_d->controller != NULL && irq_d->controller->ack != NULL)
		irq_d->controller->ack(irq_nr);
```
范围：D00-S01-reload；固定time第166–169行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E022 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
		if (p->controller->install != NULL)
			p->controller->install(irq, arg);
		if (p->controller->enable != NULL)
			p->controller->enable(irq);
```
范围：D00-S01-post-install；固定time第190–193行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E023 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::unregister_irq]
```c
		if (p->controller->disable != NULL)
			p->controller->disable(irq);
		if (p->controller->uninstall != NULL)
			p->controller->uninstall(irq);
```
范围：D00-S01-post-disable；固定time第204–207行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E024 [VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::LOCAL_TIMER_VECTOR]
```c
	#define LOCAL_TIMER_VECTOR				0xec
```
范围：D00-LOCAL_TIMER_VECTOR；固定time第109–109行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E025 [VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::FIRST_SYSTEM_VECTOR]
```c
	#define FIRST_SYSTEM_VECTOR				LOCAL_TIMER_VECTOR
```
范围：D00-FIRST_SYSTEM_VECTOR；固定time第113–113行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E026 [VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::VECTOR_IPI]
```c
	#define VECTOR_IPI(irq)				(irq + FIRST_SYSTEM_VECTOR)
```
范围：D00-VECTOR_IPI；固定time第105–105行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E027 [VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::LAPIC_LVT_TIMER_IRQ]
```c
	#define LAPIC_LVT_TIMER_IRQ			VECTOR_IPI(2)
```
范围：D00-LAPIC_LVT_TIMER_IRQ；固定time第108–108行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E028 [VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::IOAPIC_map]
```c
		uint8_t *	virt_idx_addr;
		uint32_t *	virt_data_addr;
		uint32_t *	virt_EOI_addr;
```
范围：D00-S02-IOAPIC-pointer-width；固定time第24–26行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E029 [VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```
范围：D00-build-config-declaration；固定time第58–61行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E030 [VERIFIED mykernel/arch/x86_64/mm/init.c::myos_ioremap]
```c
	return (void *)__kernel_physical_mapping_init(paddr_start,
			PFN_PHYS(PFN_UP(paddr_start + size)),
				1 << PG_LEVEL_4K, PAGE_KERNEL_NOENC, true);
```
范围：A-map-return-pointer-cast；固定time第882–884行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E031 [VERIFIED mykernel/arch/x86_64/mm/init.c::__kernel_physical_mapping_init]
```c
	return paddr_last;
```
范围：A-map-return-physical-end；固定time第863–863行。限静态源码，不证明编译、IRQ、MMIO或运行状态。

### HC102-E032 [VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE KERNEL_C_SRCS)]
```c
file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
```
范围：BUILD-source-glob；固定time第30–30行。限静态源码，不证明编译、IRQ、MMIO或运行状态。


### HC102-E033 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_channel]
```c
	uint			num;
	uint			cpu;
	uint			irq;
	uint			in_use;
	enum hpet_mode	mode;
```
主题：A-channel-fields；固定time第24–28行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E034 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_base]
```c
	uint			nr_channels;
	uint			nr_clockevents;
	uint			boot_cfg;
	hpet_channel_s	*channels;
```
主题：A-base-fields；固定time第34–37行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E035 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_address]
```c
ulong					hpet_address;
```
主题：A-address-global；固定time第48–48行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E036 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_virt_address]
```c
static void __iomem		*hpet_virt_address;
```
主题：A-mapping-pointer-global；固定time第50–50行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E037 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_freq]
```c
static ulong			hpet_freq;
```
主题：B-frequency-global；固定time第53–53行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E038 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::clocksource_hpet]
```c
	.name		= "hpet",
	.rating		= 250,
	.read		= read_hpet,
	.mask		= HPET_MASK,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
```
主题：C-register-object；固定time第60–64行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E039 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::clocksource_hpet]
```c
	.enable		= hpet_enable,
	.list		= LIST_HEAD_INIT(clocksource_hpet.list),
```
主题：C-clocksource-enable-callback；固定time第66–67行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E040 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_readl]
```c
	return readl(hpet_virt_address + a);
```
主题：A-readl-wrapper；固定time第72–72行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E041 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_writel]
```c
	writel(d, hpet_virt_address + a);
```
主题：A-writel-wrapper；固定time第76–76行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E042 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_set_mapping]
```c
	hpet_address = 0xfed00000;
	hpet_virt_address = myos_ioremap(hpet_address, HPET_MMAP_SIZE);
	flush_tlb_local();
```
主题：A-map-then-flush；固定time第80–82行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E043 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_clear_mapping]
```c
	iounmap(hpet_virt_address);
	hpet_virt_address = NULL;
```
主题：A-request-iounmap-then-NULL；固定time第86–87行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E044 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_cfg_working]
```c
	for (i = 0; i < 1000; i++) {
		if (hpet_readl(HPET_CFG) != 0xFFFFFFFF)
			return true;
	}
```
主题：A-config-working-loop；固定time第347–350行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E045 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	hpet_set_mapping();
	if (!hpet_virt_address)
		return 0;
```
主题：A-early-0-return；固定time第396–398行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E046 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	if (!hpet_cfg_working())
		goto out_nohpet;
```
主题：A-config-failure-exit；固定time第401–402行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E047 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	hpet_period = hpet_readl(HPET_PERIOD);
	if (hpet_period < HPET_MIN_PERIOD || hpet_period > HPET_MAX_PERIOD)
		goto out_nohpet;
```
主题：B-period-range；固定time第407–409行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E048 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	freq = FSEC_PER_SEC;
	do_div(freq, hpet_period);
	hpet_freq = freq;
```
主题：B-frequency-division；固定time第412–414行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E049 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	id = hpet_readl(HPET_ID);
	// hpet_print_config();

	/* This is the HPET channel number which is zero based */
	channels = ((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT) + 1;
```
主题：A-ID-and-channel-count；固定time第420–424行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E050 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	hc = kcalloc(channels, sizeof(*hc), GFP_KERNEL);
	if (!hc) {
		pr_warn("Disabling HPET.\n");
		goto out_nohpet;
```
主题：A-alloc-failure；固定time第433–436行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E051 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	hpet_base.channels = hc;
	hpet_base.nr_channels = channels;
```
主题：A-publish-channel-storage；固定time第438–439行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E052 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	cfg = hpet_readl(HPET_CFG);
	hpet_base.boot_cfg = cfg;
	cfg &= ~(HPET_CFG_ENABLE | HPET_CFG_LEGACY);
	hpet_writel(cfg, HPET_CFG);
```
主题：A-sanitize-global-config；固定time第442–445行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E053 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	for (i = 0; i < channels; i++, hc++) {
		hc->num = i;

		cfg = hpet_readl(HPET_Tn_CFG(i));
		hc->boot_cfg = cfg;
```
主题：A-per-channel-boot-cfg；固定time第450–454行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E054 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
		irq = (cfg & Tn_INT_ROUTE_CNF_MASK) >> Tn_INT_ROUTE_CNF_SHIFT;
		hc->irq = irq;

		cfg &= ~(HPET_TN_ENABLE | HPET_TN_LEVEL | HPET_TN_FSB);
		hpet_writel(cfg, HPET_Tn_CFG(i));
```
主题：A-per-channel-route-and-write；固定time第455–459行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E055 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
		cfg &= ~(HPET_TN_PERIODIC | HPET_TN_PERIODIC_CAP
			 | HPET_TN_64BIT_CAP | HPET_TN_32BIT | HPET_TN_ROUTE
			 | HPET_TN_FSB | HPET_TN_FSB_CAP);
		if (cfg)
			pr_warn("Channel #%u config: Unknown bits %#x\n", i, cfg);
```
主题：A-second-mask-check-not-written；固定time第461–465行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E056 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	if (!hpet_counting())
		goto out_nohpet;
```
主题：B-counting-gate；固定time第474–475行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E057 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	clocksource_register_hz(&clocksource_hpet, (u32)hpet_freq);
```
主题：C-register-hz-narrow-u32；固定time第479–479行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E058 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	if (id & HPET_ID_LEGSUP) {
		// hpet_legacy_clockevent_register(&hpet_base.channels[0]);
		myos_hpet_clkevt_set_state_periodic(&clocksource_hpet);
		hpet_base.channels[0].mode = HPET_MODE_LEGACY;
```
主题：A-legacy-periodic；固定time第481–484行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E059 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
		return 1;
	}
	return 0;
```
主题：A-legacy-vs-not-return；固定time第487–489行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E060 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
out_nohpet:
	kfree(hpet_base.channels);
	hpet_base.channels = NULL;
	hpet_base.nr_channels = 0;
	hpet_clear_mapping();
```
主题：A-out_nohpet-cleanup；固定time第491–495行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E061 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	hpet_address = 0;
	return 0;
```
主题：A-out_nohpet-address-null；固定time第496–497行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E062 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_stop_counter]
```c
	u32 cfg = hpet_readl(HPET_CFG);

	cfg &= ~HPET_CFG_ENABLE;
	hpet_writel(cfg, HPET_CFG);
```
主题：B-stop-counter-request；固定time第136–139行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E063 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_reset_counter]
```c
	hpet_writel(0, HPET_COUNTER);
	hpet_writel(0, HPET_COUNTER + 4);
```
主题：B-reset-low-high；固定time第144–145行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E064 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_start_counter]
```c
	uint cfg = hpet_readl(HPET_CFG);

	cfg |= HPET_CFG_ENABLE;
	hpet_writel(cfg, HPET_CFG);
```
主题：B-start-counter-request；固定time第150–153行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E065 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_restart_counter]
```c
	hpet_stop_counter();
	hpet_reset_counter();
	hpet_start_counter();
```
主题：B-restart-counter-order；固定time第158–160行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E066 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_counting]
```c
	hpet_restart_counter();

	t1 = hpet_readl(HPET_COUNTER);
	start = rdtsc();
```
主题：B-restart-counter-and-TSC；固定time第360–363行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E067 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_counting]
```c
	do {
		if (t1 != hpet_readl(HPET_COUNTER))
			return true;
		now = rdtsc();
	} while ((now - start) < 200000UL);
```
主题：B-counter-progress-bounded-loop；固定time第371–375行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E068 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	uint channel = 0;
	// u32 mult = 429496730;
	// u32 shift = 32;
	u32 mult = clksrc->mult;
	u32 shift = clksrc->shift;
```
主题：B-periodic-input；固定time第205–209行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E069 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_stop_counter();
	delta = ((uint64_t)(NSEC_PER_SEC / HZ)) * mult;
	delta >>= shift + 7;
	now = hpet_readl(HPET_COUNTER);
	cmp = now + (uint)delta;
```
主题：B-periodic-delta；固定time第214–218行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E070 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	cfg = hpet_readl(HPET_Tn_CFG(channel));
	// cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL | HPET_TN_32BIT;
	cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL;
	hpet_writel(cfg, HPET_Tn_CFG(channel));
	hpet_writel(cmp, HPET_Tn_CMP(channel));
```
主题：B-periodic-cfg-comparator；固定time第219–223行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E071 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_writel((uint)delta, HPET_Tn_CMP(channel));
	hpet_start_counter();

	return 0;
```
主题：B-periodic-compare-second-write；固定time第232–235行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E072 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::read_hpet]
```c
		return (u64)hpet_readl(HPET_COUNTER);
```
主题：B-active-clocksource-read；固定time第283–283行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E073 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_MIN_PERIOD]
```c
	#define HPET_MIN_PERIOD		100000UL
```
主题：B-period-lower-bound；固定time第63–63行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E074 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_MAX_PERIOD]
```c
	#define HPET_MAX_PERIOD		100000000UL
```
主题：B-period-upper-bound；固定time第58–58行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E075 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_ID_LEGSUP]
```c
	#define HPET_ID_LEGSUP			0x00008000
```
主题：A-legacy-bit；固定time第35–35行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E076 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_ID_NUMBER]
```c
	#define HPET_ID_NUMBER			0x00001f00
```
主题：A-channel-bits；固定time第33–33行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E077 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_TN_PERIODIC]
```c
	#define HPET_TN_PERIODIC		0x0008
```
主题：B-timer-periodic-bit；固定time第47–47行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E078 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_PERIOD]
```c
	#define HPET_PERIOD			0x004
```
主题：B-period-register-offset；固定time第13–13行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E079 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_CFG]
```c
	#define HPET_CFG			0x010
```
主题：B-config-register-offset；固定time第14–14行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E080 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_MMAP_SIZE]
```c
	#define HPET_MMAP_SIZE		1024
```
主题：A-mapping-size；固定time第10–10行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E081 [VERIFIED mykernel/include/linux/kernel/hpet.h::Tn_INT_ROUTE_CNF_MASK]
```c
	#define	Tn_INT_ROUTE_CNF_MASK		(0x3e00UL)
```
主题：A-route-IRQ-mask；固定time第72–72行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E082 [VERIFIED mykernel/include/linux/kernel/asm-generic/div64.h::do_div]
```c
	#define do_div(n,base) ({						\
				uint32_t __base = (base);			\
				uint32_t __rem;						\
				__rem = ((uint64_t)(n)) % __base;	\
				(n) = ((uint64_t)(n)) / __base;		\
```
主题：B-do_div-definition；固定time第44–48行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E083 [VERIFIED mykernel/time/misc/time_misc_const.h::FSEC_PER_SEC]
```c
	#define FSEC_PER_SEC			1000000000000000LL
```
主题：B-fsec-per-sec；固定time第34–34行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E084 [VERIFIED mykernel/time/misc/time_misc_const.h::NSEC_PER_SEC]
```c
	#define NSEC_PER_SEC			1000000000L
```
主题：B-nsec-per-sec；固定time第32–32行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E085 [VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
```
主题：B-systick-rate；固定time第9–9行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。

### HC102-E086 [VERIFIED mykernel/arch/x86_64/insns/x86msr.h::rdtsc]
```c
			DECLARE_ARGS(val, low, high);

			asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));

			return EAX_EDX_VAL(val, low, high);
```
主题：B-TSC-read-intrinsic；固定time第63–67行。只证明源级对象/参数/读写请求，执行、设备、返回成功不外推。


### HC102-E087 [VERIFIED mykernel/time/clocksource/clocksource_macro.h::clocksource_register_hz]
```c
	#define clocksource_register_hz(cs, hz) \
				__clocksource_register_scale(cs, 1, hz);
```
主题：C-register-macro；固定time行9–10。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E088 [VERIFIED mykernel/time/clocksource/clocksource_macro.h::CLOCKSOURCE_MASK]
```c
	#define CLOCKSOURCE_MASK(bits)	GENMASK_ULL((bits) - 1, 0)
```
主题：C-counter-mask；固定time行7–7。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E089 [VERIFIED mykernel/time/clocksource/clocksource_types.h::clocksource]
```c
	struct clocksource {
		u64				(*read)(clocksrc_s *cs);
		u64				mask;
		u32				mult;
		u32				shift;
```
主题：C-read-mask-mult；固定time行70–74。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E090 [VERIFIED mykernel/time/clocksource/clocksource_types.h::clocksource]
```c
		u64				max_cycles;
		const char		*name;
		List_s			list;
		u32				freq_khz;
		int				rating;
```
主题：C-rating-list；固定time行81–85。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E091 [VERIFIED mykernel/time/clocksource/clocksource_types.h::clocksource]
```c
		ulong			flags;

		int				(*enable)(clocksrc_s *cs);
		void			(*disable)(clocksrc_s *cs);
```
主题：C-flags-enable；固定time行88–91。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E092 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
	if (freq) {
```
主题：C-nonzero-freq；固定time行205–205。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E093 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
		sec = cs->mask;
		do_div(sec, freq);
		do_div(sec, scale);
		if (!sec)
```
主题：C-mask-over-freq；固定time行215–218。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E094 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
		else if (sec > 600 && cs->mask > UINT_MAX)
			sec = 600;

		clocks_calc_mult_shift(&cs->mult, &cs->shift, freq, NSEC_PER_SEC / scale, sec * scale);
```
主题：C-mult-shift；固定time行220–223。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E095 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	/* Initialize mult/shift and max_idle_ns */
	__clocksource_update_freq_scale(cs, scale, freq);
```
主题：C-register-conversion；固定time行413–414。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E096 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	clocksource_enqueue(cs);
	// clocksource_enqueue_watchdog(cs);
```
主题：C-register-enqueue；固定time行420–421。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E097 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_enqueue]
```c
	list_header_for_each_container(tmp, &clocksource_list, list) {
		/* Keep track of the place, where to insert */
		if (tmp->rating < cs->rating)
			break;
		entry = &tmp->list;
```
主题：C-rating-order；固定time行377–381。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E098 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_enqueue]
```c
	list_add(&cs->list, entry);
	clocksource_list.count++;
```
主题：C-list-count；固定time行383–384。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E099 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_find_best]
```c
	if (list_header_is_empty(&clocksource_list))
		return NULL;
```
主题：C-empty-guard；固定time行277–278。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E100 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_find_best]
```c
	list_header_for_each_container(cs, &clocksource_list, list) {
		if (skipcur && cs == curr_clocksource)
			continue;
		if (oneshot && !(cs->flags & CLOCK_SOURCE_VALID_FOR_HRES))
			continue;
```
主题：C-best-constraints；固定time行285–289。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E101 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_select]
```c
	/* Find the best suitable clocksource */
	best = clocksource_find_best(oneshot, skipcur);
	if (!best)
		return;
```
主题：C-select-best；固定time行301–304。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E102 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_select]
```c
	// if (curr_clocksource != best && !timekeeping_notify(best)) {
	// 	pr_info("Switched to clocksource %s\n", best->name);
	// 	curr_clocksource = best;
	// }
```
主题：C-switch-commented；固定time行341–344。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E103 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_done_booting]
```c
	curr_clocksource = clocksource_default_clock();
	// finished_booting = 1;
	// /*
	//  * Run the watchdog first to eliminate unstable clock sources
	//  */
```
主题：C-default-current；固定time行357–361。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E104 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_done_booting]
```c
	clocksource_select();
	// mutex_unlock(&clocksource_mutex);
	return 0;
```
主题：C-select-request；固定time行363–365。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E105 [VERIFIED mykernel/time/systick/systick.c::clocksource_jiffies]
```c
	.name				= "jiffies",
	.rating				= 1,							/* lowest valid rating*/
	// .uncertainty_margin	= 32 * NSEC_PER_MSEC,
	.read				= jiffies_read,
	.mask				= CLOCKSOURCE_MASK(32),
```
主题：C-jiffies-fields；固定time行21–25。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E106 [VERIFIED mykernel/time/systick/systick.c::jiffies_read]
```c
	return (u64) jiffies;
```
主题：C-jiffies-read；固定time行34–34。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E107 [VERIFIED mykernel/time/systick/systick.c::clocksource_default_clock]
```c
	return &clocksource_jiffies;
```
主题：C-default-return；固定time行38–38。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E108 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	clock = clocksource_default_clock();
	if (clock->enable)
		clock->enable(clock);	
	tk_setup_internals(tk, clock);
```
主题：C-timekeeping-default；固定time行253–256。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E109 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
	old_clock = tk->tkr_mono.clock;
	tk->tkr_mono.clock = clock;
	tk->tkr_mono.mask = clock->mask;
	tk->tkr_mono.cycle_last = tk_clock_read(&tk->tkr_mono);

```
主题：C-tk-clock-and-mask；固定time行134–138。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E110 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
	 * active clocksource. These value will be adjusted via NTP
	 * to counteract clock drifting.
	 */
	tk->tkr_mono.mult = clock->mult;
	tk->tkr_raw.mult = clock->mult;
```
主题：C-tk-mult-cache；固定time行181–185。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E111 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_get_ns]
```c
	return timekeeping_cycles_to_ns(tkr, tk_clock_read(tkr));
```
主题：C-time-consumer；固定time行228–228。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E112 [VERIFIED mykernel/time/timekeeping/timekeeping.h::tk_clock_read]
```c
			clocksrc_s *clock = READ_ONCE(tkr->clock);
			return clock->read(clock);
```
主题：C-clock-callback-read；固定time行81–82。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E113 [VERIFIED mykernel/time/timekeeping/timekeeping.c::do_timer]
```c
	jiffies_64 += ticks;
```
主题：C-do_timer；固定time行272–272。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E114 [VERIFIED mykernel/arch/x86_64/kernel/time.c::hpet_time_init]
```c
	hpet_enable(NULL);
```
主题：C-hpet-return-ignored；固定time行34–34。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E115 [VERIFIED mykernel/arch/x86_64/kernel/time.c::hpet_time_init]
```c
		myos_HPET_init();
```
主题：C-hpet-irq-subsequent；固定time行38–38。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E116 [VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	hpet_time_init(); /* == x86_init.timers.timer_init() */
```
主题：C-late-time-init；固定time行51–51。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E117 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	init_IRQ();
	softirq_init();
	timekeeping_init();
```
主题：C-timekeeper-earlier；固定time行163–165。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E118 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	// if (late_time_init)
		late_time_init();
```
主题：C-hpet-later；固定time行176–177。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E119 [VERIFIED mykernel/lib/list/double_list_macro.h::list_header_for_each_container]
```c
	#define list_header_for_each_container(pos, header, member)	\
				list_for_each_entry(pos, &((header)->anchor), member)
```
主题：C-list-loop；固定time行346–347。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。

### HC102-E120 [VERIFIED mykernel/lib/list/double_list.h::list_header_is_empty]
```c
			return READ_ONCE(lhdr_p->count) == 0;
		}
```
主题：C-count-guard；固定time行631–632。是代码原文；注册、选择和消费不能互相替代，硬件与运行仍待证。


### HC102-E121 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_mode]
```c
	HPET_MODE_UNUSED,
	HPET_MODE_LEGACY,
	HPET_MODE_CLOCKEVT,
	HPET_MODE_DEVICE,
```
主题：A-枚举模式成员独立定义；固定time第16–19行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E122 [VERIFIED mykernel/arch/x86_64/mm/init.c::iounmap]
```c
{

}
```
主题：A-空函数体，区别调用与解除映射；固定time第887–889行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E123 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_COUNTER]
```c
	#define HPET_COUNTER		0x0f0
```
主题：B-32bit读偏移独立宏；固定time第16–16行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E124 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_Tn_CFG]
```c
	#define HPET_Tn_CFG(n)		(0x100 + 0x20 * n)
```
主题：A-按通道配置偏移；固定time第18–18行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E125 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_Tn_CMP]
```c
	#define HPET_Tn_CMP(n)		(0x108 + 0x20 * n)
```
主题：B-按通道比较偏移；固定time第19–19行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E126 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_ID_NUMBER_SHIFT]
```c
	#define	HPET_ID_NUMBER_SHIFT	8
```
主题：A-通道数移位独立宏；固定time第37–37行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E127 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_CFG_ENABLE]
```c
	#define HPET_CFG_ENABLE			0x001
```
主题：B-计数器使能标志；固定time第40–40行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E128 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_CFG_LEGACY]
```c
	#define HPET_CFG_LEGACY			0x002
```
主题：A-legacy清位；固定time第41–41行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E129 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_TN_ENABLE]
```c
	#define HPET_TN_ENABLE			0x0004
```
主题：B-通道使能标志；固定time第46–46行。独立源码定义补锚，不扩张到硬件/运行语义。

### HC102-E130 [VERIFIED mykernel/arch/x86_64/include/asm/hpet.h::HPET_TN_SETVAL]
```c
	#define HPET_TN_SETVAL			0x0040
```
主题：B-比较值置位标志；固定time第50–50行。独立源码定义补锚，不扩张到硬件/运行语义。

## 三项层次分离

1. 映射申请与`hpet_virt_address`保存并非证明目标虚拟映射真实可访问；`myos_ioremap`返回`paddr_last`强制指针的契约异常已有FM96-F03/U08先行根。
2. `hpet_enable`计数检查后的`clocksource_register_hz`仅开始注册/换算/入队；其返回0/1、legacy分支与是否选择时钟源各自独立。
3. `clocksource_default_clock`当前返回jiffies，`timekeeping_init`绑定默认源，而`__clocksource_select`内提交新`curr_clocksource`的代码被注释；不把注册推论为timekeeper已切源。

上述三项仅为固定文本的条件性边界；旧`MYOS2-LEAD-002::CA-05`和`MYOS2-LEAD-002-CORE-CHECK-01::V14`继续有其原始证据上限。
