/*
 *	watchdog.c
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */
#include "mytypes.h"
#include "watchdog.h"
#include "reset.h"
#include "enaint.h"
#include "ioports.h"
#include "rti.h"

#define BWATCHDOG_TICK	(WATCHDOG_TICK/WATCHDOG_BASE)

static short watchdog_tick = BWATCHDOG_TICK;

#define _reset_mcu() {asm bgnd;}

/* 
 * reset_now: If in debugg mode stop the execution,
 *				 resets the MCU in other case.
 */
void
reset_now( void )
{
	_reset_mcu();
	watchdog_tick = BWATCHDOG_TICK;
}

/*
 *	kick_watchdog: Kicks The Watchdog Counter
 */
void
kick_watchdog( void )
{
	sem_disable();
	if( watchdog_tick <= 0 )
		reset_now();
	watchdog_tick = BWATCHDOG_TICK;
	sem_enable();
}

/*
 *	check_watchdog: Checks The Watchdog Counter, if reach to 0 reset.
 */
void
check_watchdog( void )
{
	if( (watchdog_tick !=0) && (--watchdog_tick <= 0) )
		reset_now();
}




