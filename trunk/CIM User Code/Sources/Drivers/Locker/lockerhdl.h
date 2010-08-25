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

#define set_lock0_a()		lock_a_ctrl(LOCKER0,CTRL_ON)
#define clear_lock0_a()		lock_a_ctrl(LOCKER0,CTRL_OFF)
#define set_lock0_b()		lock_b_ctrl(LOCKER0,CTRL_ON)
#define clear_lock0_b()		lock_b_ctrl(LOCKER0,CTRL_OFF)

#define set_lock1_a()		lock_a_ctrl(LOCKER1,CTRL_ON)
#define clear_lock1_a()		lock_a_ctrl(LOCKER1,CTRL_OFF)
#define set_lock1_b()		lock_b_ctrl(LOCKER1,CTRL_ON)
#define clear_lock1_b()		lock_b_ctrl(LOCKER1,CTRL_OFF)

#define set_nlock0_b()		nlock_b_ctrl(CTRL_ON)
#define clear_nlock0_b()	nlock_b_ctrl(CTRL_OFF)

void lock_a_ctrl( MUInt who, MUInt ctrl );
void lock_b_ctrl( MUInt who, MUInt ctrl );
void nlock_b_ctrl( MUInt ctrl );

MUInt is_unlocked( MUInt who );

MUInt is_locker_open( MUInt who );
