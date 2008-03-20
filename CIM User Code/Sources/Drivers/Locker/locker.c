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

#define TLOCK_K		(1000/STIMER_BASE)
#define TUNLOCK_K	((1000/STIMER_BASE)*60)

/*
 * Define the unlock time for every locker.
 * This timer is update from locker automata.
 */

static unsigned short lock_timer[] = { TOUT_TLOCK, TOUT_TLOCK };
static unsigned short unlock_timer = TOUT_TUNLOCK;

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
		case LOCKER1:
		put_nqueue( COND_QUEUE,	def_news
				[ ctrl == TOLOCK ?	LOCK_DOOR2_IX : UNLOCK_DOOR2_IX ] );
		break;
		case LOCKER0:
		put_nqueue( COND_QUEUE,	def_news
				[ ctrl == TOLOCK ?	LOCK_DOOR1_IX : UNLOCK_DOOR1_IX ] );
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
	MUInt ix;
	unsigned short t;

	t = tlock*TLOCK_K;
	ix = which != LOCKER0;
	if( !tlock )
		lock_timer[ ix ] = TOUT_TLOCK;
	else if( t < TOUT_TLOCK )
		lock_timer[ ix ] = TOUT_TLOCK;
	else
		lock_timer[ ix ] = t;
}

/*
 * get_tlock_timer:
 */

unsigned short
get_tlock_timer( MUInt which )
{
	return lock_timer[ which != LOCKER0 ];
}

/*
 * set_unlock_enable:
 */
void 
set_tunlock_enable( MUInt tunlock_enable )
{
	if( tunlock_enable > MAX_TUNLOCK_TIME )
		tunlock_enable = MAX_TUNLOCK_TIME;

	unlock_timer = tunlock_enable*TUNLOCK_K;
}

unsigned short
get_unlock_ena_timer( void )
{
	return unlock_timer;
}
