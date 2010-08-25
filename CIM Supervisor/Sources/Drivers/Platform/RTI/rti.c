/*
 *	rti.c
 *		Routines handle RTI service
 *
 *	Programmer: Darío S. Baliña.
 */
#include "derivative.h"

#include "mytypes.h"
#include "rti.h"
#include "ioports.h"
#include "reset.h"
#include "superv.h"
#include "sleep.h"

#define _reset_mcu() {asm bgnd;}

void
rti_init( void )
{
	SRTISC_RTICLKS = 0;
	SRTISC_RTIS = 1;
	SRTISC_RTIE = 1;
}

/*
 *	rti_handler: RTI Interrupt Service Routine.
 */
void 
interrupt 23
rti_handler( void )
{
	SRTISC_RTIACK = 1;

	do_supervisor();
	check_sleep();
}



