/*
 * locker.h
 */

#ifndef __LOCKER_H__
#define __LOCKER_H__

#include "mytypes.h"

/*
 * For locking and unlocking
 * lockers
 */

enum
{
	TOLOCK, TOUNLOCK
};

enum
{
	LOCK0_HDLR, LOCK1_HDLR, LOCK2_HDLR, LOCK3_HDLR,
	NUM_LOCKS
};

MUInt get_lock_ix( MUInt which );

/*
 * set_locker:
 * 	Send news according to desired operations on
 * 	seleted locker.	The device number is extracted from 
 * 	devices table.
 */
void set_locker( MUInt which, MUInt ctrl );

void set_tlock( MUInt which, MUInt tlock );
unsigned short get_tlock_timer( MUInt which );
void set_tunlock_enable( MUInt which, MUInt tunlock_enable );
unsigned short get_unlock_ena_timer( MUInt which );

#endif
