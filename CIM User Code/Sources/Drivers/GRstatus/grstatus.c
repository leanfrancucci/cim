/*
 * grstatus.c
 */

#include "mytypes.h"
#include "logprot.h"
#include "grstatus.h"
#include "pwrsys.h"
#include "settings.h"
#include "grqueue.h"
#include "cparse.h"
#include "cpdata.h"
#include "news.h"
#include "nqdata.h"


#if DEBUG
#include <stdio.h>
#endif

#if DEBUG
static const char *mlst[] =
{
	"LOCKED_STATUS", "UNLOCKED_STATUS",
	"LOCKING_STATUS", "UNLOCKING_STATUS"
};

static const char *mpst[] =
{
	"OPENED", "CLOSED"
};
#endif

static GR1ST_T grst;

/*
 * init_group_status:
 */
void
init_group_status( void )
{
	grst.plunger0 = ( get_cstate(PLGS1) == CLOSE_ST ) ? 
		CLOSED : OPENED;
	grst.plunger1 = ( get_cstate(PLGS2) == CLOSE_ST ) ? 
		CLOSED : OPENED;

	grst.locker0 = ( get_cstate(LOCKS1) == CLOSE_ST ) ? 
		LOCKED_STATUS : UNLOCKED_STATUS;
	grst.locker1 = ( get_cstate(LOCKS2) == CLOSE_ST ) ? 
		LOCKED_STATUS : UNLOCKED_STATUS;

	grst.stacker0 = ( get_cstate(VAL1STCK) == CLOSE_ST ) ? 
		INSTALLED : REMOVED;
	grst.stacker1 = ( get_cstate(VAL2STCK) == CLOSE_ST ) ? 
		INSTALLED : REMOVED;

	if( grst.plunger1 == OPENED )
		grst.locker1 = UNLOCKED_STATUS;

	grst.locker_error0 = grst.locker_error1 = LOCKER_OK;

/*	grst.locker0 = grst.locker1 = LOCKED_STATUS;
	grst.plunger0 = grst.plunger1 = CLOSED;
	grst.stacker0 = grst.stacker1 = INSTALLED;
	grst.locker_error0 = grst.locker_error1 = LOCKER_OK;*/
}

GR1ST_T *
get_grstat( void )
{
	return &grst;
}

/*
 * get_group_status:
 *	Return devices status as array in this order:
 *
 *	LOCKER0_STATUS, PLUNGER0_STATUS,
 *	LOCKER1_STATUS, PLUNGER1_STATUS, SAFE_BOX,
 *	STACKER0_STATUS, STACKER1_STATUS,
 *	LOCKER0_ERROR, LOCKER1_ERROR
 */
unsigned char *
get_group_status( void )
{
	static GR1ST_T rem_grst;

	dprintf( printf( "\nLocker0 Status : %s\n", mlst[grst.locker0] ) );
	dprintf( printf( "Locker1 Status : %s\n", mlst[grst.locker1] ) );
	dprintf( printf( "Plunger0 Status : %s\n", mpst[grst.plunger0] ) );
	dprintf( printf( "Plunger1 Status : %s\n", mpst[grst.plunger1] ) );
	
	if( remove_grqueue(&rem_grst) != -EMPTY_GQUEUE )
	{
		rem_grst.safe_box = set_memstat( rem_grst.safe_box );
		return (unsigned char*)&rem_grst;
	}

	grst.safe_box = set_memstat( grst.safe_box );
	return (unsigned char*)&grst;
}


/*
 * set_safebox_st:
 */
void
set_safebox_st( unsigned char state )
{
	if( grst.safe_box == state )
		return;
	grst.safe_box = state;
	put_grqueue(&grst);
}

/*
 * set_locker_state:
 * 	who: LOCKER0/LOCKER1
 * 	state: 	LOCKED_STATUS, UNLOCKED_STATUS,
 *			LOCKING_STATUS, UNLOCKING_STATUS,
 */
void
set_locker_state( MUInt who, unsigned char state )
{
	if( who == LOCKER0 )
	{
		if( grst.locker0 == state )
			return;
		grst.locker0 = state;
	}
	else
	{
		if( grst.locker1 == state )
			return;
		grst.locker1 = state;
	}
	put_grqueue(&grst);
}

/*
 * set_plunger_state:
 * 	who: PLUNGER0/PLUNGER1
 * 	state: OPENED/CLOSED
 */
void
set_plunger_state( MUInt who, unsigned char state )
{
	if( who == PLUNGER0 )
	{
		if( grst.plunger0 == state )
			return;
		grst.plunger0 = state;
	}
	else
	{
		if( grst.plunger1 == state )
			return;
		grst.plunger1 = state;
	}
	put_grqueue(&grst);
}

/*
 * set_stacker_state
 * who: STACKER0/STACKER1
 * state: INSTALLED/REMOVED 
 */
void
set_stacker_state( MUInt who, unsigned char state )
{
	if( who == STACKER0 )
	{
		if( grst.stacker0 == state )
			return;
		grst.stacker0 = state;
	}
	else
	{
		if( grst.stacker1 == state )
			return;
		grst.stacker1 = state;
	}
	put_grqueue(&grst);

}

/*
 * inform_locker_error
 *	who: LOCKER0/LOCKER1
 *	error: 	LOCKER_OK, LOCKER_DRVA, LOCKER_DRVB,
 *			LOCKER_OPEN, LOCKER_FAIL
 */
void 
inform_locker_error( MUInt who, MUInt error )
{
	if( who == LOCKER0 )
	{
		if( grst.locker_error0 == error )
			return;
		grst.locker_error0 = error;
	}
	else
	{
		if( grst.locker_error1 == error )
			return;
		grst.locker_error1 = error;
	}
	put_grqueue(&grst);
}
