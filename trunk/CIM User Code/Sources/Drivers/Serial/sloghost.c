/*
 * 	sloghost.c
 */

#include "derivative.h"
#include "mytypes.h"
#include "sloghost.h"
#include "serdefs.h"
#include "scibrtbl.h"
#include "sci.h"

void
host_init( SERIAL_T *p )
{
	host_sci1_init( p );
}

void
host_setbd( MUInt bd )
{
	host_sci1_setbd( sci_br_tbl[ bd ] );
}

void
host_sendchar( unsigned char c )
{
	host_sci1_sendchar( c );
}
