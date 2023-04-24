#include <asm/io.h>

#include <obsolete/glo.h>
#include <obsolete/arch_proto.h>
#include <obsolete/device.h>

#define	BCD2BIN(value)	(((value) & 0xf) + ((value) >> 4 ) * 10)
#define CMOS_READ(addr)					\
		({								\
			outb(0x80 | addr, 0x70);	\
			inb(0x71);					\
		})


time_s time;

void get_cmos_time(time_s *time)
{
	do
	{	time->year		= BCD2BIN(CMOS_READ(0x09)) + BCD2BIN(CMOS_READ(0x32)) * 100;
		time->month		= BCD2BIN(CMOS_READ(0x08));
		time->day		= BCD2BIN(CMOS_READ(0x07));	
		time->hour		= BCD2BIN(CMOS_READ(0x04));	
		time->minute	= BCD2BIN(CMOS_READ(0x02));
		time->second	= CMOS_READ(0x00);
	}while(time->second != CMOS_READ(0x00));
	time->second	= BCD2BIN(CMOS_READ(0x00));
	
	outb(0x00, 0x70); 
}