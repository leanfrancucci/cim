/*
 *	adc.c
 *
 *	Routines for ADC Module.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 13/09/07.
 */

#include "derivative.h"
#include "mytypes.h"
#include "adc.h"

/*
 *	adc_init: ADC module init and configuartion
 */
void
adc_init( void )
{
	ATD1PE_ATDPE0 = 1;		/* PORTB0 configured as ATD pin */
	ATD1C_ATDPU = 1;        /* Enable ADC */
	ATD1C_DJM = 1;          /* Right Justified */
	ATD1C_RES8 = 0;         /* 10-bit Resolution */
	ATD1C_PRS = 0x04;       
}

/*
 *	read_adc: Performs one conversion en the chanel 
 *			  selected and return the adc value.
 */
unsigned short
read_adc( void )
{
	/*unsigned short value;*/
	
	ATD1SC = ADC_CH0;			/* conversion triggering */
	while(!ATD1SC_CCF)
	;
   	return ATD1R;
}
