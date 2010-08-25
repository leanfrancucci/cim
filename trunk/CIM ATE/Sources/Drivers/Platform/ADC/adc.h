/*
 *	adc.c
 *
 *	Routines for ADC Module.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 13/09/07.
 */
#define ANALOG_PORT		PORT1
#define ADC_BUFF_SIZE	10
#define GAIN_K			(270000/127)
#define ADC_RES			(33000/1024)
//#define ADC_K			(GAIN_K/ADC_RES) 	/* ADC_K by calculation */
#define ADC_K			64					/* Adjusted by medisions */

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
unsigned short read_adc_ch( MUInt channel );

unsigned short adquire_adc( MUInt mask );

