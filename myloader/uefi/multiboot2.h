/*   multiboot2.h - Multiboot 2 header file. */
/*   Copyright (C) 1999,2003,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1

		/*  How many bytes from the start of the file we search for the header. */
	#define MULTIBOOT_SEARCH                        32768
	#define MULTIBOOT_HEADER_ALIGN                  8

	/*  The magic field should contain this. */
	#define MULTIBOOT2_HEADER_MAGIC                 0xe85250d6

	/*  This should be in %eax. */
	#define MULTIBOOT2_BOOTLOADER_MAGIC             0x36d76289

	/*  Alignment of multiboot modules. */
	#define MULTIBOOT_MOD_ALIGN                     0x00001000

	/*  Alignment of the multiboot info structure. */
	#define MULTIBOOT_INFO_ALIGN                    0x00000008

	/*  Flags set in the ’flags’ member of the multiboot header. */

	#define MULTIBOOT_TAG_ALIGN                  8
	#define MULTIBOOT_TAG_TYPE_END               0
	#define MULTIBOOT_TAG_TYPE_CMDLINE           1
	#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME  2
	#define MULTIBOOT_TAG_TYPE_MODULE            3
	#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO     4
	#define MULTIBOOT_TAG_TYPE_BOOTDEV           5
	#define MULTIBOOT_TAG_TYPE_MMAP              6
	#define MULTIBOOT_TAG_TYPE_VBE               7
	#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER       8
	#define MULTIBOOT_TAG_TYPE_ELF_SECTIONS      9
	#define MULTIBOOT_TAG_TYPE_APM               10
	#define MULTIBOOT_TAG_TYPE_EFI32             11
	#define MULTIBOOT_TAG_TYPE_EFI64             12
	#define MULTIBOOT_TAG_TYPE_SMBIOS            13
	#define MULTIBOOT_TAG_TYPE_ACPI_OLD          14
	#define MULTIBOOT_TAG_TYPE_ACPI_NEW          15
	#define MULTIBOOT_TAG_TYPE_NETWORK           16
	#define MULTIBOOT_TAG_TYPE_EFI_MMAP          17
	#define MULTIBOOT_TAG_TYPE_EFI_BS            18
	#define MULTIBOOT_TAG_TYPE_EFI32_IH          19
	#define MULTIBOOT_TAG_TYPE_EFI64_IH          20
	#define MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR    21

	#define MULTIBOOT_HEADER_TAG_END  0
	#define MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST  1
	#define MULTIBOOT_HEADER_TAG_ADDRESS  2
	#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS  3
	#define MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS  4
	#define MULTIBOOT_HEADER_TAG_FRAMEBUFFER  5
	#define MULTIBOOT_HEADER_TAG_MODULE_ALIGN  6
	#define MULTIBOOT_HEADER_TAG_EFI_BS        7
	#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32  8
	#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64  9
	#define MULTIBOOT_HEADER_TAG_RELOCATABLE  10

	#define MULTIBOOT_ARCHITECTURE_I386  0
	#define MULTIBOOT_ARCHITECTURE_MIPS32  4
	#define MULTIBOOT_HEADER_TAG_OPTIONAL 1

	#define MULTIBOOT_LOAD_PREFERENCE_NONE 0
	#define MULTIBOOT_LOAD_PREFERENCE_LOW 1
	#define MULTIBOOT_LOAD_PREFERENCE_HIGH 2

	#define MULTIBOOT_CONSOLE_FLAGS_CONSOLE_REQUIRED 1
	#define MULTIBOOT_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2


	#ifndef ASM_FILE

		#ifdef __PI_UEFI_H__
			typedef UINT8           multiboot_uint8_t;
			typedef UINT16          multiboot_uint16_t;
			typedef UINT32          multiboot_uint32_t;
			typedef UINT64          multiboot_uint64_t;
		#else
			typedef uint8_t         multiboot_uint8_t;
			typedef uint16_t        multiboot_uint16_t;
			typedef uint32_t        multiboot_uint32_t;
			typedef uint64_t        multiboot_uint64_t;
		#endif

		struct multiboot_header
		{
			/*  Must be MULTIBOOT_MAGIC - see above. */
			multiboot_uint32_t magic;

			/*  ISA */
			multiboot_uint32_t architecture;

			/*  Total header length. */
			multiboot_uint32_t header_length;

			/*  The above fields plus this one must equal 0 mod 2^32. */
			multiboot_uint32_t checksum;
		};
		typedef struct multiboot_header mb2_hdr_s;

		struct multiboot_header_tag
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
		};
		typedef struct multiboot_header_tag mb2_hdr_tag_s;

		struct multiboot_header_tag_information_request
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
			multiboot_uint32_t requests[0];
		};

		struct multiboot_header_tag_address
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
			multiboot_uint32_t header_addr;
			multiboot_uint32_t load_addr;
			multiboot_uint32_t load_end_addr;
			multiboot_uint32_t bss_end_addr;
		};
		typedef struct multiboot_header_tag_address mb2_hdr_tag_kaddr_s;

		struct multiboot_header_tag_entry_address
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
			multiboot_uint32_t entry_addr;
		};

		struct multiboot_header_tag_console_flags
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
			multiboot_uint32_t console_flags;
		};

		struct multiboot_header_tag_framebuffer
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
			multiboot_uint32_t width;
			multiboot_uint32_t height;
			multiboot_uint32_t depth;
		};

		struct multiboot_header_tag_module_align
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
		};

		struct multiboot_header_tag_relocatable
		{
			multiboot_uint16_t type;
			multiboot_uint16_t flags;
			multiboot_uint32_t size;
			multiboot_uint32_t min_addr;
			multiboot_uint32_t max_addr;
			multiboot_uint32_t align;
			multiboot_uint32_t preference;
		};

		struct multiboot_color
		{
			multiboot_uint8_t red;
			multiboot_uint8_t green;
			multiboot_uint8_t blue;
		};

		struct multiboot_mmap_entry
		{
			multiboot_uint64_t addr;
			multiboot_uint64_t len;
		#define MULTIBOOT_MEMORY_AVAILABLE              1
		#define MULTIBOOT_MEMORY_RESERVED               2
		#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
		#define MULTIBOOT_MEMORY_NVS                    4
		#define MULTIBOOT_MEMORY_BADRAM                 5
			multiboot_uint32_t type;
			multiboot_uint32_t zero;
		};
		typedef struct multiboot_mmap_entry mb_memmap_s;

		struct multiboot_tag
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
		};
		typedef struct multiboot_tag mbi_tag_s;

		struct multiboot_tag_string
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			char string[0];
		};
		typedef struct multiboot_tag_string mbi_string_s;

		struct multiboot_tag_module
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t mod_start;
			multiboot_uint32_t mod_end;
			char cmdline[0];
		};
		typedef struct multiboot_tag_module mbi_module_s;

		struct multiboot_tag_basic_meminfo
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t mem_lower;
			multiboot_uint32_t mem_upper;
		};
		typedef struct multiboot_tag_basic_meminfo mbi_basic_meminfo_s;

		struct multiboot_tag_bootdev
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t biosdev;
			multiboot_uint32_t slice;
			multiboot_uint32_t part;
		};
		typedef struct multiboot_tag_bootdev mbi_bootdev_s;


		struct multiboot_tag_mmap
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t entry_size;
			multiboot_uint32_t entry_version;
			struct multiboot_mmap_entry entries[0];  
		};
		typedef struct multiboot_tag_mmap mbi_mmap_s;

		struct multiboot_vbe_info_block
		{
			multiboot_uint8_t external_specification[512];
		};

		struct multiboot_vbe_mode_info_block
		{
			multiboot_uint8_t external_specification[256];
		};

		struct multiboot_tag_vbe
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;

			multiboot_uint16_t vbe_mode;
			multiboot_uint16_t vbe_interface_seg;
			multiboot_uint16_t vbe_interface_off;
			multiboot_uint16_t vbe_interface_len;

			struct multiboot_vbe_info_block vbe_control_info;
			struct multiboot_vbe_mode_info_block vbe_mode_info;
		};
		typedef struct multiboot_tag_vbe mbi_vbe_s;

		struct multiboot_tag_framebuffer_common
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;

			multiboot_uint64_t framebuffer_addr;
			// here I save scanline in pitch field
			multiboot_uint32_t framebuffer_pitch;
			multiboot_uint32_t framebuffer_width;
			multiboot_uint32_t framebuffer_height;
			multiboot_uint8_t framebuffer_bpp;
		#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED      0
		#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB          1
		#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
			multiboot_uint8_t framebuffer_type;
			multiboot_uint16_t reserved;
		};
		typedef struct multiboot_tag_framebuffer_common mbi_fb_common_s;

		struct multiboot_tag_framebuffer
		{
			struct multiboot_tag_framebuffer_common common;

			union
			{
				struct
				{
					multiboot_uint16_t framebuffer_palette_num_colors;
					struct multiboot_color framebuffer_palette[0];
				};
				struct
				{
					multiboot_uint8_t framebuffer_red_field_position;
					multiboot_uint8_t framebuffer_red_mask_size;
					multiboot_uint8_t framebuffer_green_field_position;
					multiboot_uint8_t framebuffer_green_mask_size;
					multiboot_uint8_t framebuffer_blue_field_position;
					multiboot_uint8_t framebuffer_blue_mask_size;
				};
			};
		};
		typedef struct multiboot_tag_framebuffer mbi_framebuffer_s;

		struct multiboot_tag_elf_sections
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t num;
			multiboot_uint32_t entsize;
			multiboot_uint32_t shndx;
			char sections[0];
		};

		struct multiboot_tag_apm
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint16_t version;
			multiboot_uint16_t cseg;
			multiboot_uint32_t offset;
			multiboot_uint16_t cseg_16;
			multiboot_uint16_t dseg;
			multiboot_uint16_t flags;
			multiboot_uint16_t cseg_len;
			multiboot_uint16_t cseg_16_len;
			multiboot_uint16_t dseg_len;
		};

		struct multiboot_tag_efi32
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t pointer;
		};

		struct multiboot_tag_efi64
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint64_t pointer;
		};

		struct multiboot_tag_smbios
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint8_t major;
			multiboot_uint8_t minor;
			multiboot_uint8_t reserved[6];
			multiboot_uint8_t tables[0];
		};

		struct multiboot_tag_old_acpi
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint8_t rsdp[0];
		};

		struct multiboot_tag_new_acpi
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint8_t rsdp[0];
		};

		struct multiboot_tag_network
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint8_t dhcpack[0];
		};

		struct multiboot_tag_efi_mmap
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t descr_size;
			multiboot_uint32_t descr_vers;
			multiboot_uint8_t efi_mmap[0];
		}; 
		typedef struct multiboot_tag_efi_mmap mbi_efi_mmap_s;

		struct multiboot_tag_efi32_ih
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t pointer;
		};

		struct multiboot_tag_efi64_ih
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint64_t pointer;
		};
		typedef struct multiboot_tag_efi64_ih mbi_efi64_ih_s;

		struct multiboot_tag_load_base_addr
		{
			multiboot_uint32_t type;
			multiboot_uint32_t size;
			multiboot_uint32_t load_base_addr;
		};
		typedef struct multiboot_tag_load_base_addr mbi_load_base_addr_s;

	#endif /*  ! ASM_FILE */

#endif /*  ! MULTIBOOT_HEADER */


// /*  kernel.c - the C part of the kernel */
// /*  Copyright (C) 1999, 2010  Free Software Foundation, Inc.
//  *
//  * This program is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  */

// #include "multiboot2.h"

// /*  Macros. */

// /*  Some screen stuff. */
// /*  The number of columns. */
// #define COLUMNS                 80
// /*  The number of lines. */
// #define LINES                   24
// /*  The attribute of an character. */
// #define ATTRIBUTE               7
// /*  The video memory address. */
// #define VIDEO                   0xB8000

// /*  Variables. */
// /*  Save the X position. */
// static int xpos;
// /*  Save the Y position. */
// static int ypos;
// /*  Point to the video memory. */
// static volatile unsigned char *video;

// /*  Forward declarations. */
// void cmain (unsigned long magic, unsigned long addr);
// static void cls (void);
// static void itoa (char *buf, int base, int d);
// static void putchar (int c);
// void printf (const char *format, ...);

// /*  Check if MAGIC is valid and print the Multiboot information structure
// 	 pointed by ADDR. */
// void
// cmain (unsigned long magic, unsigned long addr)
// {  
// 	struct multiboot_tag *tag;
// 	unsigned size;

// 	/*  Clear the screen. */
// 	cls ();

// 	/*  Am I booted by a Multiboot-compliant boot loader? */
// 	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
// 		{
// 			printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
// 			return;
// 		}

// 	if (addr & 7)
// 		{
// 			printf ("Unaligned mbi: 0x%x\n", addr);
// 			return;
// 		}

// 	size = *(unsigned *) addr;
// 	printf ("Announced mbi size 0x%x\n", size);
// 	for (tag = (struct multiboot_tag *) (addr + 8);
// 			 tag->type != MULTIBOOT_TAG_TYPE_END;
// 			 tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
// 																			 + ((tag->size + 7) & ~7)))
// 		{
// 			printf ("Tag 0x%x, Size 0x%x\n", tag->type, tag->size);
// 			switch (tag->type)
// 				{
// 				case MULTIBOOT_TAG_TYPE_CMDLINE:
// 					printf ("Command line = %s\n",
// 									((struct multiboot_tag_string *) tag)->string);
// 					break;
// 				case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
// 					printf ("Boot loader name = %s\n",
// 									((struct multiboot_tag_string *) tag)->string);
// 					break;
// 				case MULTIBOOT_TAG_TYPE_MODULE:
// 					printf ("Module at 0x%x-0x%x. Command line %s\n",
// 									((struct multiboot_tag_module *) tag)->mod_start,
// 									((struct multiboot_tag_module *) tag)->mod_end,
// 									((struct multiboot_tag_module *) tag)->cmdline);
// 					break;
// 				case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
// 					printf ("mem_lower = %uKB, mem_upper = %uKB\n",
// 									((struct multiboot_tag_basic_meminfo *) tag)->mem_lower,
// 									((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
// 					break;
// 				case MULTIBOOT_TAG_TYPE_BOOTDEV:
// 					printf ("Boot device 0x%x,%u,%u\n",
// 									((struct multiboot_tag_bootdev *) tag)->biosdev,
// 									((struct multiboot_tag_bootdev *) tag)->slice,
// 									((struct multiboot_tag_bootdev *) tag)->part);
// 					break;
// 				case MULTIBOOT_TAG_TYPE_MMAP:
// 					{
// 						multiboot_memory_map_t *mmap;

// 						printf ("mmap\n");
			
// 						for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
// 								 (multiboot_uint8_t *) mmap 
// 									 < (multiboot_uint8_t *) tag + tag->size;
// 								 mmap = (multiboot_memory_map_t *) 
// 									 ((unsigned long) mmap
// 										+ ((struct multiboot_tag_mmap *) tag)->entry_size))
// 							printf (" base_addr = 0x%x%x,"
// 											" length = 0x%x%x, type = 0x%x\n",
// 											(unsigned) (mmap->addr >> 32),
// 											(unsigned) (mmap->addr & 0xffffffff),
// 											(unsigned) (mmap->len >> 32),
// 											(unsigned) (mmap->len & 0xffffffff),
// 											(unsigned) mmap->type);
// 					}
// 					break;
// 				case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
// 					{
// 						multiboot_uint32_t color;
// 						unsigned i;
// 						struct multiboot_tag_framebuffer *tagfb
// 							= (struct multiboot_tag_framebuffer *) tag;
// 						void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;

// 						switch (tagfb->common.framebuffer_type)
// 							{
// 							case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
// 								{
// 									unsigned best_distance, distance;
// 									struct multiboot_color *palette;
						
// 									palette = tagfb->framebuffer_palette;

// 									color = 0;
// 									best_distance = 4*256*256;
						
// 									for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
// 										{
// 											distance = (0xff - palette[i].blue) 
// 												* (0xff - palette[i].blue)
// 												+ palette[i].red * palette[i].red
// 												+ palette[i].green * palette[i].green;
// 											if (distance < best_distance)
// 												{
// 													color = i;
// 													best_distance = distance;
// 												}
// 										}
// 								}
// 								break;

// 							case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
// 								color = ((1 << tagfb->framebuffer_blue_mask_size) - 1) 
// 									<< tagfb->framebuffer_blue_field_position;
// 								break;

// 							case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
// 								color = '\\' | 0x0100;
// 								break;

// 							default:
// 								color = 0xffffffff;
// 								break;
// 							}
						
// 						for (i = 0; i < tagfb->common.framebuffer_width
// 									 && i < tagfb->common.framebuffer_height; i++)
// 							{
// 								switch (tagfb->common.framebuffer_bpp)
// 									{
// 									case 8:
// 										{
// 											multiboot_uint8_t *pixel = fb
// 												+ tagfb->common.framebuffer_pitch * i + i;
// 											*pixel = color;
// 										}
// 										break;
// 									case 15:
// 									case 16:
// 										{
// 											multiboot_uint16_t *pixel
// 												= fb + tagfb->common.framebuffer_pitch * i + 2 * i;
// 											*pixel = color;
// 										}
// 										break;
// 									case 24:
// 										{
// 											multiboot_uint32_t *pixel
// 												= fb + tagfb->common.framebuffer_pitch * i + 3 * i;
// 											*pixel = (color & 0xffffff) | (*pixel & 0xff000000);
// 										}
// 										break;

// 									case 32:
// 										{
// 											multiboot_uint32_t *pixel
// 												= fb + tagfb->common.framebuffer_pitch * i + 4 * i;
// 											*pixel = color;
// 										}
// 										break;
// 									}
// 							}
// 						break;
// 					}

// 				}
// 		}
// 	tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
// 																	+ ((tag->size + 7) & ~7));
// 	printf ("Total mbi size 0x%x\n", (unsigned) tag - addr);
// }    

// /*  Clear the screen and initialize VIDEO, XPOS and YPOS. */
// static void
// cls (void)
// {
// 	int i;

// 	video = (unsigned char *) VIDEO;
	
// 	for (i = 0; i < COLUMNS * LINES * 2; i++)
// 		*(video + i) = 0;

// 	xpos = 0;
// 	ypos = 0;
// }

// /*  Convert the integer D to a string and save the string in BUF. If
// 	 BASE is equal to ’d’, interpret that D is decimal, and if BASE is
// 	 equal to ’x’, interpret that D is hexadecimal. */
// static void
// itoa (char *buf, int base, int d)
// {
// 	char *p = buf;
// 	char *p1, *p2;
// 	unsigned long ud = d;
// 	int divisor = 10;
	
// 	/*  If %d is specified and D is minus, put ‘-’ in the head. */
// 	if (base == 'd' && d < 0)
// 		{
// 			*p++ = '-';
// 			buf++;
// 			ud = -d;
// 		}
// 	else if (base == 'x')
// 		divisor = 16;

// 	/*  Divide UD by DIVISOR until UD == 0. */
// 	do
// 		{
// 			int remainder = ud % divisor;
			
// 			*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
// 		}
// 	while (ud /= divisor);

// 	/*  Terminate BUF. */
// 	*p = 0;
	
// 	/*  Reverse BUF. */
// 	p1 = buf;
// 	p2 = p - 1;
// 	while (p1 < p2)
// 		{
// 			char tmp = *p1;
// 			*p1 = *p2;
// 			*p2 = tmp;
// 			p1++;
// 			p2--;
// 		}
// }

// /*  Put the character C on the screen. */
// static void
// putchar (int c)
// {
// 	if (c == '\n' || c == '\r')
// 		{
// 		newline:
// 			xpos = 0;
// 			ypos++;
// 			if (ypos >= LINES)
// 				ypos = 0;
// 			return;
// 		}

// 	*(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
// 	*(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

// 	xpos++;
// 	if (xpos >= COLUMNS)
// 		goto newline;
// }

// /*  Format a string and print it on the screen, just like the libc
// 	 function printf. */
// void
// printf (const char *format, ...)
// {
// 	char **arg = (char **) &format;
// 	int c;
// 	char buf[20];

// 	arg++;
	
// 	while ((c = *format++) != 0)
// 		{
// 			if (c != '%')
// 				putchar (c);
// 			else
// 				{
// 					char *p, *p2;
// 					int pad0 = 0, pad = 0;
					
// 					c = *format++;
// 					if (c == '0')
// 						{
// 							pad0 = 1;
// 							c = *format++;
// 						}

// 					if (c >= '0' && c <= '9')
// 						{
// 							pad = c - '0';
// 							c = *format++;
// 						}

// 					switch (c)
// 						{
// 						case 'd':
// 						case 'u':
// 						case 'x':
// 							itoa (buf, c, *((int *) arg++));
// 							p = buf;
// 							goto string;
// 							break;

// 						case 's':
// 							p = *arg++;
// 							if (! p)
// 								p = "(null)";

// 						string:
// 							for (p2 = p; *p2; p2++);
// 							for (; p2 < p + pad; p2++)
// 								putchar (pad0 ? '0' : ' ');
// 							while (*p)
// 								putchar (*p++);
// 							break;

// 						default:
// 							putchar (*((int *) arg++));
// 							break;
// 						}
// 				}
// 		}
// }