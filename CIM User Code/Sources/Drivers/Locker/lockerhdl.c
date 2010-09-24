/*
 * lockerhdl.c
 */

#include "mytypes.h"
#include "lockerhdl.h"
#include "sleep.h"
#include "csdevs.h"
#include "ioports.h"
#include "sensor.h"
#include "mpdata.h"

#define LOCKER_DRV_RESP_TIME (4/SLEEP_BASE)

void
lock_a_ctrl( MUInt who, MUInt ctrl )
{
	switch( who )
	{
		case LOCK0_CTRL:
			LOCK1A = ctrl;
			break;
		case LOCK1_CTRL:
			LOCK2A = ctrl;
			break;
		case LOCK2_CTRL:
			LOCK3 = ctrl;
			break;
		case LOCK3_CTRL:
			LOCK4 = ctrl;
			break;
		default:
			break;
	}
	sleep(LOCKER_DRV_RESP_TIME);
}

void
lock_b_ctrl( MUInt who, MUInt ctrl )
{
	switch( who )
	{
		case LOCK0_CTRL:
			LOCK1B = ctrl;
			break;
		case LOCK1_CTRL:
			LOCK2B = ctrl;
			break;
		case LOCK2_CTRL:
			LOCK3 = ctrl;
			break;
		case LOCK3_CTRL:
			LOCK4 = ctrl;
			break;
		default:
			break;
	}

	sleep(LOCKER_DRV_RESP_TIME);
}

void
nlock_b_ctrl( MUInt ctrl )
{
	NOT_LOCK1B_SEC = !ctrl; 
}

MUInt
is_unlocked( MUInt who )
{
	switch( who )
	{
		case LOCK0_CTRL:
			return LOCK1_CURR == LOCK_CURR_UNLOCKED; 
		case LOCK1_CTRL:
			return LOCK2_CURR == LOCK_CURR_UNLOCKED;
		case LOCK2_CTRL:
			return 1;
		case LOCK3_CTRL:
			return 1;
		default:
			break;
	}
}

MUInt
is_notunlocked( MUInt who )
{
	switch( who )
	{
		case LOCK0_CTRL:
			return LOCK1_CURR != LOCK_CURR_UNLOCKED; 
		case LOCK1_CTRL:
			return LOCK2_CURR != LOCK_CURR_UNLOCKED;
		case LOCK2_CTRL:
			return 1;
		case LOCK3_CTRL:
			return 1;
		default:
			break;
	}	
}

