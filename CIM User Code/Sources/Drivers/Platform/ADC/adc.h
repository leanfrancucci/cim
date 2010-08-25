/*
 *	adc.c
 *
 *	Routines for ADC Module.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 13/09/07.
 */

enum
{
	ADC_CH0,
	ADC_CH1,
	ADC_CH2,
	ADC_CH3,
	ADC_CH4,
	ADC_CH5,
	ADC_CH6,
	ADC_CH7,
	ADC_VREFH = 0x1E,
	ADC_VREFL
};

/*
 *	adc_init: ADC module init and configuartion
 */
void adc_init( void );

/*
 *	read_adc: Return the last conversion value
 *				and trigger the next one. 
 */
unsigned short read_adc( void );

