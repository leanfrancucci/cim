/*
 *	sci.c
 *	Routines handle SCI service
 *
 *	Programmer: Darío S. Baliña.
 *	Date : 29/05/06.
 */

#include "derivative.h"
#include "mytypes.h"
#include "sci.h"
#include "gsqueue.h"
#include "qdata.h"


#define OVRN	0x08
#define NF		0x04
#define FE		0x02
#define PE		0x01

void 
com1_sci1_init( unsigned short baud_rate )
{
	unsigned short *pbaud;
	
	pbaud = (unsigned short*)&SCI1BDH;
	
	SCI1C2_RIE = 0;

	SCI1C1 = 0;
	SCI1C2_TE = 0;
	SCI1C2_RE = 0;

	*pbaud = baud_rate;

	SCI1C2_TE = 1;
	SCI1C2_RE = 1;
	SCI1C2_RIE = 1;
}

void 
com2_sci2_init( unsigned short baud_rate )
{
	unsigned short *pbaud;

	pbaud = (unsigned short*)&SCI2BDH;
	
	SCI2C2_RIE = 0;

/* Even Parity enable */
	SCI2C1 = 0x12;
/*********************/

	SCI2C2_TE = 0;
	SCI2C2_RE = 0;

	*pbaud = baud_rate;

	SCI2C2_TE = 1;
	SCI2C2_RE = 1;
	SCI2C2_RIE = 1;
}

void
com1_sci1_setbd( unsigned short baud_rate )
{
  while( !SCI1S1_TC || !SCI1S1_TDRE );
  
  com1_sci1_init( baud_rate );
}

void
com2_sci2_setbd( unsigned short baud_rate )
{
  while( !SCI2S1_TC || !SCI2S1_TDRE );
  
  com2_sci2_init( baud_rate );
}

void
com1_sci1_sendchar( unsigned char c )
{
  	while( !SCI1S1_TDRE );
	SCI1D = c;
	while( !SCI1S1_TC );

}

void
com2_sci2_sendchar( unsigned char c )
{
  	while( !SCI2S1_TDRE );

	SCI2D = c;
}

void
interrupt 17
com1_rcv_isr( void )
{
	volatile MUInt data;

	if( SCI1S1 & (OVRN|PE|FE|NF))
	{
		data = SCI1D;
		return;
	}
	
	put_gsqueue( COM1_QUEUE, SCI1D );
}

void
interrupt 20
com2_rcv_isr( void )
{
	volatile MUInt data;

	if( SCI2S1 & (OVRN|PE|FE|NF))
	{
		data = SCI2C3;
		data = SCI2D;
		return;
	}
	
	data = SCI2C3;
	put_gsqueue( COM2_QUEUE, SCI2D );
}




