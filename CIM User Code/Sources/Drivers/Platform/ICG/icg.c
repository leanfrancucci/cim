/*
 *	icg.c
 *
 *	Routines for ICG Module.
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 26/05/06.
 */

#include "derivative.h"
#include "icg.h"

/*
 *	icg_init: onchip clock generator init
 */
void
icg_init( void )
{
/*		Set the clock reference as:
 *		External crystal + MODE 5 - FEE -
 *		MFD(N) = 4 , RFD(R) = 1, RANGE(P)=1
 *		** Xtal: 8.192Mhz **
 *		Busclk = ( 8.192Mhz x P x (N/R) ) / 2 = 16,384Mhz */

	ICGC1 = 0x78;
	ICGC2 = 0x00;
	while( !( ICGS1_LOCK ) );
	
	FSTAT = 0x30;
	FCDIV = 0x4A;		
}


