/*
 * lockerhdl.h
 */

#include "mytypes.h"

enum
{
	CTRL_OFF,
	CTRL_ON
};

enum
{
	LOCK_CURR_UNLOCKED,
	LOCK_CURR_LOCKED
};

#define set_nlock0_b()		nlock_b_ctrl(CTRL_ON)
#define clear_nlock0_b()	nlock_b_ctrl(CTRL_OFF)

void lock_a_ctrl( MUInt who, MUInt ctrl );
void lock_b_ctrl( MUInt who, MUInt ctrl );
void nlock_b_ctrl( MUInt ctrl );

MUInt is_unlocked( MUInt who );

MUInt is_notunlocked( MUInt who );
