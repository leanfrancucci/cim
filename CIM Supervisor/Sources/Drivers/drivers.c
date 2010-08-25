/*
 *	drivers.c
 *
 *	Programmer: Dar�o S. Bali�a.
 */

#include "drivers.h"
#include "platform.h"
#include "enaint.h"
#include "superv.h"

void
drivers_init(void)
{
	platform_init();
	init_int_handler();
	acknowledge_irqs();
	enable_gral_interrupt();
	supervisor_init();
}
