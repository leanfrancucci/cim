/*
 *	irq.h
 *
 *	Routines for IRQ Module.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 14/09/07.
 */

#include "mytypes.h"

enum
{
	DISABLE_DET, ENABLE_DET
};

/*
 *	irq_init: Inits External IRQ module
 */

unsigned char is_irqpin_set( void );

void irqpin_det( MUInt ctrl );

void init_pwrfail_det( void );

