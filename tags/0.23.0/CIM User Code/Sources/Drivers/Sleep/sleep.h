/*
 *	sleep.h
 *	
 *	Programmer: Dar�o S. Bali�a.
 *	Date : 26/05/06.
 */

#ifndef __SLEEP_H__
#define __SLEEP_H__

#define _1SEC	(1000/SLEEP_BASE)
#define SLEEP_BASE	2

/*
 * sleep: Performs a delay of msec in the execution
 * 			msec must be especified as a multiplo of 
 * 			RTI cadence MSEQBASE.
 */
void sleep( unsigned short msec );
void check_sleep( void );

#endif



