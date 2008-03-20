/*
 * lockerhdl.c
 */

#include "mytypes.h"
#include "lockerhdl.h"
#include "sleep.h"
#include "csdevs.h"
#include "ioports.h"
#include "cparse.h"
#include "cpdata.h"

#define LOCKER_DRV_RESP_TIME (4/SLEEP_BASE)

void
lock_a_ctrl( MUInt who, MUInt ctrl )
{
	if( who == LOCKER0 )
		LOCK1A = ctrl;
	else
		LOCK2A = ctrl;
	sleep(LOCKER_DRV_RESP_TIME);
}

void
lock_b_ctrl( MUInt who, MUInt ctrl )
{
	if( who == LOCKER0 )
		LOCK1B = ctrl;
	else
		LOCK2B = ctrl;
	sleep(LOCKER_DRV_RESP_TIME);
}

MUInt
is_unlocked( MUInt who )
{
	if( who == LOCKER0 )
		return LOCK1_CURR == LOCK_CURR_UNLOCKED; 
	else
		return LOCK2_CURR == LOCK_CURR_UNLOCKED;
}

MUInt
is_locker_open( MUInt who )
{
	if( who == LOCKER0 )
		return ( get_cstate(LOCKS1) == OPEN_ST );
	else
		return 0;
}
