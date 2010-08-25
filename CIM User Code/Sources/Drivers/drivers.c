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
#include "wouts.h"
#include "serial.h"
#include "stimers.h"
#include "pwrsys.h"
#include "sensor.h"
#include "phylay.h"
#include "sersw.h"
#include "grstatus.h"
#include "cop.h"

MUInt
drivers_init(void)
{
	MUInt mcu_id;

	platform_init();
	init_int_handler();
	init_main_timer_interrupt();
	init_wouts();
	init_sensors();
	init_group_status();
	mcu_id = init_pwrsys();

	serial_init( CTCH ); 	 
	serial_init( VALCH );

	init_phymed();
	init_sersw();

	init_stimers();

	kick_cop_wdog();

	acknowledge_irqs();

	enable_gral_interrupt();
	return mcu_id;
}
