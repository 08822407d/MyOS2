#include <lib/string.h>
#include <sys/cdefs.h>

#include "include/arch_proto.h"
#include "../../include/printk.h"

void hwint_kbd(stack_frame_s * sf_regs)
{
	uint8_t scan_code = inb(0x60);
	color_printk(GREEN, BLACK, "KBD SCAN CODE = %#04x\n", scan_code);
}