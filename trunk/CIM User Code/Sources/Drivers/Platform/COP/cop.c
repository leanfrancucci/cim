/*
 *	cop.c
 *
 *	Routines for COP Module.
 *
 */

#include "derivative.h"
#include "cop.h"

#define COPE    0x80
#define COPT    0x40
#define STOPE   0x20
#define BKGDPE  0x02

#define _kick_cop_wdog() __RESET_WATCHDOG()

/*
 *	cop_init: Computer Operate Properly Module Init
 */
void
cop_init( void )
{
	SOPT = (COPE|COPT|STOPE|BKGDPE);
}

/*
 *	kick_cop: Feeds the COP watchdog
 */
void
kick_cop_wdog( void )
{
	_kick_cop_wdog();
}
