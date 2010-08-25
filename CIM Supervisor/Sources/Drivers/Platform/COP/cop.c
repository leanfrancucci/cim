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
#define RSTPE	0x01

/*
 *	cop_init: Computer Operate Properly Module Init
 */
void
cop_init( void )
{
//	SOPT1 = (STOPE|BKGDPE|RSTPE);
//	SOPT1 = RSTPE;
//    __RESET_WATCHDOG(); 
	SOPT1 = 0xC3;
	SOPT2_COPCLKS = 1;
}


