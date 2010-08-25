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
#include "ioportsl.h"
#include "cop.h"
#include "icg.h"
#include "lvd.h"
#include "rti.h"
#include "adc.h"

void
platform_init(void)
{
	cop_init();
	icg_init();
	lvd_init();
	ports_init();
	adc_init();
}

void
acknowledge_irqs( void )
{
	SRTISC |= 0x40;
	SCI1S1 = SCI1S1;
	SCI2S1 = SCI2S1;
}

