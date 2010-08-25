/*
 *	platform.c
 *
 *	Routines for Platform Initialization.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#include "derivative.h"

#include "platform.h"
#include "ioports.h"
#include "cop.h"
#include "ics.h"
#include "lvd.h"
#include "rti.h"

void
platform_init(void)
{
	cop_init();
	ics_init();
	lvd_init();
	ports_init();
	rti_init();
}

void
acknowledge_irqs( void )
{
	SRTISC |= 0x40;
}

