2024.08.30
	fork()调用时发现用户栈频繁错误，栈上内容不是调用者进程进入fork调用时正确值(rsp指向的值)

2023.11.28
多核平台上核唯一ID的获取中，从UEFI中的EFI_PROCESSOR_INFORMATION.ProcessorId获取到的ID是固件重设过的，不是initial APIC ID。而由CPUID.01H:EBX[32:24]获取的是initial APIC ID,根据群里大佬的说法，这个ID不能保证连续性，例如在不支持超线程的处理器上，这个ID全是偶数。在VMware WorkStation pro上运行的结果是UEFI获取的ID是连续的，而CPU拓扑显示该虚拟机并不支持超线程，从VMware社区的几个关于启用超线程问题帖子的回答也可知VMware不支持超线程。

2023.04.12
	经初步排查发现HPET中断不触发原因是在myos_switch_to_root_disk()中执行读硬盘后eflags中IF位清除。

2023.04.11
	发现在运行init.bin后不再发生进程调度切换。
	初步排查发现在运行init.bin前后不明原因造成HPET中断不再触发，导致jiffies不更新进而导致进程timeslice不变化。
