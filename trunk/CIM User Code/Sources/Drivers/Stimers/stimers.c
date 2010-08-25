/*
 * stimers.c
 *	Implements a simple software timer management.
 */

#include <string.h>

#include "stimers.h"
#include "news.h"
#include "nqdata.h"

#define MAX_TIMERS		14

enum
{
	FREE, USED
};

typedef struct
{
	unsigned used: 1;
	unsigned type: 1;
	unsigned char news;
	unsigned short tout;
	unsigned short reload;
	STCB stmr_callback;
} TIMER;

static TIMER timers[ MAX_TIMERS ];

/*
 * init_stimers:
 * 	Initialize this software module.
 */
void
init_stimers( void )
{
	TIMER *p;

	for( p = timers ; p < timers + MAX_TIMERS ; ++p )
	{
		p->used = FREE;
		p->tout = 0;
	}
}

/*
 * handler_timers:
 * 	Called from background level to management
 * 	the triggered timers.
 */
void
handler_timers( void )
{
	TIMER *p;

	for( p = timers ; p < timers + MAX_TIMERS ; ++p )
		if( p->used == USED && p->tout && !--p->tout )
		{
			p->tout = p->type == ONESHOT ? 0 : p->reload;
			if( p->stmr_callback != NULL )
				(*p->stmr_callback)( (MInt) (p - timers) );
			else
				put_nqueue( NEWS_QUEUE, def_news[ p->news ] );
		}
}

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
MInt
assign_timer( unsigned char type, STCB cb_func, unsigned char news )
{
	TIMER *p;

	for( p = timers; p < timers + MAX_TIMERS ; ++p )
		if( p->used != USED )
		{
			p->used = USED;
			p->type = type;
			p->tout = p->reload = 0;
			p->stmr_callback = cb_func;
			p->news  = news;
			return (MInt)(p - timers);
		}
	return -TIMER_NO;
}

/*
 * deassign_timer:
 * 	Free the slot timer indicated by 'thdlr' from
 * 	stimer array.
 */
void
deassign_timer( MInt thdlr )
{
	TIMER *p;

	p = &timers[ thdlr ];
	p->used = FREE;
	p->tout = 0;
}

/*
 * kick_timer:
 * 	Trigger a timer indicated by 'thdlr'
 * 	parameter. The elapsing time is configurated
 * 	by 'msec' parameter.
 * 	Return the actual time count for this timer.
 */
unsigned short
kick_timer( MInt thdlr, unsigned short msec )
{
	TIMER *p;
	unsigned short actual;
	
	p = &timers[ thdlr ];
	actual = p->tout;
	p->tout = p->reload = msec;

	return actual;
}

/*
 * kill_timer:
 *	Reset a timer indicated by 'thdlr' parameter,
 *	this operation cause a stop of timer count.
 *	Return the actual time count for this timer.
 */
unsigned short
kill_timer( MInt thdlr )
{
	return kick_timer( thdlr, 0 );
}
