/*
 * grstatus.h
 */

#ifndef __GRSTATUS_H__
#define __GRSTATUS_H__

#include "mytypes.h"

typedef struct
{
	unsigned char locker0;
	unsigned char plunger0;
	unsigned char locker1;
	unsigned char plunger1;
	unsigned char safe_box;
	unsigned char stacker0;
	unsigned char stacker1;
	unsigned char locker_error0;
	unsigned char locker_error1;
} GR1ST_T;

/*
 * Number of devices into group one
 */

#define GR1_DEVS		sizeof( GR1ST_T )

/*
 * Locker status
 */

enum
{
	LOCKED_STATUS, UNLOCKED_STATUS,
	LOCKING_STATUS, UNLOCKING_STATUS,
};

/*
 * Plunger status
 */

enum
{
	OPENED, CLOSED
};

/*
 * Stacker status
 */

enum
{
	INSTALLED, REMOVED
};

/*
 * Locker errors
 */

enum
{
	LOCKER_OK, 
	LOCKER_DRVA, LOCKER_DRVB,
	LOCKER_OPEN, LOCKER_FAIL
};



/*
 * init_group_status:
 */
void init_group_status( void );

GR1ST_T * get_grstat( void );

/*
 * get_group_status:
 *	Return devices status as array in this order:
 *
 *	LOCKER0_STATUS, PLUNGER0_STATUS,
 *	LOCKER1_STATUS, PLUNGER1_STATUS, SAFE_BOX
 *	STACKER0_STATUS, STACKER1_STATUS,
 *	LOCKER0_ERROR, LOCKER1_ERROR
 */

unsigned char *get_group_status( void );

/*
 * set_safebox_st:
 */
void set_safebox_st( unsigned char state );

/*
 * set_locker_state:
 * 	who: LOCKER0/LOCKER1
 * 	state: LOCKED_STATUS/UNLOCKED_STATUS
 */
void set_locker_state( MUInt who, unsigned char state );

/*
 * set_plunger_state:
 * 	who: PLUNGER0/PLUNGER1
 * 	state: OPENED/CLOSED
 */
void set_plunger_state( MUInt who, unsigned char state );

/*
 * set_stacker_state
 * who: STACKER0/STACKER1
 * state: INSTALLED/REMOVED 
 */
void
set_stacker_state( MUInt who, unsigned char state );

/*
 * inform_locker_error
 *	who: LOCKER0/LOCKER1
 *	error: 	LOCKER_OK, LOCKER_DRVA, LOCKER_DRVB,
 *			LOCKER_OPEN, LOCKER_FAIL
 */
void inform_locker_error( MUInt who, MUInt error );

#endif
