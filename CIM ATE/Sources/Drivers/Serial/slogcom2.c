/*
 * 	slogcom2.c
 */
#include "ioports.h"
#include "mytypes.h"
#include "slogcom2.h"
#include "serdefs.h"
#include "scibrtbl.h"
#include "sci.h"

void
com2_init( const SERIAL_T *p )
{
#ifdef __PRINTF__
	printf( "Module init_serlog " );
#endif
	com2_sci2_init( sci_br_tbl[p->baud] );
}

void
com2_setbd( MUInt bd )
{
	com2_sci2_setbd( sci_br_tbl[bd] );
}

void
com2_sendchar( unsigned char c )
{
	com2_sci2_sendchar( c );
}
