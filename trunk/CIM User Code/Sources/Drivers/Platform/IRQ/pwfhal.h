/*
 * 	pwfhal.h
 * 		Hardware abstraction layer
 * 		for power fail mangement
 *
 * 		Version 0.1 7/11/07 12:44 ARG
 *
 *		DelsatGroup S.A.
 *		Eduardo Martinez
 */

#include "mytypes.h" 

enum
{
	NORMAL_RUN, IN_PWF
};

/*
 * 	Disables only pwf interrupt
 */

void do_disable_pwf_interrupt( void );

/*
 * 	Enables only pwf interrupt
 */

void do_enable_pwf_interrupt( void );

/*
 * 	do_disable_all_interrupts
 */

void do_disable_all_interrupts( void );

/*
 * 	Reads level of pwf input
 * 	returns NORMAL_RUN or IN_PWF
 */

MUInt pwf_level( void );

/*
 * 	kick_watchdog_low
 * 		kicks hardware watchdog
 */

void kick_watchdog_low( void );

/*
 * 	do_reset:
 * 		Forces a power up
 */

void do_reset( void );

