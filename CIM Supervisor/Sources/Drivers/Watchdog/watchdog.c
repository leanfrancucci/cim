/*
 *	watchdog.c
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */
#include "mytypes.h"
#include "watchdog.h"
#include "reset.h"
#include "ioports.h"

#define _reset_mcu() {asm bgnd;}

/* 
 * reset_now: If in debugg mode stop the execution,
 *				 resets the MCU in other case.
 */
void
reset_now( void )
{
	_reset_mcu();
}

/*
 *	kick_watchdog: Kicks The Watchdog Counter
 */
void
kick_watchdog( void )
{
    __RESET_WATCHDOG(); 
}




