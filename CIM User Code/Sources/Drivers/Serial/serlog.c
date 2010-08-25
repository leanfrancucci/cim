/*
 * 	serlog.c
 */

#include "mytypes.h"
#include "serdefs.h"
#include "slogval.h"
#include "sloghost.h"

void
init_serlog( MUInt channel, const SERIAL_T *p )
{
	if( channel == CTCH )
		host_init(p);
	else
		val_init(p);	
}

void
serlog_setbd( MUInt channel, MUInt bd )
{
	if( channel == CTCH )
		host_setbd( bd );
	else
		val_setbd( bd );
}

void
serlog_sendchar( MUInt channel, const unsigned char *p )
{
	if( channel == CTCH )
		host_sendchar( *p );
	else
		val_sendchar( *p );
}


