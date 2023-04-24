#include <linux/kernel/kernel.h>
#include <linux/kernel/slab.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/completion.h>
#include <linux/lib/string.h>
#include <linux/device/tty.h>
#include <asm/processor.h>
#include <asm/io.h>

#include <obsolete/glo.h>
#include <obsolete/printk.h>
#include <obsolete/ktypes.h>

#include <obsolete/arch_proto.h>
#include <obsolete/interrupt.h>
#include <obsolete/apic.h>
#include <obsolete/keyboard.h>
#include <obsolete/device.h>

// DECLARE_COMPLETION(getcode_done);
DECLARE_SWAIT_QUEUE_HEAD(kbd_swq_hdr);

kbd_inbuf_s * p_kb = NULL;
static int shift_l,shift_r,ctrl_l,ctrl_r,alt_l,alt_r;

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
hw_int_controller_s keyboard_int_controller = 
{
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};

void init_keyboard()
{		
	ioapic_retentry_T entry;
	unsigned long i,j;

	p_kb = (kbd_inbuf_s *)kzalloc(sizeof(kbd_inbuf_s), GFP_KERNEL);
	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;

	entry.vector = VECTOR_IRQ(KEYBOARD_IRQ);
	entry.deliver_mode = APIC_ICR_IOAPIC_Fixed ;
	entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
	entry.deliver_status = APIC_ICR_IOAPIC_Idle;
	entry.polarity = APIC_IOAPIC_POLARITY_HIGH;
	entry.irr = APIC_IOAPIC_IRR_RESET;
	entry.trigger = APIC_ICR_IOAPIC_Edge;
	entry.mask = APIC_ICR_IOAPIC_Masked;
	entry.reserved = 0;

	entry.dst.physical.reserved1 = 0;
	entry.dst.physical.phy_dest = 0;
	entry.dst.physical.reserved2 = 0;

	wait_KB_write();
	outb(KBCMD_WRITE_CMD, PORT_KB_CMD);
	wait_KB_write();
	outb(KB_INIT_MODE, PORT_KB_DATA);

	for(i = 0;i<1000;i++)
		for(j = 0;j<1000;j++)
			nop();
	
	shift_l = 0;
	shift_r = 0;
	ctrl_l  = 0;
	ctrl_r  = 0;
	alt_l   = 0;
	alt_r   = 0;

	register_irq(KEYBOARD_IRQ , &entry , "PS/2 keyboard",
				 (unsigned long)p_kb, &keyboard_int_controller,
				 &keyboard_handler);
}

void keyboard_exit()
{
	unregister_irq(KEYBOARD_IRQ);
	kfree((unsigned long *)p_kb);
}

void keyboard_handler(unsigned long param, pt_regs_s * sf_regs)
{
	unsigned char x;
	x = inb(0x60);

	int xpos;
	int ypos = 0;
	char *prompt;
	if (x & FLAG_BREAK)
	{
		xpos = 18;
		prompt = "Key Up";
	}
	else
	{
		xpos = 0;
		prompt = "Key Down";
	}
	char buf[32] = {0};
	snprintf(buf, 32, "%s : (K:%02x) ", prompt, x);
	myos_tty_write_color_at(buf, strlen(buf), BLACK, GREEN, xpos, ypos);

	if(p_kb->p_head == p_kb->buf + KB_BUF_SIZE)
		p_kb->p_head = p_kb->buf;

	*p_kb->p_head = x;
	p_kb->count++;
	p_kb->p_head ++;	

	// complete(&getcode_done);
	swake_up_locked(&kbd_swq_hdr);
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
unsigned char kbd_get_scancode()
{
	unsigned char ret  = 0;

	if(p_kb->count == 0)
	{
		// wait_for_completion(&getcode_done);
		DECLARE_SWAITQUEUE(sw_node);
		set_current_state(TASK_UNINTERRUPTIBLE);
		__prepare_to_swait(&kbd_swq_hdr, &sw_node);
		schedule();
	}
	
	if(p_kb->p_tail == p_kb->buf + KB_BUF_SIZE)	
		p_kb->p_tail = p_kb->buf;

	ret = *p_kb->p_tail;
	p_kb->count--;
	p_kb->p_tail++;

	return ret;
}

char kbd_parse_scancode()
{
	unsigned char x = 0;
	int i;	
	char key = 0;	
	int make = 0;

	x = kbd_get_scancode();
	
	if(x == 0xE1)	//pause break;
	{
		key = PAUSEBREAK;
		for(i = 1;i<6;i++)
			if(kbd_get_scancode() != pausebreak_scode[i])
			{
				key = 0;
				break;
			}
	}	
	else if(x == 0xE0) //print screen
	{
		x = kbd_get_scancode();

		switch(x)
		{
			case 0x2A: // press printscreen
				if(kbd_get_scancode() == 0xE0)
					if(kbd_get_scancode() == 0x37)
					{
						key = PRINTSCREEN;
						make = 1;
					}
				break;

			case 0xB7: // UNpress printscreen
				if(kbd_get_scancode() == 0xE0)
					if(kbd_get_scancode() == 0xAA)
					{
						key = PRINTSCREEN;
						make = 0;
					}
				break;

			case 0x1d: // press right ctrl
				ctrl_r = 1;
				key = OTHERKEY;
				break;

			case 0x9d: // UNpress right ctrl
				ctrl_r = 0;
				key = OTHERKEY;
				break;
			
			case 0x38: // press right alt
				alt_r = 1;
				key = OTHERKEY;
				break;

			case 0xb8: // UNpress right alt
				alt_r = 0;
				key = OTHERKEY;
				break;		

			default:
				key = OTHERKEY;
				break;
		}
	}
	
	if(key == 0)
	{
		unsigned char * keyrow = NULL;
		int column = 0;

		make = (x & FLAG_BREAK ? 0:1);

		keyrow = &keycode_map_normal[(x & 0x7F) * MAP_COLS];

		if(shift_l || shift_r)
			column = 1;

		key = keyrow[column];
		
		switch(x & 0x7F)
		{
			case 0x2a:	//SHIFT_L:
				shift_l = make;
				key = 0;
				break;

			case 0x36:	//SHIFT_R:
				shift_r = make;
				key = 0;
				break;

			case 0x1d:	//CTRL_L:
				ctrl_l = make;
				key = 0;
				break;

			case 0x38:	//ALT_L:
				alt_l = make;
				key = 0;
				break;

			default:
				if(!make)
					key = 0;
				break;
		}			
	}
	return key;
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
unsigned char pausebreak_scode[]={0xE1,0x1D,0x45,0xE1,0x9D,0xC5};
unsigned char keycode_map_normal[NR_SCAN_CODES * MAP_COLS] = //
{
/*scan-code	unShift		Shift		*/
/*--------------------------------------------------------------*/
/*0x00*/	0,			0,
/*0x01*/	0,			0,		// ESC
/*0x02*/	'1',		'!',
/*0x03*/	'2',		'@',
/*0x04*/	'3',		'#',
/*0x05*/	'4',		'$',
/*0x06*/	'5',		'%',
/*0x07*/	'6',		'^',
/*0x08*/	'7',		'&',
/*0x09*/	'8',		'*',
/*0x0a*/	'9',		'(',
/*0x0b*/	'0',		')',
/*0x0c*/	'-',		'_',
/*0x0d*/	'=',		'+',
/*0x0e*/	'\b',		'\b',	// BACKSPACE	
/*0x0f*/	'\t',		'\t',	// TAB
// /*0x0e*/	0,			0,		// BACKSPACE	
// /*0x0f*/	0,			0,		// TAB

/*0x10*/	'q',		'Q',
/*0x11*/	'w',		'W',
/*0x12*/	'e',		'E',
/*0x13*/	'r',		'R',
/*0x14*/	't',		'T',
/*0x15*/	'y',		'Y',
/*0x16*/	'u',		'U',
/*0x17*/	'i',		'I',
/*0x18*/	'o',		'O',
/*0x19*/	'p',		'P',
/*0x1a*/	'[',		'{',
/*0x1b*/	']',		'}',
/*0x1c*/	'\n',		'\n',	// ENTER
// /*0x1c*/	0,			0,		// ENTER
/*0x1d*/	0x1d,		0x1d,	// CTRL Left
/*0x1e*/	'a',		'A',
/*0x1f*/	's',		'S',

/*0x20*/	'd',		'D',
/*0x21*/	'f',		'F',
/*0x22*/	'g',		'G',
/*0x23*/	'h',		'H',
/*0x24*/	'j',		'J',
/*0x25*/	'k',		'K',
/*0x26*/	'l',		'L',
/*0x27*/	';',		':',
/*0x28*/	'\'',		'"',
/*0x29*/	'`',		'~',
/*0x2a*/	0x2a,		0x2a,	// SHIFT Left
/*0x2b*/	'\\',		'|',
/*0x2c*/	'z',		'Z',
/*0x2d*/	'x',		'X',
/*0x2e*/	'c',		'C',
/*0x2f*/	'v',		'V',

/*0x30*/	'b',		'B',
/*0x31*/	'n',		'N',
/*0x32*/	'm',		'M',
/*0x33*/	',',		'<',
/*0x34*/	'.',		'>',
/*0x35*/	'/',		'?',
/*0x36*/	0x36,		0x36,	// SHIFT Right
/*0x37*/	'*',		'*',
/*0x38*/	0x38,		0x38,	// ALT Left
/*0x39*/	' ',		' ',	// SPACE
/*0x3a*/	0,			0,		// CAPS LOCK
/*0x3b*/	0,			0,		// F1
/*0x3c*/	0,			0,		// F2
/*0x3d*/	0,			0,		// F3
/*0x3e*/	0,			0,		// F4
/*0x3f*/	0,			0,		// F5

/*0x40*/	0,			0,		// F6
/*0x41*/	0,			0,		// F7
/*0x42*/	0,			0,		// F8
/*0x43*/	0,			0,		// F9
/*0x44*/	0,			0,		// F10
/*0x45*/	0,			0,		// NUM LOCK
/*0x46*/	0,			0,		// SCROLL LOCK
/*0x47*/	'7',		0,		/* PAD HONE */
/*0x48*/	'8',		0,		/* PAD UP */
/*0x49*/	'9',		0,		/* PAD PAGEUP */
/*0x4a*/	'-',		0,		/* PAD MINUS */
/*0x4b*/	'4',		0,		/* PAD LEFT */
/*0x4c*/	'5',		0,		/* PAD MID */
/*0x4d*/	'6',		0,		/* PAD RIGHT */
/*0x4e*/	'+',		0,		/* PAD PLUS */
/*0x4f*/	'1',		0,		/* PAD END */

/*0x50*/	'2',		0,		/* PAD DOWN */
/*0x51*/	'3',		0,		/* PAD PAGEDOWN */
/*0x52*/	'0',		0,		/* PAD INS */
/*0x53*/	'.',		0,		/* PAD DOT */
/*0x54*/	0,			0,
/*0x55*/	0,			0,
/*0x56*/	0,			0,
/*0x57*/	0,			0,		//F11
/*0x58*/	0,			0,		//F12
/*0x59*/	0,			0,		
/*0x5a*/	0,			0,
/*0x5b*/	0,			0,
/*0x5c*/	0,			0,
/*0x5d*/	0,			0,
/*0x5e*/	0,			0,
/*0x5f*/	0,			0,

/*0x60*/	0,			0,
/*0x61*/	0,			0,
/*0x62*/	0,			0,
/*0x63*/	0,			0,
/*0x64*/	0,			0,
/*0x65*/	0,			0,
/*0x66*/	0,			0,
/*0x67*/	0,			0,
/*0x68*/	0,			0,
/*0x69*/	0,			0,
/*0x6a*/	0,			0,
/*0x6b*/	0,			0,
/*0x6c*/	0,			0,
/*0x6d*/	0,			0,
/*0x6e*/	0,			0,
/*0x6f*/	0,			0,

/*0x70*/	0,			0,
/*0x71*/	0,			0,
/*0x72*/	0,			0,
/*0x73*/	0,			0,
/*0x74*/	0,			0,
/*0x75*/	0,			0,
/*0x76*/	0,			0,
/*0x77*/	0,			0,
/*0x78*/	0,			0,
/*0x79*/	0,			0,
/*0x7a*/	0,			0,
/*0x7b*/	0,			0,
/*0x7c*/	0,			0,
/*0x7d*/	0,			0,
/*0x7e*/	0,			0,
/*0x7f*/	0,			0,
};