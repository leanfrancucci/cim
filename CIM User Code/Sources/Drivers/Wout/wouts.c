/*
 * wouts.c
 */

#include "mytypes.h"
#include "ioports.h"
#include "wouts.h"
#include "enaint.h"
#include "mpdata.h"

static MUInt wout_image,kickwouts;
static MUInt vlocks_st[NUM_VLOCKS_MINORS];

#ifdef VRLY_CTRL_ENABLE
static MUInt vrly_st[NUM_VRLY_MINORS];
#endif

void 
init_wouts( void )
{
	wout_image = ALL_WOUTS_OFF;
	wout_image |= VRLY_CTRL_W;
	wout_image |= WDOG_CTRL_W;
}
void
enable_vlocks( MUInt who )
{
	MUInt min;

	switch( who )
	{
		case LOCK0_CTRL:
			min = VLOCKS_LOCKER0;
			break;
		case LOCK1_CTRL:
			min = VLOCKS_LOCKER1;
			break;
		case LOCK2_CTRL:
		case LOCK3_CTRL:
		default:
			return;
	}
	
	vlocks_st[min] = 1;
	wout_image |= VLOCKS_CTRL_W;
	WOUT_IOPORT	|= wout_image;
	WOUT_IOPORT	&= ~wout_image;
	WOUT_IOPORT	|= wout_image;
	sleep(VLOCK_ENABLE_DELAY);
}

void
disable_vlocks( MUInt who )
{
	MUInt i;
	MUInt min;

	switch( who )
	{
		case LOCK0_CTRL:
			min = VLOCKS_LOCKER0;
			break;
		case LOCK1_CTRL:
			min = VLOCKS_LOCKER1;
			break;
		case LOCK2_CTRL:
		case LOCK3_CTRL:
		default:
			return;
	}

	vlocks_st[min] = 0;
	for( i=0; i < NUM_VLOCKS_MINORS; ++i )
		if( vlocks_st[i] == 1 )
			return;
	wout_image &= ~VLOCKS_CTRL_W;
	WOUT_IOPORT &= ~VLOCKS_CTRL_W;
}

#ifdef VRLY_CTRL_ENABLE
void
enable_vrly( MUInt who )
{
	vrly_st[who] = 1;
	wout_image |= VRLY_CTRL_W;
	WOUT_IOPORT	|= wout_image;
	WOUT_IOPORT	&= ~wout_image;
	WOUT_IOPORT	|= wout_image;
}

void
disable_vrly( MUInt who )
{
	MUInt i;
	vrly_st[who] = 0;
	for( i=0; i < NUM_VRLY_MINORS; ++i )
		if( vrly_st[i] == 1 )
			return;
	wout_image &= ~VRLY_CTRL_W;
	WOUT_IOPORT &= ~VRLY_CTRL_W;
}
#endif

void
kick_wouts( void )
{
	sem_disable();
	kickwouts = 1;
	sem_enable();
}

void
proc_wouts( void )
{
	if( kickwouts )
		kickwouts = 0;
	WOUT_IOPORT ^= wout_image;
}
