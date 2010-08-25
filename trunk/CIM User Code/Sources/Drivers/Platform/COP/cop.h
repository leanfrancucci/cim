/*
 *	cop.c
 *
 *	Routines for COP Module.
 */

#ifndef _COP_H_
#define	_COP_H_

/*
 *	cop_init: Computer Operate Properly Module Init
 */
void cop_init( void );

/*
 *	kick_cop: Feeds the COP watchdog
 */
void kick_cop_wdog( void );

#endif

