/*
 * 	slogcom1.c
 */

#include "derivative.h"
#include "mytypes.h"
#include "slogcom1.h"
#include "serdefs.h"
#include "scibrtbl.h"
#include "sci.h"

void
com1_init( const SERIAL_T *p )
{
	com1_sci1_init( sci_br_tbl[p->baud] );
}

void
com1_setbd( MUInt bd )
{
	com1_sci1_setbd( sci_br_tbl[bd] );
}

void
com1_sendchar( unsigned char c )
{
	com1_sci1_sendchar( c );
}
