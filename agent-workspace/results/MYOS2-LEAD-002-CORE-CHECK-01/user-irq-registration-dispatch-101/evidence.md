---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-IRQ-REGISTRATION-DISPATCH-101
record_type: fixed_time_single_symbol_definition_and_bounded_asm_anchors
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
model_per_owner: GPT-6＋Extra（Owner指定）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
runtime_evidence: NOT_RUN
---
# 第101批：固定time的IRQ注册、IOAPIC与HPET/LVT数据/调用短锚

引用范围包含真实C函数体、全局/结构体定义、独立宏与静态汇编入口以及任务书允许的真实CMake单命令和链接器实际输出节。每项物理行连续1–5行；命令文本/位域定义不被解释成外设实际响应。R100-01旧六个ID只在本批D00追加类别纠正，不复用为新的有效定义锚，也不修改旧101/101/0。接口头中的extern/原型只作未加定义验证标签的上下文。


### IR101-E001 [VERIFIED mykernel/CMakeLists.txt::list(REMOVE_ITEM KERNEL_C_SRCS)]
```cmake
list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})
```
类别：CMAKE_COMMAND；专题D00；固定time行32–32。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E002 [VERIFIED mykernel/CMakeLists.txt::list(REMOVE_ITEM KERNEL_ASM_SRCS)]
```cmake
list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})
```
类别：CMAKE_COMMAND；专题D00；固定time行33–33。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E003 [VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE KERNEL_C_SRCS)]
```cmake
file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
```
类别：CMAKE_COMMAND；专题D00；固定time行30–30。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E004 [VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE KERNEL_ASM_SRCS)]
```cmake
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
```
类别：CMAKE_COMMAND；专题D00；固定time行31–31。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E005 [VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```
类别：CMAKE_COMMAND_CONFIG_APPEND；专题D00；固定time行58–61。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E006 [VERIFIED mykernel/arch/x86_64/kernel.lds::.text]
```ld
	.text ALIGN(_Page_Size) : AT(ADDR(.text) - __START_KERNEL_map)
	{
		_text = .;
		_stext = .;
		KEEP(*(.head.text.startup))
```
类别：LINKER_OUTPUT_SECTION；专题D00；固定time行28–32。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E007 [VERIFIED mykernel/arch/x86_64/kernel.lds::.text]
```ld
		KEEP(*(.head.text))
		*(.text*)
	} : text = 0xcccccccc
```
类别：LINKER_OUTPUT_SECTION；专题D00；固定time行33–35。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E008 [VERIFIED mykernel/scripts/target_kernel.cmake::target_include_directories(kernel)]
```cmake
target_include_directories(kernel PRIVATE
	${INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/../myloader
)
```
类别：CMAKE_COMMAND；专题D00；固定time行2–5。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E009 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
				load_direct_gdt(cpu);
```
类别：C_FUNCTION；专题D00；固定time行125–125。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E010 [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
	return next_info_tag_addr(mb2_infotag_ptr);
```
类别：C_FUNCTION；专题D00；固定time行401–401。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E011 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::irq_descriptors]
```c
myos_irq_desc_s	irq_descriptors[NR_VECTORS];
```
类别：GLOBAL_OBJECT；专题A；固定time行13–13。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E012 [VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::IOAPIC_map]
```c
	typedef struct IOAPIC_map
	{
		phys_addr_t	phys_addr;
		uint8_t *	virt_idx_addr;
		uint32_t *	virt_data_addr;
```
类别：STRUCT；专题A；固定time行21–25。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E013 [VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::hw_int_controller_s]
```c
	typedef struct {
		void (*enable)(unsigned long irq);
		void (*disable)(unsigned long irq);
		unsigned long (*install)(unsigned long irq,void * arg);
		void (*uninstall)(unsigned long irq);
```
类别：ANONYMOUS_STRUCT；专题A；固定time行29–33。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E014 [VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::hw_int_controller_s]
```c
		void (*ack)(unsigned long irq);
	} hw_int_controller_s;
```
类别：ANONYMOUS_STRUCT；专题A；固定time行34–35。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E015 [VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::myos_irq_desc_s]
```c
	typedef struct {
		hw_int_controller_s * controller;

		char * 			irq_name;
		unsigned long	parameter;
```
类别：ANONYMOUS_STRUCT；专题A；固定time行37–41。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E016 [VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::myos_irq_desc_s]
```c
		unsigned long	flags;

		void (*handler)(unsigned long parameter,
						pt_regs_s *stack_frame);
	} myos_irq_desc_s;
```
类别：ANONYMOUS_STRUCT；专题A；固定time行42–46。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E017 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
int register_irq(unsigned long irq,
				 void *arg,
				 char *irq_name,
				 unsigned long parameter,
				 hw_int_controller_s *controller,
```
类别：C_FUNCTION；专题A；固定time行175–179。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E018 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
{
	myos_irq_desc_s *p = &irq_descriptors[irq];
	p->controller = controller;
	p->irq_name = irq_name;
	p->parameter = parameter;
```
类别：C_FUNCTION；专题A；固定time行181–185。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E019 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
	p->flags = 0;
	p->handler = handler;
	if (p->controller != NULL)
	{
		if (p->controller->install != NULL)
```
类别：C_FUNCTION；专题A；固定time行186–190。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E020 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
		if (p->controller->install != NULL)
			p->controller->install(irq, arg);
		if (p->controller->enable != NULL)
			p->controller->enable(irq);
	}
```
类别：C_FUNCTION；专题A；固定time行190–194。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E021 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
	return 1;
```
类别：C_FUNCTION；专题A；固定time行195–195。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E022 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::unregister_irq]
```c
int unregister_irq(unsigned long irq)
{
	myos_irq_desc_s *p = &irq_descriptors[irq];

	if (p->controller != NULL)
```
类别：C_FUNCTION；专题A；固定time行198–202。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E023 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::unregister_irq]
```c
	{
		if (p->controller->disable != NULL)
			p->controller->disable(irq);
		if (p->controller->uninstall != NULL)
			p->controller->uninstall(irq);
```
类别：C_FUNCTION；专题A；固定time行203–207。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E024 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::unregister_irq]
```c
	p->controller = 0;
	p->irq_name = 0;
	p->parameter = 0;
	p->flags = 0;
	p->handler = 0;
```
类别：C_FUNCTION；专题A；固定time行210–214。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E025 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::unregister_irq]
```c
	return 1; 
```
类别：C_FUNCTION；专题A；固定time行216–216。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E026 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::hwint_irq_handler]
```c
void hwint_irq_handler(pt_regs_s *regs)
{
	unsigned long vec = (unsigned long)regs->irq_nr;
	int irq_nr = 0;
	myos_irq_desc_s *irq_d = &irq_descriptors[vec];	
```
类别：C_FUNCTION；专题A；固定time行160–164。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E027 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::hwint_irq_handler]
```c
	if(irq_d->handler != NULL)
		irq_d->handler(irq_d->parameter, regs);
	if(irq_d->controller != NULL && irq_d->controller->ack != NULL)
		irq_d->controller->ack(irq_nr);
```
类别：C_FUNCTION；专题A；固定time行166–169。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E028 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context]
```c
{
	unsigned long vec = (unsigned long)regs->irq_nr;

	if (vec < FIRST_EXTERNAL_VECTOR)
		exception_handler(regs);
```
类别：C_FUNCTION；专题A；固定time行120–124。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E029 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context]
```c
	else
		hwint_irq_handler(regs);

	if (!in_atomic())
		schedule();
```
类别：C_FUNCTION；专题A；固定time行125–129。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E030 [VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::NR_VECTORS]
```c
	#define NR_VECTORS						256
```
类别：MACRO；专题A；固定time行111–111。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。

### IR101-E031 [VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::FIRST_EXTERNAL_VECTOR]
```c
	#define FIRST_EXTERNAL_VECTOR			0x20
```
类别：MACRO；专题A；固定time行38–38。仅支持真实定义或唯一命令，不认证实际调用返回或机器状态。


### IR101-E032 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_enable]
```c
void IOAPIC_enable(unsigned long irq_nr)
{
	unsigned long value = 0;
	value = ioapic_rte_read((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10);
```
类别：C_FUNCTION；专题B；固定time行31–34；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E033 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_enable]
```c
	value = ioapic_rte_read((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10);
	value = value & (~0x10000UL); 
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, value);
}
```
类别：C_FUNCTION；专题B；固定time行34–37；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E034 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_disable]
```c
void IOAPIC_disable(unsigned long irq_nr)
{
	unsigned long value = 0;
	value = ioapic_rte_read((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10);
```
类别：C_FUNCTION；专题B；固定time行39–42；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E035 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_disable]
```c
	value = ioapic_rte_read((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10);
	value = value | 0x10000UL; 
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, value);
}
```
类别：C_FUNCTION；专题B；固定time行42–45；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E036 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_install]
```c
unsigned long IOAPIC_install(unsigned long irq_nr, void * arg)
{
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, *(unsigned long *)arg);
	return 1;
}
```
类别：C_FUNCTION；专题B；固定time行47–51；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E037 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_uninstall]
```c
void IOAPIC_uninstall(unsigned long irq_nr)
{
	ioapic_rte_write((irq_nr - FIRST_EXTERNAL_VECTOR) * 2 + 0x10, 0x10000UL);
}
```
类别：C_FUNCTION；专题B；固定time行53–56；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E038 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_level_ack]
```c
void IOAPIC_level_ack(unsigned long irq_nr)
{
	IOAPIC_edge_ack(irq_nr);
```
类别：C_FUNCTION；专题B；固定time行58–60；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E039 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_level_ack]
```c
				
	*ioapic_map.virt_EOI_addr = irq_nr;
}
```
类别：C_FUNCTION；专题B；固定time行61–63；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E040 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_edge_ack]
```c
void IOAPIC_edge_ack(unsigned long irq)
{
	apic_msr_write(APIC_EOI, 0);
}
```
类别：C_FUNCTION；专题B；固定time行65–68；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E041 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_read]
```c
uint64_t ioapic_rte_read(uint8_t index)
{
	uint64_t ret;

	*ioapic_map.virt_idx_addr = index + 1;
```
类别：C_FUNCTION；专题B；固定time行70–74；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E042 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_read]
```c
	mb();
	ret = *ioapic_map.virt_data_addr;
	ret <<= 32;
	mb();
```
类别：C_FUNCTION；专题B；固定time行75–78；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E043 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_read]
```c
	*ioapic_map.virt_idx_addr= index;		
	mb();
	ret |= *ioapic_map.virt_data_addr;
	mb();

```
类别：C_FUNCTION；专题B；固定time行80–84；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E044 [VERIFIED mykernel/arch/x86_64/include/asm/apic.h::apic_msr_write]
```c
	static inline void apic_msr_write(u32 reg, u32 v) {
		if (reg == APIC_DFR || reg == APIC_ID || reg == APIC_LDR ||
			reg == APIC_LVR)
			return;
		wrmsr(APIC_BASE_MSR + (reg >> 4), v, 0);
```
类别：INLINE_C_FUNCTION；专题B；固定time行156–160。仅证真实定义与源级调用请求，外部ABI、寄存器执行或运行结果未证。

### IR101-E045 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_write]
```c
void ioapic_rte_write(uint8_t index, uint64_t value)
{
	*ioapic_map.virt_idx_addr = index;
	mb();
	*ioapic_map.virt_data_addr = value & 0xffffffff;
```
类别：C_FUNCTION；专题B；固定time行90–94；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E046 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_write]
```c
	mb();
	
	value >>= 32;

	*ioapic_map.virt_idx_addr = index + 1;
```
类别：C_FUNCTION；专题B；固定time行95–99；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E047 [VERIFIED mykernel/arch/x86_64/myos/apic.c::ioapic_rte_write]
```c
	mb();
	*ioapic_map.virt_data_addr = value & 0xffffffff;
	mb();

	udelay(1);
```
类别：C_FUNCTION；专题B；固定time行100–104；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E048 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_init]
```c
void IOAPIC_init()
{
	uint8_t *IOAPIC_addr =
		(uint8_t *)phys_to_virt((phys_addr_t)IO_APIC_DEFAULT_PHYS_BASE);

```
类别：C_FUNCTION；专题B；固定time行110–114；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E049 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_init]
```c
	ioapic_map.phys_addr = (phys_addr_t)IO_APIC_DEFAULT_PHYS_BASE;
	ioapic_map.virt_idx_addr = IOAPIC_addr;
	ioapic_map.virt_data_addr = (uint32_t *)(IOAPIC_addr + 0x10);
	ioapic_map.virt_EOI_addr = (uint32_t *)(IOAPIC_addr + 0x40);
```
类别：C_FUNCTION；专题B；固定time行115–118；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E050 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_init]
```c
	*ioapic_map.virt_idx_addr = 0x00;
	mb();
	*ioapic_map.virt_data_addr = 0x0f000000;
	mb();
```
类别：C_FUNCTION；专题B；固定time行123–126；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E051 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::myos_init_bsp_intr]
```c
void myos_init_bsp_intr()
{
	i8259_disable();
	//init ioapic
	IOAPIC_init();
```
类别：C_FUNCTION；专题C/ABC；固定time行222–226。只证明函数体中的真实调用和源码次序，不证明硬件请求成功。

### IR101-E052 [VERIFIED mykernel/arch/x86_64/myos/apic.c::IOAPIC_init]
```c
	//RTE	
	for(i = 0x10;i < 0x40;i += 2)
		ioapic_rte_write(i, 0x10000 + FIRST_EXTERNAL_VECTOR + ((i - 0x10) >> 1));
```
类别：C_FUNCTION；专题B；固定time行133–135；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E053 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::ioapic_retentry_T]
```c
	typedef struct __attribute__((packed))
	{
		uint32_t	vector			:8,		//0~7
					deliver_mode	:3,		//8~10
					dest_mode		:1,		//11
```
类别：PACKED_STRUCT；专题B；固定time行444–448；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E054 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::ioapic_retentry_T]
```c
					deliver_status	:1,		//12
					polarity		:1,		//13
					irr				:1,		//14
					trigger			:1,		//15
					mask			:1,		//16
```
类别：PACKED_STRUCT；专题B；固定time行449–453；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E055 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::ioapic_retentry_T]
```c
					reserved		:15;	//17~31

		union{
			struct {
				uint32_t	reserved1	:24,//32~55
```
类别：PACKED_STRUCT；专题B；固定time行454–458；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E056 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::ioapic_retentry_T]
```c
							phy_dest	:4,	//56~59
							reserved2	:4;	//60~63
			} physical;

			struct {
```
类别：PACKED_STRUCT；专题B；固定time行459–463；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E057 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::ioapic_retentry_T]
```c
				uint32_t	reserved1		:24,//32~55
							logical_dest	:8;	//56~63
			} logical;
		} dst;
	} ioapic_retentry_T;
```
类别：PACKED_STRUCT；专题B；固定time行464–468；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E058 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::IO_APIC_DEFAULT_PHYS_BASE]
```c
	#define IO_APIC_DEFAULT_PHYS_BASE	0xfec00000
```
类别：MACRO；专题B；固定time行14–14；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E059 [VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::VECTOR_IRQ]
```c
	#define VECTOR_IRQ(irq)				(irq + FIRST_EXTERNAL_VECTOR)
```
类别：MACRO；专题B；固定time行61–61；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E060 [VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::HPET_TIMER0_IRQ]
```c
		#define HPET_TIMER0_IRQ			VECTOR_IRQ(2)
```
类别：MACRO；专题B；固定time行87–87；仅为源级MMIO/寄存器请求，不证明硬件完成。

### IR101-E061 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::myos_init_percpu_intr]
```c
void myos_init_percpu_intr()
{
	LVT_timer_init();
}
```
类别：C_FUNCTION；专题C/ABC；固定time行229–232。只证明函数体中的真实调用和源码次序，不证明硬件请求成功。

### IR101-E062 [VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h::FIRST_SYSTEM_VECTOR]
```c
	#define FIRST_SYSTEM_VECTOR				LOCAL_TIMER_VECTOR
```
类别：MACRO；专题B；固定time行113–113；仅为源级MMIO/寄存器请求，不证明硬件完成。


### IR101-E063 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::HPET_int_controller]
```c
hw_int_controller_s HPET_int_controller = 
{
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
```
类别：GLOBAL_STRUCT；专题C；固定time行514–518。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E064 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::HPET_int_controller]
```c
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};
```
类别：GLOBAL_STRUCT；专题C；固定time行519–521。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E065 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::HPET_handler]
```c
void HPET_handler(unsigned long parameter, pt_regs_s * regs)
{
	jiffies++;
	do_timer(1);

```
类别：C_FUNCTION；专题C；固定time行525–529。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E066 [VERIFIED mykernel/arch/x86_64/include/asm/ptrace.h::pt_regs]
```c
		// myos push irq vec number separately
			reg_t	irq_nr;
		/*
		 * On syscall entry, this is syscall#. On CPU exception, this is error code.
		 * On hw interrupt, it's IRQ number:
```
类别：STRUCT_FIELD；专题C；固定time行31–35。仅证真实定义与源级调用请求，外部ABI、寄存器执行或运行结果未证。

### IR101-E067 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
void myos_HPET_init()
{
	ioapic_retentry_T entry;
	//init I/O APIC IRQ2 => HPET Timer 0
	entry.vector = HPET_TIMER0_IRQ;
```
类别：C_FUNCTION；专题C；固定time行539–543。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E068 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
	entry.vector = HPET_TIMER0_IRQ;
	entry.deliver_mode = APIC_DELIVERY_MODE_FIXED;
	entry.mask = APIC_LVT_MASKED >> 16;
	entry.reserved = 0;

```
类别：C_FUNCTION；专题C；固定time行543–547。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E069 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
	entry.dst.physical.reserved1 = 0;
	entry.dst.physical.phy_dest = 0;
	entry.dst.physical.reserved2 = 0;
```
类别：C_FUNCTION；专题C；固定time行548–550。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E070 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
	register_irq(HPET_TIMER0_IRQ, &entry , "HPET0",
			0, &HPET_int_controller, &HPET_handler);
}
```
类别：C_FUNCTION；专题C；固定time行552–554。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E071 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::lvt_timer_t]
```c
	/*320*/	typedef struct { /* LVT - Timer */
				u32
					vector			:  8,
					__reserved_1	:  4,
					delivery_status	:  1,
```
类别：STRUCT；专题C；固定time行327–331。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E072 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::lvt_timer_t]
```c
					timer_mode		:  1,
					__reserved_3	: 14;
				u32
					__reserved_4[3];
			} lvt_timer_t;
```
类别：STRUCT；专题C；固定time行334–338。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E073 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::lvt_timer]
```c
lvt_timer_t lvt_timer;
```
类别：GLOBAL_OBJECT；专题C；固定time行21–21。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E074 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_int_controller]
```c
hw_int_controller_s LVT_timer_int_controller = 
{
	.enable		= NULL,
	.disable	= NULL,
	.install	= NULL,
```
类别：GLOBAL_STRUCT；专题C；固定time行22–26。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E075 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_int_controller]
```c
	.install	= NULL,
	.uninstall	= NULL,
	.ack		= LVT_timer_ack,
};
```
类别：GLOBAL_STRUCT；专题C；固定time行26–29。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E076 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_handler]
```c
void LVT_timer_handler(unsigned long parameter, pt_regs_s *regs)
{
	lvt_count++;
	if ((lvt_count % SCALE) == 0)
	{
```
类别：C_FUNCTION；专题C；固定time行31–35。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E077 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_handler]
```c
		char buf[30];
		memset(buf, 0 , sizeof(buf));
		snprintf(buf, sizeof(buf), "(LVT_timer: %ld)", lvt_count / SCALE);
		myos_tty_write_color_at(buf, sizeof(buf), BLACK, GREEN, 60, 0);
	}
```
类别：C_FUNCTION；专题C；固定time行36–40。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E078 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_init]
```c
void LVT_timer_init()
{
	*((u32 *)&lvt_timer) = LAPIC_LVT_TIMER_IRQ;
	*((u32 *)&lvt_timer) |= APIC_LVT_TIMER_PERIODIC;
	
```
类别：C_FUNCTION；专题C；固定time行43–47。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E079 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_init]
```c
	register_irq(LAPIC_LVT_TIMER_IRQ, &lvt_timer, "LVT_timer",
				 *((u32 *)&lvt_timer), &LVT_timer_int_controller,
				 &LVT_timer_handler);
```
类别：C_FUNCTION；专题C；固定time行48–50。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E080 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_init]
```c
	LVT_timer_ack(0);
}
```
类别：C_FUNCTION；专题C；固定time行52–53。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E081 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_ack]
```c
void LVT_timer_ack(unsigned long param)
{
	apic_msr_write(APIC_TMICT, 0xAFF);
	apic_msr_write(APIC_LVTT, *((u32 *)&lvt_timer));
	apic_msr_write(APIC_TDCR, APIC_TDR_DIV_128);
```
类别：C_FUNCTION；专题C；固定time行55–59。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E082 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_ack]
```c
	IOAPIC_edge_ack(param);
}
```
类别：C_FUNCTION；专题C；固定time行60–61。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E083 [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::SCALE]
```c
#define SCALE 100
```
类别：MACRO；专题C；固定time行16–16。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E084 [VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::LAPIC_LVT_TIMER_IRQ]
```c
	#define LAPIC_LVT_TIMER_IRQ			VECTOR_IPI(2)
```
类别：MACRO；专题C；固定time行108–108。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E085 [VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h::VECTOR_IPI]
```c
	#define VECTOR_IPI(irq)				(irq + FIRST_SYSTEM_VECTOR)
```
类别：MACRO；专题C；固定time行105–105。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E086 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::APIC_LVT_TIMER_PERIODIC]
```c
	#define		APIC_LVT_TIMER_PERIODIC		(1 << 17)
```
类别：MACRO；专题C；固定time行103–103。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E087 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::APIC_TMICT]
```c
	#define	APIC_TMICT				0x380
```
类别：MACRO；专题C；固定time行119–119。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E088 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::APIC_LVTT]
```c
	#define	APIC_LVTT				0x320
```
类别：MACRO；专题C；固定time行92–92。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E089 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::APIC_TDCR]
```c
	#define	APIC_TDCR				0x3E0
```
类别：MACRO；专题C；固定time行121–121。只证明源级字段、回调或请求文本，真实硬件/ABI未证。

### IR101-E090 [VERIFIED mykernel/arch/x86_64/include/asm/apicdef.h::APIC_TDR_DIV_128]
```c
	#define		APIC_TDR_DIV_128			0xA
```
类别：MACRO；专题C；固定time行131–131。只证明源级字段、回调或请求文本，真实硬件/ABI未证。


### IR101-E091 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	init_IRQ();
	softirq_init();
	timekeeping_init();
```
类别：C_FUNCTION；专题C/ABC；固定time行163–165。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E092 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	console_init();

	// if (late_time_init)
		late_time_init();
```
类别：C_FUNCTION；专题C/ABC；固定time行174–177。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E093 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	check_bugs();

	myos_init_percpu_intr();

	rest_init();
```
类别：C_FUNCTION；专题C/ABC；固定time行190–194。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E094 [VERIFIED mykernel/arch/x86_64/kernel/irqinit.c::init_IRQ]
```c

		idt_setup_apic_and_irq_gates();
		// lapic_assign_system_vectors();

```
类别：C_FUNCTION；专题C/ABC；固定time行70–73。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E095 [VERIFIED mykernel/arch/x86_64/kernel/irqinit.c::init_IRQ]
```c
	// enable bsp's apic
	myos_init_bsp_intr();
}
```
类别：C_FUNCTION；专题C/ABC；固定time行81–83。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E096 [VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_apic_and_irq_gates]
```c
	for (i = FIRST_EXTERNAL_VECTOR; i < NR_VECTORS; i++)
	{
		entry = irq_entries_start + IDT_ALIGN * (i - FIRST_EXTERNAL_VECTOR);
		set_intr_gate(i, entry);
	}
```
类别：C_FUNCTION；专题C/ABC；固定time行85–89。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E097 [VERIFIED mykernel/arch/x86_64/kernel/idt.c::idt_setup_apic_and_irq_gates]
```c
	load_idt(&idt_descr);
```
类别：C_FUNCTION；专题C/ABC；固定time行94–94。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E098 [VERIFIED mykernel/arch/x86_64/entry/entry_64.S::irq_entries_start]
```asm
SYM_CODE_START(irq_entries_start)
	vector = 0
	.rept		NR_VECTORS - FIRST_EXTERNAL_VECTOR
	endbr64
	/* External hardware interrupts has no error code */
```
类别：ASM_MACRO_ENTRY；专题C/ABC；固定time行365–369。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E099 [VERIFIED mykernel/arch/x86_64/entry/entry_64.S::irq_entries_start]
```asm
	pushq		$0
	pushq		$(vector + FIRST_EXTERNAL_VECTOR)
	jmp			sa_entintr_retp
```
类别：ASM_MACRO_ENTRY；专题C/ABC；固定time行370–372。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E100 [VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
	cld
	movq		%rsp,		%rdi
	sti

	callq 		excep_hwint_context
```
类别：ASM_FUNCTION；专题C/ABC；固定time行313–317。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E101 [VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
	testb		$3,			CS(%rsp)
	jz	2f
	swapgs
2:
	cli
```
类别：ASM_FUNCTION；专题C/ABC；固定time行319–323。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E102 [VERIFIED mykernel/arch/x86_64/entry/entry_64.S::sa_entintr_retp]
```asm
	POP_REGS
	addq		$0x10,		%rsp	// skip err-code and vec-nr
	sti
	iretq
```
类别：ASM_FUNCTION；专题C/ABC；固定time行324–327。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E103 [VERIFIED mykernel/arch/x86_64/kernel/time.c::hpet_time_init]
```c
	hpet_enable(NULL);

	// setup_default_timer_irq();
extern void myos_HPET_init(void);
		myos_HPET_init();
```
类别：C_FUNCTION；专题C/ABC；固定time行34–38。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E104 [VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	/* Setup the legacy timers */
	hpet_time_init(); /* == x86_init.timers.timer_init() */

```
类别：C_FUNCTION；专题C/ABC；固定time行50–52。汇编入口、C层调度请求与实际执行/返回分离。

### IR101-E105 [VERIFIED mykernel/sched/scheduler/preempt_macro.h::in_atomic]
```c
	#define in_atomic()	(				\
				preempt_count() != 0	\
			)
```
类别：MACRO；专题C/ABC；固定time行62–64。汇编入口、C层调度请求与实际执行/返回分离。


## 静态证明边界

Hardware, IDTR, MMIO, clocksource, callback concurrency, interrupt arrival, successful EOI and real CPU scheduling are NOT_RUN / unproven. No build, tests, patch or installation.
