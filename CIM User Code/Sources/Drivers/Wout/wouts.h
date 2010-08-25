/*
 * wouts.h
 */

#ifndef __WOUTS_H__
#define __WOUTS_H__

#include "mytypes.h"
#include "ioports.h"
#include "sleep.h"

//#define VERSION10
#ifndef VERSION10

#define WDOG_CTRL_W		0x04
#define VLOCKS_CTRL_W	0x08
#define VRLY_CTRL_W		0x10
#define WOUT_IOPORT		PTDD

#else

#define WDOG_CTRL_W		0x08
#define VLOCKS_CTRL_W	0x10
#define VRLY_CTRL_W		0x20

#define WOUT_IOPORT		PTBD
#endif

#define ALL_WOUTS		(WDOG_CTRL_W|WVRLY_CTRL_W|WVRLY_CTRL_W)
#define ALL_WOUTS_OFF	0x00

#define VLOCK_ENABLE_DELAY	(50/SLEEP_BASE)  // 20ms wout interrupt + 8ms supervisor
											 //	polling interval

#undef VRLY_CTRL_ENABLE

enum
{
	VRLY_ALARM, VRLY_HOSTPWR, 
	VRLY_BATTERY, VRLY_MEMCTRL,
	NUM_VRLY_MINORS
};

enum
{
	VLOCKS_LOCKER0, VLOCKS_LOCKER1,
	NUM_VLOCKS_MINORS
};

void init_wouts( void );

void enable_vlocks( MUInt who );
void disable_vlocks( MUInt who );

#ifdef VRLY_CTRL_ENABLE
void enable_vrly( MUInt who );
void disable_vrly( MUInt who );
#endif

void kick_wouts( void );
void proc_wouts( void );

#endif
