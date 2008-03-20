/*
 * dwosact.c
 */

#include "news.h"
#include "mpdata.h"
#include "mytypes.h"
#include "wouts.h"
#include "lockerhdl.h"
#include "grstatus.h"
#include "logprot.h"
#include "stimers.h"
#include "locker.h"
#include "news.h"
#include "dwosact.h"
#include "nqdata.h"
#include "sensor.h"
#include <stdlib.h>

static MInt lockt_dwos;
static MInt unlocke_dwos;

/*
 * Define initialization routine
 */

void
init_dwosctrl( void )
{
	GR1ST_T *pgrst;

	lockt_dwos = assign_timer( ONESHOT, NULL, TOUT_DWOS_IX );
	unlocke_dwos = assign_timer( ONESHOT, NULL, TOUTUE_DWOS_IX );
	
	pgrst = get_grstat();
	if( ( pgrst->locker1 == UNLOCKED_STATUS ) || 
		( pgrst->plunger1 == OPENED ) )
		put_nqueue( NEWS_QUEUE, def_news[ INIT_TO_UNLOCKED_D2_IX ] );
	else
		put_nqueue( NEWS_QUEUE, def_news[ INIT_TO_LOCKED_D2_IX ] );
}

/*
 * Define static routines
 */
static
void
lock_locker_wos( void )
{
	disable_vlocks( VLOCKS_LOCKER1 );
	clear_lock1_a();
	clear_lock1_b();
	if( is_unlocked( LOCKER1 ) )
		inform_locker_error( LOCKER1, LOCKER_FAIL );
	else
		inform_locker_error( LOCKER1, LOCKER_OK );
}

static
void
unlock_locker_wos( void )
{
	unsigned char lerror;

	lerror = LOCKER_OK;

	enable_vlocks(VLOCKS_LOCKER1);
	set_lock1_a();
	if( is_unlocked( LOCKER1 ) )
		lerror = LOCKER_DRVB;

	clear_lock1_a();
	set_lock1_b();
	if( is_unlocked( LOCKER1 ) )
		lerror = LOCKER_DRVA;

	set_lock1_a();
	if( !is_unlocked( LOCKER1 ) )
	{
		lerror = LOCKER_OPEN;
		put_nqueue(NEWS_QUEUE, def_news[LOCK_DOOR2_IX] );
	}

	inform_locker_error( LOCKER1, lerror );
}

/*
 * Define action routines
 */

void
lked2ulked_wos( NEWS_T *in )
{
	inform_locker_error( LOCKER1, LOCKER_FAIL );
	set_locker_state( LOCKER1, UNLOCKED_STATUS );
	set_plunger_state( PLUNGER1, OPENED );
}

void
lked2pulking_wos( NEWS_T *in )
{
	if( is_key_switch_open() || (get_unlock_ena_timer() == 0) )
		put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D2_IX] );

	set_locker_state(LOCKER1, UNLOCKING_STATUS );
	kick_timer( unlocke_dwos, get_unlock_ena_timer() );
}

void
pulking2ulked_wos( NEWS_T *in )
{
	inform_locker_error( LOCKER1, LOCKER_FAIL );
	set_locker_state( LOCKER1, UNLOCKED_STATUS );
	set_plunger_state( PLUNGER1, OPENED );
}

void
pulking2ulking_wos( NEWS_T *in )
{
	unlock_locker_wos();
	kill_timer( unlocke_dwos );
	kick_timer( lockt_dwos, get_tlock_timer(LOCKER1) );
}

void
pulking2lked_wos( NEWS_T *in )
{
	set_locker_state( LOCKER1, LOCKED_STATUS );
	kill_timer( unlocke_dwos );
}

void
ulking2ulked_wos( NEWS_T *in )
{
	set_locker_state( LOCKER1, UNLOCKED_STATUS );
	set_plunger_state( PLUNGER1, OPENED );
	put_nqueue( NEWS_QUEUE, def_news[LOCK_DOOR2_IX] );
}

void
ulking2lked_wos( NEWS_T *in )
{
	lock_locker_wos();
	set_locker_state(LOCKER1, LOCKED_STATUS );
	kill_timer( lockt_dwos );
}

void
ulked2lked_wos( NEWS_T *in )
{
	lock_locker_wos();
	set_plunger_state( PLUNGER1, CLOSED );
	set_locker_state( LOCKER1, LOCKED_STATUS );
	kill_timer( lockt_dwos );
}

void
lked2lked_wos( NEWS_T *in )
{
	set_plunger_state( PLUNGER1, CLOSED );
}

void
ulked2lking_wos( NEWS_T *in )
{
	lock_locker_wos();
	set_locker_state( LOCKER1, LOCKING_STATUS );
	kill_timer( lockt_dwos );
}

void
lking2lked_wos( NEWS_T *in )
{
	set_plunger_state( PLUNGER1, CLOSED );
	set_locker_state( LOCKER1, LOCKED_STATUS );
}

/*
 * Define state's preproccessor routines
 */

NARG_T
dwos_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case PLUNGER:
		case PROT:
		case TIM:
		case LOCKER:
			return in->news;
		default:
			return UNKNOWN;
	}
}

