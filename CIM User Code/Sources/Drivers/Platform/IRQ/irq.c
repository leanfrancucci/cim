/*
 *	irq.c
 *
 *	Routines for IRQ Module.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 14/09/07.
 */

#include "derivative.h"
#include "irq.h"
#include "cop.h"
#include "pwrhdl.h"

/*
 *	irq_init: Inits External IRQ module
 */
static
void
irq_init( void )
{
	IRQSC_IRQEDG = 0;
	IRQSC_IRQPE = 1;
	IRQSC_IRQMOD = 1;
	IRQSC_IRQACK = 1;
	IRQSC_IRQIE = 1;
}

/*
 *  irq_handler: External IRQ Interrupt Service Routine
 */
void
interrupt 2
irq_handler( void )
{
	IRQSC_IRQACK = 1;
	kick_cop_wdog();
	proc_pwrstatus_irq();
}

MUInt
is_irqpin_set( void )
{
	unsigned char i;
	asm {
            	PSHA
				LDA	#1
				BIH low 	/* branch if high */
				LDA	#0
		low:	STA i
				PULA
		};
	return i;
}

void
do_disable_pwf_interrupt( void )
{
	IRQSC_IRQIE = 0;
}

void
do_enable_pwf_interrupt( void )
{
	IRQSC_IRQACK = 1;
	IRQSC_IRQIE = 1;
}

void
init_pwrfail_det( void )
{
	irq_init();
}

