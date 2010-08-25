/*
 *	drivers.c
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#include "drivers.h"
#include "platform.h"
#include "enaint.h"
#include "maintime.h"
#include "serial.h"

void
drivers_init(void)
{
	platform_init();
	init_int_handler();
	init_main_timer_interrupt();

	serial_init( COM1CH ); 	 
	serial_init( COM2CH );

	acknowledge_irqs();

	enable_gral_interrupt();
}
