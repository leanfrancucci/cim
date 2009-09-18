/*
 * dwsact.c
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
#include "nqdata.h"
#include "dwsact.h"
#include "pwrhdl.h"
#include "vaultdef.h"
#include "settings.h"
#include "sensor.h"
#include <stdlib.h>

static MInt lockt_dws;
static MInt ulocke_dws;

/*
 * Define initialization routine
 */

void
init_dwsctrl( void )
{
	GR1ST_T *pgrst;

	lockt_dws = assign_timer( ONESHOT, NULL, TOUT_DWS_IX );
	ulocke_dws = assign_timer( ONESHOT, NULL, TOUTUE_DWS_IX );
	
	pgrst = get_grstat();

	if( pgrst->locker0 == UNLOCKED_STATUS )
		put_nqueue( NEWS_QUEUE, def_news[ INIT_TO_UNLOCKED_D1_IX ] );
	else
		put_nqueue( NEWS_QUEUE, def_news[ INIT_TO_LOCKED_D1_IX ] );
}


/*
 * Define static routines
 */
static
void
lock_locker_ws( void )
{
	disable_vlocks( VLOCKS_LOCKER0 );
	clear_lock0_a();
	clear_lock0_b();
	clear_nlock0_b();

	if( get_mcu_id() == SECONDARY_DEVICE )
		return;

	if( is_unlocked( LOCKER0 ) )
		inform_locker_error( LOCKER0, LOCKER_FAIL );
	else
		inform_locker_error( LOCKER0, LOCKER_OK );
}

static
void
unlock_locker_ws( void )
{
	unsigned char lerror;

	lerror = LOCKER_OK;

	enable_vlocks(VLOCKS_LOCKER0);

	if( get_mcu_id() == SECONDARY_DEVICE )
	{
		set_lock0_b();
		set_nlock0_b();
		return;
	}

	set_lock0_a();
	if( is_unlocked( LOCKER0 ) )
		lerror = LOCKER_DRVB;

	clear_lock0_a();
	set_lock0_b();
	if( is_unlocked( LOCKER0 ) )
		lerror = LOCKER_DRVA;

	set_lock0_a();
	if( !is_unlocked( LOCKER0 ) )
	{
		lerror = LOCKER_OPEN;
		put_nqueue(NEWS_QUEUE, def_news[LOCK_DOOR1_IX] );
	}

	inform_locker_error( LOCKER0, lerror );
}

/*
 * Define action routines
 */

void
inform_open_ws( NEWS_T *in )
{
	set_plunger_state( PLUNGER0, OPENED );
}

void
inform_close_ws( NEWS_T *in )
{
	set_plunger_state( PLUNGER0, CLOSED );
}

void
lked2pulking_ws( NEWS_T *in )
{
	if( is_key_switch_open() || (get_unlock_ena_timer(LOCKER0) == 0) )
		put_nqueue( NEWS_QUEUE, def_news[OPEN_KEYS_D1_IX] );

	set_locker_state(LOCKER0, UNLOCKING_STATUS );
	kick_timer( ulocke_dws, get_unlock_ena_timer(LOCKER0) );
}

void
lked2unlked_ws( NEWS_T *in )
{
	inform_locker_error( LOCKER0, LOCKER_FAIL );
	set_locker_state( LOCKER0, UNLOCKED_STATUS );
}

void
pulking2ulking_ws( NEWS_T *in )
{
	unlock_locker_ws();
	kill_timer( ulocke_dws );
	kick_timer( lockt_dws, get_tlock_timer(LOCKER0) );
}

void
pulking2lked_ws( NEWS_T *in )
{
	set_locker_state( LOCKER0, LOCKED_STATUS );
	kill_timer( ulocke_dws );
	kill_timer( lockt_dws );
}

void
pulking2ulked_ws( NEWS_T *in )
{
	inform_locker_error( LOCKER0, LOCKER_FAIL );
	set_locker_state( LOCKER0, UNLOCKED_STATUS );
}

#if 0
void
ulking2lked_ws( NEWS_T *in )
{
	lock_locker_ws();

	if( is_locker_open(LOCKER0) )
		inform_locker_error( LOCKER0, LOCKER_FAIL );
		
	set_locker_state( LOCKER0, LOCKED_STATUS );
	kill_timer( lockt_dws );
}
#else

void
lock_and_wait( NEWS_T *in )
{
	lock_locker_ws();
	kick_timer( lockt_dws, WAIT_UNLOCK_DWS );
}

void
ulking2lked_ws( NEWS_T *in )
{
	set_locker_state( LOCKER0, LOCKED_STATUS );
	kill_timer( lockt_dws );
}
#endif

void
ulking2ulked_ws( NEWS_T *in )
{
	set_locker_state( LOCKER0, UNLOCKED_STATUS );
	put_nqueue( NEWS_QUEUE, def_news[LOCK_DOOR1_IX] );
}

void
ulked2lking_ws( NEWS_T *in )
{
	lock_locker_ws();
	set_locker_state( LOCKER0, LOCKING_STATUS );
	kill_timer( lockt_dws );
}

void
ulked2lked_ws( NEWS_T *in )
{
	lock_locker_ws();
	set_locker_state( LOCKER0, LOCKED_STATUS );
	kill_timer( lockt_dws );
}

void
lking2lked_ws( NEWS_T *in )
{
	set_locker_state( LOCKER0, LOCKED_STATUS );
}

/*
 * Define state's preproccessor routines
 */

NARG_T
dws_ppro( const NEWS_T *in )
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

