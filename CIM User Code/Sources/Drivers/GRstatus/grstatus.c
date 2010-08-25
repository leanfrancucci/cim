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
	grst.plunger2 = ( get_cstate(PLGS3) == CLOSE_ST ) ? 
		CLOSED : OPENED;
	grst.plunger3 = ( get_cstate(PLGS4) == CLOSE_ST ) ? 
		CLOSED : OPENED;

	grst.locker0 = ( get_cstate(LOCKS1) == CLOSE_ST ) ? 
		LOCKED_STATUS : UNLOCKED_STATUS;
	grst.locker1 = ( get_cstate(LOCKS2) == CLOSE_ST ) ? 
		LOCKED_STATUS : UNLOCKED_STATUS;
	grst.locker2 = ( get_cstate(LOCKS3) == CLOSE_ST ) ? 
		LOCKED_STATUS : UNLOCKED_STATUS;
	grst.locker3 = ( get_cstate(LOCKS4) == CLOSE_ST ) ? 
		LOCKED_STATUS : UNLOCKED_STATUS;

	grst.stacker0 = ( get_cstate(VAL1STCK) == CLOSE_ST ) ? 
		INSTALLED : REMOVED;
	grst.stacker1 = ( get_cstate(VAL2STCK) == CLOSE_ST ) ? 
		INSTALLED : REMOVED;

	grst.key_switch = ( get_cstate(KEYSWITCH) == CLOSE_ST ) ? 
		CLOSED : OPENED;

	grst.locker_error0 = grst.locker_error1 = LOCKER_OK;
	grst.locker_error2 = grst.locker_error3 = LOCKER_OK;
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
 *	LOCKER0_ERROR, LOCKER1_ERROR,
 *	LOCKER2_STATUS, LOCKER3_STATUS,
 *	PLUNGER2_STATUS, PLUNGER3_STATUS,
 *	KEY_SWITCH_STATUS,
 *	LOCKER2_ERROR, LOCKER3_ERROR
 */
unsigned char *
get_group_status( void )
{
	static GR1ST_T rem_grst;

	dprintf( printf( "\nLocker0 Status : %s\n", mlst[grst.locker0] ) );
	dprintf( printf( "Locker1 Status : %s\n", mlst[grst.locker1] ) );
	dprintf( printf( "Locker2 Status : %s\n", mlst[grst.locker2] ) );
	dprintf( printf( "Locker3 Status : %s\n", mlst[grst.locker3] ) );
	dprintf( printf( "Plunger0 Status : %s\n", mpst[grst.plunger0] ) );
	dprintf( printf( "Plunger1 Status : %s\n", mpst[grst.plunger1] ) );
	dprintf( printf( "Plunger2 Status : %s\n", mpst[grst.plunger2] ) );
	dprintf( printf( "Plunger3 Status : %s\n", mpst[grst.plunger3] ) );
	dprintf( printf( "KeySwitch Status : %s\n", mpst[grst.key_switch] ) );
	
	if( remove_grqueue(&rem_grst) != -EMPTY_GQUEUE )
	{
		set_memstat( &rem_grst.safe_box );
		return (unsigned char*)&rem_grst;
	}

	set_memstat( &grst.safe_box );
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
	unsigned char *p;

	switch( who )
	{
		case LOCKER0:
			p = &grst.locker0;
			break;
		case LOCKER1:
			p = &grst.locker1;
			break;
		case LOCKER2:
			p = &grst.locker2;
			break;
		case LOCKER3:
			p = &grst.locker3;
			break;
		default:
			return;
	}

	if( *p == state )
		return;

	*p = state;
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
	unsigned char *p;

	switch( who )
	{
		case PLUNGER0:
			p = &grst.plunger0;
			break;
		case PLUNGER1:
			p = &grst.plunger1;
			break;
		case PLUNGER2:
			p = &grst.plunger2;
			break;
		case PLUNGER3:
			p = &grst.plunger3;
			break;
		default:
			return;
	}

	if( *p == state )
		return;

	*p = state;
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
	unsigned char *p;

	switch( who )
	{
		case STACKER0:
			p = &grst.stacker0;
			break;
		case STACKER1:
			p = &grst.stacker1;
			break;
		default:
			return;
	}

	if( *p == state )
		return;

	*p = state;
	put_grqueue(&grst);
}

/*
 * set_stacker_state
 * state: CLOSED/OPENED
 */
void
set_keyswitch_state( unsigned char state )
{
	if( grst.key_switch == state )
		return;

	grst.key_switch = state;

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
	unsigned char *p;

	switch( who )
	{
		case LOCKER0:
			p = &grst.locker_error0;
			break;
		case LOCKER1:
			p = &grst.locker_error1;
			break;
		case LOCKER2:
			p = &grst.locker_error2;
			break;
		case LOCKER3:
			p = &grst.locker_error3;
			break;
		default:
			return;
	}

	if( *p == error )
		return;

	*p = error;
	put_grqueue(&grst);
}
