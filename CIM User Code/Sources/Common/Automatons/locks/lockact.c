/*
 * lockact.c
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
#include "lockact.h"
#include "pwrhdl.h"
#include "vaultdef.h"
#include "settings.h"
#include "sensor.h"
#include <stdlib.h>

typedef struct
{
	MInt lockt;
	MInt ulocke;
	MUInt openkey_news_ix;
	MUInt lock_news_ix;
}LOCKACT_ST;

static LOCKACT_ST lockst[NUM_LOCKS] =
{
	{ 0,	0, OPEN_KEYS_D1_IX, LOCK_DOOR1_IX },
	{ 0,	0, OPEN_KEYS_D2_IX, LOCK_DOOR2_IX },
	{ 0,	0, OPEN_KEYS_D3_IX, LOCK_DOOR3_IX },
	{ 0,	0, OPEN_KEYS_D4_IX, LOCK_DOOR4_IX }
};

/*
 * Define initialization routine
 */

void
init_lockctrl( MUInt hdlr, MUInt tlock_ix, MUInt tulocke_ix )
{
	lockst[hdlr].lockt = assign_timer( ONESHOT, NULL, tlock_ix );
	lockst[hdlr].ulocke = assign_timer( ONESHOT, NULL, tulocke_ix );
}

void
init_lock0( void )
{
	init_lockctrl( LOCK0_HDLR, TOUT_LOCK0_IX, TOUTUE_LOCK0_IX );
}

void
init_lock1( void )
{
	init_lockctrl( LOCK1_HDLR, TOUT_LOCK1_IX, TOUTUE_LOCK1_IX );
}

void
init_lock2( void )
{
	init_lockctrl( LOCK2_HDLR, TOUT_LOCK2_IX, TOUTUE_LOCK2_IX );
}

void
init_lock3( void )
{
	init_lockctrl( LOCK3_HDLR, TOUT_LOCK3_IX, TOUTUE_LOCK3_IX );
}

/*
 * Define static routines
 */
static
void
lock_locker_ws( MUInt who )
{
	disable_vlocks( who ); 

	lock_a_ctrl(who,CTRL_OFF);
	lock_b_ctrl(who,CTRL_OFF);
	clear_nlock0_b();

	if( get_mcu_id() == SECONDARY_DEVICE )
		return;

	if( !is_notunlocked( who ) )
		inform_locker_error( who, LOCKER_FAIL );
	else
		inform_locker_error( who, LOCKER_OK );
}

static
void
unlock_locker_ws( MUInt who )
{
	unsigned char lerror;

	lerror = LOCKER_OK;

	enable_vlocks( who ); 

	if( get_mcu_id() == SECONDARY_DEVICE )
	{
		if( who == LOCKER0 )
		{
			lock_b_ctrl( who, CTRL_ON );
			set_nlock0_b();
		}
		return;
	}

	lock_a_ctrl( who, CTRL_ON );
	if( !is_notunlocked( who ) )
		lerror = LOCKER_DRVB;

	lock_a_ctrl( who, CTRL_OFF );
	lock_b_ctrl( who, CTRL_ON );
	if( !is_notunlocked( who ) )
		lerror = LOCKER_DRVA;

	lock_a_ctrl( who, CTRL_ON );
	if( !is_unlocked( who ) )
	{
		lerror = LOCKER_OPEN;
		put_nqueue(NEWS_QUEUE, 
				def_news[lockst[get_lock_ix(who)].lock_news_ix] );
	}

	inform_locker_error( who, lerror );
}

/*
 * Define action routines
 */

void
chk_keysw( NEWS_T *in )
{
	MUInt who;

	who = in->who;

	if( is_key_switch_open() || (get_unlock_ena_timer(who) == 0) )
		put_nqueue( NEWS_QUEUE, 
				def_news[lockst[get_lock_ix(who)].openkey_news_ix] );

	kick_timer( lockst[get_lock_ix(who)].ulocke, get_unlock_ena_timer(who) );
}

void
feed_lock( NEWS_T *in )
{
	MUInt who;

	who = in->who;

	unlock_locker_ws( in->who );
	kill_timer( lockst[get_lock_ix(who)].ulocke );
	kick_timer( lockst[get_lock_ix(who)].lockt, get_tlock_timer(in->who) );
}

void
release_lock( NEWS_T *in )
{
	MUInt who;

	who = in->who;

	kill_timer( lockst[get_lock_ix(who)].ulocke );
	kill_timer( lockst[get_lock_ix(who)].lockt );
	lock_locker_ws( in->who );
}

/*
 * Define state's preproccessor routines
 */

NARG_T
lock_ppro( const NEWS_T *in )
{
	switch( in->news_fly )
	{
		case PROT:
		case TIM:
		case LOCKER:
			return in->news;
		default:
			return UNKNOWN;
	}
}

