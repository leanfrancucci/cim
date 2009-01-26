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
#include "reset.h"
#include "grstatus.h"
#include "news.h"
#include "pwrsys.h"
#include "nqdata.h"
#include "pwfhal.h"
#include "dfpwf.h"
#include "watchdog.h"

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
#include "ioports.h"
/*
 *  irq_handler: External IRQ Interrupt Service Routine
 */
void
interrupt 2
irq_handler( void )
{
	GR1ST_T *grs;

	IRQSC_IRQACK = 1;
	
	if( ((grs = get_grstat())->safe_box & BATTOK) != 0 )
	{
		do_disable_pwf_interrupt();
		proc_pwrstatus_irq();
		put_nqueue(EMER_QUEUE, def_news[TOUT_FSYNC_IX] );
		return;
	}
	kick_cop_wdog();
	dfpwf_interrupt();
	do_disable_pwf_interrupt();
	for(;;)
	{
		kick_watchdog();
		kick_cop_wdog();
	}
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
	init_pwrhdl();
	irq_init();
}

