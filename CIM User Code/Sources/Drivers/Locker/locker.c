/*
 * locker.c
 */

#include "mytypes.h"
#include "locker.h"
#include "logprot.h"
#include "news.h"
#include "nqueue.h"
#include "nqdata.h"
#include "settings.h"
#include "mpdata.h"

#define TLOCK_K		(1000/STIMER_BASE)
#define TUNLOCK_K	((1000/STIMER_BASE)*60)

/*
 * Define the unlock time for every locker.
 * This timer is update from locker automata.
 */

static unsigned short lock_timer[] = { TOUT_TLOCK, TOUT_TLOCK, TOUT_TLOCK, TOUT_TLOCK };
static unsigned short unlock_timer[] = { TOUT_TUNLOCK, TOUT_TUNLOCK, TOUT_TUNLOCK, TOUT_TUNLOCK };

MUInt
get_lock_ix( MUInt which )
{
	switch( which )
	{
		case LOCK0_CTRL:
			return LOCK0_HDLR;
		case LOCK1_CTRL:
			return LOCK1_HDLR;
		case LOCK2_CTRL:
			return LOCK2_HDLR;
		case LOCK3_CTRL:
			return LOCK3_HDLR;
		default:
			return 0;
	}
}

/*
 * set_locker:
 * 	Send news according to desired operations on
 * 	seleted locker.	The device number is extracted from 
 * 	devices table.
 */

void
set_locker( MUInt which, MUInt ctrl )
{
	switch( which )
	{
		case LOCKER0:
			put_nqueue( COND_QUEUE,	def_news
				[ ctrl == TOLOCK ?	LOCK_DOOR1_IX : UNLOCK_DOOR1_IX ] );
			break;
		case LOCKER1:
			put_nqueue( COND_QUEUE,	def_news
				[ ctrl == TOLOCK ?	LOCK_DOOR2_IX : UNLOCK_DOOR2_IX ] );
			break;
		case LOCKER2:
			put_nqueue( COND_QUEUE,	def_news
				[ ctrl == TOLOCK ?	LOCK_DOOR3_IX : UNLOCK_DOOR3_IX ] );
			break;			
		case LOCKER3:
			put_nqueue( COND_QUEUE,	def_news
				[ ctrl == TOLOCK ?	LOCK_DOOR4_IX : UNLOCK_DOOR4_IX ] );
			break;						
		default:
			break;
	}
}


/*
 * set_tlock:
 */

void
set_tlock( MUInt which, MUInt tlock )
{
	unsigned short t;

	t = tlock*TLOCK_K;

	if( !tlock )
		t = TOUT_TLOCK;
	else if( t < TOUT_TLOCK )
		t = TOUT_TLOCK;

	switch( which )
	{
		case LOCKER0:
			lock_timer[LOCK0_HDLR] = t;
			break;
		case LOCKER1:
			lock_timer[LOCK1_HDLR] = t;
			break;
		case LOCKER2:
			lock_timer[LOCK2_HDLR] = t; 
			break;
		case LOCKER3:
			lock_timer[LOCK3_HDLR] = t; 
			break;
	}	
}

/*
 * get_tlock_timer:
 */

unsigned short
get_tlock_timer( MUInt which )
{
	return lock_timer[ get_lock_ix( which ) ];
}

/*
 * set_unlock_enable:
 */
void 
set_tunlock_enable( MUInt which, MUInt tunlock_enable )
{
	if( tunlock_enable > MAX_TUNLOCK_TIME )
		tunlock_enable = MAX_TUNLOCK_TIME;

	switch( which )
	{
		case LOCKER0:
			unlock_timer[LOCK0_HDLR] = tunlock_enable*TUNLOCK_K; 
			break;
		case LOCKER1:
			unlock_timer[LOCK1_HDLR] = tunlock_enable*TUNLOCK_K; 
			break;
		case LOCKER2:
			unlock_timer[LOCK2_HDLR] = tunlock_enable*TUNLOCK_K; 
			break;
		case LOCKER3:
			unlock_timer[LOCK3_HDLR] = tunlock_enable*TUNLOCK_K; 
			break;
	}
}

unsigned short
get_unlock_ena_timer( MUInt which )
{
	return unlock_timer[ get_lock_ix( which ) ];
}
