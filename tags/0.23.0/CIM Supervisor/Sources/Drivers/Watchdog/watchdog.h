/*
 *	watchdog.h
 *
 *	Routines for Watchdog Module.
 *
 */

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#define WATCHDOG_BASE 8

/*
 *	WATCHDOG_TICK: Defines The Watchdog alive time in msecs.
 */
#define WATCHDOG_TICK	2000

/*
 *	kick_watchdog: Kicks The Watchdog Counter
 */
void kick_watchdog( void );
void check_watchdog( void );

#endif
