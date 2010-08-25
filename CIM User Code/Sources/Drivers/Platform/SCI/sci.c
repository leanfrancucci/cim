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
#include "serdefs.h"
#include "scibrtbl.h"

#define OVRN	0x08
#define NF		0x04
#define FE		0x02
#define PERR	0x01

void 
host_sci1_init( SERIAL_T *p )
{
	unsigned short *pbaud;
	
	while( !SCI1S1_TC || !SCI1S1_TDRE )
		;
  
	pbaud = (unsigned short*)&SCI1BDH;
	
	SCI1C2_RIE = 0;
	SCI1C2_TE = 0;
	SCI1C2_RE = 0;

	SCI1C1 = 0;		/* set bits to default */

	SCI1C1_PE = p->parity == NO_PAR ? 0 : 1;	/* parity enable */
	SCI1C1_PT = p->parity == EVEN_PAR ? 0 : 1;	/* set parity type */

	/* Warning: the word bits is wired to 9 bits for parity enable !!! */

	if( p->parity != NO_PAR )
		SCI1C1_M = 1;			/* 9-bits */
	else
		SCI1C1_M = p->bits == BIT8 ? 0 : 1;

	*pbaud = sci_br_tbl[ p->baud ];

	SCI1C2_TE = 1;
	SCI1C2_RE = 1;
	SCI1C2_RIE = 1;
}

void 
val_sci2_init( SERIAL_T *p )
{
	unsigned short *pbaud;

	while( !SCI2S1_TC || !SCI2S1_TDRE )
		;
  
	pbaud = (unsigned short*)&SCI2BDH;
	
	SCI2C2_RIE = 0;
	SCI2C2_TE = 0;
	SCI2C2_RE = 0;

	SCI2C1 = 0;		/* set bits to default */

	SCI2C1_PE = p->parity == NO_PAR ? 0 : 1;	/* parity enable */
	SCI2C1_PT = p->parity == EVEN_PAR ? 0 : 1;	/* set parity type */

	/* Warning: the word bits is wired to 9 bits for parity enable !!! */
	if( (p->parity != NO_PAR) && (p->bits == BIT8) )
		SCI2C1_M = 1;			/* 9-bits */
	else
		SCI2C1_M = 0;

	*pbaud = sci_br_tbl[ p->baud ];

	SCI2C2_TE = 1;
	SCI2C2_RE = 1;
	SCI2C2_RIE = 1;
	SCI2C3 = 0x0F;
}

void
host_sci1_setbd( unsigned short baud_rate )
{
	unsigned short *pbaud;
	
	while( !SCI1S1_TC || !SCI1S1_TDRE )
		;
  
	pbaud = (unsigned short*)&SCI1BDH;
	
	SCI1C2_RIE = 0;
	SCI1C2_TE = 0;
	SCI1C2_RE = 0;

	*pbaud = baud_rate;

	SCI1C2_TE = 1;
	SCI1C2_RE = 1;
	SCI1C2_RIE = 1;
}

void
val_sci2_setbd( unsigned short baud_rate )
{
	unsigned short *pbaud;

	while( !SCI2S1_TC || !SCI2S1_TDRE )
		;
  
	pbaud = (unsigned short*)&SCI2BDH;
	
	SCI2C2_RIE = 0;
	SCI2C2_TE = 0;
	SCI2C2_RE = 0;

	*pbaud = baud_rate;

	SCI2C2_TE = 1;
	SCI2C2_RE = 1;
	SCI2C2_RIE = 1;
}

void
host_sci1_sendchar( unsigned char c )
{
  	while( !SCI1S1_TDRE );
	SCI1D = c;
	while( !SCI1S1_TC );
}

void
val_sci2_sendchar( unsigned char c )
{
  	while( !SCI2S1_TDRE );

	SCI2D = c;
}

void
interrupt 17
host_rcv_isr( void )
{
	volatile MUInt data;

	if( SCI1S1 & (OVRN|PERR|FE|NF))
	{
		data = SCI1D;
		return;
	}
	
	put_gsqueue( 0, SCI1D );
}

void
interrupt 19
val_err_isr( void )
{
	volatile MUInt datactrl;
	volatile MUInt dataerr;
	volatile MUInt databuff;

	dataerr  = SCI2S1 & 0x0F;
	databuff = SCI2D;
	datactrl = SCI2C3;

	if( dataerr & NF )
		put_gsqueue( 1, databuff );

}

void
interrupt 20
val_rcv_isr( void )
{
	volatile MUInt data;
	
	data = SCI2S1;
	data = SCI2C3;
	put_gsqueue( 1, SCI2D );
}

void 
disable_rcv_interrupt( MUInt ch )
{
	if( ch == VALCH )
	{
		SCI2C2_RIE = 0;
		SCI2C2_RE = 0;
	}
	else
	{
		SCI1C2_RIE = 0;
		SCI1C2_RE = 0;
	}
}

void 
enable_rcv_interrupt( MUInt ch )
{
	volatile MUInt data;

	if( ch == VALCH )
	{
		while( !SCI2S1_TC || !SCI2S1_TDRE )
			;
		data = SCI2D;
		SCI2C2_RE = 1;
		SCI2C2_RIE = 1;
	}
	else
	{
		while( !SCI1S1_TC || !SCI1S1_TDRE )
			;
		data = SCI1D;
		SCI1C2_RE = 1;
		SCI1C2_RIE = 1;
	}
}
