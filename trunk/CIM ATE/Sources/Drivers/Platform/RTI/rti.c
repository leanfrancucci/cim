/*
 *	rti.c
 *		Routines handle RTI service
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */
#include <hidef.h> 		/* for EnableInterrupts macro */
#include "derivative.h"

#include "mytypes.h"
#include "sleep.h"
#include "watchdog.h"
#include "rti.h"
#include "ioportsl.h"
#include "enaint.h"
#include "reset.h"
#include "maintime.h"

#define _reset_mcu() {asm bgnd;}

static const unsigned short irqf[]=
{
	0, 62, 125, 250, 500, 1000, 2000, 4000
};

static MUInt enable_maintime = 1;
volatile ushort mtime;

/*
 *	rti_handler: RTI Interrupt Service Routine.
 */
void 
interrupt 25
rti_handler( void )
{
	SRTISC |= 0x40;

	if( enable_maintime )
		main_timer_interrupt();

	++mtime;

}


void
init_timer_hardware( unsigned short irq_time )
{
	MUInt factor;
	const unsigned short *p;

	factor = 0;
	for( p = &irqf[1]; p < irqf+sizeof(irqf)/sizeof(*irqf); ++p )
		if( *p == irq_time )
		{
			factor = (MUInt)(p - irqf);
			break;
		}
	SRTISC = 0x30 + factor;
}

void
rti_ctrl( MUInt ctrl )
{
	enable_maintime = ctrl;	
}

