/*
 *	adc.c
 *
 *	Routines for ADC Module.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 13/09/07.
 */

#ifdef _S08_PLATFORM_
#include "derivative.h"
#endif


#include "mytypes.h"
#include "adc.h"
#include "iodefs.h"

/*
 *	adc_init: ADC module init and configuartion
 */
void
adc_init( void )
{

#ifdef _S08_PLATFORM_
	ATD1PE_ATDPE0 = 1;		/* PORTB0 configured as ATD pin */
	ATD1PE_ATDPE1 = 1;		/* PORTB1 configured as ATD pin */
	ATD1PE_ATDPE2 = 1;		/* PORTB2 configured as ATD pin */
	ATD1C_ATDPU = 1;        /* Enable ADC */
	ATD1C_DJM = 1;          /* Right Justified */
	ATD1C_RES8 = 0;         /* 10-bit Resolution */
	ATD1C_PRS = 0x04;       
#endif
}

/*
 *	read_adc: Performs one conversion en the chanel 
 *			  selected and return the adc value.
 */
unsigned short
read_adc_ch( MUInt channel )
{
#ifdef _S08_PLATFORM_
	unsigned short value;

	switch( channel )
	{
		case BIT0:
			ATD1SC = ADC_CH0;
			break;
		case BIT1:
			ATD1SC = ADC_CH1;
			break;
		case BIT2:
			ATD1SC = ADC_CH2;
			break;
		case BIT3:
			ATD1SC = ADC_CH3;
			break;
		case BIT4:
			ATD1SC = ADC_CH4;
			break;
		case BIT5:
			ATD1SC = ADC_CH5;
			break;
		case BIT6:
			ATD1SC = ADC_CH6;
			break;
		case BIT7:
			ATD1SC = ADC_CH7;
			break;
	}
	while(!ATD1SC_CCF)
	;
   	return ATD1R;
#else
	return 0;
#endif
}

unsigned short
adquire_adc( MUInt mask )
{
	MUInt i;
	static unsigned short adclvl_buff[ADC_BUFF_SIZE];
	static unsigned short adclvl;

	for( i = 0; i < ADC_BUFF_SIZE; ++i )
		adclvl_buff[i] = read_adc_ch( mask );

	for( i = 1, adclvl = adclvl_buff[0]; i < ADC_BUFF_SIZE; ++i )
	{
		adclvl += adclvl_buff[i];
	}
//	adclvl /= ADC_BUFF_SIZE;
	
	return adclvl;
}

