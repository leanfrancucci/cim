/*
 * 	slogval.c
 */
#include "ioports.h"
#include "mytypes.h"
#include "slogval.h"
#include "serdefs.h"
#include "scibrtbl.h"
#include "sci.h"

void
val_init( SERIAL_T *p )
{
	val_sci2_init( p );
}

void
val_setbd( MUInt bd )
{
	val_sci2_setbd( sci_br_tbl[ bd ] );
}

void
val_sendchar( unsigned char c )
{
	val_sci2_sendchar( c );
}
