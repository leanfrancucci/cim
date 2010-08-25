/*
 * 	lowint.h
 * 		Low level routines for
 * 		interrupt management
 */

#include "mytypes.h"

void enable_interrupt( void );
void disable_interrupt( void );

/*
 * get_enable_status:
 * Returns	1 if global interrupts ar enabled.
 * 			0 if globla interrupts ar disabled.
 */
MUInt get_enable_status( void );

