/*
 * stimers.h
 *	Implements a simple software timer management.
 */

#ifndef __STIMERS_H__
#define __STIMERS_H__

#include "mytypes.h"

#define STIMER_BASE   20

typedef void (*STCB)( MInt thdlr );

typedef enum
{
	ONESHOT, RETRIG
} STIMER_MODES;

typedef enum
{
	TIMER_OK,
	TIMER_NO, TIMER_BAD_HDL, TIMER_NOT_ASSIGNED
} STIMER_CODES;

/*
 * init_stimers:
 * 	Initialize this software module.
 */
void init_stimers( void );

/*
 * handler_timers:
 * 	Called from background level to management
 * 	the triggered timers.
 */
void handler_timers( void );

/*
 * assign_timer:
 * 	Get a timer from stimer array and return
 * 	its handler for use it through the access
 * 	points of driver.
 * 	Must be passed the timer mode and callback
 * 	function if required.
 * 	Return the timer handler number or negative
 * 	error code if otherwise.
 */
MInt assign_timer( unsigned char type, STCB cb, unsigned char news );

/*
 * deassign_timer:
 * 	Free the slot timer indicated by 'thdlr' from
 * 	stimer array.
 */
void deassign_timer( MInt thdlr );

/*
 * kick_timer:
 * 	Trigger a timer indicated by 'thdlr'
 * 	parameter. The elapsing time is configurated
 * 	by 'msec' parameter.
 * 	Return the actual time count for this timer.
 */
unsigned short kick_timer( MInt thdlr, unsigned short msec );

/*
 * kill_timer:
 *	Reset a timer indicated by 'thdlr' parameter,
 *	this operation cause a stop of timer count.
 *	Return the actual time count for this timer.
 */
unsigned short kill_timer( MInt thdlr );


#endif
